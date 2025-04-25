# CMAKE_SYSTEM_PROCESSOR - CPU architecture name (armv7l, aarch64, x86_64)
# CMAKE_BUILD_TYPE       - Build mode (debug, profile, release)
# PSDK_VERSION           - Build psdk exact version
# PSDK_MAJOR             - Build psdk major version

cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME flutter_callkit_incoming)
set(PLUGIN_NAME  flutter_callkit_incoming_platform_plugin)

project(${PROJECT_NAME} LANGUAGES CXX)

find_package (Qt5 COMPONENTS Core Network Qml Gui Quick DBus Multimedia LinguistTools REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-psabi")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(PkgConfig REQUIRED)
pkg_check_modules(FlutterEmbedder REQUIRED IMPORTED_TARGET flutter-embedder)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(HEADERS
    include/flutter_callkit_incoming/callmanager.h
    include/flutter_callkit_incoming/callmanagerdbusadaptor.h
    include/flutter_callkit_incoming/call1dbusobject.h
    include/flutter_callkit_incoming/call1dbusadaptor.h
    include/flutter_callkit_incoming/callservice1dbusinterface.h
    include/flutter_callkit_incoming/aurora_params.h
    include/flutter_callkit_incoming/dbustypes.h
    include/flutter_callkit_incoming/globals.h
    include/flutter_callkit_incoming/call_event.h
    include/flutter_callkit_incoming/utils.h
    include/flutter_callkit_incoming/flutter_callkit_incoming_plugin.h
)

set(SOURCES
    flutter_callkit_incoming_plugin.cpp
    callmanager.cpp
    callmanagerdbusadaptor.cpp
    call1dbusobject.cpp
    call1dbusadaptor.cpp
    callservice1dbusinterface.cpp
    aurora_params.cpp
    utils.cpp
)

add_library(${PLUGIN_NAME} SHARED ${SOURCES} ${HEADERS})

set_target_properties(${PLUGIN_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
target_link_libraries(${PLUGIN_NAME} PRIVATE PkgConfig::FlutterEmbedder)
target_link_libraries(${PLUGIN_NAME} PUBLIC
    Qt5::DBus
    Qt5::Multimedia
    Qt5::Quick
    ${AURORA_LDFLAGS}
)

target_include_directories(${PLUGIN_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${Qt5Core_INCLUDE_DIRS} ${Qt5DBus_INCLUDE_DIRS})
target_compile_definitions(${PLUGIN_NAME} PRIVATE PLUGIN_IMPL)
