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
    road_info_p->pre = NULL;
    road_info_p->next = NULL;

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
            head -> next = head_node;
            head_node ->pre = head;
            head = head_node;
        }

        file_p = fopen(data_path_str[i] ,"r");
        if(file_p == NULL) {
            fprintf(stderr,"%s open error\n",data_path_str[i]);
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
    head->history_road[week_t][all_mi/STEP] += speed;
    head->road_times_arr[week_t][all_mi/STEP] ++;

    ( (struct road_t *) (head->road_list->tail->value) )->speed_arr[all_mi/STEP] += speed;
    ( (struct road_t *) (head->road_list->tail->value) )->times[all_mi/STEP] ++;
    ( (struct road_t *) (head->road_list->tail->value) )->weekday = week_t;


}
void RoadInfoShow(RoadInfo * head ,char *out_name)
{
    FILE * out_file;
    out_file = fopen(out_name,"w");
    if(out_file == NULL) {
        //printf("")
        return ;
    }
    while( head ) {
        fprintf(out_file,"-----------------%d-------------\n",head->road_id);



        for(int i = 0 ; i < 7 ; i ++) {
            for(int j = 0 ; j < TIMES_DAY ; j ++) {

                //如果当前没有收集到信息 取一个默认的值
                if(head->road_times_arr[i][j] == 0) {
                    fprintf(out_file,"%d %d %lf\n",i ,j,30.0);
                } else
                    fprintf(out_file,"%d %d %lf \n",i,j,head->history_road[i][j]);

            }
        }

        head = head->next;

    }

    fclose(out_file);

}
void RoadInfoProcess(RoadInfo *head)
{
    RoadInfo * temp = head;
    while( head ) {

        for(int i = 0 ; i < 7 ; i ++) {
            for(int j = 0 ; j < TIMES_DAY ; j ++) {

                if(head->road_times_arr[i][j] == 0) {
                    head->history_road[i][j] = 30.0;
                } else {
                    head->history_road[i][j]/=head->road_times_arr[i][j];
                }

            }
        }

        head = head->next;

    }

    head = temp;

    while( head ) {
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
        }


        head = head->next;

    }

}
void RoadInfoRealease(RoadInfo * head)
{
    while(head){
        CSlistRelease(head->road_list );
        RoadInfo * temp = head;

        head = head->next;
        free(temp);
    }
}
void RoadInfoClistRealease(RoadInfo * head)
{
    while(head){
        CSlistRelease(head->road_list );
        head = head->next;
    }

}
/*为梯度下降算法准备 输入文件*/
void GradientDescentProcess(RoadInfo *head ,char * out_file_name ,int flag)
{
    char *root_path = (char *)malloc(sizeof(char)*10);
    strcpy(root_path,"./data/");
    //out_file = fopen(out_file_name,"w");
    char *file_name = (char *)malloc(sizeof(char)*100);

    while(head) {

        if(head->pre) {


            RoadInfo * pre_road = head->pre;
            CSlistNode * pre_l_head = pre_road->road_list->head;
            CSlistNode * now_l_head = head->road_list->head;


            FILE * out_file;
            memset(file_name,0,sizeof(file_name));
            strcpy(file_name ,root_path);

            if(flag == 2){
                sprintf(file_name,"%s%d-%d-test",root_path,head->road_id,STEP);
            }else {
                sprintf(file_name,"%s%d-%d-train",root_path,head->road_id,STEP);
            }


            out_file = fopen(file_name,"w");



            double now_thisroad_speed           = 0.0;
            double now_preroad_speed            = 0.0;
            double next_preroad_histroy_speed   = 0.0;
            double next_thisroad_histroy_speed  = 0.0;

            double next_thisroad_speed          = 0.0;
            int roadid = head->road_id;
            while(now_l_head ) {

                int weekday = ((struct road_t*) (now_l_head->value))->weekday;

                //if(weekday >= 2 &&weekday <= 4)
                //遍历每一个时刻的数据
                for(int i = 0 ; i < TIMES_DAY ; i ++) {
                    now_thisroad_speed =  ((struct road_t*) (now_l_head->value))->speed_arr[i];
                    now_preroad_speed = ((struct road_t*) (pre_l_head->value))->speed_arr[i];



                    if(i+1 != TIMES_DAY){
                        next_thisroad_histroy_speed =  head->history_road[weekday][i+1];
                        next_thisroad_speed = ((struct road_t*) (now_l_head->value))->speed_arr[i+1];
                    }else{
                        next_thisroad_histroy_speed = head->history_road[weekday][i]; //这个时候 就是接近午夜十二点的时刻
                        next_thisroad_speed = ((struct road_t*) (now_l_head->value))->speed_arr[i];
                    }


                    if(i+1 != TIMES_DAY)
                        next_preroad_histroy_speed =  pre_road->history_road[weekday][i+1];
                    else
                        next_preroad_histroy_speed = pre_road->history_road[weekday][i]; //这个时候 就是接近午夜十二点的时刻


                   fprintf(out_file ,"%d %d %lf %lf %lf %lf %lf\n",
                            roadid , weekday ,now_thisroad_speed ,now_preroad_speed ,next_preroad_histroy_speed ,
                            next_thisroad_histroy_speed,next_thisroad_speed);
                }
                now_l_head = now_l_head->next;
            }//end while

            fclose(out_file);


        }//end if
        //fclose(out_file);

        head = head->next;
    }// end while

    free(file_name);
    free(root_path);


}
void testInput()
{
    char * out_file_name1 = (char *)malloc(sizeof(char)*50);
    char * out_file_name2 = (char *)malloc(sizeof(char)*50);
    RoadInfo * p ;
    p = RoadInfoCreat();
    p = RoadInfoInput(p);
    RoadInfoProcess(p);
    //GradientDescentProcess(p,strcpy(out_file_name1, "./data/276-data-all") );
//    GradientDescentProcess(p,strcpy(out_file_name1, "./data/376-data-all") ,2);
//    RoadInfoShow(p,strcpy(out_file_name2,"./data/376.out") );


    RoadInfoRealease(p);

}


