%{
# include "xxx.h"
extern const AN an_template;
char database[64]={0};

int yylex();
int yyparse();

void yyerror (const char *str) { fprintf(stderr, "error: %s\n", str);}

int yywrap() { return 1; }
// main function 
int main() {
	while(1){
printf("SQL>");yyparse()
	}
	
	return	0; // calling parse funtion 
} 

%}
// 我实在是不知道这里怎么去思考他
// 有哪些数据类型是要写的
%union{
	int intval;		char *strval;
	L *logicTree;
	ANN *attr_name_list; TNN *table_name_list;
	LL lop; LV cop; LVT lvt;

}

%token 
CREATE	
DROP	
SHOW	TABLES
USE		
EXIT
	DATABASE	TABLE	
INSERT	INTO	VALUES
SELECT	
UPDATE	SET
DELETE	
		FROM	WHERE
Y_AND		Y_OR		Y_NOT
Y_TYPE_INT Y_TYPE_CHAR
%token <intval> Y_NUM
%token <strval> Y_STR Y_ID 

%left Y_OR
%left Y_AND
%right Y_NOT

%%

/* production for sql grammar */

statement: 
	_CREATE | _SHOW | _DROP | _EXIT | _USE|
	_SELECT | _INSERT| _UPDATE | _DELETE ';' '\n'

_DROP:
// Y_ID:'[a-zA-Z0-9]+'-表名/数据库名-STR变量
	DROP DATABASE Y_ID	{	DROP_D($3);		}|
	DROP TABLE Y_ID	 {	DROP_T($3);		}
_USE:
// Y_ID:'[a-zA-Z0-9]+'-数据库名-STR变量
	USE DATABASE Y_ID  {	USE_D($2);		}
_SHOW:
// Y_ID:'[a-zA-Z0-9]+'-表名/数据库名-STR变量
	SHOW DATABASE  {	SHOW_D();	}|
	SHOW TABLES  {		SHOW_T();	}

ATTR:
// Y_ID:'[a-zA-Z0-9]+'-属性名-STR变量
// Y_NUM:'[1-9][0-9]*'-长度-INT变量
// Y_TYPE_INT:'INT'-STR常量
// Y_TYPE_CHAR:'CHAR'-STR常量
	Y_ID ':' Y_TYPE_INT {
		$$ = malloc(sizeof(AN)); strcpy($$->contain.name, $1);
		*$$ = an_template;
	}|
	Y_ID ':' Y_TYPE_CHAR '(' Y_NUM ')' {
		$$ = malloc(sizeof(AN)); strcpy($$->contain.name, $1);
		$$->contain.type = $$->contain.offset = Y_num;
		$$->next = NULL;
	}
ATTR_LIST:
// ATTR_1,ATTR_2,ATTR_3 :=> 1->2->3
	ATTR {$$ = $1}|
	ATTR ',' ATTR_LIST {
		$1->next = $3;
		$$ = $1;
}
_CREATE:
	CREATE DATABASE Y_id  {					CREATE_D($3);		}|
	CREATE TABLE Y_id '(' ATTR_LIST ')'  {		CREATE_T($3, $5);	}


ATTR_NAME_LIST:
// Y_ID:'[a-zA-Z0-9]+'-属性名-STR变量
// 一般来说, ATTR_NAME 的方向是无关紧要的
// N_1,N_2,N_3 :=> 1->2->3
	'*' {	$$ = NULL;		}|
	Y_ID {
		$$ = malloc(sizeof(ANN));
		$$->next = NULL, strcpy($$->contain.name,$1);
	}|
	Y_ID ',' ATTR_NAME_LIST {
		$$ = malloc(sizeof(ANN));
		$$->next = $3, strcpy($$->contain.name,$1);
}

TABLE_NAME_LIST:
// Y_ID:'[a-zA-Z0-9]+'-属性名-STR变量
// 这里的顺序也是无关紧要的
	'*'{	$$ = NULL;		}|
	Y_ID{
		$$ = malloc(sizeof(ANN));
		$$->next = NULL, strcpy($$->contain.name,$1);
	}|
	Y_ID ',' TABLE_NAME_LIST {
		$$ = malloc(sizeof(ANN));
		$$->next = $3, strcpy($$->contain.name,$1);
}

VALUE_ITEM:
// Y_NUM:'[1-9][0-9]*'-整型值-INT变量
// Y_STR:'\'[a-zA-Z0-9]+\''-字符值-STR变量
	Y_NUM {
		$$ = malloc(sizeof(VN));
		$$->contain.v.i = $1;
		$$->next = NULL;
	}|
	Y_STR {
		$$ = malloc(sizeof(VN));
		strcpy($$->contain.v.s, $1);
		$$->next = NULL;
}
VALUE_LIST:
// 注意插入方向 与创建方向 是不一致的, 这个要修一下, 可以参见 sql.c 的 注释
// 现在为了暂时能用 , 应当是 V1 V2 V3 ::: 3->2->1
	VALUE_LIST ',' VALUE_ITEM {
		$2->next = $1;
		$$ = $1;	
}
// 否则为了好看, 应当是:
	// VALUE_ITEM, VALUE_LIST

_INSERT:
// Y_ID:'[a-zA-Z0-9]+'-表名-STR变量
	INSERT INTO Y_ID VALUES '(' VALUE_LIST ')'{
		INSERT_T($3,$6);
	}

COMP_EXP:
// Y_ID:'[a-zA-Z0-9]+'-属性名-STR变量
// Y_STR:'\'[a-zA-Z0-9]+\''-字符值-STR变量
// Y_NUM:'[1-9][0-9]*'-整型值-INT变量
	Y_ID '=' Y_ID {
		char *str = malloc(2*20*sizeof(char));
		L temp = {lop:LEAF, cop:EQ,l:{VAR,str},r:{VAR,&str[20]}};
		strcpy(str,$1),strcpy(&str[20],$3);
		$$ = malloc(sizeof(L)); *(L *) $$ = temp;
	}|
	Y_ID '>' Y_ID {
		char *str = malloc(2*20*sizeof(char));
		L temp = {lop:LEAF, cop:MORE,l:{VAR,str},r:{VAR,&str[20]}};
		strcpy(str,$1),strcpy(&str[20],$3);
		$$ = malloc(sizeof(L)); *(L *) $$ = temp;
	}|
	Y_ID '<' Y_ID {
		char *str = malloc(2*20*sizeof(char));
		L temp = {lop:LEAF, cop:LESS,l:{VAR,str},r:{VAR,&str[20]}};
		strcpy(str,$1),strcpy(&str[20],$3);
		$$ = malloc(sizeof(L)); *(L *) $$ = temp;
	}|
	Y_ID '>=' Y_ID 
	// 实际上而言, 仅仅提供 > = < 和 AND OR NOT 就够了, 所以演示一个 >= 的转换方法即可
	// 不然这个部分会很庞大
	{
		char *str = malloc(2*20*sizeof(char));strcpy(str,$1),strcpy(&str[20],$3);
		$$ = malloc(sizeof(L)*3);
		L temp[3] = {
			{lop:LEAF, cop:LESS, l:{VAR,str}, r:{VAR,&str[20]}       },
			{lop:LEAF, cop:EQ,   l:{VAR,str}, r:{VAR,&str[20]}       },
			{lop:OR,             l:{p:&(L *)$$[1]}, r:{p:&(L *)$$[2]}}	};
		memcpy($$, temp, 3*sizeof(L));
}
LOGIC_EXP:
// Y_OR : '[(OR)(or)]'
// Y_AND: '[(AND)(and)]'
	COMP_EXP			{	$$ = $1;	}|
	'(' LOGIC_EXP ')'	{	$$ = $1;	}|
	LOGIC_EXP Y_AND LOGIC_EXP {
		$$ = malloc(sizeof(L));
		L temp = {lop:AND, l:{p:$1}, r:{p:$3}};
		*(*L) $$ = temp;
	}|
	LOGIC_EXP Y_OR LOGIC_EXP {
		$$ = malloc(sizeof(L));
		L temp = {lop:OR, l:{p:$1}, r:{p:$3}};
		*(*L) $$ = temp;
}

_SELECT:
	SELECT ATTR_NAME_LIST FROM TABLE_NAME_LIST {
		SELECT_T($4, $2, NULL);
	}|
	SELECT ATTR_NAME_LIST FROM TABLE_NAME_LIST WHERE LOGIC_EXP {
		SELECT_T($4,$2, $6)
}


SET_ITEM:
// Y_ID:'[a-zA-Z0-9]+'-属性名-STR变量
// Y_NUM:'[1-9][0-9]*'-整型值-INT变量
// Y_STR:'\'[a-zA-Z0-9]+\''-字符值-STR变量
	Y_ID '=' Y_NUM{
		struct TEMP{ANN ann;VN vn;};
		$$ = malloc(sizeof(TEMP));
		VN temp_vn = {conatin:{t:INT, v:{i:$3}}, next:NULL};
		strcpy(((struct TEMP *)$$)->ann.name, $1);
		((struct TEMP *)$$)->vn =  temp_vn;
	}|
	Y_ID '=' Y_STR{
		struct TEMP{ANN ann;VN vn;};
		$$ = malloc(sizeof(TEMP));
		VN temp_vn = {conatin:{t:CHAR, v:{s:malloc(sizeof(char)*20)}}, next:NULL};
		strcpy(((struct TEMP *)$$)->ann.name, $1);
		((struct TEMP *)$$)->vn =  temp_vn;
		strcpy(((struct TEMP *)$$)->tn.contain.v.s, $1);
}
SET_LIST:
	SET_ITEM{	$$ = $1;	}|
	SET_ITEM ',' SET_LIST{
		struct TEMP{ANN ann;VN vn;};
		$1 ->ann.next = &($3->ann), $1 ->vn.next = &($3->vn);
		$$ = $1;
}

_UPDATE:
// Y_ID: '[a-zA-Z0-9]+'-表名-STR变量
	UPDATE Y_ID SET SET_LIST WHERE LOGIC_EXP {
		struct TEMP{ANN ann;VN vn;};
		UPDATE($2, &($4->ann), &($4->tn), $6);
	}|
	UPDATE Y_ID SET SET_LIST {
		struct TEMP{ANN ann;VN vn;};
		UPDATE($2, &($4->ann), &($4->tn), NULL);
}

_DELETE:
// Y_ID: '[a-zA-Z0-9]+'-表名-STR变量
	DELETE FROM Y_ID {
		DELETE_T($3, NULL);
	}|
	DELETE FROM Y_ID WHERE LOGIC_EXP {
		DELETE_T($3, $5);
	}

_EXIT:
	EXIT {exit(0);}