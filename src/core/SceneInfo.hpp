/*
 * SceneInfo.hpp
 *
 * These basic holder structs are used to return data to you
 *
 *  Created on: Feb 19, 2009
 *      Author: jima
 *      Modified: sidch
 */

#ifndef __SceneInfo_hpp__
#define __SceneInfo_hpp__

#include "Algebra3.hpp"
#include "Types.hpp"

/** Phong coefficients. */
enum
{
  MAT_MA,     //ambient
  MAT_MD,     //diffuse reflection
  MAT_MS,     //specular reflection
  MAT_MSP,    //specular reflection phong exponent, the "roughness"
  MAT_MSM,    //metalicity,
  MAT_MT,     //transparency for refracted rays
  MAT_MTN,    //refractive index of the material
  MAT_NUM_COEFFICIENTS
};

/** Frustum sides. */
enum
{
  FRUS_LEFT, FRUS_RIGHT, FRUS_BOTTOM, FRUS_TOP, FRUS_NEAR, FRUS_FAR
};

/** Light types. */
enum
{
  LIGHT_AMBIENT, LIGHT_DIRECTIONAL, LIGHT_POINT, LIGHT_SPOT
};

/** Material coefficients. */
struct MaterialInfo
{
  double k[MAT_NUM_COEFFICIENTS];  // storage for the material coefficients
  RGB color;

  MaterialInfo() {}

  MaterialInfo(RGB color, const double ka, const double kd, const double ks, const double ksp, const double ksm, const double kt, const double ktn)
  : color(color)
  {
    k[MAT_MA] = ka;
    k[MAT_MD] = kd;
    k[MAT_MS] = ks;
    k[MAT_MSP] = ksp;
    k[MAT_MSM] = ksm;
    k[MAT_MT] = kt;
    k[MAT_MTN] = ktn;
  }
};

/** Light parameters. */
struct LightInfo
{
  int type;
  RGB color;
  double falloff;
  double angularFalloff;
  double deadDistance;

  LightInfo() {}

  LightInfo(int type, RGB color, double falloff = 2, double angularFalloff = 0.1, double deadDistance = 0.1)
    : type(type), color(color), falloff(falloff), angularFalloff(angularFalloff), deadDistance(deadDistance)
  {}
};

/** Camera parameters. */
struct CameraInfo
{
  bool perspective;
  double sides[6];

  CameraInfo() {}
  CameraInfo(bool perspective, double l, double r, double b, double t, double n = 1, double f = 100)
  : perspective(perspective)
  {
    sides[FRUS_LEFT] = l;
    sides[FRUS_RIGHT] = r;
    sides[FRUS_BOTTOM] = b;
    sides[FRUS_TOP] = t;
    sides[FRUS_NEAR] = n;
    sides[FRUS_FAR] = f;
  }
};

#endif  // __SceneInfo_hpp__
