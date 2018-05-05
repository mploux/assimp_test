//
// Created by Marc on 11/04/2018.
//

#include "GlewException.hpp"

GlewException::GlewException()
	: VoxException()
{
	setName("GlewException");
	setDescription("Unknown glew exception");
}

GlewException::~GlewException() throw() {}