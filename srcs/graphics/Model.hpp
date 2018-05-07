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

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/constants.hpp>

//class ModelSkeleton;
//class ModelAnimation;

class Model
{
private:
	#define NUM_BONES_PER_VEREX 4

	struct BoneInfo
	{
		aiMatrix4x4 boneOffset;
		glm::mat4 finalTransformation;
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
	GLuint				*m_vao, *m_vbo, *m_ubo, *m_nbo, *m_wbo, *m_bbo;

	ModelSkeleton		*m_skeleton;
	ModelAnimation		*m_animations;

	std::map<std:: string, uint>	m_boneMapping;
	uint							m_numBones;
	std::vector<BoneInfo>			m_boneInfo;
	std::vector<glm::mat4> 		m_transforms;

	std::vector<float>	m_boneTransforms;
	aiMatrix4x4			m_globalInverseTransform;

	void loadBones(uint meshIndex, const aiMesh* mesh, std::vector<VertexBoneData>& bones);
	void boneTransform(float timeInSeconds, std::vector<glm::mat4 > &transforms);
	const aiNodeAnim *fineNodeAnim(const aiAnimation *anim, const std::string &name);
	void readNodes(float animationTime, const aiNode *node, aiMatrix4x4 &parent, ModelSkeleton::Node *p);

public:
	Model(const std::string &file);
	~Model();

	void build(int id, std::vector<Vec3<float> > &positions, std::vector<Vec3<float> > &normals, std::vector<Vec2<float> > &uvs, std::vector<VertexBoneData> &bones);

	void render(Shader &shader);

	ModelSkeleton	*getSkeleton() const { return m_skeleton; };
	ModelAnimation	*getAnimations() const { return m_animations; };
	const aiScene	*getScene() const { return m_scene; };
};
