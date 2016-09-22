/*
 * World.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 *      Modified: sidch
 */

#include "World.hpp"

World::World()
{
}

World::~World()
{
  // TODO Auto-generated destructor stub
}

Primitive *
World::intersect(Ray & r) const
{
  Primitive * intersecting = NULL;
    
  // iterate over primitives to find a smaller intersection
  for (auto it = primitivesBegin(); it != primitivesEnd(); it++)
  	if ((*it)->intersect(r))
  		intersecting = *it;

  return intersecting;
}

void
World::addPrimitive(Primitive * p)
{
  primitives_.push_back(p);
}

void
World::addLight(Light * l)
{
  lights_.push_back(l);
}

void
World::setAmbientLightColor(RGB ambientColor)
{
  ambientLight_.setColor(ambientColor);
}

RGB
World::getAmbientLightColor() const
{
  return ambientLight_.getColor();
}

void
World::printStats() const
{
  std::cout << "World data:" << std::endl;
  std::cout << " primitives: " << primitives_.size() << std::endl;
  std::cout << " lights: " << lights_.size() << std::endl;
}
