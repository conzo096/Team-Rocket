#include "Model.h"
#include <sstream>
Model::Model()
{
}

Model::Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;

	SetUpMesh();
}

Model::Model(const std::string &fileName)
{
	type = GL_TRIANGLES;
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
		std::fprintf(stderr, "Data incorrectly read in at $s", fileName);
		std::fprintf(stderr, loadModel.GetErrorString());
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
			aiVector3D pos = modelMesh->mVertices[j];
			vertex.position = glm::vec3(pos.x, pos.y, pos.z);
			aiVector3D norm = modelMesh->mNormals[j];
			vertex.normal = glm::vec3(norm.x, norm.y, norm.z);
			if (modelMesh->HasVertexColors(0))
			{
				aiColor4D colour = modelMesh->mColors[0][j];
				vertex.color = glm::vec4(colour.r, colour.g, colour.b, colour.a);
			}
			else
				vertex.color = glm::vec4(0.7, 0.7, 0.7, 1.0);
			auto texCoord = modelMesh->mTextureCoords[0][j];
			vertex.texCoords = glm::vec2(texCoord.x, texCoord.y);
			vertices.push_back(vertex);
		}

		// If we have face information, then add to index buffer
		if (modelMesh->HasFaces() == true)
		{
			for (unsigned int j = 0; j < modelMesh->mNumFaces; j++)
			{
				aiFace modelFace = modelMesh->mFaces[j];
				for (GLuint l = 0; l < 3; l++)
					indices.push_back(vertex_begin + modelFace.mIndices[l]);
			}
		}
		vertex_begin += modelMesh->mNumVertices;
	}
	SetUpMesh();
}



void Model::SetUpMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(BUFFERS::POSITION);
	glVertexAttribPointer(BUFFERS::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(BUFFERS::COLOR);
	glVertexAttribPointer(BUFFERS::COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	// vertex normals
	glEnableVertexAttribArray(NORMAL);
	glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	// vertex texture coords
	glEnableVertexAttribArray(TEX_COORD);
	glVertexAttribPointer(TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));
}

void Model::Draw()
{

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(type, indices.size(), GL_UNSIGNED_INT, 0);
}
