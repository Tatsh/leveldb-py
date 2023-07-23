cmake_minimum_required(VERSION 3.26.0)
set(CMAKE_SYSTEM_VERSION "10.0.17763.0" CACHE TYPE INTERNAL FORCE)
project(
  leveldb_viewer
  DESCRIPTION "A very basic LevelDB viewer."
  HOMEPAGE_URL "https://github.com/Tatsh/leveldb-py"
  VERSION 1.0.1)

find_package(
  Qt6
  COMPONENTS Gui Widgets
  REQUIRED)
find_package(leveldb REQUIRED)

qt_standard_project_setup(REQUIRES 6.5.0)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Sane flags from Kate project
add_compile_definitions(
  QT_NO_CAST_FROM_ASCII
  QT_NO_CAST_FROM_BYTEARRAY
  QT_NO_CAST_TO_ASCII
  QT_NO_SIGNALS_SLOTS_KEYWORDS
  QT_NO_URL_CAST_FROM_STRING
  QT_STRICT_ITERATORS
  QT_USE_FAST_OPERATOR_PLUS
  QT_USE_QSTRINGBUILDER)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
  if(MSVC)
    add_compile_options(/Wall /sdl)
  else()
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
endif()

qt_add_executable(leveldb-viewer src/main.cpp src/mainwindow.cpp)
set_target_properties(
  leveldb-viewer
  PROPERTIES MACOSX_BUNDLE ON
             MACOSX_BUNDLE_BUNDLE_NAME "LevelDB Viewer"
             MACOSX_BUNDLE_GUI_IDENTIFIER sh.tat.leveldb-viewer
             MSVC_RUNTIME_LIBRARY MultiThreaded
             WIN32_EXECUTABLE ON)
install(
  TARGETS leveldb-viewer
  BUNDLE DESTINATION .
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
target_include_directories(leveldb-viewer PRIVATE ${leveldb_INCLUDE_DIR})
target_link_libraries(leveldb-viewer PRIVATE leveldb::leveldb Qt6::Gui Qt6::Widgets)

if(APPLE OR WIN32)
  qt_generate_deploy_app_script(TARGET leveldb-viewer OUTPUT_SCRIPT deploy_script NO_TRANSLATIONS)
  install(SCRIPT ${deploy_script})
  set(CPACK_PACKAGE_NAME "LevelDBViewer")
  set(CPACK_PACKAGE_VENDOR "Tatsh")
  # FIXME get an icon
  if(APPLE)
    # FIXME fix name of the .app directory to be 'LevelDB Viewer'

    # FIXME set CFBundleIconFile

    # FIXME set CFBundleShortVersionString

    # FIXME set NSHumanReadableCopyright

    # FIXME set other metadata in Info.plist

    # FIXME signing issues
    set(CPACK_GENERATOR "DragNDrop;ZIP")
  else()
    # FIXME include license file in zip FIXME install path should not have version number

    # FIXME may need Visual C++ runtime

    # FIXME missing license in installer

    # FIXME NSIS does not create shortcut to app

    # FIXME Set publisher

    # FIXME Set 'file description'?

    # FIXME Set Product name

    # FIXME Set 'file version'?

    # FIXME Set Product version

    # FIXME Set Copyright

    # FIXME Set language

    # FIXME No UNIX-style packaging. Just have a single root.
    set(CPACK_GENERATOR "NSIS;ZIP")
  endif()
  include(CPack)
endif()
