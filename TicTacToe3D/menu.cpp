/*
Name: Clayton Suplinski
*/

#include <iostream>
#include "menu.h"

using namespace std;

//Create a menu object
Menu::Menu(){
}

Menu::~Menu(){}

//Initialize the menu.
bool Menu::Initialize(){
	menuOn = true;

	menuLevel = 0;

	title1="Main Menu";
	title2="";
	line1="(1) Option 1";
	line2="(2) Option 2";
	line3="(3) Option 3";
	line4="(4) Back";

	return true;
}

void Menu::Update(int input)
{
	switch(input){
	case 1:
		if(!select1 && menuLevel == 0){
			title2="1";
			select1=true;			
			menuLevel=1;
		}
		else if(select1 && !select11 && menuLevel == 1){
			title2="1 1";
			select11=true;
			menuLevel=2;
		}
		else if(select2 && !select21 && menuLevel == 1){
			title2="2 1";
			select21=true;
			menuLevel=2;
		}
		break;
	case 2:
		if(select1 && !select12 && menuLevel == 1){
			title2="1 2";
			select12=true;
			menuLevel=2;
		}
		else if(!select2 && menuLevel == 0){
			title2="2";
			select2=true;
			menuLevel=1;
		}
		else if(select2 && !select22 && menuLevel == 1){
			title2="2 2";
			select22=true;
			menuLevel=2;
		}
		break;
	case 3:
		title1="Loading...";
		select3=true;
		menuOn=false;
		break;
	}

}
