cmake_minimum_required(VERSION 2.8.4)
project(cpp)

# CMake FindThreads is broken until 3.1
#find_package(Threads REQUIRED)
set(CMAKE_THREAD_LIBS_INIT pthread)

enable_testing()
find_package(GTest)

include(ExternalProject)
if(NOT ${GTEST_FOUND}) # Download gtest if not installed
    message("No system gtest use external project")

    configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
        message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

    # The gtest/gtest_main targets carry header search path
    # dependencies automatically when using CMake 2.8.11 or
    # later. Otherwise we have to add them here ourselves.
    if (CMAKE_VERSION VERSION_LESS 2.8.11)
        include_directories("${gtest_SOURCE_DIR}/include")
    endif()

    set(GTEST_BOTH_LIBRARIES gtest gtest_main)
endif()

ExternalProject_Add(aprovaltest
    PREFIX ${CMAKE_BINARY_DIR}/aprovaltest
    URL https://github.com/approvals/ApprovalTests.cpp/releases/download/v.2.0.0/ApprovalTests.v.2.0.0.hpp
    DOWNLOAD_NO_EXTRACT 1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_BINARY_DIR}/aprovaltest/src)

add_executable(GildedRose
    GildedRose.cc
    GildedRose.h
    GildedRoseUnitTests.cc
    )
target_link_libraries(GildedRose ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME GildedRose COMMAND GildedRose)
set_property(TARGET GildedRose PROPERTY CXX_STANDARD 14)

add_executable(GildedRoseTextTests
    GildedRose.cc
    GildedRose.h
    GildedRoseTextTests.cc
    )
target_link_libraries(GildedRoseTextTests ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME GildedRoseTextTests COMMAND GildedRoseTextTests)

add_executable(GildedRoseApprovalTests
    GildedRose.cc
    GildedRose.h
    GildedRoseApprovalTests.cc
    GildedRoseApprovalMain.cc
    )
target_link_libraries(GildedRoseApprovalTests ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
set_property(TARGET GildedRoseApprovalTests PROPERTY CXX_STANDARD 14)
add_dependencies(GildedRoseApprovalTests aprovaltest)
add_test(NAME GildedRoseApprovalTests COMMAND GildedRoseApprovalTests)
