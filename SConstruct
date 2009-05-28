env = Environment(tools = ['g++', 'mingw'])

env.Append(CPPPATH = ['include', 'include/SOIL', 'src/SOIL', 'src/x3d'],
           LIBPATH = ['src/SOIL'],
           LIBS = ['boost_system'])

env.StaticLibrary("SOIL", Split("""
  src/SOIL/image_DXT.c
  src/SOIL/image_helper.c
  src/SOIL/SOIL.c
  src/SOIL/stb_image_aug.c"""))

env.StaticLibrary("x3d", Split("""
  src/x3d/Image.cpp
  src/x3d/ImageLoaderSOIL.cpp
  src/x3d/Texture.cpp
  src/x3d/TexturePool.cpp"""))