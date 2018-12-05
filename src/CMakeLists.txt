project(algotest)
# cmake -G "Eclipse CDT4 - Unix Makefiles" -D_ECLIPSE_VERSION=4.2 -DCMAKE_BUILD_TYPE=Debug ../build
cmake_minimum_required( VERSION 2.6 )

set(PROJNAME algotests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# check 64 bit
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( ARCH x64 )
else()
    set( ARCH x86 )
endif()

if( NOT CMAKE_BUILD_TYPE )
	set(CMAKE_BUILD_TYPE Debug)
endif()

if (CMAKE_BUILD_TYPE STREQUAL Debug)
	set( BUILDTYPE debug)
else()
	set( BUILDTYPE release)
endif()
message("*** ${ARCH}/${BUILDTYPE} ***")

# boost libraries
#set (BOOST_LIBS system program_options)

if( NOT Boost_DIR )
    if( EXISTS $ENV{BOOST_DIR} )
        set(Boost_DIR $ENV{BOOST_DIR})
        #message("will use boost path in env $ENV{BOOST_DIR}")
    else()
        #message("not set boost path")
    endif()
endif()
set(INCLUDES ${INCLUDES} ${Boost_DIR}/include)
#set(LIBPATHS ${LIBPATHS} ${Boost_DIR}/lib)
set(LIBPATHS ${LIBPATHS} ${Boost_DIR}/lib/${ARCH}/${BUILDTYPE})
if( NOT WIN32 )
	foreach(F ${BOOST_LIBS})
		set(LIBS ${LIBS} boost_${F})
	endforeach()
endif()
#find_library(Boost_LIBRARIES NAMES boost_system ${Boost_DIR}/lib/${ARCH}/${BUILDTYPE})
#set(LIBS ${LIBS} ${Boost_DIR}/lib/${ARCH}/${BUILDTYPE}/boost_system)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

add_definitions( -DUNITTEST_MODE -DBOOST_ALL_DYN_LINK -std=c++17)
#add_definitions( -DTEST_MODE -DBOOST_ALL_DYN_LINK -std=c++17)

LINK_DIRECTORIES(${LIBPATHS})

message("- link libs: ${LIBS}")
message("- link dirs: ${LIBPATHS}")

#----------------------------------



if( NOT WIN32 )
        set(LIBS ${LIBS} -lrt -lpthread)
endif()

set(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/src )

message("- INCLUDES dirs: ${INCLUDES}")

include_directories(${INCLUDES} )

file(GLOB SRC
	#    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        # include app.cpp if defined UNITTEST_MODE
        app.cpp
#        *.cpp
#        ReversePair.cpp
#        DIStringMatch.cpp
#        LargestRectangleArea.cpp
#        DistinctSubsequencesII.cpp
#        LargestComponentSize.cpp
#        MinScalarProduct.cpp
#        ShortestSubarrayWithSumAtLeastK.cpp
#        SwimInWater.cpp
        LongestConsecutive.cpp
)

add_executable( ${PROJNAME} ${SRC} )

target_link_libraries( ${PROJNAME} ${LIBS} ftl try_catch )


#-----------------------------------------------

function(abs_path_to_compiler_lib path lib)
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=${lib}
        COMMAND xargs readlink -f
        OUTPUT_VARIABLE abs_path
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(${path} ${abs_path} PARENT_SCOPE)
endfunction()

function(copy_compiler_lib lib)
    # The absolute path will be to the actual file (with all links resolved)
    # Link: libc++.so.1
    # Actual file: libc++.so.1.0

    # So, we copy the file and then rename to the name we link against
    # CMake doesn't seem to have a nice way to copy and rename in one step
    abs_path_to_compiler_lib(p ${lib})
    get_filename_component(so_name ${p} NAME)
    message(STATUS "Copying ${p} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${so_name}")
    file(COPY ${p} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    message(STATUS "Renaming ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${so_name} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${lib}")
    file(RENAME "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${so_name}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${lib}")
endfunction()

function(copy_compiler_libs_to_output_dir)
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
        copy_compiler_lib(libstdc++.so.6)
        copy_compiler_lib(libgcc_s.so.1)
        copy_compiler_lib(libcilkrts.so.5)

        if (USE_ASAN)
            if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.1)
                copy_compiler_lib(libasan.so.3)
            else()
                copy_compiler_lib(libasan.so.4)
            endif()
        endif()

        if (USE_TSAN)
            copy_compiler_lib(libtsan.so.0)
        endif()

        if (USE_USAN)
            copy_compiler_lib(libubsan.so.0)
        endif()
    elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
        copy_compiler_lib(libc++.so.1)
        copy_compiler_lib(libc++abi.so.1)
    else()
        return()
    endif()
endfunction()

function(copy_shared_lib_to_runtime_dir path)
    execute_process(
        COMMAND readlink -e ${path}
        RESULT_VARIABLE result
        ERROR_VARIABLE err
        OUTPUT_VARIABLE abs_path
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(NOT result STREQUAL "0")
        message(FATAL_ERROR "Unable to determine path (${path}): result=${result}, err=${err}")
    endif()

    get_filename_component(so_name ${abs_path} NAME)
    message(STATUS "Copying ${abs_path} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${so_name}")
    file(COPY ${abs_path} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    # If `path` was a symlink then rename to the symlinked name
    get_filename_component(final_name "${path}" NAME)
    message(STATUS "Renaming ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${so_name} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${final_name}")
    file(RENAME "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${so_name}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${final_name}")
endfunction()

copy_compiler_libs_to_output_dir()
