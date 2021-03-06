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
void normalize(double *arr , int len );

vector <double >  gd_train( vector <vector <double > > & train_data ,vector <double > &labels);

//准备用c实现
//double * gd_train_c( double ** train_data ,double * labels);
double * gd_train_c( double ** train_data ,double * labels ,
                    int m_train_num ,
                     int m_features_num);

void gradient_desecent(CMat *  train_feature ,
                        vector<double> & train_labels ,
                        vector <double >  &weights_vec  );
void gradient_desecent_c(CMat *  train_feature ,
                        double * train_labels ,
                        double * weights_vec  );

#endif // GRADIENT_DESCENT_H_INCLUDED
