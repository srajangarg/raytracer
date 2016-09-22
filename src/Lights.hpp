/*
 * Lights.hpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 *      Modified: sidch
 */

#ifndef __Lights_hpp__
#define __Lights_hpp__

#include "Globals.hpp"

/** Interface for a light. */
class Light
{
  protected:
    Light();
    Light(RGB const & illumination);
    Light(RGB const & illumination, double falloff, double dead_distance);
    RGB illumination_;
    double falloff_;
    double angular_falloff_;
    double dead_distance_;

  public:
    /** Destructor. */
    virtual ~Light();

    /** Set the color of the light. */
    void setColor(RGB const & c);

    /** Get the color of the light at the source. */
    virtual RGB getColor() const;

    /** Get the color of the light at a point in the scene (after falloff etc). */
    virtual RGB getColor(Vec3 const & p) const;

    /**
     * Returns the normalized vector describing the direction of light at a given position. The direction should be FROM the
     * point TO the light source. This is used in the shading calculation.
     */
    virtual Vec3 getIncidenceVector(Vec3 const & position) const = 0;

    /**
     * Get the ray from the given position to the light, used to check for shadows. The length of the ray's direction vector
     * should be the <i>unnormalized distance</i> from \a position to the light, so that hit times <= 1 indicate shadowing.
     * \a use_dist is normally set to true. For direction lights, which have no source position, the length of the direction
     * vector is arbitrary, and use_dist is set to false, indicating that the distance (i.e. time) check should be ignored --
     * all positive hit times indicate shadowing.
     */
    virtual Ray getShadowRay(Vec3 const & position, bool & use_dist) const = 0;
};

/** Ambient light, constant throughout the scene. */
class AmbientLight : public Light
{
  public:
    AmbientLight();
    AmbientLight(RGB const & illumination);

    Vec3 getIncidenceVector(Vec3 const & position) const;
    Ray getShadowRay(Vec3 const & position, bool & use_dist) const;
};

/** Point light, with a fixed location in the scene. */
class PointLight : public Light
{
  public:
    PointLight(RGB const & illumination);
    PointLight(RGB const & illumination, double falloff, double dead_distance);
    void setPosition(Vec3 const & pos);

    RGB getColor(Vec3 const & p) const;
    Vec3 getIncidenceVector(Vec3 const & position) const;
    Ray getShadowRay(Vec3 const & position, bool & use_dist) const;

  private:
    Vec3 pos_;
};

/** Direction light, all rays are parallel. */
class DirectionalLight : public Light
{
  public:
    DirectionalLight(RGB const & illumination);
    void setDirection(Vec3 const & dir);

    Vec3 getIncidenceVector(Vec3 const & position) const;
    Ray getShadowRay(Vec3 const & position, bool & use_dist) const;

  private:
    Vec3 dir_;
};

#endif  // __Lights_hpp__
