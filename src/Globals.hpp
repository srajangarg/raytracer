/*
 * Globals.hpp
 *
 *  Created on: Feb 2, 2009
 *      Author: njoubert
 *      Modified: sidch
 */

#ifndef __Globals_hpp__
#define __Globals_hpp__

#define IMPLEMENT_ME(file, line) std::cout << "METHOD NEEDS TO BE IMPLEMENTED AT " << file << ":" << line << std::endl; \
                                 std::exit(1);

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "core/Algebra3.hpp"
#include "core/Types.hpp"

static int const IMAGE_WIDTH = 512;
static int const IMAGE_HEIGHT = 512;
static int const RAYS_PER_PIXEL_EDGE = 2;

#endif  // __Globals_hpp__
