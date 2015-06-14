/*
Name: Clayton Suplinski
Project: InsertProjectNameHere
*/

#include <iostream>
#include "team.h"

using namespace std;
using namespace glm;

//Create the team
Team::Team() : Object(){
}

Team::~Team(){this->TakeDown();}

//Initialize the team.
bool Team::Initialize(int teamNo)
{
	if (this->GLReturnedError("Team::Initialize - on entry"))
		return false;

	this->teamNo = teamNo;
	
	if (this->GLReturnedError("Team::Initialize - on exit"))
		return false;

	return true;
}

//Delete everything.
void Team::TakeDown()
{
	this->vertices.clear();
	this->shader.TakeDown();
	super::TakeDown();	
}

void Team::aiCpus(){
	
}

//Draw and position the Shark elements.
void Team::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float rotY)
{
	if (this->GLReturnedError("Team::Draw - on entry"))
		return;

	int t = glutGet(GLUT_ELAPSED_TIME);
	

	if (this->GLReturnedError("Team::Draw - on exit"))
		return;
}
