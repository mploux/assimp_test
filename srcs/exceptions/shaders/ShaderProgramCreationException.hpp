//
// Created by Marc on 11/04/2018.
//

#pragma once

#include "ShaderException.hpp"

class ShaderProgramCreationException : public ShaderException
{
public:
	ShaderProgramCreationException();
	virtual ~ShaderProgramCreationException() throw() ;
};

