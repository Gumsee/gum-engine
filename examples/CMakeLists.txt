set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS ON)
set (INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dist/${CMAKE_BUILD_TYPE}/bin/")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g3 -Wall -Wextra -Wno-pedantic")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0") #Optimization

message(STATUS "Examples: CXXFLAGS: ${CMAKE_CXX_FLAGS}")

#add_compile_options(-pedantic -Wall -Wextra)

find_package(PkgConfig REQUIRED)
find_package( Threads )

include_directories(/usr/include/freetype2)
include_directories(/usr/include/libxml2)
include_directories(${CMAKE_SOURCE_DIR}/include/bullet/)
include_directories(${CMAKE_SOURCE_DIR}/include/)
include_directories(${CMAKE_SOURCE_DIR}/src/)

add_definitions(-DEXAMPLES_PATH=\"${CMAKE_SOURCE_DIR}/examples/\")

set(GUMENGINE_LIBRARIES 
    GumEngine
    bullet
    ${CMAKE_SOURCE_DIR}/lib/libMaskedOcclusionCulling.a 
    sfml-graphics
    sfml-network
    sfml-audio
    sfml-window
    sfml-system
    openal
    GLEW 
    GL
    avformat 
    swscale  
    avcodec 
    swscale 
    avutil 
    avdevice 
    avfilter
    portaudiocpp
    fftw3f 
    libassimp.so
    minizip
    libz.so
    freetype
    ncurses tinfo
    xml2
    lua5.4
    ${CMAKE_THREAD_LIBS_INIT}
)

set(EXAMPLE_FILE_LIST 
#    GUIs
#    PhysicallyBasedRendering
#    IrradiancePBR
#    ShadowMapping
#    AnimatedModel
#    Particles
#    Physics
#    MazeLearning
#    GBufferTest
#    Billboards
#    SphereGenerator
#    PostProcessingEffects
#    Ocean
#    GumMaths
)


#foreach(EXAMPLE ${EXAMPLE_FILE_LIST})
#    message(STATUS "Adding target ${EXAMPLE}")
#    add_executable(Example_${EXAMPLE} "./${EXAMPLE}/${EXAMPLE}.cpp")
#    target_link_libraries(Example_${EXAMPLE} ${GUMENGINE_LIBRARIES})
#endforeach()

file(GLOB_RECURSE EXAMPLES_SRC "./*.cpp")
add_executable(Examples ${EXAMPLES_SRC})
target_link_libraries(Examples ${GUMENGINE_LIBRARIES})

#add_executable(Example_PostProcessingEffects "./PostProcessingEffects/PostProcessingEffects.cpp")
#target_link_libraries(Example_PostProcessingEffects ${GUMENGINE_LIBRARIES})