/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates the square arena
*/

#pragma once
#include "object.h"
#include "shader.h"
#include "cylinder.h"
#include "jumbotron.h"
#include "square3.h"
#include "gengar.h"

class SquareStadium : public Object
{
public:
	SquareStadium();
	~SquareStadium();
	virtual bool Initialize(int weatherIndex);
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void StepShader();
	Shader shader;Shader solid_color;
	Square3 * floor;Square3 * wall;
	Gengar * zubat;Gengar * charizard;Gengar * blades;Gengar * mtns;Gengar * trees;
	Jumbotron * jb;
	Cylinder * base;

	float transX, transY, transZ;
private:
	void BuildNormalVisualizationGeometry();
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};