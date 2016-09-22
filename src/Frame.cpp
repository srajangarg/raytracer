/*
 * Frame.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 *      Modified: sidch
 */

#include "Frame.hpp"

Frame::Frame(int w, int h)
: image(w, h, 3)
{
}

Frame::~Frame()
{
}

void
Frame::setColor(Sample const & s, RGB c)
{
  c.clip(0, 1);

  int xi = (int)std::floor(s.x() * image.width());
  int yi = (int)std::floor((1 - s.y()) * image.height());

  unsigned char * pixel = image.pixel(yi, xi);
  pixel[0] = c.getBMPR(0, 1);
  pixel[1] = c.getBMPG(0, 1);
  pixel[2] = c.getBMPB(0, 1);
}

void
Frame::save(std::string const & path)
{
  if (!image.save(path))
    std::cerr << "Could not save frame to " << path << std::endl;
}
