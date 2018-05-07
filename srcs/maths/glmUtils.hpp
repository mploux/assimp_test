//
// Created by mploux on 07/05/18.
//

#pragma once

#include <assimp/matrix4x4.h>
#include <glm/mat4x4.hpp>

namespace glm_utils
{
	glm::mat4		assimp_to_glm(aiMatrix4x4 mat);
	aiMatrix4x4		glm_to_assimp(glm::mat4 mat);
};
