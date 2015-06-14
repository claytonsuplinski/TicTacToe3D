/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 2

Creates a cylinder object.
*/

#pragma once
#include "object.h"
#include "shader.h"
#include "sphere.h"
#include "snow.h"
#include <vector>

using namespace std;

class Precip : public Object
{
public:
	Precip();
	virtual bool Initialize();
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float radius);
	void TakeDown();
	void increasePrecip(int amount);
	void decreasePrecip(int amount);
	Shader shader;
	glm::vec3 color;

	Sphere * rain2;
	Snow * snow2;

	static const int maxParts = 1000;
	static const int minParts = 0;

	//Retrieve from main
	int weatherType;
	float movingWRTSnow;
	float RotatedY;

	int numParts;
	vector<float> partX;
	vector<float> partY;
	vector<float> partOffset;

private:
	typedef Object super;
};