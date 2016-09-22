/*
 * SceneData.hpp
 *
 *  These classes are used internally to wrap ParametricValues
 *  The Scene 'Group' and 'Instance' classes will not return them to users of the class.
 *
 *  Created on: Feb 8, 2009
 *      Author: jima
 *      Modified: sidch
 */

#ifndef __SceneData_hpp__
#define __SceneData_hpp__

#include "ParametricValue.hpp"
#include "SceneInfo.hpp"
#include <vector>

/** Interface for a transformation varying over time. */
class Transform
{
  private:
    friend class SceneLoader;

  public:
    virtual Mat4 getMatrix(int time) = 0;  // pure virtual function
    virtual ~Transform() {}
};

/** A general time-varying transform. */
class GeneralTransform : public Transform
{
  private:
    std::vector<ParametricValue *> matrix;
    friend class SceneLoader;

  public:
    Mat4 getMatrix(int time)
    {
      Mat4 out(0.0);

      if (matrix.size() == 9) // 2d matrix
      {
        out[2][2] = 1;

        for (int i = 0; i < 3; i++)
        {
          int i_ind = i < 2 ? i : 3;

          for (int j = 0; j < 3; j++)
          {
            int j_ind = j < 2 ? j : 3;
            out[i_ind][j_ind] = matrix[i * 3 + j]->getValue(time);
          }
        }
      }
      else if (matrix.size() == 16)
      {
        for (int i = 0; i < 4; i++)
        {
          for (int j = 0; j < 4; j++)
          {
            out[i][j] = matrix[i * 4 + j]->getValue(time);
          }
        }
      }
      else
      {
        out = identity3D();
      }

      return out;
    }

    ~GeneralTransform()
    {
      for (std::vector<ParametricValue *>::iterator it = matrix.begin(); it != matrix.end(); ++it)
      {
        delete *it;
      }
    }
};

/** A time-varying translation. */
class Translate : public Transform
{
    ParametricValue * translate[3];
    friend class SceneLoader;

  public:
    Mat4 getMatrix(int time)
    {
      Vec3 tr;
      tr[0] = translate[0]->getValue(time);
      tr[1] = translate[1]->getValue(time);

      if (translate[2] == NULL)
        tr[2] = 0;
      else
        tr[2] = translate[2]->getValue(time);

      return translation3D(tr);
    }

    Translate()
    {
      for (int i = 0; i < 3; i++)
        translate[i] = NULL;
    }

    ~Translate()
    {
      for (int i = 0; i < 3; i++)
        delete translate[i];
    }
};

/** A time-varying scaling transform. */
class Scale : public Transform
{
  private:
    ParametricValue * scale[3];
    friend class SceneLoader;

  public:
    Mat4 getMatrix(int time)
    {
      Vec3 sc;
      sc[0] = scale[0]->getValue(time);
      sc[1] = scale[1]->getValue(time);

      if (scale[2] == NULL)
        sc[2] = 1;
      else
        sc[2] = scale[2]->getValue(time);

      return scaling3D(sc);
    }

    Scale()
    {
      for (int i = 0; i < 3; i++)
        scale[i] = NULL;
    }

    ~Scale()
    {
      for (int i = 0; i < 3; i++)
        delete scale[i];
    }
};

/** A time-varying rotation. */
class Rotate : public Transform
{
  private:
    ParametricValue * angle;
    ParametricValue * axis[3];
    friend class SceneLoader;

  public:
    Mat4 getMatrix(int time)
    {
      Vec3 ax;

      if (axis[2] == NULL) // 2D
      {
        ax = Vec3(0, 0, 1);
      }
      else
      {
        ax = Vec3(axis[0]->getValue(time),
                  axis[1]->getValue(time),
                  axis[2]->getValue(time));
      }

      return rotation3D(ax, angle->getValue(time));
    };

    Rotate()
    {
      angle = NULL;

      for (int i = 0; i < 3; i++)
        axis[i] = NULL;
    }

    ~Rotate()
    {
      delete angle;

      for (int i = 0; i < 3; i++)
        delete axis[i];
    }
};

/** A color varying over time. */
class ParametricColor
{
  private:
    ParametricValue * color_[3];
    friend class SceneLoader;

  public:
    RGB getColor(int time)
    {
      return RGB(color_[0]->getValue(time),
                 color_[1]->getValue(time),
                 color_[2]->getValue(time));
    }

    ParametricColor()
    {
      for (int i = 0; i < 3; i++)
        color_[i] = NULL;
    }

    ~ParametricColor()
    {
      for (int i = 0; i < 3; i++)
        delete color_[i];
    }
};

/** A time-varying LOD index. */
class LOD
{
  private:
    ParametricValue * level_;
    friend class SceneLoader;

  public:
    int getLod(int time)
    {
      return int(level_->getValue(time));
    }

    LOD() : level_(NULL) {}

    ~LOD()
    {
      delete level_;
    }
};

/** A time-varying material. */
class ParametricMaterial
{
  private:
    ParametricColor * RGB_;
    ParametricValue * coefficients_[7];
    friend class SceneLoader;

  public:
    MaterialInfo getMaterial(int time)
    {
      return MaterialInfo(RGB_->getColor(time),
                          coefficients_[0]->getValue(time),
                          coefficients_[1]->getValue(time),
                          coefficients_[2]->getValue(time),
                          coefficients_[3]->getValue(time),
                          coefficients_[4]->getValue(time),
                          coefficients_[5]->getValue(time),
                          coefficients_[6]->getValue(time));
    }
    ParametricMaterial() : RGB_(NULL)
    {
      for (int i = 0; i < 7; i++)
        coefficients_[i] = 0;
    }

    ~ParametricMaterial()
    {
      delete RGB_;

      for (int i = 0; i < 7; i++)
        delete coefficients_[i];
    }
};

/** A sphere with time-varying radius. */
class ParametricSphere
{
  private:
    ParametricValue * radius_;
    ParametricMaterial * material_;
    friend class SceneLoader;

  public:
    double getRadius(int time)
    {
      return radius_->getValue(time);
    }
    MaterialInfo getMaterial(int time)
    {
      return material_->getMaterial(time);
    }

    ParametricSphere() : radius_(NULL), material_(NULL) {}

    ~ParametricSphere()
    {
      delete radius_;
    }
};

/** A time-varying camera. */
class ParametricCamera
{
  private:
    ParametricValue * perspective_;
    ParametricValue * frustum_[6];
    friend class SceneLoader;

  public:
    CameraInfo getCamera(int time)
    {
      return CameraInfo(((int)perspective_->getValue(time) != 0),
                        frustum_[0]->getValue(time),
                        frustum_[1]->getValue(time),
                        frustum_[2]->getValue(time),
                        frustum_[3]->getValue(time),
                        frustum_[4]->getValue(time),
                        frustum_[5]->getValue(time));
    }

    ParametricCamera() : perspective_(NULL)
    {
      for (int i = 0; i < 6; i++)
      {
        frustum_[i] = NULL;
      }
    }

    ~ParametricCamera()
    {
      delete perspective_;

      for (int i = 0; i < 6; i++)
      {
        delete frustum_[i];
      }
    }
};

/** A time-varying light. */
class ParametricLight
{
  private:
    ParametricValue * type_;
    ParametricColor * color_;
    ParametricValue * falloff_;
    ParametricValue * angularFalloff_;
    ParametricValue * deadDistance_;
    friend class SceneLoader;

  public:
    ParametricLight() : type_(NULL), color_(NULL), falloff_(NULL), angularFalloff_(NULL), deadDistance_(NULL) {}
    ~ParametricLight()
    {
      delete type_;
      delete color_;
      delete falloff_;
      delete angularFalloff_;
      delete deadDistance_;
    }

    LightInfo getLight(int time)
    {
      return LightInfo((int)type_->getValue(time),
                       color_->getColor(time), falloff_->getValue(time),
                       angularFalloff_->getValue(time), deadDistance_->getValue(time));
    }
};

#endif  // __SceneData_hpp__
