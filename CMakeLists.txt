cmake_minimum_required( VERSION 3.20 )

project( ShaderPack LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

if( NOT WIN32 )
    message( FATAL_ERROR "Shader Pack builds on Windows." )
endif( )

set( UR_BUILD_DEMO OFF CACHE BOOL "" FORCE )
if( DEFINED ENV{UR_ROOT} AND EXISTS "$ENV{UR_ROOT}/CMakeLists.txt" )
    set( UR_ROOT "$ENV{UR_ROOT}" CACHE PATH "ur root" )
elseif( EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../custom-framework/CMakeLists.txt" )
    set( UR_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../custom-framework" CACHE PATH "ur root" )
else( )
    set( UR_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../etc/custom-framework" CACHE PATH "ur root" )
endif( )

add_subdirectory( "${UR_ROOT}" "${CMAKE_BINARY_DIR}/ur" )

add_executable( shader-pack WIN32 src/Main.cxx )
target_link_libraries( shader-pack PRIVATE ur )
target_include_directories( shader-pack PRIVATE include src )

if( MSVC )
    target_compile_options( shader-pack PRIVATE /W4 /utf-8 /EHsc )
endif( )

set_target_properties( shader-pack PROPERTIES
    OUTPUT_NAME "ShaderPack"
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_command( TARGET shader-pack POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${UR_ROOT}/assets"
        "$<TARGET_FILE_DIR:shader-pack>/assets"
)
