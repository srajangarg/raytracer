/*
 * SceneInstance.hpp
 *
 *  Created on: Feb 12, 2009
 *      Author: jima
 *      Modified: sidch
 */

#ifndef __SceneInstance_hpp__
#define __SceneInstance_hpp__

#include "SceneData.hpp"
#include <vector>

using namespace std;

class SceneInstance
{
  public:
    string getName(); /* get the instance's name; useful for debugging */

    bool computeColor(RGB & color, int time = 0); /* get the instance's color; returns false if no color specified */
    bool computeTransform(Mat4 & mat, int time = 0); /* get the instances's transform; returns false if no transform specified */
    bool computeLOD(int & lod, int time = 0); /* get the instances's LOD, returns false if no LOD specified */

    class SceneGroup * getChild(); /* get the group which is a child of this instance */

    virtual ~SceneInstance();

  private:
    SceneInstance(); // private constructor to be called by SceneLoader only

    // private copy constructor and assignment operator to avoid copying this data
    SceneInstance(const SceneInstance &);
    SceneInstance & operator=(const SceneInstance &);

    string name_;

    vector<Transform *> transforms_;
    ParametricColor * color_;
    LOD * lod_;

    SceneGroup * child_;

    friend class SceneLoader;
};

#endif // __SceneInstance_hpp__
