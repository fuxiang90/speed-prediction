#include <queue>
#include "road.h"

static  int real_road_count = 0;
static  int real_key_node_count = 0;
static  int real_loc_road_count = 0;

//static map<int,int> locid_pos_map ;

int *locid_hash;

void nav_road_init(NavRoad * *nav_road_arr , NavRoadKeyNode *  *nav_key_node_arr)
{

    (*nav_road_arr) = (NavRoad *)malloc(sizeof(NavRoad)*MAX_ROAD);



    if((*nav_road_arr) == NULL) {
        printf("memory error\n");
        exit(0);
    }
    memset(*nav_road_arr,0,sizeof(NavRoad)*MAX_ROAD);

    (*nav_key_node_arr) = (NavRoadKeyNode *)malloc(sizeof(NavRoadKeyNode)*MAX_KEY_NODE);



    if((*nav_key_node_arr) == NULL) {
        printf("memory error\n");
        exit(0);
    }
    memset(*nav_key_node_arr , 0, sizeof(NavRoadKeyNode)*MAX_KEY_NODE);

    locid_hash = (int *)malloc(sizeof(int) *MAX_LOC_ROAD);

    memset(locid_hash,0,sizeof(int) *MAX_LOC_ROAD);//2013年6月8日 xp  下面的并不会自动赋值为0


}

void nav_road_create(NavRoad * nav_road_arr , NavRoadKeyNode *  nav_key_node_arr)
{


    FILE * fin = fopen(NAV_ROAD_FILE ,"r");

    int road_seq;
    int node_start;
    int node_end;
    double len ;
    int level;
    while(fscanf(fin,"%d%d%d%lf%d",&road_seq,&node_start,&node_end,&len,&level) != EOF) {

        nav_road_arr[road_seq].road_seq  = road_seq;
        nav_road_arr[road_seq].start_id = node_start;
        nav_road_arr[road_seq].end_id = node_end;
        nav_road_arr[road_seq].length = len;
        nav_road_arr[road_seq].locid = 0;
        real_road_count ++;

        if(real_key_node_count < node_start )
            real_key_node_count = node_start;
        if(real_key_node_count < node_end)
            real_key_node_count = node_end;

    }

    for(int i = 1 ; i <= real_key_node_count ; i ++) {
        nav_key_node_arr[i].node_seq = i;
        nav_key_node_arr[i].num_out = 0;
        nav_key_node_arr[i].num_in = 0;


    }


    nav_get_node_info(nav_road_arr,nav_key_node_arr);

    fclose(fin);
    return ;

}
void nav_road_release(NavRoad * nav_road_arr , NavRoadKeyNode *  nav_key_node_arr)
{

    free(nav_road_arr);
    nav_road_arr = NULL;
    free(nav_key_node_arr);
    nav_key_node_arr = NULL;
}
LocRoad * loc_road_arr_create(int n)
{
    //LocRoad * arr = (LocRoad *)malloc(sizeof(LocRoad) * n);
 	LocRoad * arr = new LocRoad[n];
    if(arr == NULL){
        fprintf(stdout ,"loc_road_arr_create  memory \n");
        exit(0);
    }
	for(int i = 0 ; i < n ; i ++){
		arr[i].locid = 0;
		arr[i].pre_locid = 0;
		arr[i].flag = 0;
	}
	return arr;
}
void loc_road_arr_release(LocRoad * loc_road_arr,int n )
{
    for(int i = 0 ; i < n ; i ++){

        loc_road_arr_node_free(&loc_road_arr[i]);

    }
    delete [] loc_road_arr;

}
int loc_road_arr_node_malloc(LocRoad * loc_road)
{
    loc_road->history_road = (double ** )malloc(sizeof(double *) * 7);

    assert( loc_road->history_road);

    for(int i = 0 ; i < 7 ; i ++){
        loc_road->history_road[i] = (double *) malloc(sizeof( double ) * TIMES_DAY);
        assert(loc_road->history_road[i]);

        memset(loc_road->history_road[i] ,0 ,sizeof( double ) * TIMES_DAY) ;//win 下 一定要初始化
       }

    loc_road->weights_arr = (double ** )malloc(sizeof(double *) * 7);
    assert(loc_road->weights_arr);
    for(int i = 0 ; i < 7 ; i ++){
        loc_road->weights_arr[i] = (double *) malloc(sizeof( double ) * 5);

        assert(loc_road->weights_arr[i]);
        memset(loc_road->weights_arr[i] , 0,sizeof(double )*5);


    }



    loc_road->road_times_arr = (int ** )malloc(sizeof(int *) * 7);
     //bug valgrind
    assert(loc_road->road_times_arr);
    for(int i = 0 ; i < 7 ; i ++){
        loc_road->road_times_arr[i] = (int *) malloc(sizeof( int ) * TIMES_DAY);

        assert(loc_road->road_times_arr[i]);
        memset(loc_road->road_times_arr[i] , 0,sizeof(int)*TIMES_DAY);
    }

    return 1;

}
int loc_road_arr_node_free(LocRoad * loc_road)
{
    if(loc_road ->history_road ){
        for(int i = 0 ; i < 7 ; i ++){
            //if(loc_road->history_road[i])
            free(loc_road->history_road[i] );
        }
        free(loc_road->history_road);
        loc_road->history_road = NULL;
    }

    if(loc_road -> weights_arr){
       for(int i = 0 ; i < 7 ; i ++){
            free(loc_road->weights_arr[i] );
        }
        free(loc_road->weights_arr);
        loc_road->weights_arr = NULL;
    }


    if(loc_road->road_times_arr != NULL){
         for(int i = 0 ; i < 7 ; i ++){
            if(loc_road->road_times_arr[i] != NULL)
                free(loc_road->road_times_arr[i] );
        }
        free(loc_road->road_times_arr);
        loc_road->road_times_arr = NULL;
    }
    return 1;
}

int loc_road_arr_node_free_road_times(LocRoad * loc_road)
{
    if(loc_road == NULL)
        return 0;
    for(int i = 0 ; i < 7 ; i ++){
        free(loc_road->road_times_arr[i] );

    }
    free(loc_road->road_times_arr);
    loc_road->road_times_arr = NULL;

    return 1;
}
void nav_get_node_info(NavRoad * nav_road_arr , NavRoadKeyNode *  nav_key_node_arr)
{

    for(int i = 1 ; i <= real_road_count; i ++ ) {
        int node_start =  nav_road_arr[i].start_id;
        int node_end = nav_road_arr[i].end_id;

        nav_key_node_arr[node_start ].road_out[nav_key_node_arr[node_start ].num_out  ] = i;
        nav_key_node_arr[node_start ].num_out ++;

        nav_key_node_arr[node_end ].road_in[nav_key_node_arr[node_end ].num_in  ] = i;
        nav_key_node_arr[node_end ].num_in ++;

    }


}

/*
*
327_0 166736
327_1 166737
1592_0 95887
1592_1 93634

由于 原生的 locid 是有 id_[0|1] ，其中0 1 表示方向
所以locid 可以直接 去掉_ 转成数字  以唯一表示

*/
void get_locid_seq( LocRoad * loc_road_arr , NavRoad *nav_road_arr )
{
    FILE * fin = fopen(LOCID_SEQ_FILE ,"r");

    if(fin ==NULL){
        fprintf(stdout,"%s\n",LOCID_SEQ_FILE);
    }
    char * loc_str = (char *)malloc(50);
    char * loc_int_str = (char *)malloc(50);

    int  loc_road_pos  = 0;
    int road_seq;
    //

    while(fscanf(fin,"%s %d",loc_str ,&road_seq ) != EOF) {

        int len = strlen(loc_str);
        int j = 0;
        for(int i = 0 ; i < len ; i ++) {
            if( isdigit (loc_str[i] ) ) {

                loc_int_str[j++] = loc_str[i];
            }

        }
        loc_int_str[j] = '\0';
        int locid = atoi(loc_int_str);

        //locid_seq_map.insert(map<int ,int>::value_type(locid , road_seq ) );

        if(locid_hash[locid] == 0 ) {
            locid_hash[locid] = ++loc_road_pos ;
            loc_road_arr[loc_road_pos].locid = locid;
        }

        loc_road_arr[loc_road_pos].road_seq_vec.push_back(road_seq); //valgrind 2013年6月10日

        //2013年6月8日
        loc_road_arr_node_malloc(&loc_road_arr[loc_road_pos]);
        nav_road_arr[road_seq].locid = locid;
    }

    real_loc_road_count = loc_road_pos;
    free(loc_str);
    free(loc_int_str);
    fclose(fin);

    return ;


}

int  nav_bfs(int locid , LocRoad * loc_road_arr  ,NavRoad  *nav_road_arr,NavRoadKeyNode *  nav_key_node_arr )
{

    // handle error ,add later

    int loc_pos = locid_hash[locid];

    int road_seq_start = loc_road_arr[loc_pos].road_seq_vec[0];

    int node_start = nav_road_arr[road_seq_start ].start_id;
    int pre_locid = 0;
    queue<int> key_node_seq_q;
    key_node_seq_q.push(node_start);

    while(!key_node_seq_q.empty() ) {
        int key_node_seq = key_node_seq_q.front();
        key_node_seq_q.pop();
        int num_in = nav_key_node_arr[key_node_seq].num_in;

        for(int i = 0 ; i < num_in ; i ++) {
            int pre_road_seq = nav_key_node_arr[key_node_seq].road_in[i];

            if( nav_road_arr[pre_road_seq].locid > 0 ) {
                if( nav_road_arr[pre_road_seq].locid != locid) {
                    pre_locid = nav_road_arr[pre_road_seq].locid ;
                    break;
                } else if(nav_road_arr[pre_road_seq].locid == locid) {
                    key_node_seq_q.push(nav_road_arr[pre_road_seq].start_id);

                }

            }

        }



    }
    return pre_locid;
}
void check_locid_pre( LocRoad * loc_road_arr  ,NavRoad  *nav_road_arr , NavRoadKeyNode *  nav_key_node_arr)
{

    FILE * fin = fopen("loc_pre_loc","w");
    for(int i = 1 ; i < real_loc_road_count ; i++){



        int locid = loc_road_arr[i].locid ;
        //if(locid_hash [locid] == 0 ) continue;
        if(locid == 390){
            locid ++;
            locid --;
        }

        int pre_locid = loc_road_arr[i].pre_locid;

        if(pre_locid ){
            fprintf(fin,"%d %d \n",locid ,pre_locid);
            printf("%d %d \n",locid ,pre_locid);
        }


    }
    fclose(fin);
}

void get_locid_pre( LocRoad * loc_road_arr  ,NavRoad  *nav_road_arr , NavRoadKeyNode *  nav_key_node_arr)
{
    static int count = 0;

    FILE * fout = fopen("degree","w");
    for(int i = 1 ; i < MAX_LOC_ROAD ; i++){ // bug 2013年6月5日 real_loc_road_count

        int locid = loc_road_arr[i].locid ;

        if(locid == 0) continue;

        //debug
        if(locid == 390  || locid == 3760 || locid == 3860){
            locid ++;
            locid --;
        }

        int pre_locid = nav_bfs(locid,loc_road_arr ,nav_road_arr ,nav_key_node_arr);

        //debug 2013年6月4日

        if(pre_locid == 0){

            pre_locid ++;
            pre_locid --;
            fprintf(fout,"%d\n",locid);
        }
        ////
        count ++;
        //如果没有得到上一条路 ，那么这个的上一条路就是他自己
        if(pre_locid == 0){
            loc_road_arr[i].pre_locid = locid;
        }else
            loc_road_arr[i].pre_locid = pre_locid;

    }

    printf("%d",count);
}
void road_main()
{

    NavRoad * nav_road_arr;
    NavRoadKeyNode * nav_key_node_arr ;

    nav_road_init(&nav_road_arr ,& nav_key_node_arr);

    nav_road_create(nav_road_arr,nav_key_node_arr);

    LocRoad * loc_road_arr = new LocRoad[MAX_LOC_ROAD];

    get_locid_seq(loc_road_arr ,nav_road_arr);

    get_locid_pre(loc_road_arr ,nav_road_arr ,nav_key_node_arr);

    int id = nav_bfs(390,loc_road_arr ,nav_road_arr ,nav_key_node_arr);

    printf("%d \n",id);
    check_locid_pre(loc_road_arr ,nav_road_arr ,nav_key_node_arr);


}
