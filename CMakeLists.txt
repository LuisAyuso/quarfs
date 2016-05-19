project(quarfs)
cmake_minimum_required(VERSION 2.8)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/code/cmake")

FIND_PACKAGE( Boost 1.40 COMPONENTS filesystem system REQUIRED )
FIND_PACKAGE( GLEW REQUIRED )
FIND_PACKAGE( OpenGL REQUIRED )
FIND_PACKAGE( GLFW REQUIRED )
FIND_PACKAGE( GLM REQUIRED )



INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${GLEW_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${OPENGL_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${GLFW_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${GLM_INCLUDE_DIR} )
include_directories ( "code/include" )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/code/src/*.cpp" )
add_executable(${PROJECT_NAME} ${sources}  code/src/main.cxx)


TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${Boost_LIBRARIES} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${GLEW_LIBRARIES} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${OPENGL_LIBRARIES} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${GLFW_LIBRARIES} )


# tests:
# ======

include (testing)

file(GLOB_RECURSE test_sources "${CMAKE_CURRENT_SOURCE_DIR}/code/tests/*.cc" )

foreach( test_source ${test_sources} )

    get_filename_component (unit_test ${test_source} NAME_WE )

    INCLUDE_DIRECTORIES( ${GTEST_INCLUDE_DIR} )
    INCLUDE_DIRECTORIES( ${GMOCK_INCLUDE_DIR} )


    ADD_DEFINITIONS(-DGTEST_COLOR ) 
    ADD_EXECUTABLE (${unit_test} ${test_source})

    TARGET_LINK_LIBRARIES( ${unit_test} libgtest libgtest_main libgmock)
    TARGET_LINK_LIBRARIES( ${unit_test} ${Boost_LIBRARIES} )
    TARGET_LINK_LIBRARIES( ${unit_test} ${GLEW_LIBRARIES} )
    TARGET_LINK_LIBRARIES( ${unit_test} ${OPENGL_LIBRARIES} )
    TARGET_LINK_LIBRARIES( ${unit_test} ${GLFW_LIBRARIES} )



    add_test (${unit_test} ${unit_test} )

endforeach(test_source)


enable_testing()
