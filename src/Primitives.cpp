/*
 * Primitive.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 *      Modified: sidch
 */

#include "Primitives.hpp"

Primitive::Primitive(RGB const & c, Material const & m, Mat4 const & modelToWorld)
{
  c_ = c;
  m_ = m;
  modelToWorld_ = modelToWorld;
  worldToModel_ = modelToWorld.inverse();
}

Primitive::~Primitive()
{
}

Sphere::Sphere(double radius, RGB const & c, Material const & m, Mat4 const & modelToWorld): Primitive(c, m, modelToWorld)
{
  r_ = radius;
}

bool
Sphere::intersect(Ray & ray) const
{
  Vec3 s, d;
  double dis, t;

  Ray new_ray = ray;
  new_ray.transform(worldToModel_);
  s = new_ray.start();
  d = new_ray.direction();

  dis = ((d*s)*(d*s)) - (d.length2() * (s.length2() - r_*r_));

  if (dis < 0)
    return false;

  dis = std::sqrt(dis);
  t = ((-d*s) - dis)/d.length2();

  if (t > 0 and ray.minT() > t)
  {
    ray.setMinT(t);
    return true;
  }

  return false;
}

Vec3
Sphere::calculateNormal(Vec3 const & position) const
{ 
  // norm is the point's position in the model coordinates
  Vec3 norm = worldToModel_ * position;
  // restoring the normal back to the world coordinates
  return Vec3(worldToModel_.transpose() * Vec4(norm, 0), 3).normalize();
}

//=============================================================================================================================
// Triangle and other primitives are for Assignment 3b, after the midsem. Do not do this for 3a.
//=============================================================================================================================

Triangle::Triangle(Vec3 const & v0, Vec3 const & v1, Vec3 const & v2, RGB const & c, Material const & m,
                   Mat4 const & modelToWorld)
: Primitive(c, m, modelToWorld)
{
  verts[0] = v0;
  verts[1] = v1;
  verts[2] = v2;
}

bool
Triangle::intersect(Ray & ray) const
{
  Vec3 e1, e2;  //Edge1, Edge2
  Vec3 P, Q, T;
  double det, inv_det, u, v, t;
  Ray new_ray = ray;
  new_ray.transform(worldToModel_);

  //Find vectors for two edges sharing V1
  e1 = verts[2] - verts[0];
  e2 = verts[3] - verts[0];

  //Begin calculating determinant - also used to calculate u parameter
  P = new_ray.direction() ^ e2;
  //if determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
  det = e1 * P;

  if(det > -1e-5 && det < 1e-5) return false;
  inv_det = 1.0 / det;

  //calculate distance from V1 to ray origin
  T = new_ray.start() - verts[0];

  //Calculate u parameter and test bound
  u = T * P * inv_det;
  //The intersection lies outside of the triangle
  if(u < 0 || u > 1) return false;

  //Prepare to test v parameter
  Q = T ^ e1;

  //Calculate V parameter and test bound
  v = new_ray.direction() * Q * inv_det;
  //The intersection lies outside of the triangle
  if(v < 0 || u + v  > 1) return false;

  t = e2 * Q * inv_det;

  if (t > 0 and ray.minT() > t)
  {
    ray.setMinT(t);
    return true;
  }

  return false;
}

Vec3
Triangle::calculateNormal(Vec3 const & position) const
{
  Vec3 norm = (verts[2] - verts[0]) ^ (verts[1] - verts[0]);
  return Vec3(worldToModel_.transpose() * Vec4(norm, 0), 3).normalize();;
}