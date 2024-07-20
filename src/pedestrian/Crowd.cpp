#include "Crowd.h"
Pedestrians::Pedestrians(){}
Pedestrians::~Pedestrians(){}
void Pedestrians::addPedestrian(Pedestrian pedes){
    pedestrians.push_back(pedes);
};

void Pedestrians::setNumOfpedes(int n){
numOfpedes=n;
};


void Pedestrians::setPVP(){
   for (int i = 0; i < numOfpedes; i++)
    {
        if (velo_dist[i] == Constant::V1 || velo_dist[i] == Constant::V2)
        {
            if (age_dist[i] >= 23 && age_dist[i] <= 61)
            {
                numOfPersonel++;
                continue;
            }
            numOfVisitor++;
            continue;
        }
        else
        {
            numOfPatient++;
        }
    }
};


void Pedestrians::setAge_dist(){
    age_dist=Create_Age_dist(numOfpedes);
};

void Pedestrians::setSix_group(){
    six_group=Create_dist_for_grp(numOfpedes, 6, 0, 12000);
}

void Pedestrians::setVelo_dist(){
velo_dist = Utility::getPedesVelocityBasedDDis(six_group);
};

void Pedestrians::setEvent_dist(){
    event_dist= Create_Event_dist(numOfpedes);
};


void Pedestrians::setWardlist(map<std::string, std::vector<float>> mapdata){

for (auto ward : mapdata)
    {

        string i = ward.first;
        
        if (i == "numI" || i == "S" || i == "D" || i == "mapCoords")
        {
            continue;
        }
         if (i == "A")
        {
        Ward x;
        x.setName(i);
        x.setEntrance(mapdata[i][6],mapdata[i][7]);
        x.setExit(mapdata[i][4],mapdata[i][5]);
        float wid=mapdata["A"][2]-mapdata["A"][0];
        x.setWidth(wid);
        x.setWalls();
        all_wards[i]=x;
        continue;
        }
        Ward y;
        y.setName(i);
        y.setEntrance(mapdata[i][0],mapdata[i][1]);
        y.setExit(mapdata[i][2],mapdata[i][3]);
        y.setWidth(mapdata[i][4]);
        y.setWalls();
        all_wards[i]=y;
        }


};

std::vector<Event>Pedestrians::create_Even_forEach(){
    std::vector<Event> events;
    std::vector<double> t = Create_Time_dist();
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = 0; i < 20; i++)
    {
        gen.seed(rd());
        int rdom1 = gen() % 20;
        int rdom2 = gen() % numOfpedes;
        std::vector<double> in;
        for (int j = 0; j < 6; j++)
        {
            in.push_back(event_dist[j][rdom2]);
        }
        double ti = t[rdom1];
        Event temp;
        temp.setEvent(in,ti);
        events.push_back(temp);
    }
    return events;
};



void Pedestrians::set_Journey()
{

    int k_groups = (numOfPatient + numOfPersonel) * 3 + numOfVisitor;
    std::vector<int> random_values = Create_dist_for_grp(k_groups, 9, 0, 10000);
    // tao ra cac pair
    std::vector<std::pair<Ward, int>> pairs;
    int i = 0;
    for (auto &ward : all_wards)
    {   
        if(ward.first!="A"){
        pairs.push_back(std::make_pair(ward.second, random_values[i]));
        i++;}

    }
    // tao mang cac pair
    std::vector<std::pair<Ward, int>> arrPair = pairs;
    // output

    // tao tu dien luu tru so lan xuat hien cua moi ky tu
    std::map<std::string, int> char_count;
    // Duyet qua mang cac pair(them ky tu va so lan xuat hien cua moi ky tu);
    for (int i = 0; i < all_wards.size()-1; i++)
    {
        char_count[arrPair[i].first.getName()] = arrPair[i].second;
    }
    while (std::any_of(char_count.begin(), char_count.end(), [](const std::pair<std::string, int> &p)
                       { return p.second > 0; }))
    {
        for (auto &pedestrian : pedestrians)
        {
            // kiem tra xem doi tuong co la Visitor hay khong
            if (pedestrian.getRole() == "Visitor")
            {
                auto max_char = std::max_element(char_count.begin(), char_count.end(), [](const auto &a, const auto &b)
                                                 { return a.second < b.second; });
                std::vector<Ward> temp;
                temp.push_back(all_wards[max_char->first]);
                pedestrian.setJourney(temp);

                max_char->second--;
                if (max_char->second == 0)
                {
                    char_count.erase(max_char);
                }
            }
            else
            {
                std::vector<std::string> chars;
                std::map<std::string, int> temp = char_count;

                for (int i = 0; i < 3; ++i)
                {
                    auto it = std::max_element(temp.begin(), temp.end(),
                                               [](const auto &a, const auto &b)
                                               { return a.second < b.second; });

                    chars.push_back(it->first);
                    it->second--;
                    char_count[it->first]--;
                    // Xóa ký tự đã được chọn khỏi bản sao để không chọn lại trong lần lặp tiếp theo
                    temp.erase(it);
                }
                std::vector<Ward>tempward;
                for(int i=0;i<3;i++){tempward.push_back(all_wards[chars[i]]);}
                pedestrian.setJourney(tempward);
            }
        }
    }

   
}


void Pedestrians::setAGV_impact()
{
    int M = numOfpedes;
    int nChildren, nPersonelOfBFGMEN, nElder, nBlinder, nOthers, nPersonelOfALKW;
    nChildren = nPersonelOfALKW = nPersonelOfBFGMEN = nElder = nBlinder = nOthers = 0;

    for (int i = 0; i < M; i++)
    {
        if (pedestrians[i].age < 12)
        {
            nChildren++;
        }
        else if ((pedestrians[i].getRole() == "Personel") && (pedestrians[i].getStart().getName() == "A" || pedestrians[i].getStart().getName()== "L" || pedestrians[i].getStart().getName() == "K" || pedestrians[i].getStart().getName() == "W"))
        {
            nPersonelOfALKW++;
        }
        else if ((pedestrians[i].getRole() == "Personel") && (pedestrians[i].getStart().getName() == "B" || pedestrians[i].getStart().getName() == "F" || pedestrians[i].getStart().getName() == "G" || pedestrians[i].getStart().getName() == "M" || pedestrians[i].getStart().getName() == "E" || pedestrians[i].getStart().getName() == "N"))
        {
            nPersonelOfBFGMEN++;
        }
        else if (pedestrians[i].getAge() > 60)
        {
            nElder++;
        }
        else if (pedestrians[i].getVelocity() == Constant::V6)
        {
            nBlinder++;
        }
        else
        {
            nOthers++;
        }
    }

    std::vector<std::vector<double>> impactToChildren = create_impact_dist(6, nChildren, 0.01, 0.99);
    std::vector<std::vector<double>> impactToALKW = create_impact_dist(6, nPersonelOfALKW, -0.29, 0.99);
    std::vector<std::vector<double>> impactToBFGMEN = create_impact_dist(6, nPersonelOfBFGMEN, -0.29, 0.99);
    for (int i = 0; i < nPersonelOfBFGMEN; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            impactToBFGMEN[j][i] *= -1;
        }
    }
    std::vector<std::vector<double>> impactToElder = create_impact_dist(6, nElder, -0.29, 0.99);
    for (int i = 0; i < nElder; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            impactToElder[j][i] *= -1;
        }
    }
    std::vector<std::vector<double>> impactToBlinder = create_impact_dist(6, nBlinder, -0.29, 0.99);
    for (int i = 0; i < nBlinder; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            impactToBlinder[j][i] *= -1;
        }
    }
    std::vector<std::vector<double>> impactToOthers = create_impact_dist(6, nOthers, -0.69, 0.99);



    for (int i = 0; i < M; i++)
    {
        std::vector<double> row;
        if (pedestrians[i].getAge() < 12)
        {
            for (int j = 0; j < 6; j++)
            {
                row.push_back(impactToChildren[j][nChildren - 1]);
            }
            --nChildren;
        }
        else if (pedestrians[i].getRole() == "Personel" && (pedestrians[i].getStart().getName() == "A" || pedestrians[i].getStart().getName() == "L" || pedestrians[i].getStart().getName() == "K" || pedestrians[i].getStart().getName() == "W"))
        {
            for (int j = 0; j < 6; j++)
            {
                row.push_back(impactToALKW[j][nPersonelOfALKW - 1]);
            }
            --nPersonelOfALKW;
        }
        else if (pedestrians[i].getRole() == "Personel" && (pedestrians[i].getStart().getName() == "B" || pedestrians[i].getStart().getName() == "F" || pedestrians[i].getStart().getName() == "G" || pedestrians[i].getStart().getName() == "M" || pedestrians[i].getStart().getName() == "E" || pedestrians[i].getStart().getName() == "N"))
        {
            for (int j = 0; j < 6; j++)
            {
                row.push_back(impactToBFGMEN[j][nPersonelOfBFGMEN - 1]);
            }
            --nPersonelOfBFGMEN;
        }
        else if (pedestrians[i].getAge() > 60)
        {
            for (int j = 0; j < 6; j++)
            {
                row.push_back(impactToElder[j][nElder - 1]);
            }
            --nElder;
        }
        else if (pedestrians[i].getVelocity() == Constant::V6)
        {
            for (int j = 0; j < 6; j++)
            {
                row.push_back(impactToBlinder[j][nBlinder - 1]);
            }
            --nBlinder;
        }
        else
        {
            for (int j = 0; j < 6; j++)
            {
                row.push_back(impactToOthers[j][nOthers - 1]);
            }
            --nOthers;
        }
        AGVEvent e;
        e.setEvent(row,0);
        pedestrians[i].setImpactOfAGV(e);
    }

}


void Pedestrians::setWalking_time()
{
    std::vector<int> numOfpeople_leaving = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<std::string> wardname = {"A", "L", "B", "F", "E", "M", "W", "G", "K", "N"};
    for (auto &pedes : pedestrians)
    {
        for (int i = 0; i < 10; i++)
        {
            if (pedes.getStart().getName() == wardname[i])
            {
                numOfpeople_leaving[i]++;
                break;
            }
        }
    }

    std::vector<double> walking_time_A = Create_walking_time_dist(numOfpeople_leaving[0], 0, 1000);
    std::vector<double> walking_time_L = Create_walking_time_dist(numOfpeople_leaving[1], 13, 700);
    std::vector<double> walking_time_B = Create_walking_time_dist(numOfpeople_leaving[2], 13, 700);
    std::vector<double> walking_time_F = Create_walking_time_dist(numOfpeople_leaving[3], 15, 400);
    std::vector<double> walking_time_E = Create_walking_time_dist(numOfpeople_leaving[4], 16, 700);
    std::vector<double> walking_time_M = Create_walking_time_dist(numOfpeople_leaving[5], 13, 700);
    std::vector<double> walking_time_W = Create_walking_time_dist(numOfpeople_leaving[6], 13, 700);
    std::vector<double> walking_time_G = Create_walking_time_dist(numOfpeople_leaving[7], 13, 700);
    std::vector<double> walking_time_K = Create_walking_time_dist(numOfpeople_leaving[8], 13, 700);
    std::vector<double> walking_time_N = Create_walking_time_dist(numOfpeople_leaving[9], 13, 700);

    std::vector<double> walking_time;
    for (auto &pedes : pedestrians)
    {
        if (pedes.getStart().getName() == "A")
        {
            pedes.setWalkingTime(walking_time_A[walking_time_A.size() - 1]);       
            walking_time_A.pop_back();
        }
        else if (pedes.getStart().getName() == "L")
        {
          
            pedes.setWalkingTime(walking_time_L[walking_time_L.size() - 1]);    
            walking_time_L.pop_back();
        }
        else if (pedes.getStart().getName() == "B")
        {   
            pedes.setWalkingTime(walking_time_B[walking_time_B.size() - 1]);    
            walking_time_B.pop_back();
        }
        else if (pedes.getStart().getName() == "F")
        {
            pedes.setWalkingTime(walking_time_F[walking_time_F.size() - 1]);    
            walking_time_F.pop_back();
        }
        else if (pedes.getStart().getName() == "E")
        {
            pedes.setWalkingTime(walking_time_E[walking_time_E.size() - 1]);    
            walking_time_E.pop_back();
        }
        else if (pedes.getStart().getName() == "M")
        {
            pedes.setWalkingTime(walking_time_M[walking_time_M.size() - 1]);    
            walking_time_M.pop_back();
        }
        else if (pedes.getStart().getName() == "W")
        {
            pedes.setWalkingTime(walking_time_W[walking_time_W.size() - 1]); 
            walking_time_W.pop_back();
        }
        else if (pedes.getStart().getName() == "G")
        {
            pedes.setWalkingTime(walking_time_G[walking_time_G.size() - 1]); 
            walking_time_G.pop_back();
        }
        else if (pedes.getStart().getName() == "K")
        {
            pedes.setWalkingTime(walking_time_K[walking_time_K.size() - 1]); 
            walking_time_K.pop_back();
        }
        else
        {
            pedes.setWalkingTime(walking_time_N[walking_time_N.size() - 1]); 
            walking_time_N.pop_back();
        }
    }

}

std::vector<Ward>Pedestrians::Create_ward_dist_forPersonel() {
    // Seed cho generator ngẫu nhiên
    std::random_device rd;
    std::mt19937 gen(rd());
    int n=numOfPersonel;
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
    
    std::vector<Ward> ward_for_personel;
    for(int j = 0; j < 10; j++) {
        for(int i = 0; i < elements[j]; i++) {
            ward_for_personel.push_back(all_wards[wardname[j]]);
            
        }
    }
    return ward_for_personel;
}

void Pedestrians::set_pedestrians(){
std::map<std::string, std::vector<float>> mapData2 = Utility::readMapData2("data/hospital.txt");
json inputData = Utility::readInputData("data/input.json");

setNumOfpedes(int(inputData["numOfAgents"]["value"]));
setWardlist(mapData2);
setSix_group();
setVelo_dist();
setAge_dist();
setPVP();
setEvent_dist();

std::vector<Ward>start_end_ofPersonel=Create_ward_dist_forPersonel();
for(int i=0;i<getNumOfpedes();i++){
    int index=0;
    if (velo_dist[i] == Constant::V1 || velo_dist[i] == Constant::V2)
        {
            if (age_dist[i] >= 23 && age_dist[i] <= 61)
            {
                Personel p;
                p.setRole("Personel");
                p.setID(i+1);
                p.setAge(age_dist[i]);
                p.setVelocity(velo_dist[i]);
                p.setStart(start_end_ofPersonel[index]);
                p.setEnd(start_end_ofPersonel[index]);
                p.setEvents(create_Even_forEach());
                addPedestrian(p);
                index++;
                continue;
            }
             Visitor p;
                p.setRole("Visitor");
                p.setID(i+1);
                p.setAge(age_dist[i]);
                p.setVelocity(velo_dist[i]);
                p.setStart(all_wards["A"]);
                p.setEnd(all_wards["A"]);
                p.setEvents(create_Even_forEach());
                addPedestrian(p);
            continue;
        }
        else
        {
           Patient p;
                p.setRole("Patient");
                p.setID(i+1);
                p.setAge(age_dist[i]);
                p.setVelocity(velo_dist[i]);
                p.setStart(all_wards["A"]);
                p.setEnd(all_wards["A"]);
                p.setEvents(create_Even_forEach());
                addPedestrian(p);
        }

    
}
set_Journey();
setAGV_impact();
readInput();
int n=all_wards.size();
for(auto&p:pedestrians){
p.setRoute(n);
int open=numOfpedes/2;
int neurotic=numOfpedes-open;
if(p.getAge()>=11&&neurotic>0){
        Personality neur;
        neur.setPersonaliy(4,0.6,-0.4);
        p.setPersonality(neur);
        neurotic--;
        }
    else{
        Personality op;
        op.setPersonaliy(1,0.3,-0.7);
        p.setPersonality(op);
        open--;
    }
    
}
setWalking_time();
}

void Pedestrians::export_data(){
    json pedestriansJson;
    std::ofstream forTestCSV1("data/forTest.csv");
    forTestCSV1 << "STT,ID,Role,Age,Ward(start),Des 1,Des 2,Des 3,impactOfAGV(pleasure),impactOfAGV(surprise),impactOfAGV(anger),impactOfAGV(fear),impactOfAGV(hate),impactOfAGV(sad),Personality,Velocity,Events,Events Time,Path Points,Walking time,Pleasure on time,Surprise on time,Anger on time,Fear on time,Hate on time,Sad on time\n";
    int STT=1;
    for(auto& pedes:pedestrians){
        json pedestrianJson;
        json personalityJson;
        json emotionJson;
        json eventsJson;
        json journeyJson;
        json impactJson;

        for ( auto &ev : pedes.getEvents())
        {
            json eventJson;
            eventJson["intensity"] = ev.getIntensity();
            eventJson["time"] = ev.getTime();
            eventsJson.push_back(eventJson);
        }

      

        for (Ward x : pedes.getJourney())
        {
            journeyJson.push_back({"name", x.getName()});
        }

        AGVEvent x=pedes.getImpactOfAGV();
        std::vector<double>impact=x.getIntensity();
        for(auto& k:x.getIntensity()){
            impactJson.push_back(k);
        }
        
        pedestrianJson["ID"] = pedes.getID();
        pedestrianJson["velocity"] = pedes.getVelocity();
        pedestrianJson["age"] = pedes.getAge();
        personalityJson["lambda"] = pedes.getPersonality().getLambda();
        personalityJson["positiveEmotionThreshold"] = pedes.getPersonality().getPositive();
        personalityJson["negativeEmotionThreshold"] = pedes.getPersonality().getNegative();
        pedestrianJson["personality"] = personalityJson;
        emotionJson["pleasure"] = pedes.getEmotion().getPleasure();
        emotionJson["surprise"] = pedes.getEmotion().getSurprise();
        emotionJson["anger"] = pedes.getEmotion().getAnger();
        emotionJson["fear"] = pedes.getEmotion().getFear();
        emotionJson["hate"] = pedes.getEmotion().getHate();
        emotionJson["sad"] = pedes.getEmotion().getSad();
        pedestrianJson["emotion"] = emotionJson;
        pedestrianJson["events"] = eventsJson;
        pedestrianJson["start"] = pedes.getStart().getName();
        pedestrianJson["end"] = pedes.getEnd().getName();
        pedestrianJson["journey"] = journeyJson;
        pedestrianJson["walkingTime"] = pedes.getWalkingtime();
        pedestrianJson["impactOfAGV"] = impactJson;
        //STT
        forTestCSV1<<STT<<",";
        STT++;
        //ID ,role,age,start
        forTestCSV1 <<pedestrianJson["ID"] << "," << pedes.getRole() << "," << pedestrianJson["age"] << "," << pedestrianJson["start"] << ",";
        int count = 0;
        //des 1,2,3
        for (auto& x : pedes.getJourney())
        {
            forTestCSV1 << x.getName() << ",";
            count++;
        }
        for (int i = 0; i <= 2 - count; i++)
        {
            forTestCSV1 << "N/A,";
        }
        //impactofagv
        forTestCSV1 << impact[0] << "," << impact[1] << "," << impact[2] << "," << impact[3] << "," << impact[4] << "," << impact[5] << ",";
        //personality
        if(pedes.getPersonality().getLambda()==4){
        forTestCSV1 <<"Neurotic"<< ",";} 
        else{
        forTestCSV1<< "Open"<<",";}
        
        //velocity
        forTestCSV1<<pedes.getVelocity()<< ",";
        //event
        for ( auto &ev : pedes.getEvents())
        {
            for (auto intens : ev.getIntensity())
            {
                forTestCSV1 << intens << " ";
            }
        }
        //time between event
        forTestCSV1 << ", ";
        for ( auto &ev : pedes.getEvents())
        {
            forTestCSV1 << ev.getTime()<<" ";
        }
        

        forTestCSV1 << ",";
        // path point            
        for(float r:pedes.getRoute()){
            forTestCSV1<<r<<" ";
        }
        forTestCSV1<<", ";
        //Walking time
        forTestCSV1<<pedes.getWalkingtime()<<",";
        int H=pedes.getEvents().size();
        std::vector<std::vector<double>>  emo=pedes.calculateEmotions(H);
    //emotion on time:
    for(int h=0;h<6;h++){
        for(int k=0;k<H;k++){
        forTestCSV1 << emo[h][k] << " ";}
        forTestCSV1<<",";
        }
    forTestCSV1<<"\n";  
    pedestriansJson.push_back(pedestrianJson);
    }

    std::ofstream outputJson("data/Pedestrian.txt");
    std::ofstream outputforTest("data/forTest.json");
    outputJson << std::setw(4) << pedestriansJson << std::endl;
    outputforTest << std::setw(4) << pedestriansJson << std::endl;
    outputJson.close();
    outputforTest.close();
   
}

