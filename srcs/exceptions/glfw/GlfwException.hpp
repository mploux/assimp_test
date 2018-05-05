//
// Created by Marc on 11/04/2018.
//

#pragma once

#include "exceptions/VoxException.hpp"

class GlfwException : public VoxException
{
public:
	GlfwException();
	virtual ~GlfwException() throw() ;
};

