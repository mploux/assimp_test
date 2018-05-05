//
// Created by Marc on 11/04/2018.
//

#include "GlewInitException.hpp"

GlewInitException::GlewInitException()
	: GlewException()
{
	setName("GlewInitException");
	setDescription("Unable to initialise GLEW");
}

GlewInitException::~GlewInitException() throw() {}