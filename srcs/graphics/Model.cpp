//
// Created by mploux on 04/05/18.
//

#include <iostream>
#include "Model.hpp"

Model::Model(const std::string &file)
{
	Assimp::Importer importer;
	m_scene = importer.ReadFile(file.c_str(),
											 aiProcess_Triangulate |
											 aiProcess_FlipUVs |
											 aiProcess_JoinIdenticalVertices);
	if (!m_scene)
	{
		std::cerr << "Invalid model file !" << std::endl;
		exit(EXIT_FAILURE);
	}

//	m_animations = new ModelAnimation(*this);
//	m_animations->loadAnimation();

//	m_skeleton = new ModelSkeleton(*this);
//	Mat4<float> identity = Mat4<float>().identity();
//	readNodes(0, m_scene->mRootNode, identity);
//	m_skeleton->buildMesh();

	aiMatrix4x4 globalInverseTransform = m_scene->mRootNode->mTransformation;
	globalInverseTransform.Inverse();
	m_globalInverseTransform = Mat4<float>::mat4FromAssimp(globalInverseTransform);

	m_numMeshes = m_scene->mNumMeshes;
	m_vao = new GLuint[m_scene->mNumMeshes];
	m_vbo = new GLuint[m_scene->mNumMeshes];
	m_ubo = new GLuint[m_scene->mNumMeshes];
	m_nbo = new GLuint[m_scene->mNumMeshes];
	m_bbo = new GLuint[m_scene->mNumMeshes];
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
		bones.reserve(mesh->mNumVertices);

//		loadBones(k, mesh, bones);

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
			}
		}

		build(k, indexedPpositions, indexedNormals, indexedTexcoords, bones);
	}
}

Model::~Model()
{
	delete m_skeleton;

	delete[] m_vao;
	delete[] m_vbo;
	delete[] m_ubo;
	delete[] m_nbo;
	delete[] m_bbo;
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

Mat4<float> Model::boneTransform(float timeInSeconds, std::vector<Mat4<float> > &transforms)
{
	Mat4<float> identity = Mat4<float>().identity();

	float ticksPerSecond = m_scene->mAnimations[0]->mTicksPerSecond != 0 ?
						   m_scene->mAnimations[0]->mTicksPerSecond : 25.0f;
	float timeInTicks = timeInSeconds * ticksPerSecond;
	float animationTime = fmod(timeInTicks, m_scene->mAnimations[0]->mDuration);

	readNodes(animationTime, m_scene->mRootNode, identity);

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

void Model::readNodes(float animationTime, const aiNode *node, Mat4<float> &parent)
{
	std::string nodeName(node->mName.data);

	const aiAnimation *anim = m_scene->mAnimations[0];
	const aiNodeAnim *animNode = fineNodeAnim(anim, node->mName.data);

	Mat4<float> boneTransform = Mat4<float>::mat4FromAssimp(node->mTransformation);
	if (animNode)
	{
		aiVector3D Scaling;
		ModelAnimation::interpolateNodeScale(Scaling, animationTime, animNode);
		Mat4<float> scaling = Mat4<float>::scale(Scaling.x, Scaling.y, Scaling.z);

		aiQuaternion RotationQ;
		ModelAnimation::interpolateNodeRotation(RotationQ, animationTime, animNode);
		Mat4<float> rotation = Mat4<float>::mat4FromAssimp(aiMatrix4x4t<float>(RotationQ.GetMatrix()));

		aiVector3D Translation;
		ModelAnimation::interpolateNodePosition(Translation, animationTime, animNode);
		Mat4<float> translation = Mat4<float>::translate(Translation.x, Translation.y, Translation.z);
		boneTransform = translation * rotation * scaling;
	}

	Mat4<float> finalBoneTransformation = parent * boneTransform;

	if (m_boneMapping.find(nodeName) != m_boneMapping.end()) {
		uint BoneIndex = m_boneMapping[nodeName];
		Mat4<float> boneOffset = Mat4<float>::mat4FromAssimp(m_boneInfo[BoneIndex].boneOffset);
		m_boneInfo[BoneIndex].finalTransformation = m_globalInverseTransform * finalBoneTransformation * boneOffset;
	}

	for (std::size_t i = 0; i < node->mNumChildren; i++)
		readNodes(animationTime, node->mChildren[i], finalBoneTransformation);
}

void Model::build(int i, std::vector<Vec3<float> > &positions, std::vector<Vec3<float> > &normals, std::vector<Vec2<float> > &uvs, std::vector<VertexBoneData> &bones)
{
	if (positions.size() != normals.size())
		throw std::runtime_error("Invalid Model !");

	GLfloat *vBuffer = new GLfloat[positions.size() * 3];
	GLfloat *uBuffer = new GLfloat[uvs.size() * 2];
	GLfloat *nBuffer = new GLfloat[normals.size() * 3];

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

	m_drawSize[i] = static_cast<GLsizei>(positions.size());

	glGenVertexArrays(1, &m_vao[i]);
	glGenBuffers(1, &m_vbo[i]);
	glGenBuffers(1, &m_ubo[i]);
	glGenBuffers(1, &m_nbo[i]);
	glGenBuffers(1, &m_bbo[i]);

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

//	glEnableVertexAttribArray(3);
//	glEnableVertexAttribArray(4);
//	glBindBuffer(GL_ARRAY_BUFFER, m_bbo[i]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), &bones[0], GL_STATIC_DRAW);
//	glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
//	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

	glBindVertexArray(0);

	delete[] vBuffer;
	delete[] uBuffer;
	delete[] nBuffer;
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

//	std::vector<Mat4<float> > transforms;
//	boneTransform(0, transforms);
//
//	for (int i = 0; i < bone)
//
	for (int i = 0; i < m_numMeshes; i++)
	{
		glBindVertexArray(m_vao[i]);
		glDrawArrays(GL_TRIANGLES, 0, m_drawSize[i]);
		glBindVertexArray(0);
	}

//	glDisable(GL_DEPTH_TEST);
//	m_skeleton->render(shader);
//	glEnable(GL_DEPTH_TEST);
}