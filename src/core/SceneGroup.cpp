/*
 * SceneGroup.cpp
 *
 *  Created on: Feb 12, 2009
 *      Author: jima
 *      Modified: sidch
 */

#include "SceneInstance.hpp"
#include "SceneGroup.hpp"

SceneGroup::SceneGroup()
{
  name_ = "unassigned";
  sphere_ = NULL;
  light_ = NULL;
  camera_ = NULL;
  meshMaterial_ = NULL;
  mesh_ = NULL;
}

// don't delete children, just local stuff; freeing children is SceneLoader's job
// (because cleaning up after a DAG with only local info is not good)
SceneGroup::~SceneGroup()
{
  delete sphere_;
  delete light_;
  delete camera_;
  delete mesh_;
}

int
SceneGroup::getChildCount()
{
  return int(children_.size());
}

SceneInstance *
SceneGroup::getChild(int i)
{
  return children_[i];
}

std::string
SceneGroup::getName()
{
  return name_;
}

bool
SceneGroup::computeMesh(TriangleMesh *& mesh, MaterialInfo & material, int time)  /* get a Mesh and it's material */
{
  if (mesh_ == NULL || meshMaterial_ == NULL)
    return false;

  mesh = mesh_;
  material = meshMaterial_->getMaterial(time);
  return true;
}

bool
SceneGroup::computeSphere(double & radius, MaterialInfo & material, int time)  /* get a sphere */
{
  if (sphere_ == NULL)
    return false;

  radius = sphere_->getRadius(time);
  material = sphere_->getMaterial(time);
  return true;
}

bool
SceneGroup::computeLight(LightInfo & ld, int time)  /* get light parameters */
{
  if (light_ == NULL)
    return false;
  else
    ld = light_->getLight(time);

  return true;
}

bool
SceneGroup::computeCamera(CameraInfo & cam, int time)  /* get camera frustum */
{
  if (camera_ == NULL)
    return false;
  else
    cam = camera_->getCamera(time);

  return true;
}
