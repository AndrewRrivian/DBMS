# include "xxx.h"

void update(T *t, int row, ANN *ann, VN *vn){
    for(;ann;ann = _cdr(*ann), vn = _cdr(*vn)){
        
        A *a = search_ta(_car(*ann).name);

        printf("UPP:%s:%s %d==%d",t->name,a->name, vn->contain.t,a->type)_endl
        void *p = (vn->contain.t==INT)?&vn->contain.v.i:(void *)vn->contain.v.s;
        memcpy(_t_info_loc(*t, row, a->offset), p, _type_len(*a));
    }
}

void delete(T *t, int row, ANN *ann, VN *vn){
    *(char *)_t_info_loc(*t, row, _enable) = FALSE;
}

int main(){



    CREATE_D("anbg");
    SHOW_D();

    CREATE_D("b");
    SHOW_D();

    DROP_D("anbg");
    SHOW_D();

    USE_D("b");

    AN bn = {{0,0,"a"},NULL};
    AN an = {{2,0,"b"},&bn};
    CREATE_T("c",&an);
    SHOW_T();

        AN c = {{0,0,"a"},NULL};
    AN d = {{7,0,"b"},&c};
// CREATE 的方向 "b":7->"a":0

    CREATE_T("o",&d);SHOW_T();
    //DROP_T("c");
    SHOW_T();
    VN v1 = {{CHAR,0},NULL},v2 = {{INT,4},&v1};
    strcpy(v1.contain.v.s,"abc");
    showByte(&search_t("o")->next->contain);
    showByte(&search_t("c")->next->contain);

    INSERT_T("o",&v2);
    showByte(&search_t("o")->next->contain);

//INSERT 的方向 INT->CHAR
    INSERT_T("o",&v2);
    showByte(&search_t("o")->next->contain);
    NN n2={{"b"}, NULL},n1 = {{"a"},& n2};
    INSERT_T("o",&v2);showByte(&search_t("o")->next->contain);
    TNN tnn = {{"o"},NULL};


    _endl _endl _endl _endl
    printf("%s:",_quote(update))_endl
    v2.contain.v.i = 5;
    
    ANN ann1 = {{"a"},NULL},ann2 = {{"b"},&ann1};
//    UPDATE_T("o",&ann2,&v2,NULL,update);

//SELECT 的方向 "b":7->"a":0
    SELECT_T(&tnn,NULL,NULL);
L l[2] ={    {lop:LEAF, cop:LESS,l:{VAR,NULL},r:{VAR,NULL}},
    {lop:LEAF, cop:LESS,l:{VAR,malloc(sizeof(L))},r:{VAR,NULL}}};
    VN vn1 = {contain:{t:INT,v:{s:NULL}},next:NULL};
//    UNNAMED("o",&ann2,&v2,NULL,update);
    v2.next = NULL;
// UPDATE 的方向
    UPDATE("o",&ann1,&v2,NULL);
    showByte(&search_t("o")->next->contain);
    _endl _endl _endl _endl

    SELECT_T(&tnn,NULL,NULL);
    printf("%s",_quote(DELETE_T))_endline
    DELETE("o",NULL);
//    DELETE_T("o",NULL,delete);
//    UNNAMED("o",NULL,NULL,NULL,delete);
    showByte(&search_t("o")->next->contain);
}