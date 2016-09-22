/*
 * Types.hpp
 *
 *  Created on: Feb 2, 2009
 *      Author: njoubert
 *      Modified: sidch
 */

#ifndef __Types_hpp__
#define __Types_hpp__

#include "Algebra3.hpp"
#include <cmath>
#include <iostream>
#include <limits>

/** Colors. */
enum
{
  RED, GREEN, BLUE
};

/** Phong coefficients. */
enum
{
  MA,     // ambient
  ML,     // lambertian reflection
  MS,     // specular reflection
  MSP,    // specular reflection phong exponent, the "roughness"
  MSM,    // metallicity,
  MR,     // reflectivity for bounce rays
  MT,     // transparency for refracted rays
  MTN,    // refractive index of the material (w.r.t. vacuum)
};

/****************************************************************
 *                                                              *
 *             Ray                                              *
 *                                                              *
 ****************************************************************/

class Ray
{
  private:

    Vec3 e_;
    Vec3 d_;
    double min_t_;

    Ray(Vec3 const & start, Vec3 const & dir, double min_t);

  public:

    Ray();
    Ray(Ray const & r);

    static Ray fromOriginAndDirection(Vec3 const & start, Vec3 const & dir,
                                      double min_t = std::numeric_limits<double>::infinity());
    static Ray fromOriginAndEnd(Vec3 const & start, Vec3 const & end,
                                double min_t = std::numeric_limits<double>::infinity());

    // Special functions

    void transform(Mat4 const & xf);
    Vec3 getPos(double t) const;

    // Accessor functions

    Vec3 const & start() const { return e_; }
    Vec3 const & direction() const { return d_; }
    double minT() const { return min_t_; }

    // Setter functions

    void setMinT(double t);
};

/****************************************************************
 *                                                              *
 *             Sample in 3D space with (u,v) and (i,j) coords   *
 *                                                              *
 ****************************************************************/

class Sample
{
  protected:

    double x_;
    double y_;

  public:

    Sample();
    Sample(double x, double y);
    Sample(Sample const & v);

    // Assignment and Accessor operators

    double x() const;
    double y() const;

    void setX(double new_x);
    void setY(double new_y);
};

/****************************************************************
 *                                                              *
 *             Color                                            *
 *                                                              *
 ****************************************************************/

class RGB
{
  protected:

    double c_[3];

    static double linearScale(double in_low, double in_high, double out_low, double out_high, double x);

  public:

    // Constructors

    RGB();
    RGB(double r, double g, double b);
    RGB(RGB const & c);
    RGB(RGB const & a, RGB const & b, double t); //Linearly interpolated color

    // Assignment operators

    RGB & operator = (RGB const & v); // assignment of a Vec3
    RGB & operator +=(RGB const & v); // incrementation by a Vec3
    RGB & operator -=(RGB const & v); // decrementation by a Vec3
    RGB & operator *=(double d); // multiplication by a constant
    RGB & operator /=(double d); // division by a constant
    double & operator[](int i); // indexing
    double operator[](int i) const; // read-only indexing

    // Special functions

    // Clips all the values to the range [min, max]. Usually min = 0, max = 1
    void clip(double min_val, double max_val);

    // Clips all the values to this max value. Usually max = 1
    void clip(double max_val);

    // scales all the values proportionally so that the largest value is equal to resultingMax
    void scaleToMax(double resultingMax);

    // Used to return BMP compatible values in the range [0..255], assuming input is in the range [min..max]
    unsigned char getBMPR(double min, double max) const;
    unsigned char getBMPG(double min, double max) const;
    unsigned char getBMPB(double min, double max) const;

    // Friends

    friend RGB operator -(RGB const & v); // -v1
    friend RGB operator +(RGB const & a, RGB const & b); // v1 + v2
    friend RGB operator -(RGB const & a, RGB const & b); // v1 - v2
    friend RGB operator *(RGB const & a, double d); // v1 * 3.0
    friend RGB operator *(double d, RGB const & a); // 3.0 * v1
    friend RGB operator *(RGB const & a, RGB const & b); // pointwise mult
    friend RGB operator /(RGB const & a, double d); // v1 / 3.0
    friend bool operator ==(RGB const & a, RGB const & b); // v1 == v2 ?
    friend bool operator !=(RGB const & a, RGB const & b); // v1 != v2 ?

    friend std::ostream & operator <<(std::ostream & s, RGB const & v); // output to stream
};

/****************************************************************
 *                                                              *
 *             Material                                         *
 *                                                              *
 ****************************************************************/

class Material
{
  protected:

    float p_[8]; //Storage for the 5 phong exponents

  public:

    // Constructors

    Material();
    Material(float ma, float ml, float ms, float msp, float msm,
             float mr, float mt, float mtn);
    Material(Material const & m);


    // Special functions

    double getMA() const;
    double getML() const;
    double getMS() const;
    double getMSP() const;
    double getMSM() const;
    double getMR() const;
    double getMT() const;
    double getMTN() const;
};


/****************************************************************
 *                                                              *
 *          Ray Member functions                                *
 *                                                              *
 ****************************************************************/

inline Ray::Ray()
{
}

inline Ray::Ray(Vec3 const & start, Vec3 const & dir, double min_t)
{
  e_ = start;
  d_ = dir;
  min_t_ = min_t;
}

inline Ray::Ray(Ray const & r)
{
  e_ = r.e_;
  d_ = r.d_;
  min_t_ = r.min_t_;
}

inline Ray Ray::fromOriginAndDirection(Vec3 const & start, Vec3 const & dir, double min_t)
{
  return Ray(start, dir, min_t);
}

inline Ray Ray::fromOriginAndEnd(Vec3 const & start, Vec3 const & end, double min_t)
{
  return Ray(start, end - start, min_t);
}

// Special functions

inline void Ray::transform(Mat4 const & xf)
{
  e_ = Vec3(xf * Vec4(e_, 1.0));     // divide out by the homogeneous coordinate
  d_ = Vec3(xf * Vec4(d_, 0.0), 3);  // drop the homogeneous coordinate instead of dividing by zero
}

inline Vec3 Ray::getPos(double t) const
{
  Vec3 p = e_ + t * d_;
  return p;
}

inline void Ray::setMinT(double t)
{
  min_t_ = t;
}


/****************************************************************
 *                                                              *
 *          Sample Member functions                              *
 *                                                              *
 ****************************************************************/

inline Sample::Sample()
{
  x_ = 0;
  y_ = 0;
}

inline Sample::Sample(double x, double y)
{
  x_ = x;
  y_ = y;
}

inline Sample::Sample(Sample const & v)
{
  x_ = v.x_;
  y_ = v.y_;
}

// Accessor Functions

inline double Sample::x() const
{
  return x_;
}

inline double Sample::y() const
{
  return y_;
}

inline void Sample::setX(double new_x)
{
  x_ = new_x;
}

inline void Sample::setY(double new_y)
{
  y_ = new_y;
}

/****************************************************************
 *                                                              *
 *                   Color Member Functions                     *
 *                                                              *
 ****************************************************************/

inline RGB::RGB()
{
  c_[RED] = c_[GREEN] = c_[BLUE] = 0.0;
}

inline RGB::RGB(double r, double g, double b)
{
  c_[RED] = r;
  c_[GREEN] = g;
  c_[BLUE] = b;
}

inline RGB::RGB(RGB const & c)
{
  c_[RED] = c.c_[RED];
  c_[GREEN] = c.c_[GREEN];
  c_[BLUE] = c.c_[BLUE];
}

inline RGB::RGB(RGB const & a, RGB const & b, double t)
{
  c_[RED] =   (1. - t) * a.c_[RED] +    t * b.c_[RED];
  c_[GREEN] = (1. - t) * a.c_[GREEN] +  t * b.c_[GREEN];
  c_[BLUE] =  (1. - t) * a.c_[BLUE] +   t * b.c_[BLUE];
}

// Assignment operators

inline RGB & RGB::operator =(RGB const & c)
{
  c_[RED] = c.c_[RED];
  c_[GREEN] = c.c_[GREEN];
  c_[BLUE] = c.c_[BLUE];
  return *this;
}

inline RGB & RGB::operator +=(RGB const & v)
{
  c_[RED] += v.c_[RED];
  c_[GREEN] += v.c_[GREEN];
  c_[BLUE] += v.c_[BLUE];
  return *this;
}

inline RGB & RGB::operator -=(RGB const & v)
{
  c_[RED] -= v.c_[RED];
  c_[GREEN] -= v.c_[GREEN];
  c_[BLUE] -= v.c_[BLUE];
  return *this;
}

inline RGB & RGB::operator *=(double d)
{
  c_[RED] *= d;
  c_[GREEN] *= d;
  c_[BLUE] *= d;
  return *this;
}

inline RGB & RGB::operator /=(double d)
{
  double d_inv = 1. / d;
  c_[RED] *= d_inv;
  c_[GREEN] *= d_inv;
  c_[BLUE] *= d_inv;
  return *this;
}

inline double & RGB::operator [](int i)
{
  assert(! (i < RED || i > BLUE));
  return c_[i];
}

inline double RGB::operator [](int i) const
{
  assert(! (i < RED || i > BLUE));
  return c_[i];
}

// Special functions

inline void RGB::clip(double min_val, double max_val)
{
  c_[RED]    =  std::min(std::max(c_[RED],    min_val), max_val);
  c_[GREEN]  =  std::min(std::max(c_[GREEN],  min_val), max_val);
  c_[BLUE]   =  std::min(std::max(c_[BLUE],   min_val), max_val);
}

inline void RGB::clip(double max_val)
{
  if (c_[RED] > max_val)
    c_[RED] = max_val;

  if (c_[GREEN] > max_val)
    c_[GREEN] = max_val;

  if (c_[BLUE] > max_val)
    c_[BLUE] = max_val;
}

inline void RGB::scaleToMax(double resultingMax)
{
  double maxValue = std::max(c_[RED], std::max(c_[GREEN], c_[BLUE]));
  c_[RED] = linearScale(0, maxValue, 0, resultingMax, c_[RED]);
  c_[GREEN] = linearScale(0, maxValue, 0, resultingMax, c_[GREEN]);
  c_[BLUE] = linearScale(0, maxValue, 0, resultingMax, c_[BLUE]);
}

inline unsigned char RGB::getBMPR(double min, double max) const
{
  return (unsigned char) linearScale(min, max, 0, 255, c_[RED]);
}

inline unsigned char RGB::getBMPG(double min, double max) const
{
  return (unsigned char) linearScale(min, max, 0, 255, c_[GREEN]);
}

inline unsigned char RGB::getBMPB(double min, double max) const
{
  return (unsigned char) linearScale(min, max, 0, 255, c_[BLUE]);
}

inline double RGB::linearScale(double in_low, double in_high, double out_low, double out_high, double x)
{
  return (x - in_low) / (in_high - in_low) * (out_high - out_low) + out_low;
}

// FRIENDS

inline RGB operator -(RGB const & a)
{
  return RGB(-a.c_[RED], -a.c_[GREEN], -a.c_[BLUE]);
}

inline RGB operator +(RGB const & a, RGB const & b)
{
  return RGB(a.c_[RED] + b.c_[RED], a.c_[GREEN] + b.c_[GREEN], a.c_[BLUE] + b.c_[BLUE]);
}

inline RGB operator -(RGB const & a, RGB const & b)
{
  return RGB(a.c_[RED] - b.c_[RED], a.c_[GREEN] - b.c_[GREEN], a.c_[BLUE] - b.c_[BLUE]);
}

inline RGB operator *(RGB const & a, double d)
{
  return RGB(d * a.c_[RED], d * a.c_[GREEN], d * a.c_[BLUE]);
}

inline RGB operator *(double d, RGB const & a)
{
  return a * d;
}

inline RGB operator *(RGB const & a, RGB const & b)
{
  return RGB(a.c_[RED] * b.c_[RED],
             a.c_[GREEN] * b.c_[GREEN],
             a.c_[BLUE] * b.c_[BLUE]);
}
inline RGB operator /(RGB const & a, double d)
{
  double d_inv = 1. / d;
  return RGB(a.c_[RED] * d_inv, a.c_[GREEN] * d_inv, a.c_[BLUE] * d_inv);
}

inline bool operator ==(RGB const & a, RGB const & b)
{
  return (a.c_[RED] == b.c_[RED]) && (a.c_[GREEN] == b.c_[GREEN]) && (a.c_[BLUE] == b.c_[BLUE]);
}

inline bool operator !=(RGB const & a, RGB const & b)
{
  return !(a == b);
}

// IO

inline std::ostream & operator <<(std::ostream & s, RGB const & v)
{
  return s << "{ r=" << v.c_[RED] << " g=" << v.c_[GREEN] << " b=" << v.c_[BLUE] << " }";
}


/****************************************************************
 *                                                              *
 *                   Material Member Functions                  *
 *                                                              *
 ****************************************************************/


inline Material::Material()
{
}

inline Material::Material(float ma, float ml, float ms, float msp, float msm, float mr, float mt, float mtn)
{
  p_[MA] = ma;
  p_[ML] = ml;
  p_[MS] = ms;
  p_[MSP] = msp;
  p_[MSM] = msm;
  p_[MR] = mr;
  p_[MT] = mt;
  p_[MTN] = mtn;
}

inline Material::Material(Material const & m)
{
  p_[MA] = m.p_[MA];
  p_[ML] = m.p_[ML];
  p_[MS] = m.p_[MS];
  p_[MSP] = m.p_[MSP];
  p_[MSM] = m.p_[MSM];
  p_[MR] = m.p_[MR];
  p_[MT] = m.p_[MT];
  p_[MTN] = m.p_[MTN];
}


// Accessor functions

inline double Material::getMA() const
{
  return p_[MA];
}

inline double Material::getML() const
{
  return p_[ML];
}

inline double Material::getMS() const
{
  return p_[MS];
}

inline double Material::getMSP() const
{
  return p_[MSP];
}

inline double Material::getMSM() const
{
  return p_[MSM];
}

inline double Material::getMR() const
{
  return p_[MR];
}

inline double Material::getMT() const
{
  return p_[MT];
}

inline double Material::getMTN() const
{
  return p_[MTN];
}

#endif  // __Types_hpp__
