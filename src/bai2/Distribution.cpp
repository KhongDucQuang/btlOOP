#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include "Distribution.h"
#include <random>
#include <iomanip>


  std::vector<double> Create_Age_dist(int numOfAgents){
    // Sử dụng std::random_device để sinh giá trị ngẫu nhiên để làm seed
    std::random_device rd;
    std::mt19937 gen(rd());

    // Số lượng mẫu
    int num_samples = numOfAgents;

    // Giá trị tối thiểu và tối đa
    float min_value = 5.0f;
    float max_value = 104.0f;

    // Định nghĩa phân phối normal
    std::normal_distribution<double> distribution((max_value + min_value) / 2.0, (max_value - min_value) / 6.0);

    // Tạo một vector để lưu trữ mẫu
    std::vector<double> age_samples;

    // Sinh mẫu
    for (int i = 0; i < num_samples; ++i) {
        double sample = distribution(gen);

        // Kiểm tra và giới hạn giá trị trong khoảng [min_value, max_value]
        if (sample < min_value) {
            sample = min_value;
        } else if (sample > max_value) {
            sample = max_value;
        }
     
        age_samples.push_back(std::round(sample * 10.0) / 10.0);

    }
    


    return age_samples;
}



std::vector<int> Create_dist_for_grp(int n,int grp,int lower_bound,int upper_bound) {
    // Seed cho generator ngẫu nhiên
    std::random_device rd;
    std::mt19937 gen(rd());
   
    // Điều kiện đã cho
   
    const int num_elements = grp;

    // Tính mean và standard deviation
    const double mean = static_cast<double>(n) / num_elements;
    const double std_dev = (mean - lower_bound) / 3;

    // Tạo distribution chuẩn
    std::normal_distribution<double> distribution(mean, std_dev);

    // Tạo vector chứa các phần tử
    std::vector<int> elements;
    int sum_e=0;
    // Tạo và in ra các phần tử ngẫu nhiên
    for (int i = 0; i < num_elements; ++i) {
        double random_value = distribution(gen);

        // Giới hạn giá trị trong khoảng [lower_bound, upper_bound]
        int element = std::min(std::max(static_cast<int>(random_value), lower_bound), upper_bound);
        elements.push_back(element);
        if(i!=num_elements-1){sum_e=sum_e+elements[i];}
    }
    elements[num_elements-1]=n-sum_e>0?n-sum_e:0;

    return elements;
}

std::vector<std::vector<double>> Create_Event_dist(int n) {
    const double min_value = -1.0;
    const double max_value = 1.0;

    const double mean = (min_value + max_value) / 2.0;
    const double std_dev = (max_value - min_value) / 6.0;

    std::vector<std::vector<double>> allEvents;

    for (int i = 0; i < 6; ++i) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> distribution(mean, std_dev);

        std::vector<double> sample_event;
        for (int j = 0; j < n; ++j) {
            double value = std::round(distribution(gen) * 10.0) / 10.0;
            sample_event.push_back(value);
        }

        allEvents.push_back(sample_event);
    }

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < n; ++j) {
            if (allEvents[i][j] < 0) {
                allEvents[i][j] = (-1) * std::abs(allEvents[i][j]);
            } else {
                allEvents[i][j] = std::abs(allEvents[i][j]);
            }
        }
    }

    return allEvents;   
}
std::vector<double> Create_Time_dist() {
    int n=20;
    double min_value=100.0,max_value=3600.0;
    const double mean = (min_value + max_value) / 2.0;
    const double std_dev = (max_value - min_value) / 6.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> distribution(mean, std_dev);

    std::vector<double> sample;
    for (int i = 0; i < n; ++i) {
        double value = std::round(distribution(gen) * 10.0) / 10.0;
        sample.push_back(value);
    }

    return sample;
}


std::vector<std::string> Create_ward_dist_forPersonel(int n) {
    // Seed cho generator ngẫu nhiên
    std::random_device rd;
    std::mt19937 gen(rd());
   
    // Điều kiện đã cho
    const int lower_bound = 0;
    const int upper_bound = 10000;
    const int num_elements = 10;

    // Tính mean và standard deviation
    const double mean = static_cast<double>(n) / num_elements;
    const double std_dev = mean/6.0;

    // Tạo distribution chuẩn
    std::normal_distribution<double> distribution(mean, std_dev);

    // Tạo vector chứa các phần tử
    std::vector<int> elements;
    int sum_e = 0;
    // Tạo và in ra các phần tử ngẫu nhiên
    for (int i = 0; i < num_elements; ++i) {
        double random_value = distribution(gen);

        // Giới hạn giá trị trong khoảng [lower_bound, upper_bound]
        int element = std::min(std::max(static_cast<int>(random_value), lower_bound), upper_bound);
        elements.push_back(element);
        if(i != num_elements - 1) {
            sum_e += elements[i];
        }
    }
    elements[num_elements - 1] = n-sum_e>0?n - sum_e:0;
    std::vector<std::string> wardname = {"A", "L", "B", "F", "E", "M", "W", "G", "K", "N"};
    
    std::vector<std::string> ward_for_personel;
    for(int j = 0; j < 10; j++) {
        for(int i = 0; i < elements[j]; i++) {
            ward_for_personel.push_back(wardname[j]);
            
        }
    }
    return ward_for_personel;
}

std::vector<double> Create_walking_time_dist(int n,double min_value,double max_value) {
    
    const double mean = (min_value + max_value) / 2;
    const double std_dev = 1;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> distribution(mean, std_dev);

    std::vector<double> sample;
    for (int i = 0; i < n; ++i) {
        double value = std::round(distribution(gen) * 10.0) / 10.0;
        sample.push_back(value);
    }

    return sample;
}

std::vector<std::vector<double>> create_impact_dist(int numOfSamples, int numOfValues, double minValue, double maxValue)
{
    double mean = (minValue + maxValue) / 2.0;
    double std = (maxValue - mean) / 3.0;

    // Tạo mẫu ngẫu nhiên tuân theo phân phối chuẩn
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> normalDist(mean, std); // Phân phối chuẩn với trung bình mean và độ lệch chuẩn std

    // Tạo mẫu dữ liệu tuân theo phân phối chuẩn
    std::vector<std::vector<double>> samples(6, std::vector<double>(numOfValues, 0.0));

    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < numOfValues; ++j)
        {
            double value = normalDist(gen);
            samples[i][j] = std::min(std::max(value, minValue), maxValue);
        }
    }

    return samples;
}