#include "Ward.h"

Ward::Ward(){}
Ward::~Ward(){}


 void Ward::setEntrance(float x,float y){
    entrance=Point3f(x,y,0.0);

 };

 void Ward::setExit(float x,float y){
    exit=Point3f(x,y,0.0);

 }
void Ward::setWalls(){
    Wall down=  Wall(entrance.x - width/2, entrance.y, entrance.x + width/2,entrance.y);
    Wall right=  Wall(entrance.x + width/2, entrance.y, exit.x + width/2, exit.y);
    Wall up=   Wall(exit.x - width / 2,exit.y,exit.x + width/ 2,exit.y);
    Wall left= Wall(exit.x - width / 2, exit.y, entrance.x - width/2, entrance.y); 
    walls.push_back(up);
    walls.push_back(down);
    walls.push_back(left);
    walls.push_back(right);
}

void Ward::setName(std::string n){
   this->name=n;
};

void Ward::setWidth(float wid){
   width=wid;
};