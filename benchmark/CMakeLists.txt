cmake_minimum_required(VERSION 3.10)


project(chronicle)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(benchmark
	benchmark.cpp
	${PROJECT_SOURCE_DIR}/../thirdparty/source/NanoLog.cpp)

target_compile_options(benchmark PRIVATE "/permissive-")
target_link_options(benchmark PRIVATE "/profile")


add_library(reckless STATIC IMPORTED)
set_property(TARGET reckless
  PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../thirdparty/lib/reckless.lib)

set_property(TARGET benchmark PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")


target_include_directories(benchmark PUBLIC
    "${PROJECT_SOURCE_DIR}/../include"
    "${PROJECT_SOURCE_DIR}/../thirdparty/include"
)

target_link_libraries(benchmark reckless)
