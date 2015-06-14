/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates the square arena
*/

#include <iostream>
#include "squareStadium.h"

using namespace std;
using namespace glm;

//Create the SquareStadium object
SquareStadium::SquareStadium() : Object()
{
}

SquareStadium::~SquareStadium(){
	this->TakeDown();
}

//Initialize all the elements of the SquareStadium
bool SquareStadium::Initialize(int weatherIndex)
{
	if (this->GLReturnedError("SquareStadium::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	this->floor = new Square3();this->wall = new Square3();
	this->mtns = new Gengar();this->base = new Cylinder();

	this->base->color = vec3(1,0,0);
	this->base->Initialize(10, 50, 5, 5, "phong3.vert", "phong3.frag");
	
	this->floor->Initialize(1, 1000.0f, "./textures/ufoRoof.jpg", "basic_texture_shader.vert", "basic_texture_shader2.frag");

	this->wall->Initialize(1, 50.0f, "./textures/wall3.jpg", "basic_texture_shader.vert", "basic_texture_shader2.frag");

	if (this->GLReturnedError("SquareStadium::Initialize - on exit"))
		return false;

	return true;
}

//Delete the SquareStadium
void SquareStadium::TakeDown()
{
	this->floor = NULL;this->blades = NULL;this->mtns = NULL;
	this->vertices.clear();this->shader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

//Draw and position the elements of the SquareStadium
void SquareStadium::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	if (this->GLReturnedError("SquareStadium::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	mat4 another, floorMatrix, wallMatrix, wallMatrixReset;

	modelview = translate(modelview, vec3(499.f,0.f,500.f));

	//Elements of the arena.
	floorMatrix = modelview;
	this->floor->Draw(projection, modelview, size, time);

	int subWallSpacing = 50;
	int numSubWalls = 21;

	wallMatrix = modelview;
	wallMatrix = rotate(wallMatrix, 180.f, vec3(0,1,0));
	wallMatrixReset = wallMatrix;
	for(int i=0; i<numSubWalls; i++){
		wallMatrix = translate(wallMatrixReset, vec3(i*subWallSpacing, 50, 0));
		wallMatrix = rotate(wallMatrix, 90.f, vec3(1,0,0));
		wallMatrix = rotate(wallMatrix, 180.f, vec3(0,1,0));
		wallMatrix = rotate(wallMatrix, 180.f, vec3(0,0,1));
		this->wall->Draw(projection, wallMatrix, size, 2.5*i/numSubWalls);
	}
	
	wallMatrix = translate(modelview, vec3(1, 0, -1000));
	wallMatrix = rotate(wallMatrix, 270.f, vec3(0,1,0));
	wallMatrixReset = wallMatrix;
	for(int i=0; i<numSubWalls; i++){
		wallMatrix = translate(wallMatrixReset, vec3(i*subWallSpacing, 50, 0));
		wallMatrix = rotate(wallMatrix, 90.f, vec3(1,0,0));
		wallMatrix = translate(wallMatrix, vec3(0,1000,0));
		wallMatrix = rotate(wallMatrix, 180.f, vec3(0,1,0));
		this->wall->Draw(projection, wallMatrix, size, 2.5*(1-(float)((float)i/(float)numSubWalls))+2.5);
	}

	wallMatrix = translate(modelview, vec3(1, 0, -1000));
	wallMatrix = rotate(wallMatrix, 270.f, vec3(0,1,0));
	wallMatrixReset = wallMatrix;
	for(int i=0; i<numSubWalls; i++){
		wallMatrix = translate(wallMatrixReset, vec3(i*subWallSpacing, 50, 0));
		wallMatrix = rotate(wallMatrix, 90.f, vec3(1,0,0));
		wallMatrix = rotate(wallMatrix, 180.f, vec3(0,1,0));
		wallMatrix = rotate(wallMatrix, 180.f, vec3(0,0,1));
		this->wall->Draw(projection, wallMatrix, size, (2.5*i/numSubWalls)+7.5);
	}

	wallMatrix = translate(modelview, vec3(0, 0, -1000));
	wallMatrix = rotate(wallMatrix, 180.f, vec3(0,1,0));
	wallMatrixReset = wallMatrix;
	for(int i=0; i<numSubWalls; i++){
		wallMatrix = translate(wallMatrixReset, vec3(i*subWallSpacing, 50, 0));
		wallMatrix = rotate(wallMatrix, 90.f, vec3(1,0,0));
		wallMatrix = rotate(wallMatrix, 180.f, vec3(0,1,0));
		this->wall->Draw(projection, wallMatrix, size, 2.5*(1-(float)((float)i/(float)numSubWalls))+5);
	}

	another = translate(modelview, vec3(-5, 0, -5));
	this->base->color = vec3(0.5,0,0);
	this->base->Draw(projection, another, size, 0);

	another = translate(modelview, vec3(-995, 0, -5));
	this->base->color = vec3(0.5,0.5,0);
	this->base->Draw(projection, another, size, 0);

	another = translate(modelview, vec3(-995, 0, -995));
	this->base->color = vec3(0,0.5,0);
	this->base->Draw(projection, another, size, 0);

	another = translate(modelview, vec3(-5, 0, -995));
	this->base->color = vec3(0,0,0.5);
	this->base->Draw(projection, another, size, 0);

	if (this->GLReturnedError("SquareStadium::Draw - on exit"))
		return;
}
