cmake_minimum_required (VERSION 3.0)
project (GumEngine)

#Set OS
#add_definitions(-DGUM_OS_WINDOWS) 
#add_definitions(-DGUM_OS_BSD)
#add_definitions(-DGUM_OS_MACOS)
add_definitions(-DGUM_OS_LINUX)

add_definitions(-DDEBUG)
#add_definitions(-DCHECK_GL_ERRORS)

set(GUM_INSTALLDIR "/usr/include/gumball/")
set(GUMBALL_INCLUDE_DIR "/usr/include/gumball/")
set(GUMGUI_DIRECTORY ${GUMBALL_INCLUDE_DIR})
set(GUMMATH_DIRECTORY ${GUMBALL_INCLUDE_DIR})
set(GUMESSENTIALS_DIRECTORY ${GUMBALL_INCLUDE_DIR})
set(GUMOPENGL_DIRECTORY ${GUMBALL_INCLUDE_DIR})
set(BUILD_SANITIZE false)

add_subdirectory(./ext/bullet/)
add_subdirectory(./src/)
add_subdirectory(./examples/)
