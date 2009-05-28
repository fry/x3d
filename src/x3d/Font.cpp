#include "x3d/Font.hpp"

using namespace x3d;

unsigned short Font::get_size() {
  return m_size;
}

std::string Font::get_face() {
  return m_face;
}

bool Font::load(const boost::filesystem::path& font_def) {
  boost::filesystem::ifstream file(font_def);

  std::string chunk, line, type, key, value;
  int value_int;

  std::size_t pos;
  while (!file.eof()) {
    std::getline(file, line);
    std::stringstream line_stream(line);

    // Read line type
    line_stream >> type;

    if (type == "info") {
      while (!line_stream.eof()) {
        line_stream >> chunk;
        pos = chunk.find("=");
        key = chunk.substr(0, pos);
        value = chunk.substr(pos + 1);

        if (key == "face")
          m_face = value;
        else if (key == "size")
          m_size = boost::lexical_cast<unsigned short>(value);
      }
    } else if (type == "common") {
      while (!line_stream.eof()) {
        line_stream >> chunk;
        pos = chunk.find("=");
        key = chunk.substr(0, pos);
        value_int = boost::lexical_cast<unsigned short>(chunk.substr(pos + 1));

        if (key == "lineHeight")
          m_charset.line_height = value_int;
        else if (key == "base")
          m_charset.base = value_int;
        else if (key == "scaleW")
          m_charset.width = value_int;
        else if (key == "scaleH")
          m_charset.height = value_int;
        else if (key == "pages") {
          m_charset.pages = value_int;
          m_pages.resize(value_int);
        }
      }
    } else if (type == "page") {
      unsigned short page_id;
      while (!line_stream.eof()) {
        line_stream >> chunk;
        pos = chunk.find("=");
        key = chunk.substr(0, pos);
        value = chunk.substr(pos + 1);

        if (key == "id") {
          page_id = boost::lexical_cast<unsigned short>(value);
        } else if (key == "file") {
          // TODO: Make file names with spaces in them possible.
          boost::filesystem::path file_path;
          file_path = font_def.branch_path() / value.substr(1, value.length() - 2);
          m_pages[page_id].file = file_path.string();

          // TODO: Load the texture from a texture manager
          m_pages[page_id].texture.reset(m_texture_pool.get(file_path));
        }
      }
    } else if (type == "char") {
      unsigned short char_id;

      while (!line_stream.eof()) {
        line_stream >> chunk;
        pos = chunk.find("=");
        key = chunk.substr(0, pos);
        value_int = boost::lexical_cast<int>(chunk.substr(pos + 1));

        if (key == "id")
          char_id = value_int;
        else if (key == "x")
          m_charset.characters[char_id].x = value_int;
        else if (key == "y")
          m_charset.characters[char_id].y = value_int;
        else if (key == "width")
          m_charset.characters[char_id].width = value_int;
        else if (key == "height")
          m_charset.characters[char_id].height = value_int;
        else if (key == "xoffset")
          m_charset.characters[char_id].xoffset = value_int;
        else if (key == "yoffset")
          m_charset.characters[char_id].yoffset = value_int;
        else if (key == "xadvance")
          m_charset.characters[char_id].xadvance = value_int;
        else if (key == "page")
          m_charset.characters[char_id].page = value_int;
      }
    } else if (type == "kerning") {
      unsigned short first_id;
      kerning_type kerning;

      while (!line_stream.eof()) {
        line_stream >> chunk;
        pos = chunk.find("=");
        key = chunk.substr(0, pos);
        value_int = boost::lexical_cast<int>(chunk.substr(pos + 1));

        if (key == "first")
          first_id = value_int;
        else if (key == "second")
          kerning.second = value_int;
        else if (key == "amount")
          kerning.amount = value_int;
      }

      m_charset.characters[first_id].kerning.push_back(kerning);
    }
  }
  return true;
}

void Font::render(const std::string& text, float x, float y, float z) {
  if (m_pages.empty())
    throw std::runtime_error("No font loaded");

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_LIGHTING);
  glDepthFunc(GL_ALWAYS);
  glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
  glColor4f(1, 1, 1, 1);

  // Bind texture
  m_pages[0].texture->bind();
  float currentx = x;
  for (unsigned int i = 0; i < text.size(); i ++) {
    // new line char -> new line
    if (text[i] == '\n') {
      y += m_charset.line_height;
      currentx = x;
    } else {
      float charx = m_charset.characters[text[i]].x;
      float chary = m_charset.characters[text[i]].y;
      float width = m_charset.characters[text[i]].width;
      float height = m_charset.characters[text[i]].height;
      float xoffset = m_charset.characters[text[i]].xoffset;
      float yoffset = m_charset.characters[text[i]].yoffset;

      float tx0, tx1, ty0, ty1;
      tx0 = charx/m_charset.width;
      ty0 = 1 - chary/m_charset.height; // 1 - x because we turned the texture around
      tx1 = (charx+width)/m_charset.width;
      ty1 = 1 - (chary+height)/m_charset.height;
      float x1, y1, x2, y2, x3, y3, x4, y4;
      x1 = currentx + xoffset; // TL
      y1 = y + yoffset;
      x2 = currentx + xoffset + width; // TR
      y2 = y + yoffset;
      x3 = currentx + xoffset + width; // BR
      y3 = y + yoffset + height;
      x4 = currentx + xoffset; // BL
      y4 = y + yoffset + height;
      
      glBegin(GL_QUADS);
        glTexCoord2f(tx0, ty0); glVertex3f(x1, y1, 0); //TL
        glTexCoord2f(tx0, ty1); glVertex3f(x4, y4, 0); //BL
        glTexCoord2f(tx1, ty1); glVertex3f(x3, y3, 0); //BR
        glTexCoord2f(tx1, ty0); glVertex3f(x2, y2, 0); //TR
      glEnd();

      currentx += m_charset.characters[text[i]].xadvance;

      // Kerning
      if (i + 1 < text.size()) {
        std::vector<kerning_type>::iterator iter, end;
        end = m_charset.characters[text[i]].kerning.end();

        for (iter = m_charset.characters[text[i]].kerning.begin(); iter != end; iter ++) {
          if (iter->second == text[i + 1])
            currentx += iter->amount;
        }
      }
    }
  }
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPopAttrib();
}