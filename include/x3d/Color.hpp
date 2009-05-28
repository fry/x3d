#ifndef X3D_COLOR_HPP_
#define X3D_COLOR_HPP_

namespace x3d {
  class Color {
  public:
    static const Color Red;
    static const Color Green;
    static const Color Blue;

    float r, g, b, a;

    Color(float fr, float fg, float fb, float fa = 1.0f) {
      set(fr, fg, fb, fa);
    }

    Color(float v) {
      set(v, v, v, v);
    }

    Color() {
      set(1.0f, 1.0f, 1.0f, 1.0f);
    }
    
    void set(float red, float green, float blue, float alpha = 1.0f) {
      r = red;
      g = green;
      b = blue;
      a = alpha;
    }

    void set(float luminous, float alpha = 1.0f) {
      r = g = b = luminous;
      a = alpha;
    }

    // TODO: ouch, fix this
    bool operator==(const Color& color) {
      return (color.a == a && color.r == r && color.g == g && color.b == b);
    }

    bool operator!=(const Color& color) {
      return (color.a != a || color.r != r || color.g != g || color.b != b);
    }

    operator float*() { return (float*)this; }

  };
}

#endif
