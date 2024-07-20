#ifndef PEDESTRIAN_H
#define PEDESTRIAN_H

#include <iostream>
#include <string>
#include <vector>
#include "src/wall/Wall.h"
#include <map>
#include <nlohmann/json.hpp>
#include <random>
#include "src/bai2/Distribution.h"
#include "src/utility/Utility.h"
#include <algorithm>
#include "src/constant/Constant.h"
#include <unordered_map>
#include <numeric>
#include <fstream>
#include <iomanip>
#include "src/path/voronoid.h"
#include "Ward.h"

using json = nlohmann::json;


enum class Walkability
{
    noDisability,
    crutches,
    sticks,
    wheelchairs,
    blind
};

// Class Personality
class Personality
{
public:
    int lambda;
    double positiveEmotionThreshold;
    double negativeEmotionThreshold;
    Personality();
    int getLambda(){return lambda;}
    double getPositive(){return positiveEmotionThreshold;}
    double getNegative(){return negativeEmotionThreshold;}
    void setPersonaliy(int l,double p,double n);
    
};
// Class Emotion
class Emotion
{
public:
    double pleasure=0.75;
    double surprise=0.5;
    double anger=-0.2;
    double fear=-0.2;
    double hate=-0.4;
    double sad=-0.4;
    Emotion();
    ~Emotion();
    void setEmotion(double p,double sur,double a,double f,double h,double s);
    double getPleasure(){return pleasure;}
    double getSurprise(){return surprise;}
    double getAnger(){return anger;}
    double getFear(){return fear;}
    double getHate(){return hate;}
    double getSad(){return sad;}

};
// Class Event
class Event
{
public:
    std::vector<double> intensity;
    double time;
    double getTime(){return time;}
    std::vector<double> getIntensity(){return intensity;}
    Event();
    ~Event();
    void setEvent(std::vector<double>&inte,double ti);

   };
class AGVEvent : public Event
{
public:
AGVEvent();    
~AGVEvent();
};



class Pedestrian
{
public:
    string role;
    int ID;
    Ward start;
    Ward end;
    std::vector<Ward> journey;
    double velocity;
    Personality personality;
    Emotion emotion;
    std::vector<Event> events;
    double walkingtime;
    double distance;
    double age;
    AGVEvent impactOfAGV;
    Point3f tempPoints;
    std::vector<float> route;
    //method:
    
    Pedestrian();
    ~Pedestrian();
    int getID(){return ID;}
    std::string getRole(){return role;}
    double getWalkingtime(){return walkingtime;}
    double getDistance(){return distance;}
    double getAge(){return age;}
    double getVelocity(){return velocity;}
    Ward getStart(){return start;}
    Ward getEnd(){return end;}
    std::vector<Ward> getJourney(){return journey;}
    Personality getPersonality(){return personality;}
    Emotion getEmotion(){return emotion;}
    std::vector<Event> getEvents(){return events;}
    AGVEvent getImpactOfAGV(){return impactOfAGV;}
    std::vector<float> getRoute(){return route;}

    void setRole(const std::string& newRole);
    void setID(int newID);
    void setStart(const Ward& newStart);
    void setEnd(const Ward& newEnd);
    void setJourney(const std::vector<Ward>& newJourney);
    void setVelocity(double newVelocity);
    void setPersonality(const Personality& newPersonality);
    void setEmotion(const Emotion& newEmotion);
    void setEvents(const std::vector<Event>& newEvents);
    void setWalkingTime(double newWalkingTime);
    void setDistance(double newDistance);
    void setAge(double newAge);
    void setImpactOfAGV(const AGVEvent& newImpactOfAGV);
    void setTempPoints(const Point3f& newTempPoints);
    void setRoute(int n);

   std::vector<vector<double>> calculateEmotions(int timeHorizon);
};

class Patient : public Pedestrian
{

public:

    Patient();
    ~Patient();
    Walkability walkability;
    void setWalkability(Walkability w);
  
};

class Visitor : public Pedestrian
{

public:
    Walkability walkability;
    Visitor();
    ~Visitor();
    void setWalkability(Walkability w);
};

class Personel : public Pedestrian
{
private:
public:
 Walkability walkability;
    Personel();
    ~Personel();
    void setWalkability(Walkability w);
};



#endif
