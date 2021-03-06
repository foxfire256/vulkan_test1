cmake_minimum_required(VERSION 3.0.0)
# cmake version 3.0.0 is needed for block comments, 2.8.11 was the previous
# min version
project(vulkan_test1)

# http://www.cmake.org/Wiki/CMake_Useful_Variables

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories("../common-cpp")

# this assumes you are running cmake from a build directory in the source directory
# cmake .. -G "Visual Studio 15 2017 Win64"
if(MSVC)
	ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
	ADD_DEFINITIONS(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
	ADD_DEFINITIONS(-DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)
	# WARNING: the following do not work with boost program_options
	#ADD_DEFINITIONS(-DBOOST_NO_RTTI)
	#ADD_DEFINITIONS(-DBOOST_NO_TYPEID)
	
	# /Oi enable intrinsic functions
	# /GR- disable RTTI, can't do with boost program_options
	# /std:c++17 or /std:c++14 are options
	SET(CMAKE_C_FLAGS "/MP ${CMAKE_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS "/MP /std:c++latest /Oi ${CMAKE_CXX_FLAGS}")
	
	set(LIBS ${LIBS} kernel32.lib)
	set(LIBS ${LIBS} "C:\\VulkanSDK\\1.1.82.1\\Lib\\vulkan-1.lib")
	set(LIBS ${LIBS}
		optimized "C:\\vclibs\\vclibs\\lib\\glfw3.lib"
		debug "C:\\vclibs\\vclibs\\lib\\glfw3d.lib")
	
	include_directories("C:\\vclibs\\vclibs\\include")
	include_directories("C:\\VulkanSDK\\1.1.82.1\\Include")
endif(MSVC)

if(MINGW OR MSYS)
	set(CMAKE_PREFIX_PATH "c:/msys64/mingw64")
	set(CMAKE_INCLUDE_PATH "c:/msys64/mingw64/include")
	set(CMAKE_LIBRARY_PATH "c:/msys64/mingw64/lib")
	
	set(LIBS ${LIBS} -lmingw32 -lvulkan -lglfw3)
	
	include_directories("c:/msys64/mingw64/include/eigen3")
endif(MINGW OR MSYS)

if(APPLE)
	set(CMAKE_PREFIX_PATH "/usr/local")
	set(CMAKE_INCLUDE_PATH "/usr/local/include")
	set(CMAKE_LIBRARY_PATH "/usr/local/lib")
	
	set(LIBS ${LIBS} -L/usr/local/lib  -lvulkan -lglfw3)
	
	include_directories("/usr/local/include")
	include_directories("/usr/local/include/eigen3")
endif(APPLE)

# this probably only works on openSUSE Tumbleweed
if(UNIX AND NOT APPLE)
	set(CMAKE_PREFIX_PATH "/usr")
	set(CMAKE_INCLUDE_PATH "/usr/include")
	set(CMAKE_LIBRARY_PATH "/usr/lib")

	set(LIBS ${LIBS}  -lvulkan -lglfw3)

	include_directories("/usr/include")
	include_directories("/usr/include/eigen3")

endif(UNIX AND NOT APPLE)


if(NOT MSVC)
	# better optimizations
	# -O3 still IEEE754 compliant
	# -Ofast not IEEE754 compliant, faster than -O3
	# -ffast-math not needed with -Ofast
	# -ftree-vectorize not sure how well this does
	# -mavx
	# -mavx2
	set(CMAKE_C_FLAGS_RELEASE "-O3")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	
	# force some better debugging
	# this should be gcc
	# WARNING: warnings are disabled
	if(NOT APPLE)
		set(CMAKE_C_FLAGS_DEBUG "-ggdb3 -Og")
		set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -Og -w")
	endif(NOT APPLE)
	# and this should be clang
	# WARNING: warnings are disabled on clang
	if(APPLE)
		set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
		set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -w")
	endif(APPLE)

	# enable C11/C++14 for gcc and clang
	set(EXTRA_CXX_FLAGS "-std=c++14 -Wno-deprecated-declarations -pipe")
	set(EXTRA_C_FLAGS "-std=c11 -Wno-deprecated-declarations -pipe")
	
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")
	
	# pipe keeps compilation in memory
	set(CMAKE_LD_FLAGS "-pipe")
endif(NOT MSVC)

set(SOURCE
	main.cpp
	gfx.hpp
	gfx.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE})
target_link_libraries(${PROJECT_NAME} ${LIBS})

MESSAGE( STATUS "MINGW: " ${MINGW} )
MESSAGE( STATUS "MSYS: " ${MSYS} )
MESSAGE( STATUS "MSVC: " ${MSVC} )
MESSAGE( STATUS "APPLE: " ${APPLE} )
MESSAGE( STATUS "INCLUDE_DIRECTORIES: " ${INCLUDE_DIRECTORIES} )
MESSAGE( STATUS "CMAKE_C_FLAGS: " ${CMAKE_C_FLAGS} )
MESSAGE( STATUS "CMAKE_C_FLAGS_DEBUG: " ${CMAKE_C_FLAGS_DEBUG} )
MESSAGE( STATUS "CMAKE_C_FLAGS_RELEASE: " ${CMAKE_C_FLAGS_RELEASE} )
MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )
MESSAGE( STATUS "CMAKE_CXX_FLAGS_DEBUG: " ${CMAKE_CXX_FLAGS_DEBUG} )
MESSAGE( STATUS "CMAKE_CXX_FLAGS_RELEASE: " ${CMAKE_CXX_FLAGS_RELEASE} )

