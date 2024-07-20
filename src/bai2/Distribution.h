#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <vector>
//Tạo phân phối tuổi cho n người
std::vector<double> Create_Age_dist(int numOfAgents);
//phân phối phần trăm 6 nhóm ng 
std::vector<int> Create_dist_for_grp(int n,int grp,int lower_bound,int upper_bound);
//tạo phân phối sự kiện cho n người
std::vector<std::vector<double>> Create_Event_dist(int n);
//tạo thời gian giữa 20 sự kiện cho 1 ng 
std::vector<double> Create_Time_dist();
std::vector<std::string> Create_ward_dist_forPersonel(int n);
std::vector<double> Create_walking_time_dist(int n,double min_value,double max_value);
std::vector<std::vector<double>> create_impact_dist(int numOfSamples, int numOfValues, double minValue, double maxValue);
#endif // DISTRIBUTION_H
