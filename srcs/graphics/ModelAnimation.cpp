//
// Created by mploux on 05/05/18.
//

#include <iostream>
#include "ModelAnimation.hpp"
#include "Model.hpp"

ModelAnimation::ModelAnimation(Model &model)
	: m_model(model),
	  m_time(0)
{
	m_currentAnimation = "null";
}

ModelAnimation::~ModelAnimation()
{

}

const aiNodeAnim	*ModelAnimation::Animation::findNode(const std::string &name)
{
	if (nodes.find(name) != nodes.end())
		return nodes[name];
	return nullptr;
}

void ModelAnimation::loadAnimation()
{
	for (std::size_t i = 0; i < m_model.getScene()->mNumAnimations; i++)
	{
		const aiAnimation *aiAnim = m_model.getScene()->mAnimations[i];

		if (i == 0)
		{
			std::cout << "Anim: " << aiAnim->mName.data << "\n";
			m_currentAnimation = aiAnim->mName.data;
		}

		Animation *anim = new Animation();
		anim->duration = aiAnim->mDuration;
		anim->ticksPerSecond = aiAnim->mTicksPerSecond;
		for (std::size_t j = 0; j < aiAnim->mNumChannels; j++)
		{
			const aiNodeAnim *aiNode = aiAnim->mChannels[j];
			anim->nodes[aiNode->mNodeName.data] = aiNode;
		}
		m_animations[aiAnim->mName.data] = anim;
	}
}

ModelAnimation::Animation *ModelAnimation::getCurrentAnimation()
{
	std::cout << "Animation: " << m_currentAnimation << std::endl;
	if (m_currentAnimation == "null")
	{
		return nullptr;
	}
	return m_animations[m_currentAnimation];
}

void ModelAnimation::update()
{
//	m_time += 0.001f;
//	if (m_time >= 1)
//		m_time = 0;
//
//	for (auto &p : m_model.getSkeleton()->getNodes())
//	{
//		ModelSkeleton::Node *node = p.second;
//
//		Animation *anim = getCurrentAnimation();
//
//		const aiNodeAnim *animNode = anim->findNode(node->name);
//
//		Mat4<float> animated = node->localMatrix;
//		if (animNode)
//		{
//			aiVector3D Scaling;
//			ModelAnimation::interpolateNodeScale(Scaling, m_time, animNode);
//			Mat4<float> scaling = Mat4<float>::scale(Scaling.x, Scaling.y, Scaling.z);
//			aiQuaternion RotationQ;
//			ModelAnimation::interpolateNodeRotation(RotationQ, m_time, animNode);
//			Mat4<float> rotation = Mat4<float>::mat4FromAssimp(aiMatrix4x4t<float>(RotationQ.GetMatrix()));
//			aiVector3D Translation;
//			ModelAnimation::interpolateNodePosition(Translation, m_time, animNode);
//			Mat4<float> translation = Mat4<float>::translate(Translation.x, Translation.y, Translation.z);
//			animated = translation * rotation * scaling;
//		}
//		Mat4<float> parent = Mat4<float>().identity();
//		if (node->parent)
//			parent = node->parent->animatedAbosluteMatrix;
//		node->animatedAbosluteMatrix = parent * animated;
//	}
}

void ModelAnimation::play()
{
}

static uint findRotation(float animationTime, const aiNodeAnim *node)
{
	for (uint i = 0 ; i < node->mNumRotationKeys - 1 ; i++)
		if (animationTime < (float)node->mRotationKeys[i + 1].mTime)
			return i;
	return 0;
}

static uint findPosition(float animationTime, const aiNodeAnim *node)
{
	for (uint i = 0 ; i < node->mNumPositionKeys - 1 ; i++)
		if (animationTime < (float)node->mPositionKeys[i + 1].mTime)
			return i;
	return 0;
}

static uint findScale(float animationTime, const aiNodeAnim *node)
{
	for (uint i = 0 ; i < node->mNumScalingKeys - 1 ; i++)
		if (animationTime < (float)node->mScalingKeys[i + 1].mTime)
			return i;
	return 0;
}

void ModelAnimation::interpolateNodeRotation(aiQuaternion &out, float animationTime, const aiNodeAnim *node)
{
	if (node->mNumRotationKeys == 1)
	{
		out = node->mRotationKeys[0].mValue;
		return;
	}

	uint index = findRotation(animationTime, node);
	uint nextIndex = (index + 1);
	float delta = node->mRotationKeys[nextIndex].mTime - node->mRotationKeys[index].mTime;
	float factor = (animationTime - (float)node->mRotationKeys[index].mTime) / delta;
	const aiQuaternion& start = node->mRotationKeys[index].mValue;
	const aiQuaternion& end = node->mRotationKeys[nextIndex].mValue;
	aiQuaternion::Interpolate(out, start, end, factor);
	out = out.Normalize();
}

void ModelAnimation::interpolateNodePosition(aiVector3D &out, float animationTime, const aiNodeAnim *node)
{
	if (node->mNumPositionKeys == 1)
	{
		out = node->mPositionKeys[0].mValue;
		return;
	}

	uint index = findPosition(animationTime, node);
	uint nextIndex = (index + 1);
	float delta = node->mPositionKeys[nextIndex].mTime - node->mPositionKeys[index].mTime;
	float factor = (animationTime - (float)node->mPositionKeys[index].mTime) / delta;
	const aiVector3D& start = node->mPositionKeys[index].mValue;
	const aiVector3D& end = node->mPositionKeys[nextIndex].mValue;
	out = start + (end - start) * factor;
}

void ModelAnimation::interpolateNodeScale(aiVector3D &out, float animationTime, const aiNodeAnim *node)
{
	if (node->mNumPositionKeys == 1)
	{
		out = node->mPositionKeys[0].mValue;
		return;
	}

	uint index = findScale(animationTime, node);
	uint nextIndex = (index + 1);
	float delta = node->mScalingKeys[nextIndex].mTime - node->mScalingKeys[index].mTime;
	float factor = (animationTime - (float)node->mScalingKeys[index].mTime) / delta;
	const aiVector3D& start = node->mScalingKeys[index].mValue;
	const aiVector3D& end = node->mScalingKeys[nextIndex].mValue;
	out = start + (end - start) * factor;
}
