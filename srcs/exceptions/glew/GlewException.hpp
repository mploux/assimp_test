//
// Created by Marc on 11/04/2018.
//

#pragma once

#include "exceptions/VoxException.hpp"

class GlewException : public VoxException
{
public:
	GlewException();
	virtual ~GlewException() throw() ;
};

