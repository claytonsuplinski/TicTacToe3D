/*
Name: Clayton Suplinski

*/

#pragma once
#include "object.h"
#include "shader.h"
#include "gengar.h"
#include "torus.h"
#include "cube.h"
#include <vector>

using namespace std;
using namespace glm;

class Piece : public Object
{
public:
	Piece();
	~Piece();
	virtual bool Initialize(bool circles);
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void StepShader();
	Shader shader;

	void drawX(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	
	bool usingCircles;

	Torus circle;
	Cube cross;

	vec3 boardLoc;

	static const int SIZE = 5;

	float editX, editY;

private:
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};