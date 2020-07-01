/*////////////////////
crowned :
    CREATE_D    DROP_D    USE_D    SHOW_D
crowned :
    CREATE_T DROP_T SHOW_T
    INSERT_T 
TODO:
    SELECT_T







*/////////////////////
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define _error -1
# define _report_error(func) {printf("error in func %s\n",#func);exit(1);} 

# define _realloc(p,l,s) ((p)=realloc(p,((l)*=2)*(s)))

# define _list(o) typedef struct o##N{o contain;struct o##N *next;} o##N;
# define _cdr(i) ((i).next)
# define _car(i) ((i).contain)

# define _quote(a) (#a)

# define _endl ;printf("\n");
# define _endline _endl
# define init_name_len 20//在考虑柔性

// N for name
typedef struct {char name[20];} N;
// NN for name sequence/node
_list(N);

// TNN for table_name sequence/node
typedef NN TNN;
// ANN for attritube_name sequence/node
typedef NN ANN;
//TODO : toString()

// V for value
typedef struct{enum VT{INT=0,CHAR}t; union VV{int i;char s[23];}v;}V;
// VT for value-type
typedef enum VT VT;
// VV for value-value
typedef union VV VV;
_list(V);

struct L{
    enum LL{FALSE=0,TRUE=1, AND, OR, NOT, LEAF} lop;
    enum LC{EQ, MORE, LESS} cop;
    struct LV{
        enum LVT{CONST_INT=0, CONST_STR=1, VAR} t;
        void *p;
    } l,r;
};
typedef struct L L;
typedef struct LV LV;
int eval_logic(L *l);













// A for attritube/textFiled
typedef struct {
// type     : 0代表int, 1~22代表char
    int type, offset; char name[init_name_len];
}A;_list(A);

const A a_template;     const AN an_template;

A *search_ta(const char an[]);

# define _type_len(a) ((a).type?(a).type+1:sizeof(int))
# define _delete_attr free
// int create_a(AN* an, T *t);

// T for table


typedef struct {
// attrs    : 保存所有属性
// loc_spz  : 实例信息的存储空间
// amt      : 实例信息的存储个数
// tup_siz  : 一个实例单元的大小(字节记)
// loc_len  : 实例空间的大小
    A *attrs;
    void *loc_spz;
    int attr_amt,attr_len,amt,tup_siz,loc_len;
    char name[init_name_len];
}T;
    typedef void (*BHV)(T *, int, ANN *, VN *);
int SELECT_T(TNN *tnn, ANN *ann, L *l);

int DELETE_T(const char tn[], L *l, BHV f);
int UPDATE_T(const char tn[],ANN *ann, VN *vn, L *l, BHV f);
int UNNAMED(const char tn[],ANN *ann, VN *vn, L *l, BHV f);
    # define DELETE(tn,l) UNNAMED(tn,NULL,NULL,l,delete)
    # define UPDATE(args...) UNNAMED(args,update)
# define _t_info_loc(t,x,y) (((t).loc_spz)+(x*(t).tup_siz)+(y))
# define _t_enable 0

void    SHOW_T  (void)           ;  int     CREATE_T    (const char tn[], AN *an)           ;
int     DROP_T  (const char tn[]);  int     INSERT_T    (const char tn[], VN *data);

int create_a(AN* an, T *t);
A *search_a_p(T* t, const char an[]);
int showByte(T *t);
// TN for table sequence/node
_list(T);
TN *search_t(const char tn[]);
const T t_template;     const TN tn_template;
//int init_t(const char tnn[], int tup_siz);

// TA for temp_attritube
typedef struct {A* a;int new_offset;} TA;
//const TA ta_template;     const TN ta_template;
int insert_ta(int i, TA *ta);
int create_ta(const char an[]);
// TAN for temp_attritube sequence/node
_list(TA);


// TT for temp_table


int insert_ta(int i, TA *ta);


# define _enable 0


void *tt_loc_spz;
int tt_tup_siz, tt_amt;
int ta_amt, ta_select_amt;


//int SELECT_T(TTN *tnn, TAN *tan,CONDT *cond);
//int DELETE_T(CONDT *cond);
//int UPDATE_T(AN *an,VN *vn);

// D for dataBase
typedef struct D{
    TN t_node;
    // enable 属于数组使用
    char enable, name[init_name_len];
}D;
int CREATE_D(const char dn[]);  int DROP_D  (const char dn[]);
int USE_D   (const char dn[]);  int SHOW_D  (void           );