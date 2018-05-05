//
// Created by mploux on 05/05/18.
//

#pragma once

#include <assimp/anim.h>
#include <map>
#include <assimp/scene.h>

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
	const aiScene						*m_scene;
	std::map<std::string, Animation *>	m_animations;
	std::string							m_currentAnimation;

//	static uint findRotation(float animationTime, const aiNodeAnim *node);
//	static uint findPosition(float animationTime, const aiNodeAnim *node);
//	static uint findScale(float animationTime, const aiNodeAnim *node);

public:
	ModelAnimation(const aiScene *scene);
	~ModelAnimation();

	void loadAnimation();

	static void interpolateNodeRotation(aiQuaternion &out, float animationTime, const aiNodeAnim *node);
	static void interpolateNodePosition(aiVector3D &out, float animationTime, const aiNodeAnim *node);
	static void interpolateNodeScale(aiVector3D &out, float animationTime, const aiNodeAnim *node);

	Animation *getCurrentAnimation();

	void play();
};
