#ifndef WARD_H
#define WARD_H

#include <iostream>
#include <string>
#include <vector>
#include "src/wall/Wall.h"


class Ward
{
private:
Point3f entrance;
Point3f exit;
float width;
public:
    std::string name;
   
    std::vector<Wall> walls;
    Ward();
    ~Ward();
    std::string getName(){return name;}
    Point3f getEntrance(){return entrance;}
    Point3f getExit(){return exit;}
    std::string getName(std::string n){return n;}
    float getWidth(){return width;}
    void setWidth(float wid);

    void setEntrance(float x,float y);
    void setExit(float x,float y);
    void setWalls();
    void setName(std::string n);

};



#endif
