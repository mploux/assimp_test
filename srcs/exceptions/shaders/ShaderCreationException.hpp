//
// Created by Marc on 11/04/2018.
//

#pragma once

#include "ShaderException.hpp"

class ShaderCreationException : public ShaderException
{
public:
	ShaderCreationException();
	virtual ~ShaderCreationException() throw() ;
};

