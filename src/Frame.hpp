/*
 * Frame.hpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 *      Modified: sidch
 */

#ifndef __Frame_hpp__
#define __Frame_hpp__

#include "Globals.hpp"
#include "core/Image.hpp"

/** Representation of the viewing plane on which the image is formed. */
class Frame
{
  public:
    /** Constructor. */
    Frame(int w, int h);

    /** Destructor. */
    virtual ~Frame();

    /** Set the color at a sample point. */
    void setColor(Sample const & s, RGB c);
    void save(std::string const & path);

  private:
    Image image;
};

#endif  // __Frame_hpp__
