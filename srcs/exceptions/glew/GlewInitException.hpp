//
// Created by Marc on 11/04/2018.
//

#pragma once

#include "GlewException.hpp"

class GlewInitException : public GlewException
{
public:
	GlewInitException();
	virtual ~GlewInitException() throw() ;
};

