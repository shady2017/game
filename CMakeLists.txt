cmake_minimum_required(VERSION 3.0.0)

project(All)

cmake_policy(SET CMP0051 NEW)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake/cmake_modules")

set(UCM_UNITY_BUILD         OFF CACHE BOOL "" FORCE)
set(UCM_NO_COTIRE_FOLDER    OFF CACHE BOOL "" FORCE)

option(WITH_PLUGINS "Compile plugins as separate .dll files instead of merging them with the executable" ON)

# FOR DEBUGGING BUILDS
#set(WITH_PLUGINS OFF CACHE BOOL "" FORCE)

set(COTIRE_PCH_MEMORY_SCALING_FACTOR 500)
include(third_party/ucm/cmake/ucm.cmake)
include(scripts/cmake/compiler_flags.cmake)
include(scripts/cmake/utils.cmake)

# copy a .user file that will make F5 work (will start the main executable) - see https://stackoverflow.com/questions/44699100
if(MSVC AND NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/ALL_BUILD.vcxproj.user")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake/ALL_BUILD.vcxproj.user" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()

# always build with sanitizers for clang (for now...)
# gcc has problems when loading shared objects which are built with sanitizer checks
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND NOT TOOLCHAIN STREQUAL "js")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer -g -fuse-ld=gold")
endif()

#===================================================================================================
#== OUTPUT DIRS ====================================================================================
#===================================================================================================

set(bin_dir ${TOOLCHAIN})
if(${CMAKE_GENERATOR} STREQUAL "Ninja")
    set(bin_dir "ninja")
endif()

set(OUTPUT_DIR ${PROJECT_BINARY_DIR}/../../bin/${bin_dir}/${CMAKE_BUILD_TYPE})
if(MSVC AND NOT ${CMAKE_GENERATOR} STREQUAL "Ninja")
    set(OUTPUT_DIR ${PROJECT_BINARY_DIR}/../../bin/${bin_dir})
endif()

# for the generic no-config case
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

#===================================================================================================
#== INCLUDE PATHS ==================================================================================
#===================================================================================================

set(BOOST_DIR "boost_1_66_0")
set(GLEW_DIR "glew-1.13.0")

# this is for find_package() calls
set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/third_party/${BOOST_DIR}" CACHE PATH "Boost path")

include_directories("third_party/${BOOST_DIR}")
include_directories("third_party/yama/include")

if(TOOLCHAIN STREQUAL "js")
    include_directories("emscripten/emscripten/1.37.9/system/include")
    set(WITH_PLUGINS OFF)
    ucm_add_flags(-s TOTAL_MEMORY=268435456) # 256 MB - must be a multiple of 16 MB
    ucm_add_flags(-s USE_GLFW=3)
    ucm_add_flags(--emrun)
    #ucm_add_flags(-s USE_WEBGL2=1 -s FULL_ES3=1)
    #ucm_add_flags(-s PRECISE_F32=1)
    ucm_add_flags(--preload-file "${CMAKE_CURRENT_SOURCE_DIR}/data@/") # @/ means "mount at root"
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        ucm_add_flags(-s SAFE_HEAP=1)
        ucm_add_flags(-s DEMANGLE_SUPPORT=1 -s ASSERTIONS=2 -s NO_EXIT_RUNTIME=1)
    endif()

    add_definitions(-DDOCTEST_CONFIG_DISABLE)

    # add this because dynamix uses it
    include_directories("${EMSCRIPTEN_ROOT}/system/lib/libcxxabi/include")
else()
    include_directories("third_party/${GLEW_DIR}/include")
endif()

#===================================================================================================
#== RECURSE THROUGH CMAKE ==========================================================================
#===================================================================================================

add_subdirectory(third_party)
add_subdirectory(src)
















