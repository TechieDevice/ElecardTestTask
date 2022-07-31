cmake_minimum_required (VERSION 3.8)

file (GLOB_RECURSE FILES EditorProject/*.cpp EditorProject/*.h)
set (SOURCES ${FILES})
add_executable (EditorProject ${SOURCES})
