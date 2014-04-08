project(scratchvaders)
cmake_minimum_required(VERSION 2.8)
include_directories(${CMAKE_SOURCE_DIR}/include)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")


set(sv_HDRS
	include/Game/shaders/Planet.h
        include/Game/AmmoManager.h
        include/Game/AudioIn.h
        include/Game/BaseEntity.h
        include/Game/DefenceBlock.h
        include/Game/DefenceManager.h
        include/Game/DefenceManager.h
        include/Game/Game.h
        include/Game/Invader.h
        include/Game/InvaderManager.h
        include/Game/Player.h
        include/Game/StarField.h
        include/Helpers.h)

set(sv_SRCS
	src/Ammo.cpp
        src/AmmoManager.cpp
        src/AudioIn.cpp
        src/DefenceBlock.cpp
        src/DefenceManager.cpp
        src/Game.cpp
	src/Invader.cpp
	src/InvaderManager.cpp
	src/Player.cpp
	src/Random.cpp
	src/StarField.cpp)

add_library(${PROJECT_NAME} SHARED ${sv_HDRS} ${sv_SRCS})


find_package(SFML 2 REQUIRED system window graphics audio)


add_executable(scratchvaders main.cpp)
target_link_libraries(BenchMark ${PROJECT_NAME})
target_link_libraries(BenchMark ${SFML_LIBRARIES})


	

