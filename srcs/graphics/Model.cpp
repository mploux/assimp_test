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

	m_animations = new ModelAnimation(m_scene);
	m_animations->loadAnimation();

//	m_skeleton = new ModelSkeleton(*this);
//	aiMatrix4x4 identity;
//	loadNode(0.9, m_scene->mRootNode, identity, nullptr);
//	m_skeleton->buildMesh();

	for (std::size_t k = 0; k < m_scene->mNumMeshes; k++)
	{
		aiMesh *mesh = m_scene->mMeshes[k];

		std::vector<Vec3<float> >	positions;
		std::vector<Vec3<float> >	normals;
		std::vector<Vec2<float> >	texcoords;

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

		for (std::size_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (std::size_t j = 0; j < face.mNumIndices; j++)
			{
				int index = face.mIndices[j];

				Vec3<float> position = positions[index];
				addVertex(position);

				Vec3<float> normal = normals[index];
				addNormal(normal);

				if(mesh->HasTextureCoords(0))
				{
					Vec2<float> texcoord = texcoords[index];
					addUV(texcoord);
				}
			}
		}
	}
	build();
}

Model::~Model()
{
	delete m_skeleton;
}

void Model::loadNode(float animationTime, aiNode *node, aiMatrix4x4 &parent, ModelSkeleton::Node *parentNode)
{
	if (node == nullptr)
	{
		std::cerr << "NULL" << std::endl;
		exit(1);
	}
//	ModelSkeleton::Node *newNode = new ModelSkeleton::Node();
//	newNode->name = node->mName.C_Str();

//	ModelAnimation::Animation *anim = m_animations->getCurrentAnimation();
//	const aiNodeAnim *animNode = anim->findNode(node->mName.data);

//	aiMatrix4x4 abs = parent * node->mTransformation;
//	Mat4<float> animated = Mat4<float>::mat4FromAssimp(node->mTransformation);

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

//	Mat4<float> absAnimated = Mat4<float>::mat4FromAssimp(parent) * animated;

//	newNode->localMatrix = Mat4<float>::mat4FromAssimp(node->mTransformation);
//	newNode->absoluteMatrix = Mat4<float>::mat4FromAssimp(abs);
//	newNode->animatedMatrix = animated;
//	newNode->animatedAbosluteMatrix = absAnimated;
//	newNode->parent = parentNode;

//	m_skeleton->addNode(newNode);

//	for (std::size_t i = 0; i < node->mNumChildren; i++)
//	{
//		loadNode(animationTime, node->mChildren[i], abs, newNode);
//	}
}

void Model::addVertex(Vec3<float> v)
{
	m_positions.push_back(v);
}

void Model::addUV(Vec2<float> v)
{
	m_uvs.push_back(v);
}

void Model::addNormal(Vec3<float> v)
{
	m_normals.push_back(v);
}

void Model::build()
{
	if (m_positions.size() != m_normals.size())
		throw std::runtime_error("Invalid Model !");

	m_vBuffer = new GLfloat[m_positions.size() * 3];
	m_uBuffer = new GLfloat[m_uvs.size() * 2];
	m_nBuffer = new GLfloat[m_normals.size() * 3];

	for (std::size_t i = 0; i < m_positions.size(); i++)
	{
		m_vBuffer[i * 3 + 0] = m_positions[i].getX();
		m_vBuffer[i * 3 + 1] = m_positions[i].getY();
		m_vBuffer[i * 3 + 2] = m_positions[i].getZ();
	}
	for (std::size_t i = 0; i < m_uvs.size(); i++)
	{
		m_uBuffer[i * 2 + 0] = m_uvs[i].getX();
		m_uBuffer[i * 2 + 1] = m_uvs[i].getY();
	}
	for (std::size_t i = 0; i < m_normals.size(); i++)
	{
		m_nBuffer[i * 3 + 0] = m_normals[i].getX();
		m_nBuffer[i * 3 + 1] = m_normals[i].getY();
		m_nBuffer[i * 3 + 2] = m_normals[i].getZ();
	}

	m_drawSize = static_cast<GLsizei>(m_positions.size());

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ubo);
	glGenBuffers(1, &m_nbo);
	glGenBuffers(1, &m_bbo);

	glBindVertexArray(m_vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_positions.size() * 3, m_vBuffer, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_ubo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_uvs.size() * 2, m_uBuffer, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_nbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_normals.size() * 3, m_nBuffer, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindVertexArray(0);

	m_positions.clear();
	m_uvs.clear();
	m_normals.clear();
}

void Model::render(Shader &shader)
{
	static float t = 0;

	t++;
	aiMatrix4x4 identity;
	loadNode(t * 0.001f, m_scene->mRootNode, identity, nullptr);
//	m_skeleton->buildMesh();

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, m_drawSize);
	glBindVertexArray(0);
	glDisable(GL_DEPTH_TEST);
//	m_skeleton->render(shader);
	glEnable(GL_DEPTH_TEST);
}