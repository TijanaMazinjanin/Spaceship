#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"


class Renderer {
public:
	Renderer();
	~Renderer();
	void DrawRectangle(Shader& shader, glm::vec2 position, glm::vec2 size, float rotate, const glm::vec4& color, const Texture2D& texture, bool useTexture);
	void DrawCube(Shader& shader, const glm::mat4& model, const glm::vec4& color, const Texture2D& texture, bool useTexture);
	void DrawPyramid(Shader &shader, glm::mat4 model, float angle,  const glm::vec4& color, const Texture2D& texture, bool useTexture);

private:
unsigned int rectVAO, rectVBO;
unsigned int cubeVAO, cubeVBO;
unsigned int pyramidVAO, pyramidVBO;

void setupRect();
void setupCube();
void setupPyramid();

};
