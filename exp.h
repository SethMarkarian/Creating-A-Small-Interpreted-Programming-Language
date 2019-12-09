#ifndef EXP
#define EXP

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct Exp Exp;
struct Exp {
	char* symbol;
	Exp* first;
	Exp* rest;
};

typedef struct variable variable; 
struct variable {
	void* next;
	char* name;
	Exp* e;
	int type;
	char* expString;
	char* parameters;
	int numParameters;
};


//GLOBAL variableS AND METHODS
char* currC;
char* currName;
Exp* currExp;
void* root;
variable* currS;


void e();
void c();
Exp* createExpression(Exp*, char*);
Exp* initExp();
int evaluate(char*, Exp*, int);
int add(int, int);
int sub(int, int);
int mult(int, int);
void a();
char* append(char*, char*);
void s();	
char* subset(Exp*, char*);
char* createSub(Exp*, char*);
int getWords(char*, char[10][20]);

variable* initVariable();
variable* findVariable(char*);
variable* findFunction(char*);
bool removeVariable(char*);
void v();
void d();
void l();



#endif
