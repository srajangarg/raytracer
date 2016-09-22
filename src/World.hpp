/*
 * World.hpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 *      Modified: sidch
 */

#ifndef __World_hpp__
#define __World_hpp__

#include "Globals.hpp"
#include "Lights.hpp"
#include "Primitives.hpp"

/** The World forms a container for lights and primitives in our scene. */
class World
{
  public:
    typedef std::vector<Primitive *>::const_iterator PrimitiveConstIterator;
    typedef std::vector<Light *>::const_iterator LightConstIterator;

    /** Constructor. */
    World();

    /** Destructor. */
    virtual ~World();

    /**
     * Find the intersection of a ray with the world. ray.minT() is set to the hit time of the nearest intersection point, if
     * any. Only hit times smaller than the original ray.minT() passed to the function are considered.
     */
    Primitive * intersect(Ray & r) const;

    /** Add a primitive to the world. */
    void addPrimitive(Primitive * p);

    /** Add a light to the world. */
    void addLight(Light * l);

    /** Set the color of the ambient light. */
    void setAmbientLightColor(RGB ambientColor);

    /** Get the color of the ambient light. */
    RGB getAmbientLightColor() const;

    /** Get the number of primitives. */
    int numPrimitives() const { return (int)primitives_.size(); }

    /** Get an iterator pointing to the first primitive. */
    PrimitiveConstIterator primitivesBegin() const { return primitives_.begin(); }

    /** Get an iterator pointing to the one position beyond the last primitive. */
    PrimitiveConstIterator primitivesEnd() const { return primitives_.end(); }

    /** Get the number of lights. */
    int numLights() const { return (int)lights_.size(); }

    /** Get an iterator pointing to the first light. */
    LightConstIterator lightsBegin() const { return lights_.begin(); }

    /** Get an iterator pointing to the one position beyond the last light. */
    LightConstIterator lightsEnd() const { return lights_.end(); }

    /** Print debugging stats. */
    void printStats() const;

  private:
    std::vector<Primitive *> primitives_;
    std::vector<Light *> lights_;
    AmbientLight ambientLight_;
};

#endif  // __World_hpp__
