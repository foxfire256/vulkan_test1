cmake_minimum_required(VERSION 4.0)
# cmake version 3.0.0 is needed for block comments, 2.8.11 was the previous
# min version
project(vulkan_test1)

# http://www.cmake.org/Wiki/CMake_Useful_Variables

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories("../common-cpp")

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(MACOS TRUE)
endif()

# cmake .. -G "Visual Studio 16 2019" -A x64
# cmake .. -G "Visual Studio 17 2022" -A x64
if(MSVC)
	SET(CMAKE_C_FLAGS "/MP ${CMAKE_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS "/MP ${CMAKE_CXX_FLAGS}")

	set(Boost_USE_STATIC_LIBS OFF )
	set(Boost_USE_MULTITHREADED ON)
	set(Boost_USE_STATIC_RUNTIME OFF)

	set(LIBS ${LIBS} kernel32.lib)
endif(MSVC)

if(MINGW OR MSYS OR UNIX OR APPLE)
	if(APPLE)
		set(CMAKE_PREFIX_PATH "/usr/local")
		set(CMAKE_INCLUDE_PATH "/usr/local/include")
		set(CMAKE_LIBRARY_PATH "/usr/local/lib")
		include_directories("/usr/local/include")
	elseif(UNIX AND NOT APPLE)
		set(CMAKE_PREFIX_PATH "/usr")
		set(CMAKE_INCLUDE_PATH "/usr/include")
		set(CMAKE_LIBRARY_PATH "/usr/lib")
		include_directories("/usr/include")
	elseif(MINGW OR MSYS)
		set(CMAKE_PREFIX_PATH "/c/msys64/mingw64")
		set(CMAKE_INCLUDE_PATH "/c/msys64/mingw64/include")
		set(CMAKE_LIBRARY_PATH "/c/msys64/mingw64/lib")
	endif(APPLE)

	if(APPLE)
		find_package(OpenGL REQUIRED)
		set(LIBS ${LIBS} -L/usr/local/lib)
	elseif(UNIX AND NOT APPLE)
		set(LIBS ${LIBS})
	elseif(MINGW OR MSYS)
		set(LIBS ${LIBS} -lmingw32)
	endif(APPLE)
endif(MINGW OR MSYS OR UNIX OR APPLE)

set(SOURCE
	main.cpp
	gfx.hpp
	gfx.cpp
)

find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE})
target_link_libraries(${PROJECT_NAME}
	${LIBS}
	glfw
	Vulkan::Vulkan
)

