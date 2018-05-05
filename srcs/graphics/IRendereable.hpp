//
// Created by Marc on 11/04/2018.
//

#pragma once

#include "Shader.hpp"

class IRenderable
{
public:
	virtual void render(const Shader &shader) = 0;
};
