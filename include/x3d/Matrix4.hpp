#ifndef X3D_MATRIX4_HPP_
#define X3D_MATRIX4_HPP_

#include <cmath>

#include "Vector3.hpp"

namespace x3d {
/*
  X3D MATRIX CLASS
    There really only is a pretty specialized 4x4 sized matrix needed, so it's
    a lot faster and easier to hardcode this.

    -> row | V column
  Access layout
    x1 y1 z1  x
    x2 y2 z2  y
    x3 y3 z3  z
     0  0  0  1
  Storage layout
    x1 x2 x3 0 y1 y2 y3 0 z1 z2 z3 0 x y z 1
*/
  
  template<class T> class Matrix4;
  
  typedef Matrix4<float> Matrix4f;
  typedef Matrix4<double> Matrix4d;

  template<class T = float> class Matrix4 {
  public:
    typedef T value_type;
  
    // constructors
    Matrix4() {}
    Matrix4(const Matrix4<T>& m) { *this = m; }

    // accessors
    inline T& at(unsigned short column, unsigned short row) {
      // Stored in column-major
      return m_fields[row + column * 4];
    }

    inline T& at(unsigned short i) {
      return m_fields[i];
    }

    // - const
    inline const T& at(unsigned short column, unsigned short row) const {
      return m_fields[row + column * 4];
    }

    inline const T& at(unsigned short i) const {
      return m_fields[i];
    }
    // axis-accessors
    inline x3d::Vector3<T>& x_axis() {
      return *reinterpret_cast<x3d::Vector3<T>*>(&at(0, 0));
    }

    inline x3d::Vector3<T>& y_axis() {
      return *reinterpret_cast<x3d::Vector3<T>*>(&at(1, 0));
    }

    inline x3d::Vector3<T>& z_axis() {
      return *reinterpret_cast<x3d::Vector3<T>*>(&at(2, 0));
    }

    inline x3d::Vector3<T>& translation() {
      return *reinterpret_cast<x3d::Vector3<T>*>(&at(3, 0)); 
    }
    // const
    inline const x3d::Vector3<T>& x_axis() const {
      return *reinterpret_cast<const x3d::Vector3<T>*>(&at(0, 0));
    }

    inline const x3d::Vector3<T>& y_axis() const {
      return *reinterpret_cast<const x3d::Vector3<T>*>(&at(1, 0));
    }

    inline const x3d::Vector3<T>& z_axis() const {
      return *reinterpret_cast<const x3d::Vector3<T>*>(&at(2, 0));
    }

    inline const x3d::Vector3<T>& translation() const {
      return *reinterpret_cast<const x3d::Vector3<T>*>(&at(3, 0)); 
    }

    // arithmetic operators
    Matrix4<T> operator*(const Matrix4<T>& rhs) const {
      x3d::Matrix4<T> m;
      for (unsigned short r = 0; r < 4; r ++) {
        for (unsigned short c = 0; c < 4; c ++) {
          T value = 0;
          for (unsigned short k = 0; k < 4; k ++)
            value += at(r, k) * rhs.at(k, c);
          m.at(r, c) = value;
        }
      }
      return m;
    }

    Vector3<T> transform_vector(const Vector3<T>& v) const {
      Vector3<T> nv;
      //     x comp.      y comp.            z comp.
      nv.x = x_axis().x * v.x + y_axis().x * v.y + z_axis().x * v.z;
      nv.y = x_axis().y * v.x + y_axis().y * v.y + z_axis().y * v.z;
      nv.z = x_axis().z * v.x + y_axis().z * v.y + z_axis().z * v.z;

      // translation
      nv += translation();
      return nv;
    }

    inline Vector3<T> operator*(const Vector3<T>& v) const {
      return transform_vector(v);
    }

    Matrix4<T>& operator*=(const Matrix4<T>& m) {
      *this = *this * m;
      return *this;
    }

    // copy matrix
    Matrix4<T>& operator=(const Matrix4<T>& m) {
      for (unsigned short i = 0; i < 16; i ++)
        at(i) = m.at(i);
      return *this;
    }

    // set entire matrix to a value
    Matrix4<T>& operator=(T s) {
      for (unsigned short i = 0; i < 16; i ++)
        at(i) = s;
      return *this;
    }

    void transpose() {
      Matrix4<T> m = *this;
      for (short i = 0; i < 4; i ++)
        for (short j = 0; j < 4; j ++)
          at(j, i) = m.at(i, j);
    }

    x3d::Matrix4<T> get_transpose() const {
      x3d::Matrix4<T> m = *this;
      m.transpose();
      return m;
    }

    // matrix setup functions


    // zeros the matrix
    inline void set_zero() {
      *this = 0;
    }

    // default matrix layouts
/*
    IDENTITY MATRIX
      Sets the matrix to an identity matrix
    Layout
      | 1 0 0 0 |
      | 0 1 0 0 |
      | 0 0 1 0 |
      | 0 0 0 1 |
*/
    x3d::Matrix4<T>& set_identity() {
      set_zero();
      at(0, 0) = at(1, 1) = at(2, 2) = at(3, 3) = 1;
      return *this;
    }
/*
    TRANSLATION MATRIX
      Sets the matrix to a translation matrix of the given position vector
    Layout
      | 1 0 0 x |
      | 0 1 0 y |
      | 0 0 1 z |
      | 0 0 0 1 |
*/
    x3d::Matrix4<T>& set_translate(const x3d::Vector3<T>& v) {
      set_identity();
      translation() = v;
      return *this;
    }

/*
    SCALE MATRIX
      Sets the matrix to a scale matrix of the given vector
    Layout
      | x 0 0 0 |
      | 0 y 0 0 |
      | 0 0 z 0 |
      | 0 0 0 1 |
*/
    x3d::Matrix4<T>& set_scale(const x3d::Vector3<T>& v) {
      set_identity();
      at(0, 0) = v.x;
      at(1, 1) = v.y;
      at(2, 2) = v.z;
      return *this;
    }

/*
    ROTATION MATRIX X
      Sets the matrix to a rotation matrix around the x axis
    Layout
      | 1    0     0   0 |
      | 0  cos a sin a 0 |
      | 0 -sin a cos a 0 |
      | 0    0     0   1 |
*/
    x3d::Matrix4<T>& set_rotation_x(T angle) {
      T cosa = x3d::Math<T>::cos(angle);
      T sina = x3d::Math<T>::sin(angle);

      set_identity();
      at(5) = cosa;
      at(6) = -sina;
      at(9) = sina;
      at(10) = cosa;
      return *this;
    }

/*
    ROTATION MATRIX Y
      Sets the matrix to a rotation matrix around the y axis
    Layout
      | cos a 0 -sin a  0 |
      |   0   1    0    0 |
      | sin a 0  cos a  0 |
      |   0   0    0    1 |
*/
    x3d::Matrix4<T>& set_rotation_y(T angle) {
      T cosa = x3d::Math<T>::cos(angle);
      T sina = x3d::Math<T>::sin(angle);

      set_identity();
      at(0) = cosa;
      at(2) = sina;
      at(8) = -sina;
      at(10) = cosa;
      return *this;
    }

/*
    ROTATION MATRIX Z
      Sets the matrix to a rotation matrix around the z axis
    Layout
      |  cos a sin a 0 0 |
      | -sin a cos a 0 0 |
      |    0     0   1 0 |
      |    0     0   0 1 |
*/
    x3d::Matrix4<T>& set_rotation_z(T angle) {
      T cosa = x3d::Math<T>::cos(angle);
      T sina = x3d::Math<T>::sin(angle);

      set_identity();
      at(0) = cosa;
      at(1) = -sina;
      at(4) = sina;
      at(5) = cosa;
      return *this;
    }

/*
    ROTATION MATRIX
      Sets the matrix to a rotation matrix around a vector (all three axes)
*/
    x3d::Matrix4<T>& set_rotation(x3d::Vector3<T> v, T angle) {
      v.normalize();
      T c = x3d::Math<T>::cos(angle);
      T co = 1 - c;
      T s = x3d::Math<T>::sin(angle);
      
      set_identity();

      at(0, 0) = v.x * v.x * co + c;
      at(0, 1) = v.y * v.x * co + v.z * s;
      at(0, 2) = v.x * v.z * co - v.y * s;
      at(1, 0) = v.x * v.y * co - v.z * s;
      at(1, 1) = v.y * v.y * co + c;
      at(1, 2) = v.y * v.z * co + v.x * s;
      at(2, 0) = v.x * v.z * co + v.y * s;
      at(2, 1) = v.y * v.z * co - v.x * s;
      at(2, 2) = v.z * v.z * co + c;
      return *this;
    }

    std::string serialize() {
      boost::format frm = boost::format(
"\
|%+5f %+5f %+5f %+5f|\n\
|%+5f %+5f %+5f %+5f|\n\
|%+5f %+5f %+5f %+5f|\n\
|%+5f %+5f %+5f %+5f|\n\
");
      frm % at(0, 0) % at(1, 0) % at(2, 0) % at(3, 0);
      frm % at(0, 1) % at(1, 1) % at(2, 1) % at(3, 1);
      frm % at(0, 2) % at(1, 2) % at(2, 2) % at(3, 2);
      frm % at(0, 3) % at(1, 3) % at(2, 3) % at(3, 3);
      return frm.str();
    }
    // Automatic type conversion when required
    operator T*() { return &at(0, 0); }
  private:
    T m_fields[4 * 4];
  };
}

#endif
