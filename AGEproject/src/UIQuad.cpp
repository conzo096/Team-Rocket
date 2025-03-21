#include "UIQuad.h"
#include <glm\gtc\type_ptr.hpp>
void UIQuad::Render()
{
	if (isActive)
	{
		unsigned int length = static_cast<unsigned int>(strlen(text.c_str()));
		std::vector<glm::vec2> vertices;
		std::vector<glm::vec2> UVs;
		for (unsigned int i = 0; i < length; i++)
		{
			glm::vec2 vertex_up_left = glm::vec2(xPos + i*size, yPos + size);
			glm::vec2 vertex_up_right = glm::vec2(xPos + i*size + size, yPos + size);
			glm::vec2 vertex_down_right = glm::vec2(xPos + i*size + size, yPos);
			glm::vec2 vertex_down_left = glm::vec2(xPos + i*size, yPos);

			vertices.push_back(vertex_up_left);
			vertices.push_back(vertex_down_left);
			vertices.push_back(vertex_up_right);

			vertices.push_back(vertex_down_right);
			vertices.push_back(vertex_up_right);
			vertices.push_back(vertex_down_left);

			char character = text[i];
			double uv_x = (character % 16) / 16.0f;
			double uv_y = (character / 16) / 16.0f;
			uv_y = (1 - uv_y - 0.0625);
			glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
			glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, uv_y);
			glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
			glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y + 1.0f / 16.0f));
			/*	UVs.push_back(uv_up_left);
			UVs.push_back(uv_down_left);
			UVs.push_back(uv_up_right);

			UVs.push_back(uv_down_right);
			UVs.push_back(uv_up_right);
			UVs.push_back(uv_down_left);*/

			UVs.push_back(uv_down_left);
			UVs.push_back(uv_up_left);
			UVs.push_back(uv_down_right);

			UVs.push_back(uv_up_right);
			UVs.push_back(uv_down_right);
			UVs.push_back(uv_up_left);
		}

		glBindVertexArray(Text2DVertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
		glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


		// Bind shader
		glUseProgram(shader);
		// Bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ResourceHandler::Get().GetTexture("Font"));
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(Text2DUniformID, 0);


		// Set colour field.
		glUniform4fv(ResourceHandler::Get().GetShader("Font")->GetUniformLocation("texColour"), 1, glm::value_ptr(textColour));


		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}
}