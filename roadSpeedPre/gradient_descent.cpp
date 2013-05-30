#include "gradient_descent.h"

static int features_num ;
static int train_num;
static int train_times;


//准备定死 ,每次重复计算 mean 和 squre 很烦人啊
const double mean    = 46.8272044935;
const double square  =13.8657378774;
void normalize(double *arr , int len )
{
    for(int i = 1 ; i < len ; i ++){
        arr[i] = (arr[i] - mean) / (1.5*square) ;
    }
}

void gaussian2(CMat * features_vec)
{
    int n = features_vec->row;
    //归一化
    for(int i =  0 ; i < n ; i ++){

        features_vec->data [i*features_num ] = 1;
        for(int j = 1 ; j < features_num ; j ++){
            features_vec->data [i*features_num +j] = ( features_vec->data [i*features_num +j] - mean)/ (1.5 *square );
        }
    }
}

void gaussian(CMat * features_vec)
{
    int n = features_vec->row;
    vector<double > u_vec;
    vector<double > q_vec;
    u_vec.resize(features_num );
    q_vec.resize(features_num);
    for(int i =  0 ; i < n ; i ++){

        for(int j = 0 ; j < features_num ; j ++){
            u_vec[j]  += features_vec->data [i*features_num +j];
        }
    }

    //计算均值
    for(int j = 0 ; j < features_num ; j ++){
        u_vec[j] /= n;
    }


    // 计算方差
    for(int j = 0 ; j < features_num ; j ++){
        for(int i =  0 ; i < n ; i ++){
            q_vec[j]  += ( features_vec->data [i*features_num +j] - u_vec[j]) * (features_vec->data [i*features_num +j] - u_vec[j]);
        }
         q_vec[j] /= n;
         q_vec[j]  = sqrt(q_vec[j]);
    }



    //归一化
    for(int i =  0 ; i < n ; i ++){

        features_vec->data [i*features_num ] = 1;
        for(int j = 1 ; j < features_num ; j ++){
            features_vec->data [i*features_num +j] = ( features_vec->data [i*features_num +j] - u_vec[j])/ (1.5 *q_vec[j] );
        }
    }


}

void gradient_desecent(CMat *  train_feature , vector<double> & train_labels , vector <double >  &weights_vec  )
{

    weights_vec.resize(features_num);
    for(int i = 0 ; i < features_num ; i ++){
        weights_vec[i] = 1;
    }
    //double theta0 = 0.000000002 ; //学习速度

    double  theta0 = 0.0001;

    std::vector<double > delta(features_num,0.0);

    for(int i = 0 ; i < train_times ; i ++) {

        for(int i = 0 ; i < features_num ; i ++){
            delta[i] = 0;
        }

        for(int j = 0 ; j < train_num ; j ++){
            double o  = 0;
            for(int k = 0 ; k < features_num ; k ++){
                o += weights_vec[k]*(train_feature->data[j*features_num +k]);
            }

            //delta[0] = theta0*(train_labels[j] - o);
            for(int k = 0 ; k < features_num ; k ++){
                delta[k]  += theta0 *(train_labels[j] - o) *(train_feature->data[j*features_num +k]);
            }
        }

        for(int k = 0 ; k < features_num ; k ++){
            weights_vec[k]  += delta[k]  ;
        }

        //
    }


//    for(int k = 0 ; k < features_num ; k ++){
//        cout << weights_vec[k] <<" " ;
//    }
//    cout << "\n";
}
/*
在 train_data  会加一个1
*/
vector <double >  gd_train( vector <vector <double > > & train_data ,vector <double > &labels)
{
    train_num = train_data.size();
    features_num = train_data[0].size() + 1;

    train_times = 1000;

    vector <double > weights_vec;
    CMat * train_feature = CMatCreate(train_num ,features_num);

    for(int i = 0 ; i < train_num ; ++i){

        train_feature->data[i*features_num ] = 1;
        for(int j = 1 ; j < features_num ; ++j){

            train_feature->data[i*features_num + j]  = train_data[i][j-1];
        }
    }

    //gaussian(train_feature);
    gaussian2(train_feature);

    gradient_desecent(train_feature ,labels ,weights_vec);



    return  weights_vec;

}

