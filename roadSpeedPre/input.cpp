#include "input.h"




//可能需要事先计算有多少天的数据
RoadInfo * RoadInfoCreat()
{
    RoadInfo * road_info_p = (RoadInfo * )malloc(sizeof(struct road_info_t));

    if(road_info_p == NULL) {
        return NULL;
    }
    road_info_p->mtime.year = 2012;
    road_info_p->mtime.mouth = 11;
    road_info_p->mtime.day = 21;
    //road_info_p->pre = NULL;
    //road_info_p->next = NULL;

    road_info_p->road_list = CSlistCreate();

    return road_info_p;
}

RoadInfo * RoadInfoInput(RoadInfo * head )
{
    RoadInfo *head_tmp = head;
    //char data_path_str[][50] = {"./data/376.txt","./data/386.txt","./data/2252.txt"};
    char data_path_str[][50] = {"./data/txt/39_0.txt","./data/txt/40_0.txt"};
    char in_str[MAX_STRLEN];
    FILE *file_p;
    struct date_t  now_time;
    for(int i = 0 ; i < 2 ; i ++) {
        if(i != 0 ) {
            RoadInfo * head_node = RoadInfoCreat();
            //head -> next = head_node;
            //head_node ->pre = head;
            head = head_node;
        }

        file_p = fopen(data_path_str[i] ,"r");
        if(file_p == NULL) {
            fprintf(stdout,"%s open error\n",data_path_str[i]);
            return NULL;
        }
        //读取文件内容
        while(fgets(in_str,MAX_STRLEN ,file_p)) {

            if(strlen(in_str) <  DATA_LEN-5) continue;



            getInfoStr(in_str ,head,&now_time);

            //printf("%d\n",strlen(in_str));
        }

    }

    return head_tmp;

}

//88_0 60.80 2012-11-21 0:20:00
//给到的数据在0点的时候 00：00：00
/*
解析 txt 里面的字符串信息

88_0 60.80 2012-11-21 0:20:00
*/
void getInfoStr(char * in_str ,RoadInfo * head ,struct date_t * mtime)
{
    char *token = strtok(in_str," ");

    //将 88_0 转成 880
    char loc_str[10];
    int j = 0;
    for(int i = 0 ; i < (int)strlen(token) ; i ++){
        if( isdigit( token[i])  ){
            loc_str[j++] = token[i];
        }
    }
    loc_str[j] = '\0';


    int road_id = atoi(loc_str);
    head->road_id = road_id;

    token = strtok(NULL," ");
    double speed = strtod(token,NULL);
    assert(speed > 0.02);

    token = strtok(NULL," ");
    int y,m,d;
    sscanf(token,"%d-%d-%d",&y,&m,&d);

    struct date_t tmp;
    tmp.year= y;
    tmp.mouth = m;
    tmp.day = d;

    //通过时间来控制，是否新增一个节点
    if(tmp.cmp (mtime) != 0) {
        (*mtime) = tmp;
        // 相对于 cslist 而言 这更像是用户程序  我自己定义list 里面的数据结构
        struct road_t * road_node = (struct road_t * )malloc(sizeof (struct road_t) );
        if(road_node == NULL)return ;
        road_node->weekday = 0;
        memset(road_node->times,0 ,sizeof(road_node->times)  );
        memset(road_node->speed_arr,0 ,sizeof(road_node->speed_arr)  );
        CSlistAddNodeTail(head->road_list ,road_node);
    }
    if(head->mtime.cmp(&tmp) > 0 ) {
        return ;
    }

    token = strtok(NULL," ");
    int h,mi,s;
    //有当到达零点的时候 没有时间这个数据
    if(token == NULL || strlen(token) == 0){
        h = 0;
        mi = 0 ;//m = 0;  // bug fix 2013年5月31日
        s = 0;
    }else {
        sscanf(token,"%d:%d:%d",&h,&mi,&s);
    }



    int week_t = ZellerFun(y,m,d) ;
    int all_mi = h * 60 +mi;



    if(speed < 0.02 ) {
        all_mi ++;
        all_mi --;
    }
    //head->history_road[week_t][all_mi/STEP] += speed;
    //head->road_times_arr[week_t][all_mi/STEP] ++;

    ( (struct road_t *) (head->road_list->tail->value) )->speed_arr[all_mi/STEP] += speed;
    ( (struct road_t *) (head->road_list->tail->value) )->times[all_mi/STEP] ++;
    ( (struct road_t *) (head->road_list->tail->value) )->weekday = week_t;


}

void getInfoStr(char * in_str ,
                RoadInfo * head,
                struct date_t * mtime,
                struct  LocRoad_t * loc_road )
{
    char *token = strtok(in_str," ");

    //将 88_0 转成 880
    char loc_str[10];
    int j = 0;
    for(int i = 0 ; i < (int)strlen(token) ; i ++){
        if( isdigit( token[i])  ){
            loc_str[j++] = token[i];
        }
    }
    loc_str[j] = '\0';


    int road_id = atoi(loc_str);
    head->road_id = road_id;

    token = strtok(NULL," ");
    double speed = strtod(token,NULL);
    assert(speed > 0.02);

    token = strtok(NULL," ");
    int y,m,d;
    sscanf(token,"%d-%d-%d",&y,&m,&d);

    struct date_t tmp;
    tmp.year= y;
    tmp.mouth = m;
    tmp.day = d;

    //通过时间来控制，是否新增一个节点
    if(tmp.cmp (mtime) != 0) {
        (*mtime) = tmp;
        // 相对于 cslist 而言 这更像是用户程序  我自己定义list 里面的数据结构
        struct road_t * road_node = (struct road_t * )malloc(sizeof (struct road_t) );
        if(road_node == NULL)return ;
        road_node->weekday = 0;
        memset(road_node->times,0 ,sizeof(road_node->times)  );
        memset(road_node->speed_arr,0 ,sizeof(road_node->speed_arr)  );
        CSlistAddNodeTail(head->road_list ,road_node);
    }
    if(head->mtime.cmp(&tmp) > 0 ) {
        return ;
    }

    token = strtok(NULL," ");
    int h,mi,s;
    //有当到达零点的时候 没有时间这个数据
    if(token == NULL || strlen(token) == 0){
        h = 0;
        mi = 0 ;//m = 0;  // bug fix 2013年5月31日
        s = 0;
    }else {
        sscanf(token,"%d:%d:%d",&h,&mi,&s);
    }



    int week_t = ZellerFun(y,m,d) ;
    int all_mi = h * 60 +mi;



    if(speed < 0.02 ) {
        all_mi ++;
        all_mi --;
    }
    //head->history_road[week_t][all_mi/STEP] += speed;
    loc_road->history_road[week_t][all_mi/STEP] += speed;
    //head->road_times_arr[week_t][all_mi/STEP] ++;
    loc_road->road_times_arr[week_t][all_mi/STEP] ++;

    ( (struct road_t *) (head->road_list->tail->value) )->speed_arr[all_mi/STEP] += speed;
    ( (struct road_t *) (head->road_list->tail->value) )->times[all_mi/STEP] ++;
    ( (struct road_t *) (head->road_list->tail->value) )->weekday = week_t;


}
void RoadInfoShow(RoadInfo * head ,char *out_name)
{
//    FILE * out_file;
//    out_file = fopen(out_name,"w");
//    if(out_file == NULL) {
//        //printf("")
//        return ;
//    }
//    while( head ) {
//        fprintf(out_file,"-----------------%d-------------\n",head->road_id);
//
//
//
//        for(int i = 0 ; i < 7 ; i ++) {
//            for(int j = 0 ; j < TIMES_DAY ; j ++) {
//
//                //如果当前没有收集到信息 取一个默认的值
////                if(head->road_times_arr[i][j] == 0) {
////                    fprintf(out_file,"%d %d %lf\n",i ,j,30.0);
////                } else
////                    fprintf(out_file,"%d %d %lf \n",i,j,head->history_road[i][j]);
//
//            }
//        }
//
//        head = head->next;
//
//    }

    //fclose(out_file);

}

void RoadInfoRealease(RoadInfo * head)
{
    if(head){
        CSlistRelease(head->road_list );
        RoadInfo * temp = head;

        //head = head->next;
        free(temp);
    }
}
void RoadInfoClistRealease(RoadInfo * head)
{
    if(head){
        CSlistRelease(head->road_list );
       // head = head->next;
    }

}


void testInput()
{


    RoadInfo * p ;
    p = RoadInfoCreat();
    p = RoadInfoInput(p);
    //RoadInfoProcess(p);
    //GradientDescentProcess(p,strcpy(out_file_name1, "./data/276-data-all") );
//    GradientDescentProcess(p,strcpy(out_file_name1, "./data/376-data-all") ,2);
//    RoadInfoShow(p,strcpy(out_file_name2,"./data/376.out") );


    RoadInfoRealease(p);

}


