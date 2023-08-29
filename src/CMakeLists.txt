#
# Source
#
file(GLOB_RECURSE SRC 
"./*.cpp"

)
add_library(${PROJECT_NAME}_trgt STATIC ${SRC})

include_directories(${CMAKE_SOURCE_DIR}/external/)

 
    if(OPENGL_FOUND)
            include_directories(${OPENGL_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for OPENGL cannot not found)
        endif() 
    if(DEFINED GUM_OS_BSD)
        if(X11_FOUND)
            include_directories(${X11_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for X11 cannot not found)
        endif() 
    endif() 
    if(DEFINED GUM_OS_LINUX)
        if(X11_FOUND)
            include_directories(${X11_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for X11 cannot not found)
        endif() 
    endif() 
    if(GUMGUI_FOUND)
            include_directories(${GUMGUI_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMGUI cannot not found)
        endif() 
    if(GUMESSENTIALS_FOUND)
            include_directories(${GUMESSENTIALS_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMESSENTIALS cannot not found)
        endif() 
    if(GUMOPENGL_FOUND)
            include_directories(${GUMOPENGL_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMOPENGL cannot not found)
        endif() 
        if(GUMGRAPHICS_FOUND)
            include_directories(${GUMGRAPHICS_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMGRAPHICS cannot not found)
        endif() 
    if(GUMCODECS_FOUND)
            include_directories(${GUMCODECS_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMCODECS cannot not found)
        endif() 
    if(GUMPRIMITIVES_FOUND)
            include_directories(${GUMPRIMITIVES_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMPRIMITIVES cannot not found)
        endif() 
    if(GUMDESKTOP_FOUND)
            include_directories(${GUMDESKTOP_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMDESKTOP cannot not found)
        endif() 
    if(GUMSYSTEM_FOUND)
            include_directories(${GUMSYSTEM_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMSYSTEM cannot not found)
        endif() 
    if(GUMGLFW_FOUND)
            include_directories(${GUMGLFW_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMGLFW cannot not found)
        endif() 
    if(GUMMATHS_FOUND)
            include_directories(${GUMMATHS_INCLUDE_DIRS})
        else()
            message(FATAL_ERROR Library for GUMMATHS cannot not found)
        endif() 
    include_directories(${CMAKE_SOURCE_DIR}/external/bullet3/src/)

 

install(
    FILES gum-engine.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
) 

file(GLOB HEADERS Engine/2D/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/2D/
) 

file(GLOB HEADERS Engine/2D/Layer/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/2D/Layer/
) 

file(GLOB HEADERS Engine/3D/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/3D/
) 

file(GLOB HEADERS Engine/3D/Object/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/3D/Object/
) 

file(GLOB HEADERS Engine/3D/Object/Skeletal/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/3D/Object/Skeletal/
) 

file(GLOB HEADERS Engine/3D/Lightning/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/3D/Lightning/
) 

file(GLOB HEADERS Engine/General/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/General/
) 

file(GLOB HEADERS Engine/Rendering/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/Rendering/
) 

file(GLOB HEADERS Engine/Lightning/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/Lightning/
) 

file(GLOB HEADERS Engine/Lightning/ShadowMapping/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/Lightning/ShadowMapping/
) 

file(GLOB HEADERS Engine/Loaders/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/Loaders/
) 

file(GLOB HEADERS Engine/Managers/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/Managers/
) 

file(GLOB HEADERS Engine/Material/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/Material/
) 

file(GLOB HEADERS Engine/Particle/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/Particle/
) 

file(GLOB HEADERS Engine/Physics/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/Physics/
) 

file(GLOB HEADERS Engine/PostProcessing/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/PostProcessing/
) 

file(GLOB HEADERS Engine/PostProcessing/Effects/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/PostProcessing/Effects/
) 

file(GLOB HEADERS Engine/PostProcessing/ImageCorrection/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/PostProcessing/ImageCorrection/
) 

file(GLOB HEADERS Engine/PostProcessing/OutlineRenderer/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/PostProcessing/OutlineRenderer/
) 

file(GLOB HEADERS Engine/Texture/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/Texture/
) 

file(GLOB HEADERS Engine/Shaders/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Engine/Shaders/
)




set(COMBINED_LIB ${CMAKE_BINARY_DIR}/lib${PROJECT_NAME}.a)

add_custom_target(${PROJECT_NAME}_createcombineddir
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/combined/
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
    DEPENDS ${PROJECT_NAME}_trgt
    VERBATIM
)

add_custom_command( #combinedtarget ALL
    OUTPUT ${COMBINED_LIB}
    
    COMMAND ar -x $<TARGET_FILE:BulletSoftBody>
    COMMAND ar -x $<TARGET_FILE:BulletDynamics>
    COMMAND ar -x $<TARGET_FILE:BulletCollision>
    COMMAND ar -x $<TARGET_FILE:LinearMath>
    COMMAND ar -x $<TARGET_FILE:${PROJECT_NAME}_trgt>
    COMMAND sh -c "ar -qcs \"${COMBINED_LIB}\" *.o"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/combined/
    DEPENDS  BulletSoftBody BulletDynamics BulletCollision LinearMath ${PROJECT_NAME}_createcombineddir ${PROJECT_NAME}_trgt
    COMMENT "Combining libs"
    VERBATIM
)

add_custom_target(${PROJECT_NAME}_combinedlibtarget ALL DEPENDS ${COMBINED_LIB})

# Create an library target out of the library compilation result
add_library(${PROJECT_NAME} STATIC IMPORTED GLOBAL)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_combinedlibtarget)

# Specify where the library is and where to find the headers
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    IMPORTED_LOCATION "${COMBINED_LIB}"
)

install(FILES ${COMBINED_LIB}
    DESTINATION ${CMAKE_INSTALL_LIBDIR})

#install(
#    EXPORT ${PROJECT_NAME}Targets
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
#    FILE ${PROJECT_NAME}Targets.cmake
#)

configure_package_config_file("Config.cmake.in"
    "${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    PATH_VARS CMAKE_INSTALL_LIBDIR
)

#write_basic_package_version_file(
#	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
#	VERSION 1.0.0
#	COMPATIBILITY SameMajorVersion
#)

### Install Config and ConfigVersion files
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
#          "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

export(PACKAGE ${PROJECT_NAME})

