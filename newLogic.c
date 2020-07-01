#include "xxx.h"

const L an_l={lop:AND},or_l = {lop:OR}, no_l={lop:NOT};
const L eq_l = {lop:LEAF, cop:EQ},
        mo_l = {lop:LEAF, cop:MORE},
        le_l = {lop:LEAF, cop:LESS};

static void *lookup(A *a, LV *lv){ return a==NULL?lv->p:tt_loc_spz + a->offset;}
static int compare(const int *a,const int *b){
    if (*a==*b) return EQ; else return *a>*b?MORE:LESS;
}
static int eval_compare(L *l){//能进入这个过程, 必定是 LEAF
    A *ta_l = (l->l.t == VAR)?search_ta(l->l.p):NULL,
     *ta_r = (l->r.t == VAR)?search_ta(l->r.p):NULL;
    enum LVT l_t = ta_l?(!!ta_l->type):l->l.t, r_t = ta_r?(!!ta_r->type):l->l.t;
    if(l_t == r_t)
        if(l_t == CONST_INT)
            return (l->cop == compare(lookup(ta_l, &l->l), lookup(ta_r, &l->r)));
        else if(l->cop == EQ)
                return !strcmp(lookup(ta_l, &l->l), lookup(ta_r, &l->r));
            else _report_error(eval_compare)
    else _report_error(eval_compare);
}
int eval_logic(L *l){
    if(!l) return TRUE;
    switch(l->lop){
        case AND:   return eval_logic(l->l.p) && eval_logic(l->r.p);
        case OR:    return eval_logic(l->l.p) || eval_logic(l->r.p);
        case NOT:   return !eval_logic(l->r.p);
        case LEAF:  return eval_compare(l);
        default:    return (l->cop==TRUE);
    }
}