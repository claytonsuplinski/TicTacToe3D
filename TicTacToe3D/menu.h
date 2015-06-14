/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a shark object out of basic shapes.
*/

#pragma once

using namespace std;

class Menu
{
public:
	Menu();
	~Menu();
	virtual bool Initialize();
	virtual void Update(int input);

	bool menuOn;
	int menuLevel;

	/////////////

	bool select1;

	bool select11;
	bool select12;
	bool select13;

	/////////////

	bool select2;

	bool select21;
	bool select22;
	bool select23;

	/////////////

	bool select3;

	string title1;
	string title2;

	string line1;
	string line2;
	string line3;
	string line4;

private:
	
};