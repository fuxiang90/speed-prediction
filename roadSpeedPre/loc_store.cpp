#include "loc_store.h"



void store_loc_in_file( LocRoad * loc_road_arr  )
{

    FILE * fout = fopen(txt_loc_store_path ,"w");
    if(fout == NULL){
        fprintf(stdout ,"%s open error\n",txt_loc_store_path);
        exit(0);
    }

    for(int i = 1 ; i < MAX_LOC_ROAD ; i++){
        int locid = loc_road_arr[i].locid;
        int pre_locid = loc_road_arr[i].pre_locid;

        if(locid == 0) continue;
        if(loc_road_arr[i].flag != 1) continue;
        fprintf(fout,"%d %d %d \n",i ,locid ,pre_locid);

        for(int j = 0 ; j < 7 ; j ++){
            for(int k = 0 ; k < TIMES_DAY ; k++){
                fprintf(fout ,"%lf ",loc_road_arr[i].history_road[j][k]);
            }
            fprintf(fout ,"\n");
        }

         for(int j = 0 ; j < 7 ; j ++){
            for(int k = 0 ; k < 5 ; k++){
                fprintf(fout ,"%lf ",loc_road_arr[i].weights_arr[j][k]);
            }
            fprintf(fout ,"\n");
        }
    }

}
void get_loc_from_file( LocRoad * loc_road_arr)
{
    FILE * fin = fopen(txt_loc_store_path ,"r");
    if(fin == NULL){
        fprintf(stdout ,"%s open error\n",txt_loc_store_path);
        exit(0);
    }
    int i ;
    int locid ;
    int pre_locid;
    while(fscanf(fin,"%d%d%d\n",&i ,&locid ,&pre_locid)!= EOF){

        //debug
        if(locid == 3860){
            locid ++;
            locid --;
        }


        locid_hash[locid] = i;
        loc_road_arr[i].locid =locid;
        loc_road_arr[i].pre_locid = pre_locid;

        //2013年6月8日
        loc_road_arr_node_malloc(&loc_road_arr[i]);
        for(int j = 0 ; j < 7 ; j ++){
            for(int k = 0 ; k < TIMES_DAY ; k++){
                fscanf(fin ,"%lf",&loc_road_arr[i].history_road[j][k]);
            }
            //fprintf(fout ,"\n");
        }

         for(int j = 0 ; j < 7 ; j ++){
            for(int k = 0 ; k < 5 ; k++){
                fscanf(fin ,"%lf",&loc_road_arr[i].weights_arr[j][k]);
            }

        }
    }


}
