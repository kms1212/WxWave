cmake_minimum_required(VERSION 3.12)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include (TestBigEndian)
include (CheckSymbolExists)

find_package(LibXml2 REQUIRED)
find_package(nlohmann_json 3.10.0 REQUIRED)
find_package(wxWidgets REQUIRED COMPONENTS core base)

include (${wxWidgets_USE_FILE})

project("WxWave")

file(GLOB WXWAVE_SOURCES
    "${CMAKE_SOURCE_DIR}/src/*"
    "${CMAKE_SOURCE_DIR}/src/file/*"
    "${CMAKE_SOURCE_DIR}/src/ui/*")
# add_executable(wxwave MACOSX_BUNDLE ${WXWAVE_SOURCES})
add_executable(wxwave ${WXWAVE_SOURCES})

target_compile_features(wxwave PRIVATE c_std_11)
target_compile_features(wxwave PRIVATE cxx_std_17)

add_compile_options(
    -Wall -Werror
    -Wno-unused-function -Wno-unused-variable -Wno-unused-private-field
    -pedantic -pedantic-errors)

target_include_directories(wxwave PRIVATE "/opt/ghdl/include")
target_include_directories(wxwave PRIVATE "${CMAKE_SOURCE_DIR}/lib/libwxwave/include")
target_include_directories(wxwave PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_include_directories(wxwave PRIVATE "${CMAKE_BINARY_DIR}")
target_include_directories(wxwave PUBLIC ${LIBXML2_INCLUDE_DIRS})

target_link_libraries(wxwave PUBLIC "/opt/ghdl/lib/libghw.dylib")
target_link_libraries(wxwave PUBLIC ${LIBXML2_LIBRARIES})
target_link_libraries(wxwave PUBLIC ${wxWidgets_LIBRARIES})
target_link_libraries(wxwave PUBLIC nlohmann_json::nlohmann_json)

set_target_properties(wxwave PROPERTIES
    BUILD_RPATH "/opt/ghdl/"
    INSTALL_RPATH "/opt/ghdl/"
)
set_target_properties(wxwave PROPERTIES
    BUNDLE True
	MACOSX_BUNDLE_INFO_STRING                       ""
	MACOSX_BUNDLE_ICON_FILE                         ""
    MACOSX_BUNDLE_GUI_IDENTIFIER                    com.kms1212.wxwave
    MACOSX_BUNDLE_BUNDLE_NAME                       WxWave
    MACOSX_BUNDLE_BUNDLE_VERSION                    "0.1"
    MACOSX_BUNDLE_LONG_VERSION_STRING               "0.1"
    MACOSX_BUNDLE_SHORT_VERSION_STRING              "0.1"
    MACOSX_BUNDLE_COPYRIGHT                         "Copyright © 2025 by kms1212, All rights reserved"
    MACOSX_BUNDLE_INFO_PLIST                        ${CMAKE_SOURCE_DIR}/dist/macosx_bundle_info.plist.in
)

add_subdirectory(lib)
