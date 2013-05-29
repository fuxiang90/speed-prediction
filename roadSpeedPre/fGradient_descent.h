#ifndef FGRADIENT_DESCENT_H_INCLUDED
#define FGRADIENT_DESCENT_H_INCLUDED



#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "CMat.h"
using namespace std;





void  gradientMain();
//试着将 fGradientDescent 这里的类抽象 ，重新实现 Train 和 Pridict 函数
class fGradientDescentBus
{
public:
    fGradientDescentBus(int _features){
        features_num = _features;
    }

    ~fGradientDescentBus();
    void Train();
    void Predict();
    void Set_train_file_name(string & file_string);
    void Set_test_file_name(string & file_string);
    void SetTrainTimes(int _t){
        train_times = _t;
    }
    /*返回 对下一时刻的预测*/
    double predictNow(double speed_arr[] ,int weekday ,int roadid ,int time_now);
private:
    string train_file_name;
    string test_file_name;

    int features_num;
    int train_num ;
    int test_num;
    int train_times;
    double weigth0;
    //std::vector<sample_type> train_feature;

    CMat *train_feature;
    std::vector<double> train_labels;

    //std::vector<sample_type> test_feature;

    CMat *test_feature;
    std::vector<double> test_labels;

    std::vector<double> weights_vec;
    std::vector<double > u_vec ;
    std::vector<double> q_vec ;
    void gradientDesecent();

    void gaussian( CMat * features_vec);



};

void  gradientBusMain();

#endif // FGRADIENT-DESCENT_H_INCLUDED
