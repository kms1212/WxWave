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
add_executable(wxwave ${WXWAVE_SOURCES})

file(COPY ${CMAKE_SOURCE_DIR}/assets/xsd DESTINATION ${CMAKE_BINARY_DIR})

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

add_subdirectory(lib)
