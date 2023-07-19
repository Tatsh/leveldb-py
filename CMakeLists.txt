cmake_minimum_required(VERSION 3.26.0)
project(leveldb_viewer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

find_package(
  Qt6
  COMPONENTS Gui Widgets
  REQUIRED)
find_package(leveldb REQUIRED)

# Sane flags from Kate project
add_definitions(
  -DQT_NO_CAST_FROM_ASCII
  -DQT_NO_CAST_FROM_BYTEARRAY
  -DQT_NO_CAST_TO_ASCII
  -DQT_NO_SIGNALS_SLOTS_KEYWORDS
  -DQT_NO_URL_CAST_FROM_STRING
  -DQT_STRICT_ITERATORS
  -DQT_USE_FAST_OPERATOR_PLUS
  -DQT_USE_QSTRINGBUILDER)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
  add_compile_options(
    -Wall
    -Wextra
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-deprecated-copy-with-user-provided-dtor
    -Wno-exit-time-destructors
    -Wno-global-constructors
    -Wno-inconsistent-missing-destructor-override
    -Wno-padded
    -Wno-redundant-parens)
endif()

include_directories(.)
add_executable(leveldb-viewer src/main.cpp src/mainwindow.cpp)
target_link_libraries(leveldb-viewer leveldb Qt6::Gui Qt6::Widgets)