project(markov)

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

include(FindThreads)
include(CheckTypeSize)
include(CheckIncludeFile)
include(CheckCXXCompilerFlag)
include(ExternalProject)

CHECK_CXX_COMPILER_FLAG("-std=c++11" CXX11)
if (NOT CXX11)
    message(FATAL_ERROR "C++ compiler doesn't support C++11")
endif()

include_directories(${markov_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W")

find_package(Boost COMPONENTS serialization system locale)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "couldn't find all required boost components")
endif()

include_directories(${Boost_INCLUDE_DIRS})

set(SRCS    ${markov_SOURCE_DIR}/numerator.cpp
            ${markov_SOURCE_DIR}/model.cpp
            ${markov_SOURCE_DIR}/tokenizer.cpp
)

add_library(markov-static STATIC ${SRCS})
set_target_properties(markov-static PROPERTIES OUTPUT_NAME markov)
set_target_properties(markov-static PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(markov-static PROPERTIES COMPILE_FLAGS "-std=c++11")

SET(LINKLIBS
    markov-static
    ${Boost_LIBRARIES}
)

add_executable(mc-build mc-build.cpp)
set_target_properties(mc-build PROPERTIES COMPILE_FLAGS "-std=c++11")
target_link_libraries(mc-build markov-static ${LINKLIBS})

add_executable(mc-print mc-print.cpp)
set_target_properties(mc-print PROPERTIES COMPILE_FLAGS "-std=c++11")
target_link_libraries(mc-print markov-static ${LINKLIBS})

add_executable(mc-generate mc-generate.cpp)
set_target_properties(mc-generate PROPERTIES COMPILE_FLAGS "-std=c++11")
target_link_libraries(mc-generate markov-static ${LINKLIBS})

enable_testing()
add_subdirectory(tests)
