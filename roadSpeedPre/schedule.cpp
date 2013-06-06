
#include "schedule.h"

using namespace std;


ScheduleGraph * graph_init()
{
    ScheduleGraph * graph = (ScheduleGraph *)malloc(sizeof(struct ScheduleGraph_t));
    if(graph == NULL){
        printf("memory error \n");
        exit(0);
    }

    memset(graph ,0 ,sizeof (ScheduleGraph));
    for(int i = 0 ; i < MAX_LOC_ROAD ; i ++){

        graph->indegree[i] = -1;
        graph->degree[i] = 0;
    }

    return graph;

}
void set_graph_info(ScheduleGraph * graph ,LocRoad * loc_road_arr)
{
    for(int i = 1 ; i < MAX_LOC_ROAD ; i++){
        if(loc_road_arr[i].locid != 0 ){
            int start = loc_road_arr[i].pre_locid;

            int end = loc_road_arr[i].locid;

            if(start == 0)
                continue;


            if(graph->indegree[end]  == -1)
                graph->indegree[end]  = 0;
            if(graph->indegree[start]  == -1)
                graph->indegree[start]  = 0;



            if(start == end ){
                continue;
            }

            int pos = graph->degree[start];
            graph->edge_arr[start][pos] = end;
            graph->indegree[end] += 1;
            graph->degree[start] ++ ; //pos ++;  //bug 2013年6月4日


            //debug
            //printf("%d %d\n",start ,end);
            assert(pos < MAX_DEGREE) ;

        }

    }




}
void graph_topo_sort(int * * schedule_arr  ,int *n , ScheduleGraph * graph )
{
    queue<int> node_q;
    (*schedule_arr) = (int *)malloc(sizeof(int)*MAX_LOC_ROAD);
    if((*schedule_arr) == NULL){

        fprintf(stderr ,"memory error\n");
        exit(0);
    }
    for(int i = 1 ; i < MAX_LOC_ROAD ; i ++){
        if(graph->indegree[i] == 0){
            node_q.push(i);
        }

    }
    //
    printf("%d\n",node_q.size());
    FILE * fin = fopen(txt_cycle_path,"r");
    if(fin == NULL){
        fprintf(stderr,"%s\n open error",txt_cycle_path);
        exit(0);
    }

    int locid = 0;

    while(fscanf(fin,"%d",&locid)!=EOF){
        node_q.push(locid);

        graph->indegree[locid] = -1;
    }

    fclose(fin);

    ////////////////////////////////////////
    int schedule_arr_pos = 0;
    int * used = (int *)malloc(sizeof(int) * MAX_LOC_ROAD);
    memset(used,0,sizeof(int) * MAX_LOC_ROAD);

    while( !node_q.empty() ){
        int id = node_q.front();
        node_q.pop();
        (*schedule_arr)[schedule_arr_pos++] = id;
        used[id] =1;

        for(int i = 0 ; i < graph->degree[id] ; i ++){
            int end = graph->edge_arr[id][i];

            //if(used[end] == 0){
            graph->indegree[end] --;
            //}



            if(  graph->indegree[end]  == 0  ){
                //used[end] = 1;
                node_q.push(end);
            }

        }


    }// end while

    free(used);

    //debug
    FILE * fout = fopen("schedule","w");
    if(fout == NULL){
        fprintf(stderr," open error");
        exit(0);
    }

    for(int i = 0 ; i < schedule_arr_pos  ; i ++){
        fprintf(fout ,"%d \n",(*schedule_arr)[i]);
    }


    fclose(fout);

    //end debug
     (*n) = schedule_arr_pos ;
}
////test

void schedule_main()
{
    NavRoad * nav_road_arr;

    NavRoadKeyNode * nav_key_node_arr ;

    //1% 的内存
    nav_road_init(&nav_road_arr ,& nav_key_node_arr);

    nav_road_create(nav_road_arr,nav_key_node_arr);

    //5%
    LocRoad * loc_road_arr = new LocRoad[MAX_LOC_ROAD];

    get_locid_seq(loc_road_arr ,nav_road_arr);

    get_locid_pre(loc_road_arr ,nav_road_arr ,nav_key_node_arr);

    ScheduleGraph * graph =  graph_init();

    set_graph_info( graph ,loc_road_arr);

    int * schedule_arr ;
    int n;
    graph_topo_sort(& schedule_arr  , &n ,graph);

}
