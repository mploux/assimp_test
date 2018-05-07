cmake_minimum_required(VERSION 3.5)

set(NAME ft_vox)

project(${NAME})

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(SOURCE_FILES
	srcs/main.cpp
	srcs/Core.cpp
	srcs/Core.hpp
	srcs/exceptions/VoxException.cpp
	srcs/exceptions/VoxException.hpp
	srcs/exceptions/glfw/GlfwException.cpp
	srcs/exceptions/glfw/GlfwException.hpp
	srcs/exceptions/glfw/GlfwInitException.cpp
	srcs/exceptions/glfw/GlfwInitException.hpp
	srcs/exceptions/glfw/GlfwWindowCreationException.cpp
	srcs/exceptions/glfw/GlfwWindowCreationException.hpp
	srcs/exceptions/glew/GlewException.cpp
	srcs/exceptions/glew/GlewException.hpp
	srcs/exceptions/glew/GlewInitException.cpp
	srcs/exceptions/glew/GlewInitException.hpp
	srcs/exceptions/shaders/ShaderException.cpp
	srcs/exceptions/shaders/ShaderException.hpp
	srcs/exceptions/shaders/ShaderProgramCreationException.cpp
	srcs/exceptions/shaders/ShaderProgramCreationException.hpp
	srcs/exceptions/shaders/ShaderCreationException.cpp
	srcs/exceptions/shaders/ShaderCreationException.hpp
	srcs/graphics/Display.cpp
	srcs/graphics/Display.hpp
	srcs/world/World.cpp
	srcs/world/World.hpp
	srcs/graphics/IRendereable.hpp
	srcs/graphics/Shader.cpp
	srcs/graphics/Shader.hpp
	srcs/utils/FileUtils.cpp
	srcs/utils/FileUtils.hpp
	srcs/world/chunks/Chunk.cpp srcs/world/chunks/Chunk.hpp
	srcs/world/blocks/ABlock.cpp
	srcs/world/blocks/ABlock.hpp
	srcs/world/blocks/StoneBlock.cpp
	srcs/world/blocks/StoneBlock.hpp
	srcs/world/blocks/EmptyBlock.cpp
	srcs/world/blocks/EmptyBlock.hpp
	srcs/graphics/Camera.cpp
	srcs/graphics/Camera.hpp
	srcs/maths/Vec3.hpp
	srcs/maths/Vec2.hpp
	srcs/maths/Mat4.hpp
	srcs/maths/Maths.hpp
	srcs/inputs/Input.cpp
	srcs/inputs/Input.hpp
	srcs/graphics/textures/Texture.cpp
	srcs/graphics/textures/Texture.hpp
	srcs/graphics/textures/TextureDDS.cpp
	srcs/world/blocks/TntBlock.cpp
	srcs/world/blocks/TntBlock.hpp
	srcs/world/blocks/GlassBlock.cpp
	srcs/world/blocks/GlassBlock.hpp
	srcs/world/blocks/Blocks.cpp
	srcs/world/blocks/Blocks.hpp
	srcs/world/chunks/ChunkRendererG24.cpp
	srcs/world/chunks/ChunkRendererG4.cpp
	srcs/world/chunks/ChunkRendererG4.hpp
	srcs/world/chunks/ChunkRendererG24.hpp
	srcs/world/chunks/ChunkRenderer.cpp
	srcs/world/chunks/ChunkRenderer.hpp
        srcs/graphics/Model.cpp srcs/graphics/Model.hpp srcs/graphics/ModelSkeleton.cpp srcs/graphics/ModelSkeleton.hpp srcs/graphics/ModelBone.cpp srcs/graphics/ModelBone.hpp srcs/graphics/ModelAnimation.cpp srcs/graphics/ModelAnimation.hpp srcs/maths/glmUtils.hpp srcs/maths/glmUtils.cpp)

find_package(OpenGL REQUIRED)

add_subdirectory(deps/glm)

add_subdirectory(deps/assimp-4.1.0)

add_definitions(-DGLEW_STATIC)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(deps/glfw)

set(BUILD_UTILS OFF CACHE BOOL "" FORCE)
add_subdirectory(deps/glew/build/cmake)

include_directories(srcs/)
add_executable(${NAME} ${SOURCE_FILES})

target_include_directories(${NAME} PUBLIC ${OPENGL_INCLUDE_DIR} deps/assimp-4.1.0/include deps/glm deps/glfw/include deps/glew/include)
target_link_libraries(${NAME} ${OPENGL_gl_LIBRARY} assimp glm glfw glew)


if (WIN32)
#	add_custom_command(TARGET ${NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
#					   ${CMAKE_BINARY_DIR}/bin/glew32.dll
#					   $<TARGET_FILE_DIR:${NAME}>)
	add_custom_command(TARGET ${NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_BINARY_DIR}/bin/glew32d.dll
		$<TARGET_FILE_DIR:${NAME}>)
endif (WIN32)
