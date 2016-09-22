/*
 * Scene.cpp
 *
 *  Created on: Feb 8, 2009
 *      Author: jima
 *      Modified: sidch
 */

#include "Scene.hpp"
#include "SceneLoader.hpp"
#include <sstream>

void
tab(int level)
{
  while ((level--) > 0)
  {
    cout << "\t";
  }
}

void
printNode(SceneInstance * n, int level = 0)
{
  tab(level);
  std::cout << "Node " << n->getName() << " {" << std::endl;

  if (n->getChild())
  {
    SceneGroup * below = n->getChild();

    for (int i = 0; i < below->getChildCount(); i++)
    {
      printNode(below->getChild(i), level + 1);
    }
  }

  tab(level);
  std::cout << "}" << std::endl;
}

void Scene::printScene()
{
  printNode(getRoot());
}

Scene::Scene(string const & path)
{
  loader_ = new SceneLoader(*this, path);
  std::cout << "Loaded scene:" << std::endl;
  printScene();
}

Scene::~Scene()
{
  delete loader_;
}
