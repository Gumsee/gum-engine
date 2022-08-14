set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS ON)
set (INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dist/${CMAKE_BUILD_TYPE}/bin/")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g3 -Wall -Wextra -Wno-unused-parameter -Wno-reorder -Wno-pedantic")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0") #-Og
if(BUILD_SANITIZE)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak") #-fsanitize=undefined,float-divide-by-zero,signed-integer-overflow,leak
endif()

message(STATUS "GumEngine: CXXFLAGS: ${CMAKE_CXX_FLAGS}")

#add_compile_options(${CMAKE_CXX_FLAGS})

#
# Directories
#
include_directories(${CMAKE_SOURCE_DIR}/ext/bullet3/src/)
include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(/usr/include/freetype2/)
include_directories(${GUMGUI_DIRECTORY})
include_directories(${GUMMATH_DIRECTORY})
include_directories(${GUMESSENTIALS_DIRECTORY})
include_directories(${GUMOPENGL_DIRECTORY})

link_directories(${CMAKE_SOURCE_DIR}/lib/)

#
# Source
#
file(GLOB_RECURSE SRC "./*.cpp")

add_library(GumEngine STATIC ${SRC})
target_link_libraries(GumEngine ${CMAKE_THREAD_LIBS_INIT} )


file(GLOB HEADERS "./Engine/Lightning/*.h")
install(FILES ${HEADERS} DESTINATION ${GUM_INSTALLDIR}/Engine/Lightning/)

file(GLOB HEADERS "./Engine/Managers/*.h")
install(FILES ${HEADERS} DESTINATION ${GUM_INSTALLDIR}/Engine/Managers/)

file(GLOB HEADERS "./Engine/Material/*.h")
install(FILES ${HEADERS} DESTINATION ${GUM_INSTALLDIR}/Engine/Material/)

file(GLOB HEADERS "./Engine/Object/*.h")
install(FILES ${HEADERS} DESTINATION ${GUM_INSTALLDIR}/Engine/Object/)

file(GLOB HEADERS "./Engine/Particle/*.h")
install(FILES ${HEADERS} DESTINATION ${GUM_INSTALLDIR}/Engine/Particle/)

file(GLOB HEADERS "./Engine/Physics/*.h")
install(FILES ${HEADERS} DESTINATION ${GUM_INSTALLDIR}/Engine/Physics/)

file(GLOB HEADERS "./Engine/PostProcessing/*.h")
install(FILES ${HEADERS} DESTINATION ${GUM_INSTALLDIR}/Engine/PostProcessing/)

file(GLOB HEADERS "./Engine/Texture/*.h")
install(FILES ${HEADERS} DESTINATION ${GUM_INSTALLDIR}/Engine/Texture/)

file(GLOB HEADERS "./Engine/General/*.h")
install(FILES ${HEADERS} DESTINATION ${GUM_INSTALLDIR}/Engine/General/)

file(GLOB HEADERS "./Engine/Loaders/*.h")
install(FILES ${HEADERS} DESTINATION ${GUM_INSTALLDIR}/Engine/Loaders/)


install(FILES "./gum-engine.h" DESTINATION ${GUM_INSTALLDIR}/ )

install(TARGETS ${CMAKE_PROJECT_NAME} EXPORT
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
