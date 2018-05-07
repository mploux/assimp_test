//
// Created by mploux on 04/05/18.
//

#include <iostream>
#include <Core.hpp>
#include <maths/glmUtils.hpp>
#include "Model.hpp"

Model::Model(const std::string &file)
{
	Assimp::Importer importer;
	m_scene = importer.ReadFile(file.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (!m_scene)
	{
		std::cerr << "Invalid model file !" << std::endl;
		exit(EXIT_FAILURE);
	}

//	for (int i = 0; i < m_scene->mNumAnimations; i++)
//	{
//		std::cout << "Animation: " << m_scene->mAnimations[i]->mName.data << std::endl;
//	}

//	m_animations = new ModelAnimation(*this);
//	m_animations->loadAnimation();

	m_skeleton = new ModelSkeleton(*this);
//	Mat4<float> identity = Mat4<float>().identity();
//	readNodes(0, m_scene->mRootNode, identity);

	aiMatrix4x4 globalInverseTransform = m_scene->mRootNode->mTransformation;
//	globalInverseTransform.Inverse();
	m_globalInverseTransform = globalInverseTransform.Inverse();

	m_numMeshes = m_scene->mNumMeshes;
	m_vao = new GLuint[m_scene->mNumMeshes];
	m_vbo = new GLuint[m_scene->mNumMeshes];
	m_ubo = new GLuint[m_scene->mNumMeshes];
	m_nbo = new GLuint[m_scene->mNumMeshes];
	m_bbo = new GLuint[m_scene->mNumMeshes];
	m_wbo = new GLuint[m_scene->mNumMeshes];
	m_drawSize = new int[m_scene->mNumMeshes];

	for (std::size_t k = 0; k < m_scene->mNumMeshes; k++)
	{
		aiMesh *mesh = m_scene->mMeshes[k];

		std::vector<Vec3<float> >	positions;
		std::vector<Vec3<float> >	normals;
		std::vector<Vec2<float> >	texcoords;
		std::vector<VertexBoneData>	bones;

		positions.reserve(mesh->mNumVertices);
		normals.reserve(mesh->mNumVertices);
		texcoords.reserve(mesh->mNumVertices);
		bones.resize(mesh->mNumVertices);

		loadBones(k, mesh, bones);

		for (std::size_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vec3<float> position;
			position.setX(mesh->mVertices[i].x);
			position.setY(mesh->mVertices[i].y);
			position.setZ(mesh->mVertices[i].z);
			positions.push_back(position);

			Vec3<float> normal;
			normal.setX(mesh->mNormals[i].x);
			normal.setY(mesh->mNormals[i].y);
			normal.setZ(mesh->mNormals[i].z);
			normals.push_back(normal);

			if(mesh->HasTextureCoords(0))
			{
				Vec2<float> texcoord;
				texcoord.setX(mesh->mTextureCoords[0][i].x);
				texcoord.setY(mesh->mTextureCoords[0][i].y);
				texcoords.push_back(texcoord);
			}
		}

		std::vector<Vec3<float> >	indexedPpositions;
		std::vector<Vec3<float> >	indexedNormals;
		std::vector<Vec2<float> >	indexedTexcoords;
		std::vector<VertexBoneData>	indexedBonesData;

		for (std::size_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (std::size_t j = 0; j < face.mNumIndices; j++)
			{
				int index = face.mIndices[j];

				Vec3<float> position = positions[index];
				indexedPpositions.push_back(position);

				Vec3<float> normal = normals[index];
				indexedNormals.push_back(normal);

				if(mesh->HasTextureCoords(0))
				{
					Vec2<float> texcoord = texcoords[index];
					indexedTexcoords.push_back(texcoord);
				}

				indexedBonesData.push_back(bones[index]);
			}
		}
		build(k, indexedPpositions, indexedNormals, indexedTexcoords, indexedBonesData);
	}

	boneTransform(0, m_transforms);
	m_skeleton->buildMesh();
}

Model::~Model()
{
	delete m_skeleton;

	delete[] m_vao;
	delete[] m_vbo;
	delete[] m_ubo;
	delete[] m_nbo;
	delete[] m_bbo;
	delete[] m_wbo;
}

//void Model::VertexBoneData::addBoneData(uint boneID, float weight)
//{
//	for (uint i = 0 ; i < NUM_BONES_PER_VEREX ; i++)
//	{
//		if (weights[i] == 0.0)
//		{
//			ids[i] = boneID;
//			weights[i] = weight;
//			return;
//		}
//	}
//}

//void Model::loadNode(float animationTime, const aiNode *node, aiMatrix4x4 &parent, ModelSkeleton::Node *parentNode)
//{
//	if (node == nullptr)
//		return;
//
//	ModelSkeleton::Node *newNode = new ModelSkeleton::Node();
//	newNode->name = node->mName.C_Str();
//
//	aiMatrix4x4 abs = parent * node->mTransformation;
//	Mat4<float> animated = Mat4<float>::mat4FromAssimp(node->mTransformation);
//
//	newNode->localMatrix = Mat4<float>::mat4FromAssimp(node->mTransformation);
//	newNode->absoluteMatrix = Mat4<float>::mat4FromAssimp(abs);
//
//	ModelAnimation::Animation *anim = m_animations->getCurrentAnimation();
//
//	const aiNodeAnim *animNode = anim->findNode(newNode->name);
//
//	if (animNode)
//	{
//		aiVector3D Scaling;
//		ModelAnimation::interpolateNodeScale(Scaling, animationTime, animNode);
//		Mat4<float> scaling = Mat4<float>::scale(Scaling.x, Scaling.y, Scaling.z);
//		aiQuaternion RotationQ;
//		ModelAnimation::interpolateNodeRotation(RotationQ, animationTime, animNode);
//		Mat4<float> rotation = Mat4<float>::mat4FromAssimp(aiMatrix4x4t<float>(RotationQ.GetMatrix()));
//		aiVector3D Translation;
//		ModelAnimation::interpolateNodePosition(Translation, animationTime, animNode);
//		Mat4<float> translation = Mat4<float>::translate(Translation.x, Translation.y, Translation.z);
//		animated = translation * rotation * scaling;
//	}
//
//	Mat4<float> absAnimated = Mat4<float>::mat4FromAssimp(parent) * animated;
//	newNode->animatedMatrix = animated;
//	newNode->animatedAbosluteMatrix = absAnimated;
//	newNode->parent = parentNode;
//
//	m_skeleton->addNode(newNode);
//
//	for (std::size_t i = 0; i < node->mNumChildren; i++)
//		loadNode(animationTime, node->mChildren[i], abs, newNode);
//}

void Model::loadBones(uint meshIndex, const aiMesh* mesh, std::vector<VertexBoneData>& bones)
{
	for (uint i = 0 ; i < mesh->mNumBones ; i++)
	{
		uint boneIndex = 0;
		std::string boneName(mesh->mBones[i]->mName.data);

		if (m_boneMapping.find(boneName) == m_boneMapping.end())
		{
			boneIndex = m_numBones;
			m_numBones++;
			BoneInfo bi;
			m_boneInfo.push_back(bi);
		}
		else
		{
			boneIndex = m_boneMapping[boneName];
		}

		m_boneMapping[boneName] = boneIndex;
		m_boneInfo[boneIndex].boneOffset = mesh->mBones[i]->mOffsetMatrix;

		for (uint j = 0 ; j < mesh->mBones[i]->mNumWeights ; j++)
		{
			uint vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;
			bones[vertexID].addBoneData(boneIndex, weight);
		}
	}
}

void Model::boneTransform(float timeInSeconds, std::vector<glm::mat4 > &transforms)
{
	aiMatrix4x4 identity;

	std::cout << "Animation: " << m_scene->mAnimations[0]->mName.C_Str() << std::endl;

	float ticksPerSecond = m_scene->mAnimations[0]->mTicksPerSecond != 0 ?
						   m_scene->mAnimations[0]->mTicksPerSecond : 25.0f;
	float timeInTicks = timeInSeconds * ticksPerSecond;
	float animationTime = fmod(timeInTicks, m_scene->mAnimations[0]->mDuration);

	readNodes(0, m_scene->mRootNode, identity, nullptr);

	transforms.resize(m_numBones);

	for (uint i = 0 ; i < m_numBones ; i++)
	{
		transforms[i] = m_boneInfo[i].finalTransformation;
	}
}

const aiNodeAnim *Model::fineNodeAnim(const aiAnimation *anim, const std::string &name)
{
	for (uint i = 0 ; i < anim->mNumChannels ; i++)
	{
		const aiNodeAnim* pNodeAnim = anim->mChannels[i];
		if (std::string(pNodeAnim->mNodeName.data) == name)
			return pNodeAnim;
	}
	return nullptr;
}

void Model::readNodes(float animationTime, const aiNode *node, aiMatrix4x4 &parent, ModelSkeleton::Node *p)
{
	std::string nodeName(node->mName.data);

	const aiAnimation *anim = m_scene->mAnimations[0];
	const aiNodeAnim *animNode = fineNodeAnim(anim, node->mName.data);

	aiMatrix4x4 aiFinalBoneTransformation = parent * node->mTransformation;
	glm::mat4 boneTransform = glm_utils::assimp_to_glm(node->mTransformation);

//	if (animNode)
//	{
//		aiVector3D Scaling;
//		ModelAnimation::interpolateNodeScale(Scaling, animationTime, animNode);
//		glm::mat4 scaling = glm::scale(glm::mat4(1.0f), glm::vec3(Scaling.x, Scaling.y, Scaling.z));
//
//		aiQuaternion RotationQ;
//		ModelAnimation::interpolateNodeRotation(RotationQ, animationTime, animNode);
//		glm::mat4 rotation = Mat4<float>::mat4FromAssimp(aiMatrix4x4t<float>(RotationQ.GetMatrix()));
//
//		aiVector3D Translation;
//		ModelAnimation::interpolateNodePosition(Translation, animationTime, animNode);
//		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(Translation.x, Translation.y, Translation.z));
////		boneTransform = translation * rotation * scaling;
//	}

	glm::mat4 finalBoneTransformation = glm_utils::assimp_to_glm(aiFinalBoneTransformation);
	ModelSkeleton::Node *n = new ModelSkeleton::Node();
	n->name = node->mName.C_Str();
	n->animatedAbosluteMatrix = finalBoneTransformation;
	n->parent = p;
	m_skeleton->addNode(n);

	if (m_boneMapping.find(nodeName) != m_boneMapping.end()) {
		uint BoneIndex = m_boneMapping[nodeName];
		aiMatrix4x4 trs = m_globalInverseTransform * aiFinalBoneTransformation * m_boneInfo[BoneIndex].boneOffset;

//		glm::mat4 boneOffset = glm_utils::assimp_to_glm(m_boneInfo[BoneIndex].boneOffset);
		m_boneInfo[BoneIndex].finalTransformation = glm_utils::assimp_to_glm(trs);
	}

	for (std::size_t i = 0; i < node->mNumChildren; i++)
		readNodes(animationTime, node->mChildren[i], aiFinalBoneTransformation, n);
}

void Model::build(int i, std::vector<Vec3<float> > &positions, std::vector<Vec3<float> > &normals, std::vector<Vec2<float> > &uvs, std::vector<VertexBoneData> &bones)
{
	if (positions.size() != normals.size())
		throw std::runtime_error("Invalid Model !");

	GLfloat *vBuffer = new GLfloat[positions.size() * 3];
	GLfloat *uBuffer = new GLfloat[uvs.size() * 2];
	GLfloat *nBuffer = new GLfloat[normals.size() * 3];
	GLuint *bBuffer = new GLuint[bones.size() * 4];
	GLfloat *wBuffer = new GLfloat[bones.size() * 4];

	for (std::size_t i = 0; i < positions.size(); i++)
	{
		vBuffer[i * 3 + 0] = positions[i].getX();
		vBuffer[i * 3 + 1] = positions[i].getY();
		vBuffer[i * 3 + 2] = positions[i].getZ();
	}
	for (std::size_t i = 0; i < uvs.size(); i++)
	{
		uBuffer[i * 2 + 0] = uvs[i].getX();
		uBuffer[i * 2 + 1] = uvs[i].getY();
	}
	for (std::size_t i = 0; i < normals.size(); i++)
	{
		nBuffer[i * 3 + 0] = normals[i].getX();
		nBuffer[i * 3 + 1] = normals[i].getY();
		nBuffer[i * 3 + 2] = normals[i].getZ();
	}
	for (std::size_t i = 0; i < bones.size(); i++)
	{
		wBuffer[i * 4 + 0] = bones[i].weights[0];
		wBuffer[i * 4 + 1] = bones[i].weights[1];
		wBuffer[i * 4 + 2] = bones[i].weights[2];
		wBuffer[i * 4 + 3] = bones[i].weights[3];

		bBuffer[i * 4 + 0] = bones[i].ids[0];
		bBuffer[i * 4 + 1] = bones[i].ids[1];
		bBuffer[i * 4 + 2] = bones[i].ids[2];
		bBuffer[i * 4 + 3] = bones[i].ids[3];
	}

	std::cout << "Bones: " << bones.size() << std::endl;
	std::cout << "Vertices: " << positions.size() << std::endl;

	m_drawSize[i] = static_cast<GLsizei>(positions.size());

	glGenVertexArrays(1, &m_vao[i]);
	glGenBuffers(1, &m_vbo[i]);
	glGenBuffers(1, &m_ubo[i]);
	glGenBuffers(1, &m_nbo[i]);
	glGenBuffers(1, &m_bbo[i]);
	glGenBuffers(1, &m_wbo[i]);

	glBindVertexArray(m_vao[i]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * positions.size() * 3, vBuffer, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_ubo[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * uvs.size() * 2, uBuffer, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_nbo[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals.size() * 3, nBuffer, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, m_bbo[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * bones.size() * 4, bBuffer, GL_STATIC_DRAW);
	glVertexAttribIPointer(3, 4, GL_UNSIGNED_INT, 0, nullptr);

	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, m_wbo[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * bones.size() * 4, wBuffer, GL_STATIC_DRAW);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindVertexArray(0);

	delete[] vBuffer;
	delete[] uBuffer;
	delete[] nBuffer;
	delete[] bBuffer;
	delete[] wBuffer;
}

void Model::render(Shader &shader)
{
//	static float t = 0;
//
//	t++;
//	std::cout << "m_scene" << m_scene << std::endl;
//	aiMatrix4x4 identity;
//	m_skeleton->clearNodes();
//	loadNode(0, m_scene->mRootNode, identity, nullptr);
//	m_skeleton->buildMesh();
//	m_animations->update();

//
//	for (int i = 0; i < bone)
//

	for (int i = 0; i < m_numMeshes; i++)
	{
//		std::cout << "Trs size: " << m_transforms.size() << std::endl;
		for (int i = 0; i < m_transforms.size(); i++)
		{
			std::string uniform = "bonesTransforms[" + std::to_string(i) + "]";
			shader.setUniform(uniform.c_str(), m_transforms[i]);
//			shader.setUniform(uniform.c_str(), glm::mat4(1.0f));
		}

		glBindVertexArray(m_vao[i]);
		glDrawArrays(GL_TRIANGLES, 0, m_drawSize[i]);
		glBindVertexArray(0);
	}

	glDisable(GL_DEPTH_TEST);
	m_skeleton->render(shader);
	glEnable(GL_DEPTH_TEST);
}