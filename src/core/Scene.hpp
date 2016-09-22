/*
 * Scene.hpp
 *
 *  Created on: Feb 8, 2009
 *      Author: jima
 *      Modified: sidch
 */

#ifndef __Scene_hpp__
#define __Scene_hpp__

#include "SceneInstance.hpp"
#include "SceneGroup.hpp"

class SceneLoader;

/** The main scene class -- constructor calls the scene loader, getRoot gives a starting point for scene traversal. */
class Scene
{
  public:
    /** Construct the scene from a file. */
    Scene(std::string const & path);

    /** Destructor. */
    ~Scene();

    /** Get the root of the scene hierarchy, which is the starting point for a scene traversal. */
    SceneInstance const * getRoot() const { return root_; }

    /** Get the root of the scene hierarchy, which is the starting point for a scene traversal. */
    SceneInstance * getRoot() { return root_; }

    /** For debug purposes -- display the loaded structure. */
    void printScene();

  private:
    friend class SceneLoader;

    SceneInstance * root_;  ///< The starting point for traversing the scene DAG.
    SceneLoader * loader_;  ///< Loader handles the file reading bits.

    // Private copy constructor and assignment operator to avoid shallow copies
    Scene(Scene const &) {}
    Scene & operator=(Scene const &) { return *this; }
};

#endif  // __Scene_hpp__
