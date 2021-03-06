#include "Globals.hpp"
#include "View.hpp"
#include "World.hpp"
#include "Frame.hpp"
#include "Lights.hpp"
#include "core/Scene.hpp"
#include <unordered_map>
#define EPS 5e-4
using namespace std;

//****************************************************
// Global Variables
//****************************************************

Scene * scene = NULL;
World * world = NULL;
View * view = NULL;
Mat4 viewToWorld = identity3D();
Frame * frame = NULL;
int max_trace_depth = 2;
bool smooth_normals = false;

// Get the shaded appearance of the primitive at a given position, as seen along a ray. The returned value should be the sum of
// the shaded colors w.r.t. each light in the scene. DO NOT include the result of recursive raytracing in this function, just
// use the ambient-diffuse-specular formula. DO include testing for shadows, individually for each light.
RGB
getShadedColor(Primitive const & primitive, Vec3 const & pos, Ray const & ray)
{
  RGB amb(0, 0, 0), lamb(0, 0, 0), spec(0, 0, 0);
  RGB C(primitive.getColor()), S, I;
  Vec3 r, u, n, i;
  bool use_dist;

  double ma = primitive.getMaterial().getMA();
  double ms = primitive.getMaterial().getMS();
  double ml = primitive.getMaterial().getML();
  double msp = primitive.getMaterial().getMSP();
  double msm = primitive.getMaterial().getMSM();
  double reach_light, ratio = 0, trans;
  
  // ambient component : Ma * C * A
  amb = ma * C * world->getAmbientLightColor();
  
  for (auto it = world->lightsBegin(); it != world->lightsEnd(); it++)
  {  
    trans = 1;
    auto light = *it;
    vector<Ray> shadows = light->getShadowRays(pos, use_dist);
    reach_light = shadows.size();

    // iterate over shadows
    for (auto shadow : shadows)
    {
      auto inter = world->intersect(shadow);

      if (inter != NULL) {
        // if the light is passing through a transparent object, consider it
        if (inter->getMaterial().getMT() > 0)
            trans = inter->getMaterial().getMT();
        // else decrease the number of shadow rays reaching the object
        else if (not use_dist or (shadow.minT() > 0 and shadow.minT() < 1))
            reach_light--;
      }
    }

    // how many shadow rays did not collide
    ratio = reach_light / (double) shadows.size();
    // if none of them passed thorugh, skip lambertian and specular
    if (ratio < EPS)
      continue;

    // normal and incidence vectors, light color
    n = primitive.calculateNormal(pos);
    i = light->getIncidenceVector(pos);
    I = light->getColor(pos);

    // lambertian component : Ml * C * I * max(i.n, 0)
    lamb += trans * ratio * ml * C * I * std::max(i * n, (double)0);

    // S                    : Msm * C + (1-Msm) * RBG(1, 1, 1)  
    // r                    : 2(i.n)n  - i   
    S = msm * C + (1 - msm) * RGB(1, 1, 1); 
    r = 2*((i * n) * n) - i;
    u = ray.direction();
    u.normalize();

    // specular component   : Ms * S * I * (max(-r.u, 0))^Msp                 
    spec += trans * ratio * ms * S * I * pow(std::max(-r*u, (double)0), msp);
  }

  return amb + spec + lamb;
}

inline Ray
getReflectedRay(Primitive *z, Vec3 &pos, Ray &ray)
{
  Vec3 r, n, i;
  n = z->calculateNormal(pos);
  i = ray.direction();
  r = i - 2*((i * n) * n);
  return Ray::fromOriginAndDirection(pos + EPS*r, r);
}

inline Ray
getRefractedRay(Primitive *z, Vec3 &pos, Ray &ray)
{
  Vec3 r, n, u;
  double cost1, cost2, to_mu, fr_mu;

  u = ray.direction();
  u.normalize();
  n = z->calculateNormal(pos);

  // is the ray entering the primitve or leaving it?
  if (ray.mu() < 1 + EPS) {
    to_mu = z->getMaterial().getMTN();
    fr_mu = 1;
  } else {
    to_mu = 1;
    fr_mu = z->getMaterial().getMTN();
    n = -n;
  }

  // refraction formulas
  cost1 = - (u * n);
  cost2 = sqrt(1 - ((fr_mu/to_mu) * (fr_mu/to_mu) * (1 - cost1*cost1)));
  r = (fr_mu/to_mu)*u + (((cost1 * fr_mu)/to_mu) - cost2)*n;

  return Ray::fromOriginAndDirection(pos + EPS*r, r, std::numeric_limits<double>::infinity(), to_mu);
}
// Raytrace a single ray backwards into the scene, calculating the total color (summed up over all reflections/refractions) seen
// along this ray.
RGB
traceRay(Ray & ray, int depth)
{   
  if (depth > max_trace_depth)
    return RGB(0, 0, 0);

  auto z = world->intersect(ray);

  if (z != NULL) {

    // intersection point
    auto pos = ray.getPos(ray.minT());
    // get color at that point
    auto col = getShadedColor(*z, pos, ray);

    // reflection
    Ray refl = getReflectedRay(z, pos, ray);
    col += z->getMaterial().getMR() * z->getColor() * traceRay(refl, depth+1);

    // refraction
    if (z->getMaterial().getMT() > 0)
    {
      Ray refr = getRefractedRay(z, pos, ray);
      col += z->getMaterial().getMT() * traceRay(refr, depth+1);
    }

    return col;
  }
    
  return RGB(0, 0, 0);
}

// Main rendering loop.
void
renderWithRaytracing()
{
  Sample sample;   // Point on the view being sampled.
  Ray ray;         // Ray being traced from the eye through the point.
  RGB c;           // Color being accumulated per pixel.

  int const rpp = view->raysPerPixel();

  for (int yi = 0; yi < view->height(); ++yi)
  {
    for (int xi = 0; xi < view->width(); ++xi)
    {
      c = RGB(0, 0, 0);
      for (int ri = 0; ri < rpp; ++ri)
      {
        view->getSample(xi, yi, ri, sample);
        ray = view->createViewingRay(sample);  // convert the 2d sample position to a 3d ray
        ray.transform(viewToWorld);            // transform this to world space
        c += traceRay(ray, 0);
      }

      frame->setColor(sample, c / (double)rpp);
    }
  }
}

// This traverses the loaded scene file and builds a list of primitives, lights and the view object. See World.hpp.
void
importSceneToWorld(SceneInstance * inst, Mat4 localToWorld, int time)
{
  if (inst == NULL)
    return;

  Mat4 nodeXform;
  inst->computeTransform(nodeXform, time);
  localToWorld = localToWorld * nodeXform;
  SceneGroup * g = inst->getChild();

  if (g == NULL)   // for example if the whole scene fails to load
  {
    std::cout << "ERROR: We arrived at an instance with no child?!" << std::endl;
    return;
  }

  int ccount = g->getChildCount();

  for (int i = 0; i < ccount; i++)
  {
    importSceneToWorld(g->getChild(i), localToWorld, 0);
  }

  CameraInfo f;

  if (g->computeCamera(f, time))
  {
    viewToWorld = localToWorld;

    if (view != NULL)
      delete view;

    Vec3 eye(0.0, 0.0, 0.0);
    Vec3 LL(f.sides[FRUS_LEFT], f.sides[FRUS_BOTTOM], -f.sides[FRUS_NEAR]);
    Vec3 UL(f.sides[FRUS_LEFT], f.sides[FRUS_TOP], -f.sides[FRUS_NEAR]);
    Vec3 LR(f.sides[FRUS_RIGHT], f.sides[FRUS_BOTTOM], -f.sides[FRUS_NEAR]);
    Vec3 UR(f.sides[FRUS_RIGHT], f.sides[FRUS_TOP], -f.sides[FRUS_NEAR]);
    view = new View(eye, LL, UL, LR, UR, IMAGE_WIDTH, IMAGE_HEIGHT, RAYS_PER_PIXEL_EDGE);
  }

  LightInfo l;

  if (g->computeLight(l, time))
  {
    if (l.type == LIGHT_AMBIENT)
    {
      RGB amb = world->getAmbientLightColor();
      world->setAmbientLightColor(amb + l.color);
    }
    else if (l.type == LIGHT_DIRECTIONAL)
    {
      DirectionalLight * li = new DirectionalLight(l.color);
      Vec3 dir(0, 0, -1);
      li->setDirection(localToWorld * dir);
      world->addLight(li);
    }
    else if (l.type == LIGHT_POINT)
    {
      PointLight * li = new PointLight(l.color, l.falloff, l.deadDistance);
      Vec3 pos(0, 0, 0);
      li->setPosition(localToWorld * pos);
      world->addLight(li);
    }
    else if (l.type == LIGHT_AREA)
    { 
      AreaLight * li = new AreaLight(l.color, l.falloff, l.deadDistance, l.offset);
      Vec3 pos(0, 0, 0);
      li->setPosition(localToWorld * pos);
      world->addLight(li);
    }
    else if (l.type == LIGHT_SPOT)
    {
      throw "oh no";
    }
  }

  double r;
  MaterialInfo m;

  if (g->computeSphere(r, m, time))
  {
    Material mat(m.k[0], m.k[1], m.k[2], m.k[3], m.k[4], m.k[MAT_MS], m.k[5], m.k[6]);
    Sphere * sph = new Sphere(r, m.color, mat, localToWorld);
    world->addPrimitive(sph);
  }

  TriangleMesh * t;
  unordered_map<MeshVertex*, vector<pair<Triangle*, int>> > vertexToTriangles;

  if (g->computeMesh(t, m, time))
  {
    Material mat(m.k[0], m.k[1], m.k[2], m.k[3], m.k[4], m.k[MAT_MS], m.k[5], m.k[6]);

    for (auto it = t->triangles.begin(); it != t->triangles.end(); ++it)
    {
      Triangle * tri = new Triangle(
        t->vertices[ (**it).ind[0] ]->pos,
        t->vertices[ (**it).ind[1] ]->pos,
        t->vertices[ (**it).ind[2] ]->pos,
        m.color, mat, localToWorld, smooth_normals);
      world->addPrimitive(tri);

      if (smooth_normals)
      {
        // map vertex to which triangles this verttex belongs to
        for (int i = 0; i < 3; ++i)
        {
          if(vertexToTriangles.find(t->vertices[(**it).ind[i]]) == vertexToTriangles.end())
            vertexToTriangles[t->vertices[(**it).ind[i]]] = {make_pair(tri, i)};
          else
            vertexToTriangles[t->vertices[(**it).ind[i]]].push_back(make_pair(tri, i));
        }
      }
    }

    if (smooth_normals)
    {
      // calculate average normal at each vertex, and place them at norms
      // in each of the respective triangles
      for (auto it : vertexToTriangles)
      { 
          Vec3 res(0, 0, 0);

          for (auto tripair : it.second)
            res += tripair.first->calculateFaceNormal();
          res.normalize();
          for (auto tripair : it.second)
              tripair.first->norms[tripair.second] = res;
      }
    }
  }

  std::cout << "Imported scene file" << std::endl;
}

int
main(int argc, char ** argv)
{
  if (argc < 3)
  {
    std::cout << "Usage: " << argv[0] << " scene.scd output.png [max_trace_depth]" << std::endl;
    return -1;
  }

  if (argc >= 4)
    max_trace_depth = atoi(argv[3]);
  if (argc >= 5)
    smooth_normals = (atoi(argv[4]) == 1);

  cout << "Max trace depth = " << max_trace_depth << endl;
  if (smooth_normals)
    cout << "Using smooth normals for triangle meshes" << endl;

  // Load the scene from the disk file
  scene = new Scene(argv[1]);

  // Setup the world object, containing the data from the scene
  world = new World();
  importSceneToWorld(scene->getRoot(), identity3D(), 0);
  world->printStats();

  // Set up the output framebuffer
  frame = new Frame(IMAGE_WIDTH, IMAGE_HEIGHT);

  // Render the world
  renderWithRaytracing();

  // Save the output to an image file
  frame->save(argv[2]);
  std::cout << "Image saved!" << std::endl;
}
