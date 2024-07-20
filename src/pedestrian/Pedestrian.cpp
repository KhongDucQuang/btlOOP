#include "Pedestrian.h"


//emotion
Emotion::Emotion(){
    pleasure=0.75; 
    surprise=0.5; 
    anger=-0.2; 
    fear=-0.2; 
    hate=-0.4; 
    sad=-0.4;
}
Emotion::~Emotion(){};
 void Emotion::setEmotion(double p,double sur,double a,double f,double h,double s){
    pleasure=p; 
    surprise=sur; 
    anger=a; 
    fear=f; 
    hate=h; 
    sad=s;
 };

//event:
Event::Event(){}
Event::~Event(){}
void Event::setEvent(std::vector<double>&inte,double ti){
        intensity=inte;
        time=ti;
};
//AGVEvent
AGVEvent::AGVEvent(){
Event();    
}
AGVEvent::~AGVEvent(){}


//personality
Personality::Personality(){
};
void Personality::setPersonaliy(int l,double p,double n){
lambda=l;
positiveEmotionThreshold=p;
negativeEmotionThreshold=n;
};


//Pedestrian
Pedestrian::Pedestrian(){};
Pedestrian::~Pedestrian(){};
void Pedestrian::setRole(const std::string& newRole) { role = newRole; }
void Pedestrian::setID(int newID) { ID = newID; }
void Pedestrian::setStart(const Ward& newStart) { start = newStart; }
void Pedestrian::setEnd(const Ward& newEnd) { end = newEnd; }
void Pedestrian::setJourney(const std::vector<Ward>& newJourney) { journey = newJourney; }
void Pedestrian::setVelocity(double newVelocity) { velocity = newVelocity; }
void Pedestrian::setPersonality(const Personality& newPersonality) { personality = newPersonality; }
void Pedestrian::setEmotion(const Emotion& newEmotion) { emotion = newEmotion; }
void Pedestrian::setEvents(const std::vector<Event>& newEvents) { events = newEvents; }
void Pedestrian::setWalkingTime(double newWalkingTime) { walkingtime = newWalkingTime; }
void Pedestrian::setDistance(double newDistance) { distance = newDistance; }
void Pedestrian::setAge(double newAge) { age = newAge; }
void Pedestrian::setImpactOfAGV(const AGVEvent& newImpactOfAGV) { impactOfAGV = newImpactOfAGV; }
void Pedestrian::setTempPoints(const Point3f& newTempPoints) { tempPoints = newTempPoints; }
void Pedestrian::setRoute(int n) { 
float x1=start.getExit().x-0.1;
        float y1=start.getExit().y+0.1;
        std::vector<float>temppoint;
        //start:
        temppoint.push_back(x1);
        temppoint.push_back(y1);
        //checkpoint:
        for(Ward w:journey){
            
        temppoint.push_back(w.getEntrance().x-0.1);
        temppoint.push_back(w.getEntrance().y-1);
        float x2=w.getExit().x-0.1;
        float y2=w.getExit().y+0.1;
        temppoint.push_back(x2);
        temppoint.push_back(y2);
        }
         temppoint.push_back(end.getEntrance().x-0.1);
         temppoint.push_back(end.getEntrance().y-1);
        //
        for(int j=0;j<temppoint.size();j+=4){
            route.push_back(temppoint[j]);
            route.push_back(temppoint[j+1]);
            point s=point(temppoint[j],temppoint[j+1]);
            point e=point(temppoint[j+2],temppoint[j+3]);
            std::vector<int> f=find_path(s,e,n);
            for(float a:f){route.push_back(a);}
            route.push_back(temppoint[j+2]);
            route.push_back(temppoint[j+3]);  

        }
    
    
 }

//Patient
Patient::Patient(){
    Pedestrian();
};
Patient::~Patient(){};
void Patient::setWalkability(Walkability w){
walkability=w;
}

//Visitor
Visitor::Visitor(){
    Pedestrian();
}
Visitor::~Visitor(){}

void Visitor::setWalkability(Walkability w){
walkability=w;
}

Personel::Personel(){
    Pedestrian();
}
Personel::~Personel(){}
void Personel::setWalkability(Walkability w){
    walkability=w;
}

std::vector<vector<double>> Pedestrian::calculateEmotions(int timeHorizon)
{

    double lambda = getPersonality().getLambda();
    std::vector<Event> ev=getEvents();
    std::vector<vector<double>> events;
    for (int i = 0; i < 6; i++)
    {
        vector<double> temp;
        for (int j = 0; j < 20; j++)
        {   
            temp.push_back(ev[j].getIntensity()[i]);
        }
        events.push_back(temp);
    }

    int times[19];
    for (int i = 0; i < 19; ++i)
    {
        times[i] = Utility::randomInt(4, 10); // Random giá trị từ 4 đến 10
    }
    vector<vector<double>> allEmotions = {{0.75}, {0.5}, {-0.2}, {-0.2}, {-0.4}, {-0.4}};
    int lastTime = 0;
    int index = 0;
    int H = timeHorizon;
    for (int i = 0; i < H; i++)
    {
        double p = allEmotions[0][i];
        double su = allEmotions[1][i];
        double a = allEmotions[2][i];
        double f = allEmotions[3][i];
        double h = allEmotions[4][i];
        double sa = allEmotions[5][i];

        if (i - lastTime == times[index])
        {
            index++;
            lastTime = i;
            p += allEmotions[0][i] * exp(-lambda) + events[0][index];
            su += allEmotions[1][i] * exp(-lambda) + events[1][index];
            a += allEmotions[2][i] * exp(-lambda) + events[2][index];
            f += allEmotions[3][i] * exp(-lambda) + events[3][index];
            h += allEmotions[4][i] * exp(-lambda) + events[4][index];
            sa += allEmotions[5][i] * exp(-lambda) + events[5][index];
        }

        allEmotions[0].push_back(p);
        allEmotions[1].push_back(su);
        allEmotions[2].push_back(a);
        allEmotions[3].push_back(f);
        allEmotions[4].push_back(h);
        allEmotions[5].push_back(sa);
    }

    return allEmotions;
}