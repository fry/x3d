#ifndef X3D_VECTOR3_HPP_
#define X3D_VECTOR3_HPP_

#include <boost/format.hpp>
#include <string>
#include "Math.hpp"

namespace x3d {
  template<class T> class Vector3;

  typedef Vector3<float> Vector3f;
  typedef Vector3<double> Vector3d;
  typedef Vector3<int> Vector3i;

  template<class T> class Vector3 {
  public:
    static const Vector3<T> Zero;
    static const Vector3<T> UnitX;
    static const Vector3<T> UnitY;
    static const Vector3<T> UnitZ;

    typedef T value_type;
    T x, y, z;

    // Constructor
    Vector3() {}
    Vector3(const T a) { *this = a; }
    Vector3(const T a, const T b, const T c) { x = a; y = b; z = c; }
    Vector3(const Vector3<T> &v) { *this = v; }

    // Set operators
    Vector3<T>& operator=(const T v) { x = y = z = v; return *this; }
    Vector3<T>& operator=(const Vector3<T> &v) { x = v.x; y = v.y; z = v.z; return *this;}
    Vector3<T> operator-() const {
      Vector3<T> v;
      v.x = -x; v.y = -y; v.z = -z;
      return v;
    }

    // Comparison
    bool operator==(const Vector3<T>& v) const {
      return (x3d::Math<T>::abs(x - v.x) <= (T)0.001f
        && x3d::Math<T>::abs(y - v.y) <= (T)0.001f
        && x3d::Math<T>::abs(z - v.z) <= (T)0.001f);
    }

    bool operator!=(const Vector3<T>& v) {
      return !(*this == v);
    }

    // Vector & Scalar
    Vector3<T> operator+(const T s) const { return Vector3<T>(x+s, y+s, z+s); }
    Vector3<T> operator-(const T s) const { return Vector3<T>(x-s, y-s, z-s); }
    Vector3<T> operator*(const T s) const { return Vector3<T>(x*s, y*s, z*s); }
    Vector3<T> operator/(const T s) const { return Vector3<T>(x/s, y/s, z/s); }
    Vector3<T>& operator+=(const T s) { x += s; y += s; z += s; return *this; }
    Vector3<T>& operator-=(const T s) { x -= s; y -= s; z -= s; return *this; }
    Vector3<T>& operator*=(const T s) { x *= s; y *= s; z *= s; return *this; }
    Vector3<T>& operator/=(const T s) { x /= s; y /= s; z /= s; return *this; }

    // Vector & Vector
    Vector3<T> operator+(const Vector3<T>& v) const { return Vector3<T>(x+v.x, y+v.y, z+v.z); }
    Vector3<T> operator-(const Vector3<T>& v) const { return Vector3<T>(x-v.x, y-v.y, z-v.z); }
    Vector3<T> operator*(const Vector3<T>& v) const { return Vector3<T>(x*v.x, y*v.y, z*v.z); }
    Vector3<T> operator/(const Vector3<T>& v) const { return Vector3<T>(x/v.x, y/v.y, z/v.z); }
    Vector3<T>& operator+=(const Vector3<T>& v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vector3<T>& operator-=(const Vector3<T>& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    Vector3<T>& operator*=(const Vector3<T>& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
    Vector3<T>& operator/=(const Vector3<T>& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

    // Dot Product
    T operator|(const Vector3<T>& v) const { return x*v.x + y*v.y + z*v.z; }
    // Cross Product
    Vector3<T> operator^(const Vector3<T>& v) const { return Vector3<T>(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); }

    double magnitude_squared() const { return static_cast<double>(x*x + y*y + z*z); }
    double magnitude() const { return x3d::Math<double>::sqrt(magnitude_squared()); }

    Vector3<T>& normalize() {
      T length = static_cast<T>(magnitude());
      if (length == 0 || length == 1)
        return *this;
      return *this /= length;
    }

    Vector3<T> get_normalized() {
      Vector3<T> v = *this;
      return v.normalize();
    }

    // Automatic Type conversion/casting
    //template<class T2> operator Vector3<T2>() const { return Vector3<T2>(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z)); }
    operator T*() { return (T*)this; }
    template<class T2> Vector3(const Vector3<T2> &v) { x = static_cast<T>(v.x); y = static_cast<T>(v.y); z = static_cast<T>(v.z); }

    std::string serialize() {
      return (boost::format("%d | %d | %d") % x % y % z).str();
    }
  };

  template<class T> inline Vector3<T> operator*(const T& value, const Vector3<T>& v) {
    return x3d::Vector3<T>(v.x*value, v.y*value, v.z*value);
  }

  // Normal calculation functions
  template<class T> inline Vector3<T> normal_vector(const Vector3<T>& v1, const Vector3<T>& v2) {
    Vector3<T> n = v1 ^ v2;
    n.normalize();
    return n;
  }

  template<class T> inline Vector3<T> normal_vector(const Vector3<T>& v1, const Vector3<T>& v2, const Vector3<T>& v3) {
    return normal_vector(v2 - v1, v3 - v1);
  }
}

// static consts initialization
template<class T> const x3d::Vector3<T> x3d::Vector3<T>::Zero = x3d::Vector3<T>(0, 0, 0);
template<class T> const x3d::Vector3<T> x3d::Vector3<T>::UnitX = x3d::Vector3<T>(1, 0, 0);
template<class T> const x3d::Vector3<T> x3d::Vector3<T>::UnitY = x3d::Vector3<T>(0, 1, 0);
template<class T> const x3d::Vector3<T> x3d::Vector3<T>::UnitZ = x3d::Vector3<T>(0, 0, 1);

#endif
