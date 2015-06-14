/*
Name: Clayton Suplinski
Project: InsertProjectNameHere
*/

#pragma once
#include "object.h"
#include "shader.h"
#include "sphere.h"
#include <vector>

using namespace std;
using namespace glm;

class Team : public Object
{
public:
	Team();
	~Team();
	virtual bool Initialize(int teamNo);
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void aiCpus();

	Shader shader;

	int teamNo;

	bool userTeam;

private:
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};