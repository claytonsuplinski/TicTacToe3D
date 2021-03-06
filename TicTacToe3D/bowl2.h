/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a textured bowl shape
*/

#pragma once
#include "object2.h"
#include "shader2.h"
#include "texturedshader.h"
#include "TextureManager.h"

class Bowl2 : public Object2
{
public:
	Bowl2();
	virtual bool Initialize(int slices, float radius, char* filename, char* v, char* f);
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void StepShader();
	char * filename;
	unsigned int texID;

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
