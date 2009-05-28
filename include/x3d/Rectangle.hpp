#ifndef _X3D_RECTANGLE_HPP_
#define _X3D_RECTANGLE_HPP_

namespace x3d {
  template<typename T> struct Rectangle;

  typedef Rectangle<float> Rectanglef;
  typedef Rectangle<double> Rectangled;
  typedef Rectangle<int> Rectanglei;

  template<typename T> struct Rectangle {
    T left, top, right, bottom;
  };
}

#endif
