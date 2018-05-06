//
// Created by mploux on 05/05/18.
//

#pragma once

#include <assimp/anim.h>
#include <map>
#include <assimp/scene.h>
#include "ModelSkeleton.hpp"

class Model;

class ModelAnimation
{
public:
	struct	Animation
	{
		double										duration;
		double										ticksPerSecond;
		std::map<std::string, const aiNodeAnim*>	nodes;

		const aiNodeAnim	*findNode(const std::string &name);
	};

private:
	Model								&m_model;
	std::map<std::string, Animation *>	m_animations;
	std::string							m_currentAnimation;

	float								m_time;

//	static uint findRotation(float animationTime, const aiNodeAnim *node);
//	static uint findPosition(float animationTime, const aiNodeAnim *node);
//	static uint findScale(float animationTime, const aiNodeAnim *node);

public:
	ModelAnimation(Model &model);
	~ModelAnimation();

	void loadAnimation();

	static void interpolateNodeRotation(aiQuaternion &out, float animationTime, const aiNodeAnim *node);
	static void interpolateNodePosition(aiVector3D &out, float animationTime, const aiNodeAnim *node);
	static void interpolateNodeScale(aiVector3D &out, float animationTime, const aiNodeAnim *node);

	Animation *getCurrentAnimation();

	void update();

	void play();
};
