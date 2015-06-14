/*
Name: Clayton Suplinski
*/

#include <iostream>
#include "board.h"

using namespace std;
using namespace glm;

Board::Board() : Object(){
}

Board::~Board(){this->TakeDown();}

//Initialize all the Shark elements.
bool Board::Initialize(vec3 color)
{
	if (this->GLReturnedError("Board::Initialize - on entry"))
		return false;

	this->gridLine.color = vec3(0.5, 0.5, 0.5);
	this->gridLine.Initialize(1,1,1, "phong.vert", "phong.frag");

	this->x.Initialize(false);
	this->o.Initialize(true);

	this->p1Marker = vec3(0,0,0);
	this->p2Marker = vec3(0,0,0);

	this->whoseTurn = X_TURN;
	this->userTurn = true;
	this->twoPlayer = true;

	if (this->GLReturnedError("Board::Initialize - on exit"))
		return false;

	return true;
}

void Board::placeMarker(){

}

void Board::moveMarker(int mag, vec3 direction){
	if(userTurn){
		if(whoseTurn == X_TURN){
			p1Marker += vec3(mag, mag, mag)*direction;	
			if(p1Marker.x > 2 || p1Marker.y > 2 || p1Marker.z > 2 ||
				p1Marker.x < 0 || p1Marker.y < 0 || p1Marker.z < 0){
					p1Marker -= vec3(mag, mag, mag)*direction;
			}
		}
		else{
			p2Marker += vec3(mag, mag, mag)*direction;
			if(p2Marker.x > 2 || p2Marker.y > 2 || p2Marker.z > 2 ||
				p2Marker.x < 0 || p2Marker.y < 0 || p2Marker.z < 0){
					p2Marker -= vec3(mag, mag, mag)*direction;
			}
		}
	}
}

bool Board::checkMove(vec3 move){
	for(int i=0; i<activePieces.size(); i++){
		if(activePieces.at(i).pos == move){
			return false;
		}
	}
	return true;
}

bool Board::checkWin(bool circles){
	int x=0;int y=0;int z=0;int diagX=0;int diagY=0;int diagZ=0;int diag=0;
	if(circles){
		if(activeOs.size() > 2){
			for(int i=0; i<activeOs.size(); i++){
				x=0;y=0;z=0;diag=0;diagX=0;diagY=0;diagZ=0;
				if(activeOs.at(i).pos != vec3(1,1,1)){
					for(int j=0; j<activeOs.size(); j++){
						if(i != j){
							//Check x , y, z, and diagonal
							if(activeOs.at(i).pos.x != activeOs.at(j).pos.x && 
								activeOs.at(i).pos.y == activeOs.at(j).pos.y &&
								activeOs.at(i).pos.z == activeOs.at(j).pos.z){
									x++;
									if(x==2){
										oWins = true;
										return true;
									}
							}
							//Check y
							else if(activeOs.at(i).pos.y != activeOs.at(j).pos.y && 
								activeOs.at(i).pos.x == activeOs.at(j).pos.x &&
								activeOs.at(i).pos.z == activeOs.at(j).pos.z){
									y++;
									if(y==2){
										oWins = true;
										return true;
									}
							}
							//Check z
							else if(activeOs.at(i).pos.z != activeOs.at(j).pos.z && 
								activeOs.at(i).pos.x == activeOs.at(j).pos.x &&
								activeOs.at(i).pos.y == activeOs.at(j).pos.y){
									z++;
									if(z==2){
										oWins = true;
										return true;
									}
							}
							else if(activeOs.at(i).pos.x != 1 && activeOs.at(i).pos.y != 1
								|| activeOs.at(i).pos.x != 1 && activeOs.at(i).pos.z != 1
								|| activeOs.at(i).pos.z != 1 && activeOs.at(i).pos.y != 1){
								//Check diagX
								if(activeOs.at(j).pos == activeOs.at(i).pos+vec3(0,1,1) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(0,2,2)
									|| activeOs.at(j).pos == activeOs.at(i).pos+vec3(0,-1,1) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(0,-2,2)
									|| activeOs.at(j).pos == activeOs.at(i).pos+vec3(0,-1,-1) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(0,-2,-2)
									|| activeOs.at(j).pos == activeOs.at(i).pos+vec3(0,1,-1) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(0,2,-2)){
										diagX++;
										if(diagX==2){
											oWins = true;
											return true;
										}
								}
								//Check diagY
								else if(activeOs.at(j).pos == activeOs.at(i).pos+vec3(1,0,1) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(2,0,2)
									|| activeOs.at(j).pos == activeOs.at(i).pos+vec3(-1,0,1) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(-2,0,2)
									|| activeOs.at(j).pos == activeOs.at(i).pos+vec3(-1,0,-1) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(-2,0,-2)
									|| activeOs.at(j).pos == activeOs.at(i).pos+vec3(1,0,-1) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(2,0,-2)){
										diagY++;
										if(diagY==2){
											oWins = true;
											return true;
										}
								}
								//Check diagZ
								else if(activeOs.at(j).pos == activeOs.at(i).pos+vec3(1,1,0) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(2,2,0)
									|| activeOs.at(j).pos == activeOs.at(i).pos+vec3(-1,1,0) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(-2,2,0)
									|| activeOs.at(j).pos == activeOs.at(i).pos+vec3(-1,-1,0) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(-2,-2,0)
									|| activeOs.at(j).pos == activeOs.at(i).pos+vec3(1,-1,0) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(2,-2,0)){
										diagZ++;
										if(diagZ==2){
											oWins = true;
											return true;
										}
								}
								//Check diag
								else if(activeOs.at(i).pos.x != 1 && activeOs.at(i).pos.y != 1 && activeOs.at(i).pos.z != 1){
										if(activeOs.at(j).pos == activeOs.at(i).pos+vec3(1,1,1) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(2,2,2)
									|| activeOs.at(j).pos == activeOs.at(i).pos+vec3(1,1,-1) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(2,2,-2)
									|| activeOs.at(j).pos == activeOs.at(i).pos+vec3(1,-1,1) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(2,-2,2)
									|| activeOs.at(j).pos == activeOs.at(i).pos+vec3(1,-1,-1) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(2,-2,-2)
									|| activeOs.at(j).pos == activeOs.at(i).pos+vec3(-1,1,1) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(-2,2,2)
									|| activeOs.at(j).pos == activeOs.at(i).pos+vec3(-1,1,-1) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(-2,2,-2)
									|| activeOs.at(j).pos == activeOs.at(i).pos+vec3(-1,-1,1) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(-2,-2,2)
									|| activeOs.at(j).pos == activeOs.at(i).pos+vec3(-1,-1,-1) || activeOs.at(j).pos == activeOs.at(i).pos+vec3(-2,-2,-2)){
											diag++;
											if(diag==2){
												oWins = true;
												return true;
											}
										}
								}
							} ///
						}
					}
				}
			}
		}
	}
	else{
		if(activeXs.size() > 2){
			for(int i=0; i<activeXs.size(); i++){
				x=0;y=0;z=0;diag=0;diagX=0;diagY=0;diagZ=0;
				if(activeXs.at(i).pos != vec3(1,1,1)){
					for(int j=0; j<activeXs.size(); j++){
						if(i != j){
							//Check x , y, z, and diagonal
							if(activeXs.at(i).pos.x != activeXs.at(j).pos.x && 
								activeXs.at(i).pos.y == activeXs.at(j).pos.y &&
								activeXs.at(i).pos.z == activeXs.at(j).pos.z){
									x++;
									if(x==2){
										xWins = true;
										return true;
									}
							}
							//Check y
							else if(activeXs.at(i).pos.y != activeXs.at(j).pos.y && 
								activeXs.at(i).pos.x == activeXs.at(j).pos.x &&
								activeXs.at(i).pos.z == activeXs.at(j).pos.z){
									y++;
									if(y==2){
										xWins = true;
										return true;
									}
							}
							//Check z
							else if(activeXs.at(i).pos.z != activeXs.at(j).pos.z && 
								activeXs.at(i).pos.x == activeXs.at(j).pos.x &&
								activeXs.at(i).pos.y == activeXs.at(j).pos.y){
									z++;
									if(z==2){
										xWins = true;
										return true;
									}
							}
							else if(activeXs.at(i).pos.x != 1 && activeXs.at(i).pos.y != 1
								|| activeXs.at(i).pos.x != 1 && activeXs.at(i).pos.z != 1
								|| activeXs.at(i).pos.z != 1 && activeXs.at(i).pos.y != 1){
								//Check diagX
								if(activeXs.at(j).pos == activeXs.at(i).pos+vec3(0,1,1) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(0,2,2)
									|| activeXs.at(j).pos == activeXs.at(i).pos+vec3(0,-1,1) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(0,-2,2)
									|| activeXs.at(j).pos == activeXs.at(i).pos+vec3(0,-1,-1) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(0,-2,-2)
									|| activeXs.at(j).pos == activeXs.at(i).pos+vec3(0,1,-1) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(0,2,-2)){
										diagX++;
										if(diagX==2){
											xWins = true;
											return true;
										}
								}
								//Check diagY
								else if(activeXs.at(j).pos == activeXs.at(i).pos+vec3(1,0,1) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(2,0,2)
									|| activeXs.at(j).pos == activeXs.at(i).pos+vec3(-1,0,1) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(-2,0,2)
									|| activeXs.at(j).pos == activeXs.at(i).pos+vec3(-1,0,-1) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(-2,0,-2)
									|| activeXs.at(j).pos == activeXs.at(i).pos+vec3(1,0,-1) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(2,0,-2)){
										diagY++;
										if(diagY==2){
											xWins = true;
											return true;
										}
								}
								//Check diagZ
								else if(activeXs.at(j).pos == activeXs.at(i).pos+vec3(1,1,0) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(2,2,0)
									|| activeXs.at(j).pos == activeXs.at(i).pos+vec3(-1,1,0) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(-2,2,0)
									|| activeXs.at(j).pos == activeXs.at(i).pos+vec3(-1,-1,0) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(-2,-2,0)
									|| activeXs.at(j).pos == activeXs.at(i).pos+vec3(1,-1,0) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(2,-2,0)){
										diagZ++;
										if(diagZ==2){
											xWins = true;
											return true;
										}
								}
								//Check diag
								else if(activeXs.at(i).pos.x != 1 && activeXs.at(i).pos.y != 1 && activeXs.at(i).pos.z != 1){
										if(activeXs.at(j).pos == activeXs.at(i).pos+vec3(1,1,1) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(2,2,2)
									|| activeXs.at(j).pos == activeXs.at(i).pos+vec3(1,1,-1) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(2,2,-2)
									|| activeXs.at(j).pos == activeXs.at(i).pos+vec3(1,-1,1) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(2,-2,2)
									|| activeXs.at(j).pos == activeXs.at(i).pos+vec3(1,-1,-1) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(2,-2,-2)
									|| activeXs.at(j).pos == activeXs.at(i).pos+vec3(-1,1,1) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(-2,2,2)
									|| activeXs.at(j).pos == activeXs.at(i).pos+vec3(-1,1,-1) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(-2,2,-2)
									|| activeXs.at(j).pos == activeXs.at(i).pos+vec3(-1,-1,1) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(-2,-2,2)
									|| activeXs.at(j).pos == activeXs.at(i).pos+vec3(-1,-1,-1) || activeXs.at(j).pos == activeXs.at(i).pos+vec3(-2,-2,-2)){
											diag++;
											if(diag==2){
												xWins = true;
												return true;
											}
										}
								}
							} ///
						}
					}
				}
			}
		}
	}
	return false;
}

void Board::userMove(){
	pieces tmp;
	if(userTurn && whoseTurn == X_TURN){
		if(checkMove(p1Marker)){
			tmp.isO = false;
			tmp.pos = p1Marker;
			whoseTurn = O_TURN;
			activePieces.push_back(tmp);
			activeXs.push_back(tmp);
			checkWin(false);
		}
	}
	else if(userTurn && whoseTurn == O_TURN){
		if(checkMove(p2Marker)){
			tmp.isO = true;
			tmp.pos = p2Marker;
			whoseTurn = X_TURN;
			activePieces.push_back(tmp);
			activeOs.push_back(tmp);
			checkWin(true);
		}
	}
}

void Board::aiMove(){

}

void Board::TakeDown()
{
	this->x.TakeDown();this->o.TakeDown();
	this->gridLine.TakeDown();
	this->vertices.clear();
	this->shader.TakeDown();
	super::TakeDown();	
}

//Draw and position the Shark elements.
void Board::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float rotY)
{
	if (this->GLReturnedError("Board::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);
	mat4 another;

	drawGridLines(projection, modelview, size, rotY);

	another = modelview;

	if(whoseTurn == X_TURN){
		another = translate(another, vec3(-(p1Marker.x-1)*gridSize/3, (p1Marker.y+0.5)*gridSize/3, (p1Marker.z+0.5)*gridSize/3));
		x.Draw(projection, another, size, rotY);
	}
	else{
		another = translate(another, vec3(-(p2Marker.x-1)*gridSize/3, (p2Marker.y+0.5)*gridSize/3, (p2Marker.z+0.5)*gridSize/3));
		o.Draw(projection, another, size, rotY);
	}

	for(int i=0; i<activePieces.size(); i++){
		another = modelview;
		another = translate(another, vec3(-(activePieces.at(i).pos.x-1)*gridSize/3, 
			(activePieces.at(i).pos.y+0.5)*gridSize/3, (activePieces.at(i).pos.z+0.5)*gridSize/3));
		if(activePieces.at(i).isO){
			o.Draw(projection, another, size, rotY);
		}
		else{
			x.Draw(projection, another, size, rotY);
		}
	}
	
	if (this->GLReturnedError("Board::Draw - on exit"))
		return;
}

void Board::drawGridLines(const mat4 & projection, mat4 modelview, const ivec2 & size, const float rotY)
{
	mat4 another, gridScale;
	another = modelview;

	another = translate(modelview, vec3(-gridSize/2,gridSize/3,gridSize/3));
	gridScale = scale(another, vec3(gridSize,1,1));
	this->gridLine.Draw(projection, gridScale, size, 0);

	another = translate(modelview, vec3(-gridSize/2,2*gridSize/3,gridSize/3));
	gridScale = scale(another, vec3(gridSize,1,1));
	this->gridLine.Draw(projection, gridScale, size, 0);

	another = translate(modelview, vec3(-gridSize/6,0,gridSize/3));
	gridScale = scale(another, vec3(1,gridSize,1));
	this->gridLine.Draw(projection, gridScale, size, 0);

	another = translate(modelview, vec3(gridSize/6,0,gridSize/3));
	gridScale = scale(another, vec3(1,gridSize,1));
	this->gridLine.Draw(projection, gridScale, size, 0);



	another = translate(modelview, vec3(-gridSize/2,gridSize/3,2*gridSize/3));
	gridScale = scale(another, vec3(gridSize,1,1));
	this->gridLine.Draw(projection, gridScale, size, 0);

	another = translate(modelview, vec3(-gridSize/2,2*gridSize/3,2*gridSize/3));
	gridScale = scale(another, vec3(gridSize,1,1));
	this->gridLine.Draw(projection, gridScale, size, 0);

	another = translate(modelview, vec3(-gridSize/6,0,2*gridSize/3));
	gridScale = scale(another, vec3(1,gridSize,1));
	this->gridLine.Draw(projection, gridScale, size, 0);

	another = translate(modelview, vec3(gridSize/6,0,2*gridSize/3));
	gridScale = scale(another, vec3(1,gridSize,1));
	this->gridLine.Draw(projection, gridScale, size, 0);



	another = translate(modelview, vec3(-gridSize/6,2*gridSize/3,0));
	gridScale = scale(another, vec3(1,1,gridSize));
	this->gridLine.Draw(projection, gridScale, size, 0);

	another = translate(modelview, vec3(gridSize/6,2*gridSize/3,0));
	gridScale = scale(another, vec3(1,1,gridSize));
	this->gridLine.Draw(projection, gridScale, size, 0);

	another = translate(modelview, vec3(-gridSize/6,gridSize/3,0));
	gridScale = scale(another, vec3(1,1,gridSize));
	this->gridLine.Draw(projection, gridScale, size, 0);

	another = translate(modelview, vec3(gridSize/6,gridSize/3,0));
	gridScale = scale(another, vec3(1,1,gridSize));
	this->gridLine.Draw(projection, gridScale, size, 0);
}





