cmake_minimum_required (VERSION 2.8)

set(PROJECT_NAME image_processing)
set(SRC_DIR src)
set(INCLUDE_DIR include)
set(SOURCES
	${SRC_DIR}/image_processing_png.cpp
	${SRC_DIR}/main.cpp)

include_directories(
	${SRC_DIR}
	${INCLUDE_DIR})

add_definitions(
	-std=c++1y
	-Wall
	-Wextra
	-Werror)

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} ${SOURCES})
