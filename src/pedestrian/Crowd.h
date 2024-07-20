#include "Pedestrian.h"
#include "Ward.h"
#include "nlohmann/json.hpp"
#include "src/bai2/Distribution.h"
#include "src/utility/Utility.h"
#include <unordered_map>
#include "src/path/voronoid.h"
#include <fstream>
class Pedestrians{
public:
std::vector<Pedestrian> pedestrians;
int numOfpedes=0;
int numOfPersonel = 0;
int numOfVisitor = 0;
int numOfPatient = 0;
//data sample:
std::map<std::string,Ward>all_wards;
std::vector<double>age_dist;
std::vector<int>six_group;
std::vector<double>velo_dist;
std::vector<std::vector<double>> event_dist;
//method:
Pedestrians();
~Pedestrians();
void setNumOfpedes(int n);
int getNumOfpedes(){return numOfpedes;}
void addPedestrian(Pedestrian pedes);
int getPedesSize(){return pedestrians.size();}
int getNumPersonel(){return numOfPersonel;}
int getNumVisitor(){return numOfVisitor;}
int getNumPatient(){return numOfPatient;}
void setPVP();
//

//data 
std::vector<double>getAge_dist(){return age_dist;}
void setAge_dist();

std::vector<int> getSix_grp(){return six_group;}
void setSix_group();

std::vector<double> getVelo_dist(){return velo_dist;}
void setVelo_dist();

std::vector<std::vector<double>> getEventdist(){return event_dist;}
void setEvent_dist();

//ward
void setWardlist(map<std::string, std::vector<float>> mapdata);
std::map<std::string,Ward>getWardlist(){return all_wards;}
//event( create event for each pedestrian)
std::vector<Event> create_Even_forEach();
//Journey
void set_Journey();
//start end of personel:
std::vector<Ward> Create_ward_dist_forPersonel();

//agv impact
void setAGV_impact();
//walking time
void setWalking_time();

void set_pedestrians();

void export_data();



};