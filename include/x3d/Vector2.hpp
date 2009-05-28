#ifndef X3D_VECTOR2_HPP_
#define X3D_VECTOR2_HPP_

#include "Math.hpp"

namespace x3d {
  template<typename T> class Vector2;

  typedef Vector2<float> Vector2f;
  typedef Vector2<double> Vector2d;
  typedef Vector2<int> Vector2i;

  template<typename T> class Vector2 {
  public:
    typedef T value_type;
    T x, y;

    // Constructor
    Vector2() {}
    Vector2(const T a, const T b) { x = a; y = b; }
    Vector2(const Vector2<T> &v) { x = v.x; y = v.y; }

    // Set operators
    Vector2<T>& operator=(const T v) { x = y = v; return *this; }
    Vector2<T>& operator=(const Vector2<T> &v) { x = v.x; y = v.y; return *this;}
    Vector2<T>& operator-() { x = -x; y = -y; return *this; }

    // Vector & Scalar
    Vector2<T> operator+(const T s) const { return Vector2<T>(x+s, y+s); }
    Vector2<T> operator-(const T s) const { return Vector2<T>(x-s, y-s); }
    Vector2<T> operator*(const T s) const { return Vector2<T>(x*s, y*s); }
    Vector2<T> operator/(const T s) const { return Vector2<T>(x/s, y/s); }
    Vector2<T>& operator+=(const T s) { x += s; y += s; return *this; }
    Vector2<T>& operator-=(const T s) { x -= s; y -= s; return *this; }
    Vector2<T>& operator*=(const T s) { x *= s; y *= s; return *this; }
    Vector2<T>& operator/=(const T s) { x /= s; y /= s; return *this; }

    // Vector & Vector
    Vector2<T> operator+(const Vector2<T>& v) const { return Vector2<T>(x+v.x, y+v.y); }
    Vector2<T> operator-(const Vector2<T>& v) const { return Vector2<T>(x-v.x, y-v.y); }
    Vector2<T> operator*(const Vector2<T>& v) const { return Vector2<T>(x*v.x, y*v.y); }
    Vector2<T> operator/(const Vector2<T>& v) const { return Vector2<T>(x/v.x, y/v.y); }
    Vector2<T>& operator+=(const Vector2<T>& v) { x += v.x; y += v.y; return *this; }
    Vector2<T>& operator-=(const Vector2<T>& v) { x -= v.x; y -= v.y; return *this; }
    Vector2<T>& operator*=(const Vector2<T>& v) { x *= v.x; y *= v.y; return *this; }
    Vector2<T>& operator/=(const Vector2<T>& v) { x /= v.x; y /= v.y; return *this; }

    // Dot Product
    T operator|(const Vector2<T>& v) const { return x*v.x + y*v.y; }

    double magnitude_squared() const { return static_cast<double>(x*x + y*y); }
    double magnitude() const { return x3d::Math<double>::sqrt(magnitude_squared()); }

    Vector2<T>& normalize() {
      T length = magnitude();
      if (length == 0)
        return *this;
      return *this /= length;
    }

    // Automatic Type conversion/casting
    template<class T2> operator Vector2<T2>() const { return Vector2<T2>(static_cast<T>(x), static_cast<T>(y)); }
    operator T*() { return (T*)this; }
    template<class T2> Vector2(const Vector2<T2> &v) { x = static_cast<T>(v.x); y = static_cast<T>(v.y); }

    // TODO: stop using boost here perhaps?
    /*std::string serialize() {
      return (boost::format("%d | %d") % x % y).str();
    }*/
  };
  
  // TODO: create Size its own class (rename x/y to width/height)?
  typedef Vector2f Size2f;
  typedef Vector2d Size2d;
  typedef Vector2i Size2i;
}

#endif
