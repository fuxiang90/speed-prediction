#include "prediction.h"

extern int *locid_hash;

void predict_init()
{

}

RoadInfo * predict_roadinfo_create()
{

    RoadInfo * road_info_arr = (RoadInfo * )malloc(sizeof(RoadInfo) * (MAX_LOC_ROAD +1));

    if(road_info_arr == NULL) {
        fprintf(stdout ,"predict_roadinfo_create  memory error\n");
        return NULL;
    }

    for(int i = 0  ; i <= MAX_LOC_ROAD ; i ++) {
        RoadInfo * road_info_p = &(road_info_arr[i]);
        road_info_p->mtime.year = 2012;
        road_info_p->mtime.mouth = 11;
        road_info_p->mtime.day = 21;
        //road_info_p->pre = NULL;
        //road_info_p->next = NULL;

        road_info_p->road_list = CSlistCreate();
    }


    return road_info_arr;

}
void  predict_roadinfo_release(RoadInfo *road_info_arr)
{
    free(road_info_arr);
    road_info_arr = NULL;

}
void get_road_info(RoadInfo * road_info_arr)
{

    //get file name 下一次应该写个函数得到某个目录下指定后缀的目录名

    // 将nav_road_arr 和 road_info_arr 下标对应起来
//    char filename[][20] = { "376_0.txt","39_0.txt","40_0.txt","386_0.txt","2252_0.txt"};
//    //
//    int len = (sizeof(filename))/(sizeof(filename[0]));
//    char *file_name =  (char *)malloc(sizeof(char) * MAX_FILE_NAME_LEN);
//    for(int i = 0 ; i < len -2 ; i ++ ) {
//        int locid  = get_locid_str(filename[i]);
//        int loc_pos = locid_hash[locid] ;
//
//        sprintf(file_name ,"%s%s",txt_root_path ,filename[i]);
//        get_road_info_filename( &road_info_arr[loc_pos] ,file_name );
//        road_info_arr[loc_pos].flag = 1;
//    }
//
//    free(file_name);//bug fix add  2013年6月5日


}

/*这里传入的head 必须实现分配好内存*/
int get_road_info_filename(RoadInfo * head , char *filename  ,  LocRoad * loc_road_arr ,int loc_pos)
{
    char in_str[MAX_STRLEN];
    struct date_t  now_time;//valgrind 提示的bug  需要初始化
    //
    now_time.year = 2000;
    now_time.mouth = 1;
    now_time.day = 1;


    FILE * file_p = fopen(filename ,"r");
    if(file_p == NULL) {
        //fprintf(stdout,"%s open error\n",filename);
        return 0;
    }
    //读取文件内容
    while(fgets(in_str,MAX_STRLEN ,file_p)) {

        if(strlen(in_str) <  DATA_LEN-5) continue;


        getInfoStr(in_str ,head,&now_time,&loc_road_arr[loc_pos]);

    }
    fclose(file_p);// bug fix 2013年5月31日

    return 1;

}
/*
现在是采取 把所有的速度信息读进内存 ，这样到最后肯定会遇到内存瓶颈
1 遍历一次可以得到 所有的历史数据
2 遍历第二次 可以得到 训练的 train 数据  ，这次下次要改进的地方  2013-5-30
*/
void get_road_info_history(RoadInfo  *road_info_arr,LocRoad * loc_road_arr)
{
//
//    for(int i =1 ; i <= MAX_LOC_ROAD ; i++ ) {
//        if(road_info_arr[i].flag == 0) {
//            continue;
//        }
//        int locid = road_info_arr[i].road_id;
//        int loc_pos = locid_hash[locid];
//
//        assert(loc_pos == i );
//        if(loc_pos != i ) {
//            printf("locid_hash error \n");
//        }
//
//        int pre_locid =loc_road_arr[loc_pos].pre_locid;
//        //int pre_locid =loc_road_arr[locid].pre_locid; // bug fix 2013年5月31日
//
//        int pre_locid_pos = locid_hash[pre_locid];
//        if(pre_locid == 0 || road_info_arr[pre_locid_pos].flag == 0  ) {
//            continue;
//        }
//
//        //debug
//        printf("%d\n",locid );
//
//        get_road_info_history_node(&road_info_arr[i] , &road_info_arr[pre_locid_pos] ,loc_road_arr );
//
//
//    }
}

static void get_info_str(char * in_str ,double * speed ,int * weekday, int * hh  ,int * mm)
{

    char *token = strtok(in_str," ");

    //将 88_0 转成 880
//    char loc_str[10];
//    int j = 0;
//    for(int i = 0 ; i < (int)strlen(token) ; i ++) {
//        if( isdigit( token[i])  ) {
//            loc_str[j++] = token[i];
//        }
//    }
//    loc_str[j] = '\0';
//
//
//    int road_id = atoi(loc_str);
//    road_info->road_id = road_id;

    token = strtok(NULL," ");
    (*speed) = strtod(token,NULL);


    token = strtok(NULL," ");
    int y,m,d;
    sscanf(token,"%d-%d-%d",&y,&m,&d);

//    (*now_time).year

    token = strtok(NULL," ");
    int h,mi,s;
    //有当到达零点的时候 没有时间这个数据
    if(token == NULL || strlen(token) == 0) {
        h = 0;
        mi = 0 ;//m = 0;  // bug fix 2013年5月31日
        s = 0;
    } else {
        sscanf(token,"%d:%d:%d",&h,&mi,&s);
    }


    (*hh) = h;
    (*mm) = m;

    (*weekday) = ZellerFun(y,m,d) ;




}
void get_road_info_history_node(RoadInfo  *road_info ,RoadInfo * pre_road_info ,LocRoad * loc_road_arr)
{
//    RoadInfo * pre_road = pre_road_info;
//
//    CSlistNode * pre_l_head = pre_road->road_list->head;
//    CSlistNode * now_l_head = road_info->road_list->head;
//
//
//    double now_thisroad_speed           = 0.0;
//    double now_preroad_speed            = 0.0;//这个可能不要
//    double now_preroad_histroy_speed    = 0.0;
//    double next_preroad_histroy_speed   = 0.0;
//    double next_thisroad_histroy_speed  = 0.0;
//
//    double next_thisroad_speed          = 0.0;
//    int roadid = road_info->road_id;
//
//
//    vector < vector <double > > train_data[7];
//    vector<double > data ;
//    vector<double > label[7];
//    data.resize(4);
//    while(now_l_head ) {
//
//        int weekday = ((struct road_t*) (now_l_head->value))->weekday;
//
//
//        //遍历每一个时刻的数据
//        for(int i = 0 ; i < TIMES_DAY ; i ++) {
//            now_thisroad_speed =  ((struct road_t*) (now_l_head->value))->speed_arr[i];
//            now_preroad_speed = ((struct road_t*) (pre_l_head->value))->speed_arr[i];
//
//
//
//            if(i+1 != TIMES_DAY) {
//                next_thisroad_histroy_speed =  road_info->history_road[weekday][i+1];
//                next_thisroad_speed = ((struct road_t*) (now_l_head->value))->speed_arr[i+1];
//            } else {
//                next_thisroad_histroy_speed = road_info->history_road[weekday][i]; //这个时候 就是接近午夜十二点的时刻
//                next_thisroad_speed = ((struct road_t*) (now_l_head->value))->speed_arr[i];
//            }
//
//
//            now_preroad_histroy_speed =  pre_road->history_road[weekday][i];
//
//            if(i+1 != TIMES_DAY)
//
//                next_preroad_histroy_speed =  pre_road->history_road[weekday][i+1];
//            else
//                next_preroad_histroy_speed = pre_road->history_road[weekday][i]; //这个时候 就是接近午夜十二点的时刻
//
//
////            printf("%d %d %lf %lf %lf %lf %lf\n",
////                    roadid , weekday ,now_thisroad_speed ,now_preroad_histroy_speed ,next_preroad_histroy_speed ,
////                    next_thisroad_histroy_speed,next_thisroad_speed);
//
//            data[0] = now_thisroad_speed ;
//            data[1] = now_preroad_histroy_speed ;
//            data[2] = next_preroad_histroy_speed;
//            data[3] = next_thisroad_histroy_speed;
//
//            train_data[weekday].push_back(data);
//            label[weekday].push_back(next_thisroad_speed);
//
//
//        }
//        now_l_head = now_l_head->next;
//    }//end while
//
//    //store data
//    char * train_file_name = (char *)malloc(MAX_FILE_NAME_LEN);
//    sprintf(train_file_name,"./data/train/%d_data",roadid);
//    FILE * fout = fopen(train_file_name,"w");
//    for(int i = 0 ; i <= 6 ; i ++) {
//        double w ;
//
//
//        for(int k = 0 ; k < train_data[i].size() ; k ++) {
//            for(int j = 0 ; j < 4 ; j++) {
//                w = (train_data[i][k][j]);
//                fprintf(fout,"%lf " ,w);
//            }
//            if(k +1 == train_data[i].size() ) {
//                w = train_data[i][k][0];
//                fprintf(fout ,"%lf", w);
//            } else {
//                w = train_data[i][k+1][0];
//                fprintf(fout ,"%lf", w);
//            }
//            fprintf(fout ,"\n");
//        }
//    }
//    fclose(fout);
//    /////
//
//
//    //最后训练的权值 要保存在 loc 里面
//    for(int i = 0 ; i <= 6 ; i ++) {
//        vector<double > w = gd_train(train_data[i] ,label[i]);
//
//        int locid = road_info->road_id ;
//        int loc_pos = locid_hash[locid];
//
//        for(int j = 0 ; j < w.size() ; j++)
//            loc_road_arr[loc_pos].weights_arr[i][j] = w[j];
//
//    }


}

/*
当某一条路处理之后 那么 得到他的历史数据
*/
void road_info_process_one(RoadInfo * head ,LocRoad * loc_road)
{
//    for(int i = 0 ; i < 7 ; i ++) {
//        for(int j = 0 ; j < TIMES_DAY ; j ++) {
//
//            if(head->road_times_arr[i][j] == 0) {
//                head->history_road[i][j] = 30.0;
//            } else {
//                head->history_road[i][j]/=head->road_times_arr[i][j];
//            }
//
//        }
//    }//end for
//    ///////////////////////////


    //因为也有一个 step（大于两分钟） ，所以要做一个整理
    CSlistNode * l_head = head->road_list->head;
    //遍历某条路 每一天的数据
    while(l_head ) {

        //遍历每一个时刻的数据
        for(int i = 0 ; i < TIMES_DAY ; i ++) {
            // debug
            if( ((struct road_t*) (l_head->value))->speed_arr[i]  < 0.2) {
                ((struct road_t*) (l_head->value))->speed_arr[i]  =  loc_road->history_road[((struct road_t*) (l_head->value))->weekday ][i];
            } else
                ((struct road_t*) (l_head->value))->speed_arr[i] /= ((struct road_t*) (l_head->value))->times[i] ;



        }
        l_head = l_head->next;
    }//end while
}

void loc_road_info_process_one( LocRoad * loc_road)
{
    for(int i = 0 ; i < 7 ; i ++) {
        for(int j = 0 ; j < TIMES_DAY ; j ++) {

            if(loc_road->road_times_arr[i][j] == 0) {
                loc_road->history_road[i][j] = 30.0;
            } else {
                loc_road->history_road[i][j]/=loc_road->road_times_arr[i][j];
            }

        }
    }//end for

    //loc_road_arr_node_free_road_times(loc_road);

}
void road_info_process(RoadInfo  *road_info_arr )
{

//    for(int i = 0 ; i <= MAX_LOC_ROAD ; i ++) {
//
//        if(road_info_arr[i].flag == 0) {
//            continue;
//        }
//        RoadInfo * head = &(road_info_arr[i]);
//
//        road_info_process_one(head);
//    }


}

void copy_history_roadinfo_locroad(RoadInfo  *road_info_arr , LocRoad * loc_road_arr )
{
//    for(int i = 1 ; i <= MAX_LOC_ROAD ; i ++) {
//        if(road_info_arr[i].flag == 0) {
//            continue;
//        }
//
////        for(int j = 0 ; j < 7 ; j ++){
////            for(int k = 0 ; k < TIMES_DAY ; k ++){
////                loc_road_arr[i].history_road[j][k] = road_info_arr[i].history_road[j][k];
////            }
////        }
//
//        //
//        memcpy(&(loc_road_arr[i].history_road)  , &(road_info_arr[i].history_road) , sizeof(double)*7*TIMES_DAY );
//
//    }

}

int prediction_train_loc_road(double * * speed_arr  ,
                              LocRoad * loc_road_arr ,int weekday ,int h ,int m ,double speed ,int locid ,int next_time )
{

    int next_step = next_time/STEP;
    if(next_step < 1) next_step = 1;

    int now_step = (h*60 + m)/STEP;
    (*speed_arr) = (double * )malloc(sizeof(double ) * next_step);

    int loc_pos = locid_hash[locid];


    int pre_locid = loc_road_arr[loc_pos].pre_locid;
    int pre_loc_pos  = locid_hash[pre_locid];
    //double (*weight)[5] = (double * [5]) &(loc_road_arr[loc_pos].weights_arr[weekday] );
    //2013年6月8日
    double (*weight) =  (loc_road_arr[loc_pos].weights_arr[weekday] );

    double speeds[5] ;
    double pre_speed;
    for(int j = 0 ; j < next_step ; j ++) {

        if(now_step + j == TIMES_DAY  ) {
            now_step -= TIMES_DAY;
            weekday = (weekday+1)%7;
        }
        speeds[0]    =1 ;
        speeds[1]    = speed;
        speeds[2]    = loc_road_arr[pre_loc_pos].history_road[weekday][now_step + j];
        speeds[3]    = loc_road_arr[pre_loc_pos].history_road[weekday][(now_step+ j + 1)%TIMES_DAY];
        speeds[4]    = loc_road_arr[loc_pos].history_road[weekday][(now_step+ j + 1)%TIMES_DAY];
        pre_speed = 0.0;
        normalize(speeds ,5);
        for(int i = 0 ; i < 5 ; i ++) {

            //pre_speed += (*weight)[i] * speeds[i]; //2013年6月8日
            pre_speed += (weight)[i] * speeds[i];
        }


        (*speed_arr)[j] = pre_speed;


    }

    return next_step;

}

double  prediction_train_loc_road_next(LocRoad * loc_road_arr ,
                                       int weekday ,int h ,int m ,double speed ,int locid ,int pre_time )
{

    double pre_speed = 0;

    int loc_pos = locid_hash[locid];
    int pre_locid = loc_road_arr[loc_pos].pre_locid;
    int pre_loc_pos  = locid_hash[pre_locid];
    //double (*weight)[5] =  &(loc_road_arr[loc_pos].weights_arr[weekday] );//2013年6月8日
    double (*weight) =  (loc_road_arr[loc_pos].weights_arr[weekday] );

    int t = (h*24 + m)/STEP;
    double speed_arr[5] ;
    speed_arr[0]    =1 ;
    speed_arr[1]    = speed;
    speed_arr[2]    = loc_road_arr[pre_loc_pos].history_road[weekday][t];
    speed_arr[3]    = loc_road_arr[pre_loc_pos].history_road[weekday][t+1];
    speed_arr[4]    = loc_road_arr[loc_pos].history_road[weekday][t+1];

    normalize(speed_arr ,5);
    for(int i = 0 ; i < 5 ; i ++) {

//        pre_speed += (*weight)[i] * speed_arr[i];//2013年6月8日
        pre_speed += (weight)[i] * speed_arr[i];
    }

    //printf("%lf\n",pre_speed);

    return pre_speed;

}
//test 检查 梯度下降的准确性
void check_train_loc_road(LocRoad * loc_road_arr )
{
    FILE * fin = fopen("./data/txt/376_0.txt","r");

    char in_str[MAX_STRLEN];
    struct date_t  now_time;
    if(fin == NULL) {
        printf("./data/txt/39_0.txt open error\n");
        return ;
    }
    //读取文件内容
    double speed = 0.0;
    int weekday;
    int h;
    int m;
    double pre_speed = 0;

    int n = 0;
    int right_num =0;
    while(fgets(in_str,MAX_STRLEN ,fin)) {

        if(strlen(in_str) <  DATA_LEN-5) continue;

        printf("%lf %lf\n" ,speed ,pre_speed);

        if(fabs (speed - pre_speed) <  5.0 ) {
            right_num ++;
        }
        get_info_str(in_str,&speed ,&weekday ,&h,&m);

        pre_speed = prediction_train_loc_road_next(loc_road_arr ,weekday ,h,m,speed,3760,1);


        n ++;
    }

    printf("%lf\n",right_num *1.0 / n);


}
/*
1  得到道路信息
2  训练
3  储存模型
4  释放道路

*/
void schedule_train_loc_road(int * schedule_arr  ,int n, LocRoad * loc_road_arr ,RoadInfo  *road_info_arr )
{
    char filename[MAX_FILE_NAME_LEN];

    char *file_name =  (char *)malloc(sizeof(char) * MAX_FILE_NAME_LEN );
    assert(file_name );
    for(int i = 0 ; i < n ; i ++) {

        int locid = schedule_arr[i];
        int loc_pos = locid_hash[locid];

        get_filename(locid,filename);

        //sprintf(file_name ,"%s%s",txt_root_path ,filename);
        snprintf(file_name ,MAX_FILE_NAME_LEN ,"%s%s",txt_root_path ,filename);

        //
        if ( 0 != get_road_info_filename( &road_info_arr[loc_pos] ,file_name , loc_road_arr ,loc_pos) ) {

            loc_road_info_process_one(&loc_road_arr[loc_pos]);
            road_info_process_one(&road_info_arr[loc_pos] ,&loc_road_arr[loc_pos]);
            //loc_road_info_process_one(&loc_road_arr[loc_pos]);
            road_info_arr[loc_pos].flag = 1; //表示历史数据已经生成

            loc_road_arr[loc_pos].flag = 1;
        } else {
            continue;
        }


        int pre_locid = loc_road_arr[loc_pos].pre_locid;
        int pre_loc_pos = locid_hash[pre_locid];
        if(road_info_arr[pre_loc_pos].flag == 1 ) {
            train_loc_road_c(locid ,pre_locid ,road_info_arr,loc_road_arr );
//            train_loc_road(locid ,pre_locid ,road_info_arr,loc_road_arr );
        }

        loc_road_arr_node_free_road_times(&loc_road_arr[loc_pos]);
        RoadInfoClistRealease(&road_info_arr[loc_pos]);


    }

    free(file_name);

}

void train_loc_road(int locid ,int pre_locid , RoadInfo  *road_info_arr,LocRoad * loc_road_arr)
{
    //这个是暂时没有bug
    int locpos = locid_hash[locid];
    int pre_locpos = locid_hash[pre_locid];
    //add 2013年6月10日
    if(road_info_arr[pre_locpos].flag != 1 ){
        fprintf(stdout,"train_loc_road : pre_locpos no history\n");
        return ;
    }

    RoadInfo * road_info = &(road_info_arr[locpos]);
    LocRoad  * pre_road = &(loc_road_arr[pre_locpos]);



    CSlistNode * now_l_head = road_info->road_list->head;


    double now_thisroad_speed           = 0.0;
    double now_preroad_histroy_speed    = 0.0;
    double next_thisroad_histroy_speed  = 0.0;
    double next_preroad_histroy_speed   = 0.0;


    double next_thisroad_speed          = 0.0;
    int roadid = road_info->road_id;


    vector < vector <double > > train_data[7];
    vector<double > data ;
    vector<double > label[7];
    data.resize(4);
    while(now_l_head ) {

        int weekday = ((struct road_t*) (now_l_head->value))->weekday;


        //遍历每一个时刻的数据
        for(int i = 0 ; i < TIMES_DAY ; i ++) {
            now_thisroad_speed =  ((struct road_t*) (now_l_head->value))->speed_arr[i];




            if(i+1 != TIMES_DAY) {
                next_thisroad_histroy_speed =  loc_road_arr[locpos].history_road[weekday][i+1];
                next_thisroad_speed = ((struct road_t*) (now_l_head->value))->speed_arr[i+1];
            } else {
                //next_thisroad_histroy_speed = road_info->history_road[weekday][i]; //这个时候 就是接近午夜十二点的时刻
                next_thisroad_histroy_speed = loc_road_arr[locpos].history_road[weekday][i]; //这个时候 就是接近午夜十二点的时刻
                next_thisroad_speed = ((struct road_t*) (now_l_head->value))->speed_arr[i];
            }


            now_preroad_histroy_speed =  pre_road->history_road[weekday][i];

            if(i+1 != TIMES_DAY)

                next_preroad_histroy_speed =  pre_road->history_road[weekday][i+1];
            else
                next_preroad_histroy_speed = pre_road->history_road[weekday][i]; //这个时候 就是接近午夜十二点的时刻


//            printf("%d %d %lf %lf %lf %lf %lf\n",
//                    roadid , weekday ,now_thisroad_speed ,now_preroad_histroy_speed ,next_preroad_histroy_speed ,
//                    next_thisroad_histroy_speed,next_thisroad_speed);

            data[0] = now_thisroad_speed ;
            data[1] = now_preroad_histroy_speed ;
            data[2] = next_preroad_histroy_speed;
            data[3] = next_thisroad_histroy_speed;

            train_data[weekday].push_back(data);
            label[weekday].push_back(next_thisroad_speed);


        }
        now_l_head = now_l_head->next;
    }//end while

    //store data
    char * train_file_name = (char *)malloc(20);
    sprintf(train_file_name,"./data/%d_data",roadid);
    FILE * fout = fopen(train_file_name,"w");
    for(int i = 0 ; i <= 6 ; i ++) {
        double w ;


        for(int k = 0 ; k < train_data[i].size() ; k ++) {
            for(int j = 0 ; j < 4 ; j++) {
                w = (train_data[i][k][j]);
                fprintf(fout,"%lf " ,w);
            }
            if(k +1 == train_data[i].size() ) {
                w = train_data[i][k][0];
                fprintf(fout ,"%lf", w);
            } else {
                w = train_data[i][k+1][0];
                fprintf(fout ,"%lf", w);
            }
            fprintf(fout ,"\n");
        }
    }
    fclose(fout);
    /////


    //最后训练的权值 要保存在 loc 里面
    for(int i = 0 ; i <= 6 ; i ++) {
        vector<double > w = gd_train(train_data[i] ,label[i]);

        int locid = road_info->road_id ;
        int loc_pos = locid_hash[locid];

        for(int j = 0 ; j < w.size() ; j++)
            loc_road_arr[loc_pos].weights_arr[i][j] = w[j];

    }

}
void train_loc_road_c(int locid ,int pre_locid , RoadInfo  *road_info_arr,LocRoad * loc_road_arr)
{
    int locpos = locid_hash[locid];
    int pre_locpos = locid_hash[pre_locid];
    //add 2013年6月10日
    if(road_info_arr[pre_locpos].flag != 1 ){
        fprintf(stdout,"train_loc_road : pre_locpos no history\n");
        return ;
    }

    RoadInfo * road_info = &(road_info_arr[locpos]);
    LocRoad  * pre_road = &(loc_road_arr[pre_locpos]);



    CSlistNode * now_l_head = road_info->road_list->head;


    double now_thisroad_speed           = 0.0;
    double now_preroad_histroy_speed    = 0.0;
    double next_thisroad_histroy_speed  = 0.0;
    double next_preroad_histroy_speed   = 0.0;


    double next_thisroad_speed          = 0.0;
    int roadid = road_info->road_id;


    double * *  train_data[7];
    double data[4] ;
    double * label[7];

    int weekday_num[7] = {0};

    while(now_l_head ) {
        int weekday = ((struct road_t*) (now_l_head->value))->weekday;
        weekday_num[weekday] += TIMES_DAY ;
        now_l_head = now_l_head->next;
    }//end while
    //train_data = (double ***)malloc(sizeof(double ** ) * 7);
    for(int i = 0 ; i < 7 ; i++){
        train_data[i] = (double * * )malloc(sizeof(double *) *weekday_num[i] );
        assert(train_data[i]);
        if(train_data[i] == NULL){
            printf("train_data[i] memory error\n");
        }
        for(int j = 0 ; j < weekday_num[i] ;j ++ ){
            //train_data[i][j]  = (double *)malloc(sizeof(double) * 4 );
            train_data[i][j]  = (double *)calloc(4 ,sizeof(double)  );
            assert(train_data[i][j]);
            if(train_data[i][j] == NULL){
                printf("train_data[i][j] memory error\n");
            }
        }
    }
    for(int i = 0 ; i < 7 ; i++){
        //label[i] = (double *  )malloc(sizeof(double ) *weekday_num[i] );
        label[i] = (double *  )calloc(weekday_num[i] ,sizeof(double )  );
        assert(label[i]);
        if(label[i] == NULL){
            printf("label[i] memory error\n");
        }

    }


    int weekday_pos[7] = {0};
    now_l_head = road_info->road_list->head;
    while(now_l_head ) {

        int weekday = ((struct road_t*) (now_l_head->value))->weekday;


        //遍历每一个时刻的数据
        for(int i = 0 ; i < TIMES_DAY ; i ++) {
            now_thisroad_speed =  ((struct road_t*) (now_l_head->value))->speed_arr[i];




            if(i+1 != TIMES_DAY) {
                next_thisroad_histroy_speed =  loc_road_arr[locpos].history_road[weekday][i+1];
                next_thisroad_speed = ((struct road_t*) (now_l_head->value))->speed_arr[i+1];
            } else {
                //next_thisroad_histroy_speed = road_info->history_road[weekday][i]; //这个时候 就是接近午夜十二点的时刻
                next_thisroad_histroy_speed = loc_road_arr[locpos].history_road[weekday][i]; //这个时候 就是接近午夜十二点的时刻
                next_thisroad_speed = ((struct road_t*) (now_l_head->value))->speed_arr[i];
            }


            now_preroad_histroy_speed =  pre_road->history_road[weekday][i];

            if(i+1 != TIMES_DAY)

                next_preroad_histroy_speed =  pre_road->history_road[weekday][i+1];
            else
                next_preroad_histroy_speed = pre_road->history_road[weekday][i]; //这个时候 就是接近午夜十二点的时刻


//            printf("%d %d %lf %lf %lf %lf %lf\n",
//                    roadid , weekday ,now_thisroad_speed ,now_preroad_histroy_speed ,next_preroad_histroy_speed ,
//                    next_thisroad_histroy_speed,next_thisroad_speed);

            int pos = weekday_pos[weekday];
            train_data[weekday][ pos ] [0] = now_thisroad_speed ;
            train_data[weekday][ pos ] [1] = now_preroad_histroy_speed ;
            train_data[weekday][ pos ] [2] = next_preroad_histroy_speed ;
            train_data[weekday][ pos ] [3] = next_thisroad_histroy_speed ;

            label[weekday][ pos ] = next_thisroad_speed;


             weekday_pos[weekday] ++;


        }
        now_l_head = now_l_head->next;
    }//end while

    //store data
    char * train_file_name = (char *)malloc(MAX_FILE_NAME_LEN);
    //sprintf(train_file_name,"./data/train/%d_data",roadid);//bug 2013年6月10日
    snprintf(train_file_name,MAX_FILE_NAME_LEN,"./data/train/%d_data",roadid);
    FILE * fout = fopen(train_file_name,"w");
    for(int i = 0 ; i <= 6 ; i ++) {
        double w ;


        for(int k = 0 ; k < weekday_num[i] ; k ++) {
            for(int j = 0 ; j < 4 ; j++) {
                w = (train_data[i][k][j]);
                fprintf(fout,"%lf " ,w);
            }
            if(k +1 == weekday_num[i]  ) {
                w = train_data[i][k][0];
                fprintf(fout ,"%lf", w);
            } else {
                w = train_data[i][k+1][0];
                fprintf(fout ,"%lf", w);
            }
            fprintf(fout ,"\n");
        }
    }
    fclose(fout);
    /////


    //最后训练的权值 要保存在 loc 里面

    for(int i = 0 ; i <= 6 ; i ++) {

        double * w = gd_train_c(train_data[i] ,label[i],weekday_num[i],4);

        int locid = road_info->road_id ;
        int loc_pos = locid_hash[locid];

        for(int j = 0 ; j < 4 ; j++)
            loc_road_arr[loc_pos].weights_arr[i][j] = w[j];

        free(w);

    }

    //2013年6月10日
     for(int i = 0 ; i < 7 ; i++){
        for(int j = 0 ; j < weekday_num[i] ;j ++ ){
            free(train_data[i][j]) ;
            train_data[i][j] = NULL;

        }
        free(train_data[i]);
        train_data[i] = NULL;
    }
    for(int i = 0 ; i < 7 ; i++){

        free(label[i]);
        label[i] = NULL;
    }


}

int history_train_loc_road(double *  *speed_arr  ,LocRoad * loc_road_arr ,
                           int weekday ,int h ,int m  ,int locid ,int next_time )
{
    int next_step = next_time/STEP;
    if(next_step < 1) next_step = 1;

    int now_step = (h*60 + m)/STEP;
    (*speed_arr) = (double * )malloc(sizeof(double ) * next_step);

    int loc_pos = locid_hash[locid];
    for(int i = 0 ; i < next_step ; i ++) {
        if(TIMES_DAY == now_step + i ) {
            weekday  = (weekday+1)%7;
            now_step -= TIMES_DAY;
        }
        (*speed_arr)[i] = loc_road_arr[loc_pos].history_road[weekday][ (now_step + i) ];
    }


    return next_step;




}
void predict_test(LocRoad * loc_road_arr)
{
    int weekday ;
    int h;
    int  m;
    double now_speed;
    int locid;
    int times;
    printf("输入 星期 小时 分钟 当前速度 ，道路id ，预测时间\n ");
    while(scanf("%d",&weekday) !=EOF) {

        if(weekday == -1) {
            break ;
        }
        scanf("%d%d%lf%d%d",&h,&m,&now_speed,&locid,&times);
        printf("========history_train_loc_road===============\n");
        double * speed_arr;
        int speed_arr_len = history_train_loc_road( &speed_arr ,loc_road_arr ,weekday ,h ,m  ,locid ,times);

        for(int i = 0 ; i < speed_arr_len ; i ++) {
            printf("%lf\n" , speed_arr[i]);
        }
        free(speed_arr);


        ////
        printf("========prediction_train_loc_road===============\n");
        speed_arr_len = prediction_train_loc_road( &speed_arr ,loc_road_arr ,weekday ,h ,m  ,now_speed,locid ,times);

        for(int i = 0 ; i < speed_arr_len ; i ++) {
            printf("%lf\n" , speed_arr[i]);
        }
        free(speed_arr);

        printf("输入 星期 小时 分钟 当前速度 ，道路id ，预测时间\n ");
    }
}
void predict_main()
{



    LocRoad * loc_road_arr = loc_road_arr_create(MAX_LOC_ROAD);


    locid_hash = (int *)malloc(sizeof(int) *MAX_LOC_ROAD);
    get_loc_from_file(loc_road_arr);


    // for test
    predict_test(loc_road_arr);




}
void predict_schedule_main()
{
    NavRoad * nav_road_arr;
    NavRoadKeyNode * nav_key_node_arr ;

    //1% 的内存
    nav_road_init(&nav_road_arr ,& nav_key_node_arr);

    nav_road_create(nav_road_arr,nav_key_node_arr);

    //5%
    //LocRoad * loc_road_arr = new LocRoad[MAX_LOC_ROAD];
    LocRoad * loc_road_arr = loc_road_arr_create(MAX_LOC_ROAD);

    get_locid_seq(loc_road_arr ,nav_road_arr);

    get_locid_pre(loc_road_arr ,nav_road_arr ,nav_key_node_arr);
    /////
    RoadInfo * road_info_arr = predict_roadinfo_create();

    ScheduleGraph * graph =  graph_init();

    set_graph_info( graph ,loc_road_arr);

    int * schedule_arr = NULL; // 2013年6月10日 int * schedule_arr ；
    int n = 0;//2013年6月10日

    graph_topo_sort(&schedule_arr  , &n ,graph);


    schedule_train_loc_road(schedule_arr ,n ,loc_road_arr , road_info_arr);

    //copy_history_roadinfo_locroad(road_info_arr ,loc_road_arr );


    //free(road_info_arr);

    predict_roadinfo_release(road_info_arr);


    nav_road_release(nav_road_arr ,nav_key_node_arr);//add 2013年6月10日
    // for test


    predict_test(loc_road_arr);


    store_loc_in_file(loc_road_arr);


    // free 内存
    loc_road_arr_release(loc_road_arr ,MAX_LOC_ROAD);


}
