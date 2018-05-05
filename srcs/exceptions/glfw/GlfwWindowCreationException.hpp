//
// Created by Marc on 11/04/2018.
//

#pragma once

#include "GlfwException.hpp"

class GlfwWindowCreationException : public GlfwException
{
public:
	GlfwWindowCreationException();
	virtual ~GlfwWindowCreationException() throw() ;
};

