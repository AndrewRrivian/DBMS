# include "xxx.h"

D dbs[10];int using;

// 纯粹基于D的操作
// CREATE DROP USE SHOW 以及LOAD与PERSIST(持久化)
#define _D_LOOP(a,b) for(int i=0;i<10;i++)if(dbs[i].enable){a}else{b}

static int search_d(const char dn[]){
    _D_LOOP( {if(!strcmp(dn,dbs[i].name))return i;}, )return -1;
} 

int CREATE_D(const char dn[]){
    if(search_d(dn)>=0) _report_error(CREATE_D);
    _D_LOOP(, {strcpy(dbs[i].name,dn);return!(dbs[i].enable=1);} )
}
int DROP_D(const char dn[]){
    int index; if((index = search_d(dn))!=-1) return dbs[index].enable=0;
    else  {printf("%s %d\t\n",dn,index);_report_error(DROP_D)};
}
int USE_D(const char dn[]){
    if((using = search_d(dn))!=-1) return 0; _report_error(USE_D);
}
int SHOW_D(void){
    printf("SHOW_D:\n");
    _D_LOOP( printf("\t%s\t\n",dbs[i].name);, ) return 0;
}

//TODO: int LOAD_D(){}
//TODO: int PERSIST_D(){}