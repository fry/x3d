#ifndef X3D_FONT_HPP_
#define X3D_FONT_HPP_

#if defined(__WIN32__)
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <gl/gl.h>
#else
  #include <gl/gl.h>
#endif

#include <vector>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/shared_ptr.hpp>

#include "x3d/TexturePool.hpp"

// TODO: Finish, clean up (replace immediate mode drawing)
namespace x3d {
  class Font {
    struct kerning_type {
      short second;
      short amount;
    };

    struct character_type {
      short x, y;
      short width, height;
      short xoffset, yoffset;
      short xadvance;
      short page;
      std::vector<kerning_type> kerning;

      character_type(): x(0), y(0), width(0), height(0), xoffset(0), yoffset(0),
        xadvance(0), page(0) {}
    };
    struct charset_type {
      short line_height;
      short base;
      short width, height;
      short pages;

      character_type characters[256];
    };
    struct page_type {
      unsigned short id;
      std::string file;
      boost::shared_ptr<x3d::Texture> texture;

      page_type(): id(0), file() {}
    };

    charset_type m_charset;
    std::vector<page_type> m_pages;
    std::string m_face;
    unsigned short m_size;
    x3d::TexturePool& m_texture_pool;
  public:
    Font(x3d::TexturePool& texture_pool): m_texture_pool(texture_pool) {}
    Font(const boost::filesystem::path& font_def, x3d::TexturePool& texture_pool): m_texture_pool(texture_pool) {
      load(font_def);
    }

    bool load(const boost::filesystem::path& font_def);
    void render(const std::string& text, float x = 0, float y = 0, float z = 0);

    unsigned short get_size();
    std::string get_face();
  };
}
#endif
