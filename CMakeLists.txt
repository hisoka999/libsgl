cmake_minimum_required(VERSION 3.8)

project(sgl VERSION 0.1 DESCRIPTION "simple game library")


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(MINGW)
    set(SDL2_LIBRARY sdl2)
    set(SDL2MAIN_LIBRARY SDL2main)
    set(SDL2_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2)
    set(SDL2_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2/include)

    set(SDL2_IMAGE_LIBRARY sdl2_image)
    set(SDL2_IMAGE_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2)
    set(SDL2_IMAGE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2/include)

    set(SDL2TTF_LIBRARY sdl2ttf)
    set(SDL2TTF_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2)
    set(SDL2TTF_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2/include)

    set(SDL2_MIXER_LIBRARY sdl2ttf)
    set(SDL2_MIXER_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2)
    set(SDL2_MIXER_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2/include)

    set(Intl_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/gettext/include )

    #message("${PROJECT_SOURCE_DIR}/deps/gettext/lib/")

    link_directories(${SDL2_DIR}/lib/ ${PROJECT_SOURCE_DIR}/deps/gettext/lib/ )
elseif (WIN32)
    set(SDL2_LIBRARY sdl2)
    set(SDL2MAIN_LIBRARY SDL2main)
    set(SDL2_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2)
    set(SDL2_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2/include)

    set(SDL2_IMAGE_LIBRARY sdl2_image)
    set(SDL2_IMAGE_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2)
    set(SDL2_IMAGE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2/include)

    set(SDL2TTF_LIBRARY sdl2ttf)
    set(SDL2TTF_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2)
    set(SDL2TTF_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2/include)

    set(SDL2_MIXER_LIBRARY SDL2_mixer)
    set(SDL2_MIXER_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2)
    set(SDL2_MIXER_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2/include)

    link_directories(${SDL2_DIR}/lib/x64)
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_gfx)
find_package(SDL2TTF REQUIRED)
find_package(ZLIB)
#find_package(Intl)


#FIND_PACKAGE(Boost COMPONENTS locale)
message( "${MINGW32_LIBRARY} ${SDL2_LIBRARIES}")


INCLUDE_DIRECTORIES()
#file(GLOB_RECURSE SRC battle *.cpp)
#file(GLOB_RECURSE SRC character *.cpp)
#file(GLOB_RECURSE SRC dialogs *.cpp)
#file(GLOB_RECURSE SRC engine/* *.cpp)

file(GLOB_RECURSE SRC
    engine/*)
#message ("${SRC}")



set (MAGIC_ENUM_ROOT "deps")
set (MAGIC_ENUM_SRC "${PROJECT_BINARY_DIR}/${MAGIC_ENUM_ROOT}/magic_enum/magic_enum-src/include" )
include(magic_enum)
fetch_magic_enum(
    ${PROJECT_SOURCE_DIR}/cmake
    ${PROJECT_BINARY_DIR}/${MAGIC_ENUM_ROOT}/magic_enum
    )


message(${MAGIC_ENUM_SRC})

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}" ${SDL2_INCLUDE_DIR} ${GETTEXT_INCLUDE_DIR} ${Intl_INCLUDE_DIR} ${MAGIC_ENUM_SRC})
if (WIN32)
    set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set (WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()


add_library(${PROJECT_NAME} SHARED ${SRC})





if( UNIX)
option(ENGINE_TESTS "Build the tests" ON)
if(ENGINE_TESTS)

    include(googletest)
    include(GoogleTest)
    fetch_googletest(
        ${PROJECT_SOURCE_DIR}/cmake
        ${PROJECT_BINARY_DIR}/googletest
        )
    enable_testing() 
    add_subdirectory(tests)
    #add_executable(SimpleTest )


endif()
endif()
if(MINGW)
    IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
        message("debug mode")
        target_link_libraries(${PROJECT_NAME} SDL2)
    else()
        target_link_libraries(${PROJECT_NAME} mingw32 SDL2main SDL2 -mwindows)
    ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)
elsif(WIN32)
    target_link_libraries(${PROJECT_NAME} SDL2main SDL2 -mwindows)
else()
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
endif()


target_link_libraries(${PROJECT_NAME} SDL2_ttf)
target_link_libraries(${PROJECT_NAME} SDL2_image)
target_link_libraries(${PROJECT_NAME} SDL2_mixer)
if(UNIX)

endif()

if( UNIX)
target_link_libraries(${PROJECT_NAME} SDL2_gfx)
target_link_libraries(${PROJECT_NAME} pthread)
endif()


INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_NAME})

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl2-2.0-0 (>= 2.0), libsdl2-ttf-2.0-0 (>= 2.0), libsdl2-image-2.0-0 (>= 2.0), libsdl2-gfx-2.0-0 (>= 2.0), libsdl2-mixer-2.0-0 (>= 2.0)")
set(CPACK_PACKAGE_VERSION "${GAME_VERSION_MAJOR}.${GAME_VERSION_MINOR}")
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Stefan Lüdtke") #required
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "sota is a 2d isometric economy simulation game.")
set(CPACK_DEBIAN_PACKAGE_SECTION "games")
INCLUDE(CPack)

