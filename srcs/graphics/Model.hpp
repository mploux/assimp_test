//
// Created by mploux on 04/05/18.
//

#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <maths/Vec3.hpp>
#include <maths/Vec2.hpp>
#include <vector>
#include <GL/glew.h>
#include <maths/Mat4.hpp>
#include "Shader.hpp"
#include "ModelSkeleton.hpp"
#include "ModelAnimation.hpp"

//class ModelSkeleton;
//class ModelAnimation;

class Model
{
private:
	#define NUM_BONES_PER_VEREX 4

	struct BoneInfo
	{
		aiMatrix4x4 boneOffset;
		Mat4<float> finalTransformation;
	};

	struct VertexBoneData
	{
		uint ids[NUM_BONES_PER_VEREX];
		float weights[NUM_BONES_PER_VEREX];

		void init()
		{
			bzero(ids, NUM_BONES_PER_VEREX);
			bzero(weights, NUM_BONES_PER_VEREX);
		};

		void addBoneData(uint boneID, float weight)
		{
			for (uint i = 0 ; i < NUM_BONES_PER_VEREX ; i++)
			{
				if (weights[i] == 0.0)
				{
					ids[i] = boneID;
					weights[i] = weight;
					return;
				}
			}
		}
	};

	const aiScene		*m_scene;

	int					m_numMeshes;
	GLsizei				*m_drawSize;
	GLuint				*m_vao, *m_vbo, *m_ubo, *m_nbo, *m_bbo;

	ModelSkeleton		*m_skeleton;
	ModelAnimation		*m_animations;

	std::map<std:: string, uint>	m_boneMapping;
	uint							m_numBones;
	std::vector<BoneInfo>			m_boneInfo;

	std::vector<float>	m_boneTransforms;
	Mat4<float>			m_globalInverseTransform;

	void loadBones(uint meshIndex, const aiMesh* mesh, std::vector<VertexBoneData>& bones);
	Mat4<float> boneTransform(float timeInSeconds, std::vector<Mat4<float> > &transforms);
	const aiNodeAnim *fineNodeAnim(const aiAnimation *anim, const std::string &name);
	void readNodes(float animationTime, const aiNode *node, Mat4<float> &parent);

public:
	Model(const std::string &file);
	~Model();

	void build(int id, std::vector<Vec3<float> > &positions, std::vector<Vec3<float> > &normals, std::vector<Vec2<float> > &uvs, std::vector<VertexBoneData> &bones);

	void render(Shader &shader);

	ModelSkeleton	*getSkeleton() const { return m_skeleton; };
	ModelAnimation	*getAnimations() const { return m_animations; };
	const aiScene	*getScene() const { return m_scene; };
};
