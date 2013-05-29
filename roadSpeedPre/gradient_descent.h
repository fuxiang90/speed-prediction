/*
之前写的梯度下降的类 不太适合

重写一个接口 传入 一些数据 直接返回 权重

*/
#ifndef GRADIENT_DESCENT_H_INCLUDED
#define GRADIENT_DESCENT_H_INCLUDED

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "CMat.h"
#include "config.h"






using namespace std;




void gaussian(CMat * features_vec) ;

vector <double >  gd_train( vector <vector <double > > & train_data ,vector <double > &labels);

void gradient_desecent(CMat *  train_feature , vector<double> & train_labels , vector <double >  &weights_vec  );

#endif // GRADIENT_DESCENT_H_INCLUDED
