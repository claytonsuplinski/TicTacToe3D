/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a center-aligned framebuffer-textured square object
*/

#pragma once
#include "object2.h"
#include "shader2.h"
#include "texturedshader.h"
#include "TextureManager.h"

class Square4 : public Object2
{
public:
	Square4();
	virtual bool Initialize(int x, float s, char* v, char* f);
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void StepShader();
	GLuint fboID;

private:
	typedef Object2 super;
	int shader_index;

	Shader2 pattern_shader;
	Shader2 basic_shader;
	Shader2 colored_shader;
	Shader2 play_shader;
	TexturedShader basic_texture_shader;
	std::vector<Shader2 *> shaders;
};
