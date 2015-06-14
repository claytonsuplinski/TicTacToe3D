/*
Name: Clayton Suplinski
Project: InsertProjectNameHere
*/

#include <iostream>
#include "piece.h"

using namespace std;
using namespace glm;

//Create a Shark object
Piece::Piece() : Object(){
}

Piece::~Piece(){this->TakeDown();}

//Initialize all the Shark elements.
bool Piece::Initialize(bool circles)
{
	if (this->GLReturnedError("Piece::Initialize - on entry"))
		return false;

	this->usingCircles = circles;

	if(circles){
		this->circle.color=vec3(1,0,0);
		this->circle.Initialize(4,0.4,0.1,"phong.vert","phong.frag");
	}
	else{
		this->cross.color=vec3(0,0,1);
		this->cross.Initialize(1,1,1,"phong.vert","phong.frag");
	}

	editX = 0;editY = 0;

	boardLoc = vec3(0,0,0);

	if (this->GLReturnedError("Piece::Initialize - on exit"))
		return false;

	return true;
}

//Delete the Shark elements.
void Piece::TakeDown()
{
	this->circle.TakeDown();this->cross.TakeDown();
	this->vertices.clear();
	this->shader.TakeDown();
	super::TakeDown();	
}

//Draw and position the Shark elements.
void Piece::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float rotY)
{
	if (this->GLReturnedError("Piece::Draw - on entry"))
		return;

	float ct = float(glutGet(GLUT_ELAPSED_TIME)) / 10.0f;

	glEnable(GL_DEPTH_TEST);
	mat4 another, scaler;
	another = modelview;

	another = rotate(another, fmod(ct, 360.f), vec3(0,1,0));

	another = scale(another, vec3(SIZE, SIZE, SIZE));

	another = translate(another, vec3(0, 0.3, 0));

	if(usingCircles){
		this->circle.Draw(projection, another, size, rotY);
	}
	else{
		another = translate(another, vec3(-0.265, 0.4475, 0.1));
		drawX(projection, another, size, rotY);
	}
	
	if (this->GLReturnedError("Piece::Draw - on exit"))
		return;
}

void Piece::drawX(const mat4 & projection, mat4 modelview, const ivec2 & size, const float rotY)
{
	float ct = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	glEnable(GL_DEPTH_TEST);

	mat4 xScale;
	xScale = modelview;
	xScale = rotate(xScale, 90.f, vec3(0,1,0));
	xScale = rotate(xScale, 45.f, vec3(1,0,0));		
	xScale = translate(xScale, vec3(0, -0.625, 0.375));
	xScale = scale(xScale, vec3(0.2, 1, 0.2));
	this->cross.Draw(projection, xScale, size, rotY);

	xScale = modelview;
	xScale = rotate(xScale, 90.f, vec3(0,1,0));
	xScale = rotate(xScale, 45.f, vec3(1,0,0));
	xScale = rotate(xScale, 90.f, vec3(1,0,0));
	xScale = scale(xScale, vec3(0.2, 1, 0.2));
	this->cross.Draw(projection, xScale, size, rotY);
}
