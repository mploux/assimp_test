//
// Created by Marc on 11/04/2018.
//

#pragma once

#include "GlfwException.hpp"

class GlfwInitException : public GlfwException
{
public:
	GlfwInitException();
	virtual ~GlfwInitException() throw() ;
};

