#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <string>
#include <vector>
#include "GLShader.h"
#include "assimp/Importer.hpp"
#include "assimp/PostProcess.h"
#include "assimp/Scene.h"
#include <map>

struct Transform
{
	glm::vec3 scale;
	glm::quat orientation;
	glm::vec3 position;
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec4 color;
};

struct Texture
{
	unsigned int id;
	std::string type;
};

enum BUFFERS {POSITION, COLOR, NORMAL, TEX_COORD};



class Model
{
	unsigned int VAO, VBO, EBO;
	void SetUpMesh();



public:
	Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	Transform transform;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;


	void Draw(GLShader shader);

	Model(const std::string& fileName)
	{
		// Create model importer
		Assimp::Importer loadModel;
		// Read in the model data 
		const aiScene *model = loadModel.ReadFile(fileName, aiProcess_Triangulate
			| aiProcess_GenSmoothNormals
			| aiProcess_ValidateDataStructure
			| aiProcess_FindInvalidData
			| aiProcess_FixInfacingNormals
			| aiProcess_ImproveCacheLocality
			| aiProcess_GenUVCoords);
		// Check that data has been read in correctly
		if (!model)
		{
			// Display error
			fprintf(stderr,"Data incorrectly read in at %s",fileName.c_str());
			fprintf(stderr, loadModel.GetErrorString());
			// Throw exception
			throw std::runtime_error("Error reading in model file");
		}
		// Vectors to store model data.
	
		unsigned int vertex_begin = 0;
		// Loop throw each sub-mesh.
		for (unsigned int i = 0; i < model->mNumMeshes; i++)
		{
			// Get the sub-mesh.
			aiMesh *modelMesh = model->mMeshes[i];
			// get the vertex positions.
			
			for (unsigned int j = 0; j < modelMesh->mNumVertices; j++)
			{
				Vertex vertex;
				const aiVector3D pos = modelMesh->mVertices[j];
				vertex.position = glm::vec3(pos.x, pos.y, pos.z);
				const aiVector3D norm = modelMesh->mNormals[j];
				vertex.normal = glm::vec3(norm.x, norm.y, norm.z);
				if (modelMesh->HasVertexColors(0))
				{
					const aiColor4D colour = modelMesh->mColors[0][j];
					vertex.color = glm::vec4(colour.r, colour.g, colour.b, colour.a); 
				}
				else
					vertex.color = glm::vec4(0.7,0.7,0.7,1.0);
				const auto tex_coord = modelMesh->mTextureCoords[0][j];
				vertex.texCoords = glm::vec2(tex_coord.x, tex_coord.y);
				vertices.push_back(vertex);
			}

			// If we have face information, then add to index buffer
			if (modelMesh->HasFaces() == true)
			{
				for (unsigned int j = 0; j < modelMesh->mNumFaces; j++)
				{
					const aiFace modelFace = modelMesh->mFaces[j];
					for (GLuint l = 0; l < 3; l++)
						indices.push_back(vertex_begin + modelFace.mIndices[l]);
				}
			}
			vertex_begin += modelMesh->mNumVertices;
		}
		SetUpMesh();
	}

};