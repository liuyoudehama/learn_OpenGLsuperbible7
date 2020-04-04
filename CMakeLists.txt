cmake_minimum_required (VERSION 2.6)

project (superbible7_playground)

#set(CMAKE_VERBOSE_MAKEFILE ON)

IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LINUX -std=c++0x")
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

include_directories( include )
include_directories(extern/glfw-3.0.4/include)

add_library(sb7
            sb7/sb7.cpp
            sb7/sb7color.cpp
            sb7/sb7ktx.cpp
            sb7/sb7object.cpp
            sb7/sb7shader.cpp
            sb7/sb7textoverlay.cpp
            sb7/gl3w.c
)
find_package(OpenGL)

if(WIN32)
	set(COMMON_LIBS sb7 optimized glfw3 debug glfw3_d ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})
elseif (UNIX)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GLFW REQUIRED glfw3)
	set(COMMON_LIBS sb7 glfw3 X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL rt dl)
else()
	set(COMMON_LIBS sb7)
endif()

if(MSVC)
    configure_file(${PROJECT_SOURCE_DIR}/build/templates/vs2013.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${EXAMPLE}.vcxproj.user @ONLY)
endif(MSVC)

add_executable(playground playground.cpp)
target_link_libraries(playground ${COMMON_LIBS})