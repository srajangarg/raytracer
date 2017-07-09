/*
 * Lights.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 *      Modified: sidch
 */

#include "Lights.hpp"

Light::Light()
{
  RGB black(0, 0, 0);
  illumination_ = black;
  falloff_ = 0;
  angular_falloff_ = 0;
  dead_distance_ = 1;
}

Light::Light(RGB const & illumination)
{
  illumination_ = illumination;
}

Light::Light(RGB const & illumination, double falloff, double dead_distance)
{
  illumination_ = illumination;
  falloff_ = falloff;
  dead_distance_ = dead_distance;
}

Light::~Light()
{}

RGB Light::getColor() const
{
  return illumination_;
}

RGB Light::getColor(Vec3 const & p) const
{
  return illumination_;
}

void
Light::setColor(RGB const & c)
{
  illumination_ = c;
}

AmbientLight::AmbientLight()
{
  // intentionally empty
}

AmbientLight::AmbientLight(RGB const & illumination) : Light(illumination)
{
  // intentionally empty
}

Vec3
AmbientLight::getIncidenceVector(Vec3 const & position) const
{
  throw "AMBIENT LIGHTS DO NOT HAVE A SENSE OF DIRECTION OR POSITION`";
}

vector<Ray> AmbientLight::getShadowRays(Vec3 const & position, bool & use_dist) const
{
  throw "AMBIENT LIGHTS DO NOT HAVE A SENSE OF DIRECTION OR POSITION";
}

PointLight::PointLight(RGB const & illumination) : Light(illumination)
{
  // intentionally empty
}

PointLight::PointLight(RGB const & illumination, double falloff, double dead_distance)
: Light(illumination, falloff, dead_distance)
{
  // intentionally empty
}

RGB
PointLight::getColor(Vec3 const & p) const
{
  // return attenuated light
  return illumination_ / pow((p - pos_).length() + dead_distance_, falloff_);
}

void
PointLight::setPosition(Vec3 const & pos)
{
  pos_ = pos;
}

Vec3
PointLight::getIncidenceVector(Vec3 const & position) const
{ 
  return (pos_ - position).normalize();
}

vector<Ray>
PointLight::getShadowRays(Vec3 const & position, bool & use_dist) const
{
  use_dist = true;
  vector<Ray> v = {Ray::fromOriginAndEnd(position, pos_)};
  return v;
}

AreaLight::AreaLight(RGB const & illumination) : Light(illumination)
{
  // intentionally empty
}

AreaLight::AreaLight(RGB const & illumination, double falloff, double dead_distance, double offset)
: Light(illumination, falloff, dead_distance)
{
  offset_ = offset;
}

RGB
AreaLight::getColor(Vec3 const & p) const
{
  // return attenuated light
  return illumination_ / pow((p - pos_).length() + dead_distance_, falloff_);
}

void
AreaLight::setPosition(Vec3 const & pos)
{
  pos_ = pos;
}

Vec3
AreaLight::getIncidenceVector(Vec3 const & position) const
{ 
  return (pos_ - position).normalize();
}

vector<Ray>
AreaLight::getShadowRays(Vec3 const & position, bool & use_dist) const
{
  use_dist = true;
  vector<Ray> v;

  // generated rays from position to all the offset point lights
  for (double i = -1; i < 2; i++)
    for (double j = -1; j < 2; j++)
        for (double k = -1; k < 2; k++)
          v.push_back(Ray::fromOriginAndEnd(position, pos_ 
                                          + Vec3(i*offset_, j*offset_, k*offset_)));
  return v;
}

DirectionalLight::DirectionalLight(RGB const & illumination) : Light(illumination)
{
  // intentionally empty
}

void
DirectionalLight::setDirection(Vec3 const & dir)
{
  dir_ = dir;
  dir_.normalize();
}

Vec3
DirectionalLight::getIncidenceVector(Vec3 const & position) const
{
  return -dir_;
}

vector<Ray>
DirectionalLight::getShadowRays(Vec3 const & position, bool & use_dist) const
{
  use_dist = false;
  vector<Ray> v = {Ray::fromOriginAndDirection(position, -dir_)};
  return v;
}
