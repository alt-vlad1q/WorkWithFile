cmake_minimum_required(VERSION 3.13)
project(WorkWithFile)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -Wall -Werror=unused-result -Werror=return-type -lrt")
set(CMAKE_CXX_FLAGS_DEBUG    "${CMAKE_CXX_FLAGS_RELEASE} -g")
set(CMAKE_CXX_FLAGS          "${CMAKE_CXX_FLAGS_RELEASE}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/lib)

add_executable(${PROJECT_NAME} main.cpp)

find_package(Boost REQUIRED COMPONENTS iostreams)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

find_package(Qt5 REQUIRED COMPONENTS Core Quick)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick)

find_package (Threads)
target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(${PROJECT_NAME} rt)

#find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS ${QTDIR} ENV QTDIR PATH_SUFFIXES bin)
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${PROJECT_NAME}>)

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#COMMAND ${WINDEPLOYQT_EXECUTABLE} --qmldir ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
