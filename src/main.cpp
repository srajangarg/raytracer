#include "Globals.hpp"
#include "View.hpp"
#include "World.hpp"
#include "Frame.hpp"
#include "Lights.hpp"
#include "core/Scene.hpp"

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
  
  // ambient component : Ma * C * A
  amb = ma * C * world->getAmbientLightColor();
  
  for (auto it = world->lightsBegin(); it != world->lightsEnd(); it++)
  { 
    auto light = *it;
    auto shadow = light->getShadowRay(pos, use_dist);
    auto inter = world->intersect(shadow);

    // an intersection occured, don't add up spec and lamb components
    if (inter != NULL)
      if (not use_dist or (shadow.minT() > 0 and shadow.minT() < 1))
        continue;

    // normal and incidence vectors, light color
    n = primitive.calculateNormal(pos);
    i = light->getIncidenceVector(pos);
    I = light->getColor(pos);

    // lambertian component : Ml * C * I * max(i.n, 0)
    lamb += ml * C * I * std::max(i * n, (double)0);

    // S                    : Msm * C + (1-Msm) * RBG(1, 1, 1)  
    // r                    : 2(i.n)n  - i   
    S = msm * C + (1 - msm) * RGB(1, 1, 1); 
    r = 2*((i * n) * n) - i;
    u = ray.direction();
    u.normalize();

    // specular component   : Ms * S * I * (max(-r.u, 0))^Msp                 
    spec += ms * S * I * pow(std::max(-r*u, (double)0), msp);
  }

  return spec + amb + lamb;
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

    Vec3 r, n, i, pos;
    Ray refl;
    
    // intersection point
    pos = ray.getPos(ray.minT());
    // get color at that point
    auto col = getShadedColor(*z, pos, ray);

    // calculate reflected ray
    n = z->calculateNormal(pos);
    i = ray.direction();
    r = i - 2*((i * n) * n);
    refl = Ray::fromOriginAndDirection(pos, r);

    // add color of reflected ray
    col += z->getMaterial().getMR() * z->getColor() * traceRay(refl, depth+1);
    return col;

  } else {
    
    return RGB(0, 0, 0);
  }
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

  if (g->computeMesh(t, m, time))
  {
    Material mat(m.k[0], m.k[1], m.k[2], m.k[3], m.k[4], m.k[MAT_MS], m.k[5], m.k[6]);

    for (vector<MeshTriangle *>::iterator it = t->triangles.begin(); it != t->triangles.end(); ++it)
    {
      Triangle * tri = new Triangle(
        t->vertices[ (**it).ind[0] ]->pos,
        t->vertices[ (**it).ind[1] ]->pos,
        t->vertices[ (**it).ind[2] ]->pos,
        m.color, mat, localToWorld);
      world->addPrimitive(tri);
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

  cout << "Max trace depth = " << max_trace_depth << endl;

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
