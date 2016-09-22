/*
 * View.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 *      Modified: sidch
 */

#include "View.hpp"

// This file assumes all vertices/directions expressed in homogeneous coordinates (Vec3) have w = 1 / 0.

View::View(Vec3 const & eye, Vec3 const & ll, Vec3 const & ul, Vec3 const & lr, Vec3 const & ur,
           int pixels_wide, int pixels_high, int rays_per_pixel_edge)
{
  eye_ = eye;	 // you should use this for the viewing ray
  LL_ = ll;
  UL_ = ul;
  LR_ = lr;
  UR_ = ur;
  pixels_wide_ = pixels_wide;
  pixels_high_ = pixels_high;
  rays_per_pixel_edge_ = rays_per_pixel_edge;
}

View::~View()
{
}

void
View::getSample(int pixel_x, int pixel_y, int ray_index, Sample & s) const
{
  // Some random jitter to break up patterns
  double jitter_x = 0.25 * (std::rand() / (double)RAND_MAX);
  double jitter_y = 0.25 * (std::rand() / (double)RAND_MAX);

  double pixel_sub_x = (ray_index % rays_per_pixel_edge_ + 0.5 + jitter_x) / (double)rays_per_pixel_edge_;
  double pixel_sub_y = (ray_index / rays_per_pixel_edge_ + 0.5 + jitter_y) / (double)rays_per_pixel_edge_;

  s.setX((pixel_x + pixel_sub_x) / (double)pixels_wide_);
  s.setY((pixel_y + pixel_sub_y) / (double)pixels_high_);
}

Vec3
View::getSamplePosition(Sample const & s) const
{
  Vec3 lower = (1 - s.x()) * LL_ + s.x() * LR_;
  Vec3 upper = (1 - s.x()) * UL_ + s.x() * UR_;
  return (1 - s.y()) * lower + s.y() * upper;
}

Ray
View::createViewingRay(Sample const & s) const
{
  Vec3 p = getSamplePosition(s);
  Vec3 d = getViewVector(p);
  return Ray::fromOriginAndDirection(eye_, d);
}

Vec3
View::getViewVector(Vec3 const & pos) const
{
  Vec3 diff = pos - eye_;
  diff.normalize();
  return diff;
}

int
View::width() const
{
  return pixels_wide_;
}

int
View::height() const
{
  return pixels_high_;
}

int
View::raysPerPixel() const
{
  return rays_per_pixel_edge_ * rays_per_pixel_edge_;
}

int
View::raysPerPixelEdge() const
{
  return rays_per_pixel_edge_;
}
