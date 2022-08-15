cmake_minimum_required (VERSION 3.0)
project (GumEngine)

#Set OS
#add_definitions(-DGUM_OS_WINDOWS) 
#add_definitions(-DGUM_OS_BSD)
#add_definitions(-DGUM_OS_MACOS)
add_definitions(-DGUM_OS_LINUX)

#add_definitions(-DDEBUG)
#add_definitions(-DCHECK_GL_ERRORS)

set(GUM_INSTALLDIR "/usr/include/gumball/")
set(GUMBALL_INCLUDE_DIR "/usr/include/gumball/")
set(GUMGUI_DIRECTORY ${GUMBALL_INCLUDE_DIR})
set(GUMMATH_DIRECTORY ${GUMBALL_INCLUDE_DIR})
set(GUMESSENTIALS_DIRECTORY ${GUMBALL_INCLUDE_DIR})
set(GUMOPENGL_DIRECTORY ${GUMBALL_INCLUDE_DIR})
set(BUILD_SANITIZE false)

set(BUILD_PYBULLET OFF CACHE INTERNAL "")
set(BUILD_CPU_DEMOS OFF CACHE INTERNAL "")
set(BUILD_OPENGL3_DEMOS OFF CACHE INTERNAL "")
set(BUILD_BULLET2_DEMOS OFF CACHE INTERNAL "")
set(BUILD_EXTRAS OFF CACHE INTERNAL "")
set(BUILD_UNIT_TESTS OFF CACHE INTERNAL "")



#add_subdirectory(./ext/bullet3/)
add_subdirectory(./src/)
#add_subdirectory(./examples/)
