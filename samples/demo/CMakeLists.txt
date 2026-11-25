cmake_minimum_required(VERSION 3.12)
project(demo VERSION 1.3.0 LANGUAGES CXX)

set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS version")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/demo.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/libpdfsolidconversionsdk.dylib
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/../../lib"
)
