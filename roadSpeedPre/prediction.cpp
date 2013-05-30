#include "prediction.h"

extern int *locid_hash;

RoadInfo * predict_init()
{

    RoadInfo * road_info_arr = (RoadInfo * )malloc(sizeof(RoadInfo) * (MAX_LOC_ROAD +1));

    if(road_info_arr == NULL) {
        return NULL;
    }

    for(int i = 0  ; i <= MAX_LOC_ROAD ; i ++) {
        RoadInfo * road_info_p = &(road_info_arr[i]);
        road_info_p->mtime.year = 2012;
        road_info_p->mtime.mouth = 11;
        road_info_p->mtime.day = 21;
        road_info_p->pre = NULL;
        road_info_p->next = NULL;

        road_info_p->road_list = CSlistCreate();
    }


    return road_info_arr;

}
void get_road_info(RoadInfo * road_info_arr)
{

    //get file name 下一次应该写个函数得到某个目录下指定后缀的目录名

    char filename[][20] = { "39_0.txt","40_0.txt"};
    //

    char *file_name =  (char *)malloc(sizeof(char) * MAX_FILE_NAME_LEN);
    for(int i = 0 ; i < 2 ; i ++ ) {
        int locid  = get_locid_str(filename[i]);
        int loc_pos = locid_hash[locid] ;

        sprintf(file_name ,"%s%s",txt_root_path ,filename[i]);
        get_road_info_filename( &road_info_arr[loc_pos] ,file_name );
        road_info_arr[loc_pos].flag = 1;
    }


}

/*这里传入的head 必须实现分配好内存*/
void get_road_info_filename(RoadInfo * head , char *filename )
{
    char in_str[MAX_STRLEN];
    struct date_t  now_time;
    FILE * file_p = fopen(filename ,"r");
    if(file_p == NULL) {
        printf("%s open error\n",filename);
        return ;
    }
    //读取文件内容
    while(fgets(in_str,MAX_STRLEN ,file_p)) {

        if(strlen(in_str) <  DATA_LEN-5) continue;

        getInfoStr(in_str ,head,&now_time);

    }

}
void get_road_info_history(RoadInfo  *road_info_arr,LocRoad * loc_road_arr)
{

    for(int i =1 ; i <= MAX_LOC_ROAD ; i++ ) {
        if(road_info_arr[i].flag == 0) {
            continue;
        }
        int locid = road_info_arr[i].road_id;
        int loc_pos = locid_hash[locid];

        assert(loc_pos == i );
        if(loc_pos != i ) {
            printf("locid_hash error \n");
        }


        int pre_locid =loc_road_arr[locid].pre_locid;

        int pre_locid_pos = locid_hash[pre_locid];
        if(pre_locid == 0 || road_info_arr[pre_locid_pos].flag == 0  ) {
            continue;
        }


        get_road_info_history_node(&road_info_arr[i] , &road_info_arr[pre_locid_pos] ,loc_road_arr );


    }
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
    if(strlen(token) == 0) {
        h = 0;
        m = 0;
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
    RoadInfo * pre_road = pre_road_info;

    CSlistNode * pre_l_head = pre_road->road_list->head;
    CSlistNode * now_l_head = road_info->road_list->head;


    double now_thisroad_speed           = 0.0;
    double now_preroad_speed            = 0.0;//这个可能不要
    double now_preroad_histroy_speed    = 0.0;
    double next_preroad_histroy_speed   = 0.0;
    double next_thisroad_histroy_speed  = 0.0;

    double next_thisroad_speed          = 0.0;
    int roadid = road_info->road_id;


    vector < vector <double > > train_data[7];
    vector<double > data ;
    vector<double > label[7];
    data.resize(4);
    while(now_l_head ) {

        int weekday = ((struct road_t*) (now_l_head->value))->weekday;

        //if(weekday >= 2 &&weekday <= 4)
        //遍历每一个时刻的数据
        for(int i = 0 ; i < TIMES_DAY ; i ++) {
            now_thisroad_speed =  ((struct road_t*) (now_l_head->value))->speed_arr[i];
            now_preroad_speed = ((struct road_t*) (pre_l_head->value))->speed_arr[i];



            if(i+1 != TIMES_DAY) {
                next_thisroad_histroy_speed =  road_info->history_road[weekday][i+1];
                next_thisroad_speed = ((struct road_t*) (now_l_head->value))->speed_arr[i+1];
            } else {
                next_thisroad_histroy_speed = road_info->history_road[weekday][i]; //这个时候 就是接近午夜十二点的时刻
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
    FILE * fout = fopen("train","w");
    for(int i = 0 ; i <= 6 ; i ++){
        double w ;


        for(int k = 0 ; k < train_data[i].size() ; k ++){
            for(int j = 0 ; j < 4 ; j++){
                w = (train_data[i][k][j]);
                fprintf(fout,"%lf " ,w);
            }
            if(k +1 == train_data[i].size() ){
                w = train_data[i][k][0];
                fprintf(fout ,"%lf", w);
            }else {
                w = train_data[i][k+1][0];
                fprintf(fout ,"%lf", w);
            }
            fprintf(fout ,"\n");
        }
    }
    fclose(fout);
    /////


    //最后训练的权值 要保存在 loc 里面
    for(int i = 0 ; i <= 6 ; i ++){
        vector<double > w = gd_train(train_data[i] ,label[i]);

        int locid = road_info->road_id ;
        int loc_pos = locid_hash[locid];

        for(int j = 0 ; j < w.size() ; j++)
            loc_road_arr[loc_pos].weights_arr[i][j] = w[j];

    }


}
void road_info_process(RoadInfo  *road_info_arr )
{

    for(int i = 0 ; i <= MAX_LOC_ROAD ; i ++){

        if(road_info_arr[i].flag == 0){
            continue;
        }
        RoadInfo * head = &(road_info_arr[i]);

        for(int i = 0 ; i < 7 ; i ++) {
            for(int j = 0 ; j < TIMES_DAY ; j ++) {

                if(head->road_times_arr[i][j] == 0) {
                    head->history_road[i][j] = 30.0;
                } else {
                    head->history_road[i][j]/=head->road_times_arr[i][j];
                }

            }
        }//end for
        ///////////////////////////


        CSlistNode * l_head = head->road_list->head;
        //遍历某条路 每一天的数据
        while(l_head ) {

            //遍历每一个时刻的数据
            for(int i = 0 ; i < TIMES_DAY ; i ++) {
                  // debug
                if( ((struct road_t*) (l_head->value))->speed_arr[i]  < 0.2){
                    ((struct road_t*) (l_head->value))->speed_arr[i]  =  head->history_road[((struct road_t*) (l_head->value))->weekday ][i];
                }else
                    ((struct road_t*) (l_head->value))->speed_arr[i] /= ((struct road_t*) (l_head->value))->times[i] ;



            }
            l_head = l_head->next;
        }//end while


    }


}

void copy_history_roadinfo_locroado(RoadInfo  *road_info_arr , LocRoad * loc_road_arr )
{
    for(int i = 1 ; i <= MAX_LOC_ROAD ; i ++){
        if(road_info_arr[i].flag == 0){
            continue;
        }

//        for(int j = 0 ; j < 7 ; j ++){
//            for(int k = 0 ; k < TIMES_DAY ; k ++){
//                loc_road_arr[i].history_road[j][k] = road_info_arr[i].history_road[j][k];
//            }
//        }

        //
        memcpy(&(loc_road_arr[i].history_road)  , &(road_info_arr[i].history_road) , sizeof(double)*7*TIMES_DAY );

    }

}

double  prediction_train_loc_road(LocRoad * loc_road_arr ,
    int weekday ,int h ,int m ,double speed ,int locid ,int pre_time )
{

    double pre_speed = 0;

    int loc_pos = locid_hash[locid];
    int pre_locid = loc_road_arr[loc_pos].pre_locid;
    int pre_loc_pos  = locid_hash[pre_locid];
    double (*weight)[5] =  &(loc_road_arr[loc_pos].weights_arr[weekday] );

    int t = (h*24 + m)/STEP;
    double speed_arr[5] ;
    speed_arr[0]    =1 ;
    speed_arr[1]    = speed;
    speed_arr[2]    = loc_road_arr[pre_loc_pos].history_road[weekday][t];
    speed_arr[3]    = loc_road_arr[pre_loc_pos].history_road[weekday][t+1];
    speed_arr[4]    = loc_road_arr[loc_pos].history_road[weekday][t+1];

    normalize(speed_arr ,5);
    for(int i = 0 ; i < 5 ; i ++){

        pre_speed += (*weight)[i] * speed_arr[i];
    }

    //printf("%lf\n",pre_speed);

    return pre_speed;

}
//test 检查 梯度下降的准确性
void check_train_loc_road(LocRoad * loc_road_arr )
{
    FILE * fin = fopen("./data/txt/39_0.txt","r");

    char in_str[MAX_STRLEN];
    struct date_t  now_time;
    if(fin == NULL) {
        printf("./data/txt/39_0.txt open error\n");
        return ;
    }
    //读取文件内容
    double speed;
    int weekday;
    int h;
    int m;
    double pre_speed = 0;

    int n = 0;
    int right_num =0;
    while(fgets(in_str,MAX_STRLEN ,fin)) {

        if(strlen(in_str) <  DATA_LEN-5) continue;

        printf("%lf %lf\n" ,speed ,pre_speed);

        if(fabs (speed - pre_speed) <  5.0 ){
            right_num ++;
        }
        get_info_str(in_str,&speed ,&weekday ,&h,&m);

        pre_speed = prediction_train_loc_road(loc_road_arr ,weekday ,h,m,speed,390,1);


        n ++;
    }

    printf("%lf\n",right_num *1.0 / n);


}
void predict_main()
{
    NavRoad * nav_road_arr;
    NavRoadKeyNode * nav_key_node_arr ;

    nav_road_init(&nav_road_arr ,& nav_key_node_arr);

    nav_road_create(nav_road_arr,nav_key_node_arr);

    LocRoad * loc_road_arr = new LocRoad[MAX_LOC_ROAD];

    get_locid_seq(loc_road_arr ,nav_road_arr);

    get_locid_pre(loc_road_arr ,nav_road_arr ,nav_key_node_arr);
    /////
    RoadInfo * road_info_arr = predict_init();

    get_road_info(road_info_arr);


    road_info_process(road_info_arr);

    get_road_info_history(road_info_arr,loc_road_arr);

    copy_history_roadinfo_locroado(road_info_arr ,loc_road_arr );


    prediction_train_loc_road( loc_road_arr ,1 ,10 ,10 ,20 ,390 ,2);

    check_train_loc_road(loc_road_arr);
    printf("debug \n");




}

