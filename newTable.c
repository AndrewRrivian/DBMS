# include "xxx.h"

extern D dbs[10];
extern int using;
//attr_amt,attr_len,amt,tup_siz,loc_len
const T t_template = {
    attrs:NULL, loc_spz:NULL,
    attr_amt:0, attr_len:10, amt:0, tup_siz:1, loc_len:20, name:""};
const TN tn_template = {
    contain:{
        attrs:NULL, loc_spz:NULL,
        attr_amt:0, attr_len:10, amt:0, tup_siz:1, loc_len:20, name:""},
    next:NULL};
const A a_template = {0,0,""};
const AN an_template = {{0,4,""}, NULL};
// 纯粹基于T的操作只有
// CREATE DROP SHOW INSERT 其余的要结合where

int showByte(T *t){printf("%s:%s","showByte",t->name)_endline
    int size = t->tup_siz;
    for(int i=0;i<t->amt;i++){
        for(int j=0;j<size;j++)
            printf(" %.2x" ,*(char*)(t->loc_spz+i*size+j));
        _endline
    }_endline
}

void SHOW_T(void){
    printf("%s.D:",dbs[using].name)_endline
    for(TN *tn = _cdr(dbs[using].t_node); tn; tn = _cdr(*tn)) {
        printf("%s.T:%s", dbs[using].name, _car(*tn).name)_endline
        for(int i=0; i< _car(*tn).attr_amt; i++)
            printf("\t%s:%d", _car(*tn).attrs[i].name, _car(*tn).attrs[i].type);
        _endline
    }
}

// search_t 根据 表名 从 所选数据库 中查找对应 表结点的前一个结点
TN *search_t(const char tn[]){
    for(TN *i = &dbs[using].t_node; _cdr(*i); i = _cdr(*i))
        if(!strcmp(_car(*_cdr(*i)).name, tn)) return i;
    return NULL;//不用 report 是因为 CREATE 需要
}
// init_t 作为 CREATE_T 的子操作
static int init_t(const char tnn[], int tup_siz){
    T *t = &_car(*_cdr(dbs[using].t_node));
    t->tup_siz = tup_siz, t->loc_spz =  malloc(20 * tup_siz);
    return !strcpy(t->name,tnn);
}
// create_a 接受一个 属性-流, 并且将其添加进入对应的 表 中, 然后返回所操作的 属性 的个数
int create_a(AN* an, T *t){
    int offset = !_enable;
    for(t->attrs = calloc(t->attr_len, sizeof(A)); an; an=_cdr(*an), t->attr_amt += 1){
        if(t->attr_len <= t->attr_amt*2) _realloc(t->attrs, t->attr_len, sizeof(A));
        t->attrs[t->attr_amt] = _car(*an);//*((A*)an);
        t->attrs[t->attr_amt].offset = offset;
        offset+=_type_len(_car(*an));
    } return offset;
}
// CREATE_T 接受一个 表名 以及 属性-流, 从而向 所选数据库 中添加一张表 
int CREATE_T(const char tn[], AN *an){
    if(search_t(tn)) {printf("%s",tn);_report_error(CREATE_T)};
    TN *temp_1 = calloc(1,sizeof(TN));
    *temp_1 = tn_template;
    _cdr(*temp_1) = _cdr(dbs[using].t_node);
    _cdr(dbs[using].t_node) = temp_1;
    return init_t(tn, create_a(an, &_car(*temp_1)));
}
// DROP_T 接受一个 表名 , 从而向 所选数据库 中删除 对应表
int DROP_T(const char tn[]){
    TN *temp_1 = search_t(tn), *temp_2 = _cdr(*_cdr(*temp_1));
    free(_car(*_cdr(*temp_1)).loc_spz), _delete_attr(_car(*_cdr(*temp_1)).attrs);
    free(_cdr(*temp_1)), _cdr(*temp_1) = temp_2;
    return 0;
}
// search_a 在指定 表 中查找一个 an 的属性名, 并返回其索引
A *search_a_p(T* t, const char an[]){
    for(int i = 0; i<t->attr_amt; i++)
        if(!strcmp(an,t->attrs[i].name))
            return &t->attrs[i];
    {printf("%s:\t",an);_report_error(search_a_p)};
}

// find_place_t 用于在 表 中找一块可用区域
static int find_place_t(T *t){int i=0;
    for(;i<t->amt;i++) { if(!*(char*)_t_info_loc(*t, i, 0)) break;} return i;
}
// INSERT_T 接受一个 表名 以及 值-流, 从而向 所选数据库 中删除 对应表
// 我暂时不想管这两个流的垃圾回收问题
int INSERT_T(const char tn[], VN *data){
    T *t = &_car(*_cdr(*search_t(tn)));
    if(t->amt <= t->loc_len*2)
        _realloc(t->loc_spz, t->loc_len, t->tup_siz);
    int Wrow = find_place_t(t);

    NN temp[t->attr_amt], *meta;
    for(int i=0;i<t->attr_amt;i++){
        temp[i].next = meta;
        strcpy(temp[i].contain.name, t->attrs[i].name);
        meta = &temp[i];
    }
    
    for(; data; data = _cdr(*data), meta = _cdr(*meta)){
        A *a = search_a_p(t, _car(*meta).name);
        printf("PPP:%s:%s %d==%d",t->name,a->name, data->contain.t,a->type)_endl
        if((!!a->type) != _car(*data).t){
            
            _report_error(INSERT_T);
        }
        memcpy(_t_info_loc(*t,Wrow, a->offset), &_car(*data).v, _type_len(*a));
    } return !(t->amt += (*(char*)_t_info_loc(*t,t->amt,0)=1));
}