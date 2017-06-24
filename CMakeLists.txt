cmake_minimum_required(VERSION 3.7)
project(RayTracing)

FIND_PACKAGE( OpenCV REQUIRED core highgui imgproc)
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Object.h Material.h Color.h  Collision.h Plane.h Rectangle.h Sphere.h Camera.h Scene.h Light.h Raytracing.h  AreaLight.h  PointLight.h)
add_executable(RayTracing ${SOURCE_FILES})

target_link_libraries(RayTracing ${OpenCV_LIBS})