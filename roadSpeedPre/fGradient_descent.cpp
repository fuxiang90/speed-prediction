# include "fGradient_descent.h"



static void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

void fGradientDescentBus::gradientDesecent()
{

    weights_vec.resize(features_num);
    for(int i = 0 ; i < features_num ; i ++){
        weights_vec[i] = 1;
    }
    double theta0 = 0.0002 ; //学习速度

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


    for(int k = 0 ; k < features_num ; k ++){
        cout << weights_vec[k] <<" " ;
    }
    cout << "\n";
}


void fGradientDescentBus::gaussian(CMat * features_vec)
{
    int n = features_vec->row;
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

//////////////////////////////////////////////////////////////////////////////////
void fGradientDescentBus::Train()
{

    if(train_file_name == ""){
        train_file_name = "traindata";
    }
    ifstream fin(train_file_name.c_str());

    int n ;
    fin>>n;
    train_num = n;

    train_feature = CMatCreate(train_num ,features_num);
    for(int i = 0 ; i < n ; ++i){
        double label;


        int no_a ;
        int no_b;
        fin>>no_a >>no_b;


        double num;

        //sample_type temp;
        //temp(0)  = 1;
        train_feature->data[i*features_num ] = 1;
        for(int j = 1 ; j < features_num ; ++j){
            fin>>num;
            train_feature->data[i*features_num + j]  = num;
        }
        fin>>label;
        train_labels.push_back(label);
        //train_feature.push_back(temp);
    }
    fin.close();

    gaussian(train_feature);
    // 梯度下降算法
    // f(x) = w0 * x0 +w1 * x1 + w2*x2 + w3*x+w4*x4;x0  = 1
    gradientDesecent();
    weigth0 = 0.0;

    for(int i = 0 ; i < train_num ; i ++){
        double o  = 0;
        for(int k = 0 ; k < features_num ; k ++){
            o += weights_vec[k]*  ( train_feature->data[i * features_num + k] );
        }
        cout << o << "real is" << train_labels[i] <<endl;

        weigth0 = (train_labels[i] - o);
    }

    cout <<weigth0/train_num << endl;
    cout << "-----------------------------------------\n" <<endl;

}

void fGradientDescentBus::Predict()
{
    if(test_file_name == ""){
        test_file_name = "testdata";
    }
    ifstream fin(test_file_name.c_str());
    int n ;
    fin>>n;
    test_num = n;

    test_feature = CMatCreate(test_num ,features_num);
    for(int i = 0 ; i < n ; ++i){
        double label;


        int no_a ;
        int no_b;
        fin>>no_a >>no_b;

        double num;
        //sample_type temp;
        //temp(0)  = 1;
        test_feature->data [i*features_num ] = 1;
        for(int j = 1 ; j < features_num ; ++j){
            fin>>num;
            test_feature->data [i*features_num + j] = num;
        }
        fin>>label;
        test_labels.push_back(label);

        //test_feature.push_back(temp);
    }
    fin.close();

      //归一化
    for(int i =  0 ; i < test_num ; i ++){
        test_feature->data [i*features_num ]  = 1;
        for(int j = 1 ; j < features_num ; j ++){
           test_feature->data [i*features_num + j] = (test_feature->data [i*features_num + j] - u_vec[j])/ (1.5 *q_vec[j] );
        }
    }

    //增加一个输出结果
    string result_file_name(__DATE__);
    result_file_name   += __TIME__;
    replaceAll(result_file_name," ", "");
    result_file_name = "./log/" + result_file_name;
    ofstream result_out(result_file_name.c_str());
    int right_num = 0;
    double h = 0.0;
    int low_one_num = 0;
    for(int i = 0 ; i < test_num ; i ++){
        double o  = 0;
        for(int k = 0 ; k < features_num ; k ++){
            o += weights_vec[k]*test_feature->data [i*features_num + k];
        }
        cout.setf(ios::fixed);
        cout << setprecision(8)<< o << " real is " <<setprecision(8) <<test_labels[i] <<endl;

        result_out.setf(ios::fixed);
        result_out << setprecision(8)<< o << " real is " <<setprecision(8) <<test_labels[i] <<endl;
        double t =  fabs(o - test_labels[i]) ;
        if(t  < 3)low_one_num ++;
        h += t ;
        if(fabs(o - test_labels[i]) < 4){
            right_num ++;
        }
    }
    cout << right_num*1.0 / test_num << endl;
    cout << "average delt is s:" << h/test_num << endl;
    cout << low_one_num << " " << test_num<< endl;


    result_out << right_num*1.0 / test_num << endl;
    result_out << "average delt is s:" << h/test_num << endl;
    result_out << low_one_num << " " << test_num<< endl;



    result_out.close();

}

double fGradientDescentBus::predictNow(double speed_arr[] ,int weekday ,int roadid ,int time_now)
{
    //size_t len = sizeof(speed_arr);

    //if(len !=  (size_t)features_num){
     //   return -0.1;
    //}
    for(int k = 1 ; k < features_num ; k ++){
        speed_arr[k] = (speed_arr[k] - u_vec[k])/ (1.5 *q_vec[k] );

    }

    double result = 0;
    for(int k = 0 ; k < features_num ; k ++){
            result += weights_vec[k]*speed_arr[k];
    }

    return result;
}
void fGradientDescentBus::Set_train_file_name(string & file_name)
{
    train_file_name = file_name;
}
void fGradientDescentBus::Set_test_file_name(string & file_name)
{
    test_file_name = file_name;
}


fGradientDescentBus::~fGradientDescentBus()
{
    CMatRelease(train_feature);
    CMatRelease(test_feature);

}


void  gradientBusMain()
{
    string train_file("./data/train/386-15-train");
    string test_file("./data/test/386-15-test");
    fGradientDescentBus test(5) ;
    test.SetTrainTimes(1000);

    test.Set_train_file_name(train_file);
    test.Set_test_file_name(test_file);
    test.Train();
    test.Predict();
}

