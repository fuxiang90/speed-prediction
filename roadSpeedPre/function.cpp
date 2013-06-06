#include "function.h"



int date_t::cmp(struct date_t * p){

    if(year != p->year){
        return year > p->year ?1:-1;
    }

    if(mouth != p->mouth){
        return mouth > p->mouth ?1:-1;
    }


    if(day != p->day){
        return day > p->day ?1:-1;
    }

    return 0;

}



//返回时间是哪个星期
int ZellerFun(int yearnumber ,int monthnumber ,int daynumber)
{
    int c, d, G;
    c = yearnumber / 100;
    if (monthnumber < 3) {
        monthnumber = monthnumber + 10;
        d = (yearnumber % 100) - 1;
    } else {
        monthnumber = monthnumber - 2;
        d = (yearnumber % 100);
    }
    G = int((2.6 * monthnumber - .2) + daynumber + d + (d/4) + (c/4) - 2 * c) % 7 ;
    if (G < 0) {
        G = G + 7;
    }
    return G;

}
/*
void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}
*/

/*
    * str 原始字符串 from要替换的 to替换的新字符串
*/
char * replaceAll(char * str, char * from, char * to  ,int new_size = MAX_STRLEN)
{
    size_t len_str = strlen(str);
    if(len_str == 0 )return NULL;

    size_t len_from = strlen(from);
    if(len_from == 0) return str;

    size_t len_to = strlen(to);
    char *new_str = (char *)malloc(new_size * sizeof(char));
    if(new_str == NULL) return NULL;
    size_t start_pos = 0;
    size_t i,j,k;
    for(i = 0 ; i < len_str ; i ++){
        if(from[0] ==  str[i]){
            //j = i + 1;
            i ++;
            for(k = 1 ; k < len_from ;k++){
                if(i < len_str &&str[i] ==  from[k]){
                    i ++;
                }else {
                    break;
                }
            }//end for

            if(k== len_from){
                for(j = 0 ; j < len_to ; j++){
                    new_str[start_pos ++] = to[j];
                }
            }
            i --;
        }else {
            new_str[start_pos ++]  = str[i];
        }

    }//end for

    new_str[start_pos ] = '\0';
    free(str);
    return new_str;
}

char * replaceAllSpace(char * str)
{
    char *from = (char *)malloc(10 * sizeof(char));
    char *to  = (char *)malloc(10 * sizeof(char));
    strcpy(from," ");
    strcpy(to,"");

    char * result = replaceAll(str,from,to);
    free(from);
    free(to);
    return result;
}
int get_locid_str(char * str)
{
    int len = strlen(str);
    char * loc_str =  (char * )malloc(sizeof(char)* len);
    int j = 0;
    for (int i = 0 ; i < len ; i ++){
        if(isdigit( str[i])){
            loc_str[j++] = str[i];
        }
    }

    loc_str[j] = '\0';

    int id = atoi(loc_str);

    free(loc_str);

    return id;

}

int get_dir_filenames( char  * * filename_arr  ,int max_file_num)
{

    int filenum = 0;
    const char *dir = txt_root_path;

    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if( (dp = opendir(dir))  == NULL )
    {
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return -1;
    }

    while( (entry = readdir(dp)) != NULL)
    {
        if( S_ISDIR(statbuf.st_mode) )
        {
//            if( strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0 )
//                continue;
        }
        else
        {
            if(entry->d_name[0] == '.')
                continue;
            strcpy(filename_arr[filenum++], entry->d_name);

        }
    }

    closedir(dp);

    return filenum;

}

void get_filename(int locid ,char str[])
{
    sprintf(str,"%d",locid);
    int len = strlen(str);
    char last = str[len-1];
    str[len-1] = '_';
    str[len] = last ;
    str[len+1] = '.';
    str[len+2] = 't';
    str[len+3] = 'x';
    str[len+4] = 't';
    str[len+5] = '\0';
}
void testFun()
{

    //printf("%d\n",ZellerFun(2049,10,1));

//    char * str = (char *)malloc(100 * sizeof(char));
//    char *from = (char *)malloc(100 * sizeof(char));
//    char *to  = (char *)malloc(100 * sizeof(char));
//
//    strcpy(str ,__DATE__);
//    strcpy(from," ");
//    strcpy(to,"");
//
//    printf( "%s",replaceAll(str,from,to) );


//    char ** filename_arr  = (char **) malloc ( sizeof(char *) * MAX_LOC_ROAD);
//
//    MAX_TXT_NAME_LEN
//    for(int i = 0  ; i < MAX_LOC_ROAD ; i++){
//        filename_arr[i] = (char *)malloc(MAX_TXT_NAME_LEN);
//    }
//
//    int n = get_dir_filenames(filename_arr ,MAX_TXT_NAME_LEN);
//
//    for(int i = 0 ; i < n ; i ++){
//        printf("%s\n",filename_arr[i]);
//    }

    char str[20];
    get_filename(122,str);

    printf("%s\n",str);

}

