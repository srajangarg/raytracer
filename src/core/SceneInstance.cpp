/*
 * SceneInstance.cpp
 *
 *  Created on: Feb 12, 2009
 *      Author: jima
 *      Modified: sidch
 */

#include "SceneInstance.hpp"
#include "SceneGroup.hpp"

SceneInstance::SceneInstance()
{
  name_ = "unassigned";
  color_ = NULL;
  lod_ = NULL;
  child_ = NULL;
}

SceneInstance::~SceneInstance()
{
  delete color_;
  delete lod_;

  for (vector<Transform *>::iterator it = transforms_.begin(); it != transforms_.end(); ++it)
    delete *it;
}

SceneGroup * SceneInstance::getChild()
{
  return child_;
}


string SceneInstance::getName()
{
  return name_;
}

bool SceneInstance::computeColor(RGB & color, int time)
{
  if (color_ == NULL)
    return false;
  else
    color = color_->getColor(time);

  return true;
}

bool SceneInstance::computeLOD(int & lod, int time)
{
  if (lod_ == NULL)
    return false;
  else
    lod = lod_->getLod(time);

  return true;
}

bool SceneInstance::computeTransform(Mat4 & mat, int time)
{
  mat = identity3D();

  if (transforms_.empty())
    return false;
  else
  {
    for (vector<Transform *>::reverse_iterator it = transforms_.rbegin(); it != transforms_.rend(); ++it)
    {
      mat = mat * (**it).getMatrix(time);
    }
  }

  return true;
}
