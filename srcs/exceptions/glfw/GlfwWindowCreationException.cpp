//
// Created by Marc on 11/04/2018.
//

#include "GlfwWindowCreationException.hpp"

GlfwWindowCreationException::GlfwWindowCreationException()
	: GlfwException()
{
	setName("GlfwWindowCreationException");
	setDescription("Unable to create a GLFW window");
}

GlfwWindowCreationException::~GlfwWindowCreationException() throw() {}
