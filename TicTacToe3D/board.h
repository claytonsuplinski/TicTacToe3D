/*
Name: Clayton Suplinski
Project: InsertProjectNameHere
*/

#pragma once
#include "object.h"
#include "shader.h"
#include "sphere.h"
#include "cube.h"
#include "piece.h"
#include <vector>

using namespace std;
using namespace glm;

class Board : public Object
{
public:
	Board();
	~Board();
	virtual bool Initialize(vec3 color);
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void drawGridLines(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void aiMove();
	void userMove();
	bool checkMove(vec3 move);
	bool checkWin(bool circles);
	void moveMarker(int mag, vec3 direction);
	void placeMarker();
	Shader shader;

	Cube gridLine;

	Piece x;
	Piece o;

	static const int gridSize = 50;

	int teamNo;
	int userNo;	

	struct pieces{
		bool isO;
		vec3 pos;
	};

	vector<pieces> activePieces;
	vector<pieces> activeOs;
	vector<pieces> activeXs;
	
	vec3 p1Marker;
	vec3 p2Marker;

	static const int X_TURN = 1;
	static const int O_TURN = 2;

	int whoseTurn;
	bool userTurn;
	bool twoPlayer;

	bool xWins;
	bool oWins;

private:
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};