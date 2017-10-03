#include "Model.h"
#include <sstream>
Model::Model()
{
}

Model::Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	SetUpMesh();
}

Model::Model(const std::string &fileName)
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
	glEnableVertexAttribArray(BUFFERS::NORMAL);
	glVertexAttribPointer(BUFFERS::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(BUFFERS::TEX_COORD);
	glVertexAttribPointer(BUFFERS::TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
}

void Model::Draw()
{
	//unsigned int diffuseNr = 1;
	//unsigned int specularNr = 1;
	//for (GLuint i = 0; i < this->textures.size(); i++)
	//{
	//	glActiveTexture(GL_TEXTURE0 + i);

	//	std::string name = this->textures[i].type;
	//	std::string number = (name == "texture_diffuse") ? std::to_string(diffuseNr++) : std::to_string(specularNr++);

	//	glUniform1i(glGetUniformLocation(shader.GetId(), ("material." + name + number).c_str()), i);
	//	glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	//}
	//glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Model::DrawStrip()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);
}

void Model::CreatePlane(float spacing, unsigned int xSize, unsigned int ySize)
{
	vertices.clear();
	for (int i = 0; i < ySize; i++)
	{
		for (int j = 0; j < xSize; j++)
		{
			Vertex v;
			v.position = glm::vec3(i*spacing, 0.0f, j*spacing);
			v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
			v.texCoords = glm::vec2(i*spacing, j*spacing);
			v.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			vertices.push_back(v);
		}
	}

	indices.clear();
	for (int i = 0; i < ySize - 1; i++)
	{
		if ((i & 1) == 0)
		{ // even rows
			for (int j = 0; j < xSize; j++)
			{
				indices.push_back(j + ((i + 1) * xSize));
				indices.push_back(j + (i*xSize));
				
			}
		}
		else
		{ // odd rows
			for (int j = xSize - 1; j > 0; j--)
			{
				indices.push_back(j - 1 + (i * xSize));
				indices.push_back(j + ((i + 1) * xSize));

			}
		}
	}
	SetUpMesh();
}
