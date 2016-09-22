/*
 * View.hpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 *      Modified: sidch
 */

#ifndef __View_hpp__
#define __View_hpp__

#include "Globals.hpp"

/** View holds all the information about our camera. This includes how to sample across the viewport. */
class View
{
  public:
    /** Constructor. */
    View(Vec3 const & eye, Vec3 const & ll, Vec3 const & ul, Vec3 const & lr, Vec3 const & ur,
         int pixels_wide, int pixels_high, int rays_per_pixel_edge);

    /** Destructor. */
    virtual ~View();

    /** Get the width of the viewport. */
    int width() const;

    /** Get the height of the viewport. */
    int height() const;

    /** Get the number of rays per pixel. */
    int raysPerPixel() const;

    /** Get the number of rays per pixel. */
    int raysPerPixelEdge() const;

    /**
     * Get a sampled point from the viewport, corresponding to the \a ray_index 'th ray passing through the pixel region
     * (\a pixel_x, \a pixel_y).
     *
     * @param pixel_x The x coordinate (column) of the pixel.
     * @param pixel_y The y coordinate (row) of the pixel.
     * @param ray_index The index of the ray through the pixel, in the range [0, raysPerPixel() - 1].
     * @param s Used to return the sampled point.
     */
    void getSample(int pixel_x, int pixel_y, int ray_index, Sample & s) const;

    /** Get the world-space point corresponding to a given sample. */
    Vec3 getSamplePosition(Sample const & s) const;

    /** Get a ray from the eye to the given sample p. */
    Ray createViewingRay(Sample const & s) const;

    /** Constructs a normalized vector pointing from the given position to the camera. */
    Vec3 getViewVector(Vec3 const & position) const;

  private:
    Vec3 eye_;
    Vec3 LL_;
    Vec3 UL_;
    Vec3 LR_;
    Vec3 UR_;

    int pixels_wide_;
    int pixels_high_;
    int rays_per_pixel_edge_;
};

#endif // __View_hpp__
