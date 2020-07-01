# include "xxx.h"

const T *tempT[5];
const NN nn_template={{""},NULL};
A ta_arr [50], ta_select[50];
/* *
tt_loc_spz=NULL;
tt_tup_siz=tt_amt=0;
ta_amt=ta_select_amt=0;
*/
int insert_tt(T *t){
    if(tt_amt<5){
        tt_tup_siz += t->tup_siz - _enable;
        if(ta_amt+t->attr_amt <=50){
            printf("%s:%d",_quote(insert_tt),t->amt);
            {for(int i=0;i<t->attr_amt;i++) ta_arr[ta_amt+i] = t->attrs[i];}
            //memcpy(&ta_arr[ta_amt], t->attrs, t->attr_amt);
            ta_amt += t->attr_amt;
        return !(tempT[tt_amt++] = t);
    } }{printf("%x %d:",t,tt_amt);_report_error(insert_tt)};
}
A *search_ta(const char an[]){
    for(int i=0; i<ta_amt;i++){
        if(!strcmp(an, ta_arr[i].name))
            return &ta_arr[i];
    } return (A *)-1;
}
int show_tt(){printf("%s:\t", _quote(show_tt));
    printf("ta:%d, tt:%d, select:%d, size:%d",ta_amt,tt_amt,ta_select_amt,tt_tup_siz)_endline
    for(int i = 0,ret = 0; i < tt_amt ; i ++){
        printf("%s:",tempT[i]->name)_endline
        T *t = (T*)tempT[i];
        for(int  j = 0; j < tempT[i]->attr_amt; j++, ret++){
            A *a = &t->attrs[j];
            printf("\t%s:t=%d, o=%d",a->name, a->type, a->offset);//_endline
        }
    }_endline return 0;
}

int create_tt(const TNN *tnn, const ANN *ann){
    for(;tnn;tnn = _cdr(*tnn)){
        int temp_amt = ta_amt; const int temp_siz = tt_tup_siz;
        insert_tt(&(search_t(_car(*tnn).name)->next->contain));
        for(; temp_amt<ta_amt; temp_amt++)  ta_arr[temp_amt].offset +=temp_siz;
    }
    if(ann != NULL){
        A *temp = NULL;
        for(int i=0; ann; ann = _cdr(*ann),i++)
            if((temp = search_ta(_car(*ann).name))!=(A *)-1)
                ta_select[i] = *temp;
            else _report_error(search_ta)
    } else memcpy(ta_select,ta_arr,(ta_select_amt = ta_amt)*sizeof(A));
    return !(tt_loc_spz = calloc(1,tt_tup_siz));
}

int delete_tt(){
    free(tt_loc_spz);
    return tt_amt = ta_amt = ta_select_amt = tt_tup_siz = 0;
}
void show_cache(void){printf("%s:\t",_quote(show_cache));
    for(int j=0;j<tt_tup_siz;j++) printf("%.2x ",*(char*)(tt_loc_spz+j))_endline
}
int output_tt(){
    for(int i=0;i<ta_select_amt;i++){
        void *located = tt_loc_spz + ta_select[i].offset;
        if(ta_select[i].type)
            printf("%s\t", located);
        else printf("%d\t", *(int *)located);
    }_endline
}
//int eval_logic(L *l){return TRUE;};




int UPDATE_T(const char tn[],ANN *ann, VN *vn, L *l, BHV f){
    TNN temp = nn_template; strcpy(temp.contain.name,tn);
    create_tt(&temp, NULL); show_tt();
    T *t = (T *)tempT[0];
    for(int row=0;row<t->amt;row++)
        if(*(char *)_t_info_loc(*t, row, _enable)){
            memcpy(tt_loc_spz ,_t_info_loc(*t,row,_enable), t->tup_siz-_enable);
            if(eval_logic(l)){
                //BHV *(char *)_t_info_loc(*t, row, _enable) = FALSE;
                f(t,row,ann,vn);
            }
        }
    return delete_tt();
}

/* 算了就这样吧, 反正已经很魔幻了
int UNNAMED(const char tn[],ANN *ann, VN *vn, L *l, BHV f){
    TNN temp = nn_template; strcpy(temp.contain.name,tn);
    create_tt(&temp, NULL); show_tt();
    T *t = (T *)tempT[0];
    for(int row=0;row<t->amt;row++)
        if(*(char *)_t_info_loc(*t, row, _enable)){
            memcpy(tt_loc_spz ,_t_info_loc(*t,row,_enable), t->tup_siz-_enable);
            if(eval_logic(l))
                f(t,row,ann,vn);
        }
    return delete_tt();
}*/
int DELETE_T(const char tn[], L *l, BHV f){
    TNN temp = nn_template; strcpy(temp.contain.name,tn);
    create_tt(&temp,NULL); show_tt();
    T *t = (T *)tempT[0];
    for(int row=0;row<t->amt;row++)
        if(*(char *)_t_info_loc(*t, row, _enable)){
            memcpy(tt_loc_spz ,_t_info_loc(*t,row,_enable), t->tup_siz-_enable);
            if(eval_logic(l))
                f(t,row,NULL,NULL);
        }
    return delete_tt();
}
int SELECT_T(TNN *tnn, ANN *ann, L *l){
    create_tt(tnn,ann);
    show_tt();
int select_t(int index, int offset, L *l){
    if(index < tt_amt){
        for(int row=0; row <tempT[index]->amt;row++)
            if(*(char *)_t_info_loc(*tempT[index], row, _enable)){
                const T *t = tempT[index];
                memcpy(tt_loc_spz+offset ,_t_info_loc(*t,row,0), t->tup_siz-_enable);
                select_t(index+1, offset+t->tup_siz-_enable , l);
            }
    }else if(eval_logic(l))  output_tt();

    return 0;
}   return select_t(0,0,l),delete_tt();
}