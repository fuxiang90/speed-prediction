#include <queue>
#include "road.h"

static  int real_road_count = 0;
static  int real_key_node_count = 0;
static  int real_loc_road_count = 0;

static map<int,int> locid_pos_map ;

int *locid_hash;

void nav_road_init(NavRoad * *nav_road_arr , NavRoadKeyNode *  *nav_key_node_arr)
{

    (*nav_road_arr) = (NavRoad *)malloc(sizeof(NavRoad)*MAX_ROAD);

    if((*nav_road_arr) == NULL) {
        printf("memory error\n");
        exit(0);
    }

    (*nav_key_node_arr) = (NavRoadKeyNode *)malloc(sizeof(NavRoadKeyNode)*MAX_KEY_NODE);

    if((*nav_key_node_arr) == NULL) {
        printf("memory error\n");
        exit(0);
    }

    locid_hash = (int *)malloc(sizeof(int) *MAX_LOC_ROAD);


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

    char * loc_str = (char *)malloc(50);
    char * loc_int_str = (char *)malloc(50);

    int  loc_road_pos  = 0;
    int road_seq;
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

        loc_road_arr[loc_road_pos].road_seq_vec.push_back(road_seq);

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
    for(int i = 1 ; i <= real_loc_road_count ; i++){

        int locid = loc_road_arr[i].locid ;

        if(locid ==37910 ){
            locid ++;
            locid --;
        }
        int pre_locid = nav_bfs(locid,loc_road_arr ,nav_road_arr ,nav_key_node_arr);

        printf("%d %d \n",locid ,pre_locid);

    }
}

void get_locid_pre( LocRoad * loc_road_arr  ,NavRoad  *nav_road_arr , NavRoadKeyNode *  nav_key_node_arr)
{

    for(int i = 1 ; i <= real_loc_road_count ; i++){

        int locid = loc_road_arr[i].locid ;
        int pre_locid = nav_bfs(locid,loc_road_arr ,nav_road_arr ,nav_key_node_arr);

        loc_road_arr[locid].pre_locid = pre_locid;
    }
}
void road_main()
{

    NavRoad * nav_road_arr;
    NavRoadKeyNode * nav_key_node_arr ;

    nav_road_init(&nav_road_arr ,& nav_key_node_arr);

    nav_road_create(nav_road_arr,nav_key_node_arr);

    LocRoad * loc_road_arr = new LocRoad[MAX_LOC_ROAD];

    get_locid_seq(loc_road_arr ,nav_road_arr);



//    nav_bfs(3270,loc_road_arr ,nav_road_arr ,nav_key_node_arr);

    check_locid_pre(loc_road_arr ,nav_road_arr ,nav_key_node_arr);


}
