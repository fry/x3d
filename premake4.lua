function set_win32_defines()
  configuration "windows" 
    defines { "WIN32", "__WIN32__" }
end

solution "x3d"
  configurations { "Debug", "Release" }
  
  configuration "Debug"
    set_win32_defines()
    defines { "DEBUG" }
    flags { "Symbols" }
    targetdir "bin/debug"
  
  configuration "Release"
    set_win32_defines()
    defines { "NDEBUG" }
    flags { "Optimize", "OptimizeSpeed" }
    targetdir "bin/release"

  location "build"

  project "sandbox"
    kind "ConsoleApp"
    language "C++"
    includedirs { "include" }
    files { "src/sandbox/*.hpp", "src/sandbox/*.cpp" }
    links { "glew", "x3d", "Win32OpenGLRenderer", "DirectX9Renderer" }

  project "Core"
    kind "ConsoleApp"
    language "C++"
    files { "include/x3d/*.hpp", "src/x3d/*.hpp", "src/x3d/*.cpp" }

  project "log4cpp"
    kind "StaticLib"
    language "C++"
    files { "include/log4cpp/**.hh", "include/log4cpp/**.h", "src/log4cpp/*.hh", "src/log4cpp/*.h", "src/log4cpp/*.cpp" }
    includedirs { "include" }
    configuration "windows"
      links { "kernel32", "user32", "ws2_32", "advapi32" }

  project "SOIL"
    kind "StaticLib"
    language "C++"
    includedirs { "include" }
    files { "include/SOIL/*.h", "src/SOIL/*.h", "src/SOIL/*.c" }
    includedirs { "include/SOIL" }

  project "glew"
    kind "StaticLib"
    language "C++"
    files { "include/GL/*.h", "src/glew/*.c" }
    defines { "GLEW_STATIC", "GLEW_MX" }
    includedirs { "include" }

  project "Win32Device"
    kind "StaticLib"
    language "C++"
    includedirs { "include" }
    files { "src/x3d/Win32Device.cpp",
            "src/x3d/Subsystem.cpp" }
    links { "log4cpp" }

  project "Win32OpenGLRenderer"
    kind "StaticLib"
    language "C++"
    includedirs { "include" }
    files { "src/x3d/OpenGL*.cpp",
            "src/x3d/Win32OpenGLRenderer.cpp",
            "src/x3d/Subsystem.cpp" }
    links { "Win32Device", "opengl32", "glu32" }

  project "DirectX9Renderer"
    kind "StaticLib"
    language "C++"
    includedirs { "include" }
    files { "src/x3d/DirectX9Renderer.cpp",
            "src/x3d/Subsystem.cpp" }
    links { "Win32Device" }
    configuration "Debug"
      links { "d3dx9d", "d3d9" }
    configuration "Release"
      links { "d3dx9", "d3d9" }

  project "x3d"
    kind "StaticLib"
    language "C++"
    includedirs { "include" }
    files { "include/x3d/*.hpp", "src/x3d/*.hpp", "src/x3d/*.cpp" }
    includedirs { "include" }
    links { "SOIL" }

--[[
solution "sandbox"
  configurations { "Debug", "Release" }
  
  location "build"

  configuration "Debug"
    defines { "DEBUG" }
    flags { "Symbols" }
    targetdir "bin/debug"
  
  configuration "Release"
    defines { "NDEBUG" }
    flags { "Optimize", "OptimizeSpeed" }
    targetdir "bin/release"

  project "sandbox"
    kind "ConsoleApp"
    language "C++"
    includedirs { "include" }
    files { "src/sandbox/*.hpp", "src/sandbox/*.cpp" }
    includedirs { "include" }
    links { "x3d", "Win32OpenGLRenderer", "DirectX9Renderer" }
]]--

if _ACTION == "clean" then
  os.rmdir("bin")
  os.rmdir("build")
end