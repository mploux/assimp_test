//
// Created by Marc on 11/04/2018.
//

#include "GlfwException.hpp"

GlfwException::GlfwException()
	: VoxException()
{
	setName("GlfwException");
	setDescription("Unknown glfw exception");
}

GlfwException::~GlfwException() throw() {}