#ifndef X3D_MATH_HPP_
#define X3D_MATH_HPP_

#include <cmath>

namespace x3d {
  template<typename T = float> class Math {
  public:
    static const T PI;
    static inline T log2(T x) {
      return static_cast<T>(::log(static_cast<double>(x))/::logf(2.0)); }
    static inline T sin(T x) {
      return ::sin(static_cast<double>(x)); }
    static inline T cos(T x) { 
      return ::cos(static_cast<double>(x)); }
    static inline T pow2(unsigned int x) { 
      return static_cast<T>(1 << x); }
    static inline T abs(T x) {
      return static_cast<T>(::fabs(static_cast<T>(x))); }
    //static inline unsigned int L1_range(const x3d::vector3d& a, const x3d::vector3d& b) {
    //  return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z); }
    static inline T sqrt(T x) {
      return static_cast<T>(::sqrt(static_cast<double>(x))); }
    static inline T to_rad(T angle) {
      return angle * (1.0f / PI);
    }
    static inline int floor(T x) {
      static_cast<T>(::floor(static_cast<double>(x)));
    }
  };
}

template<typename T>
const T x3d::Math<T>::PI = 3.14159265;

#endif
