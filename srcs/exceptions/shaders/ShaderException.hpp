//
// Created by Marc on 11/04/2018.
//

#pragma once

#include "exceptions/VoxException.hpp"

class ShaderException : public VoxException
{
public:
	ShaderException();
	virtual ~ShaderException() throw() ;
};

