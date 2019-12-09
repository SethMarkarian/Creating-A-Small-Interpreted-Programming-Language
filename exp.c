//I tried to separate the symbol table into another file and it kept on giving me errors I have never seen before. 
//So instead, I 

#include "run.h"
#include "exp.h"
//#include "st.c"

//Declares global variables storing current expression and the current string of characters
char* currC;
char* currName;
Exp* currExp;
void* root;
variable* currS;

//This method allocates space for an Expression, used in createExpression class.
Exp* initExp1() {
	Exp* exp;
	exp = (Exp*)malloc(sizeof(struct Exp));
	return exp;
}

//This method allocates space for an Expresions first, used in createExpression class.
Exp* initExp2() {
	Exp* exp;
	exp->first = (Exp*)malloc(sizeof(Exp));
	return exp;
}

//This function gets called from menu function, creates a char* whihc temporarily stores
//the expression entered, creates an expression in the createExpression class,
//and sets the global variables equal to their respective char* and Exp*.
void c() {
	char* c;
	c = (char*)malloc(100 * sizeof(char));
	printf("\nPlease enter an expression: ");
	scanf(" %[^\n]", &c[0]);
	
	if(findVariable("foo") != NULL) {
		removeVariable("foo");
	}
	currC = c;
	Exp* exp;
	if(*c == '(') c++;
	variable* var = initVariable();
	var->name = "foo";
	currName = "foo";
	var->type = 0;
	var->expString = c;
	if(root == NULL) {
		root = var;
		currS = var;
	}
	else {
		currS->next = var;
		currS = var;
	}
	
	currExp = createExpression(exp, c);
	var->e = currExp;

	menu();
}

//Fairly complicated method, in short, creates the exp recursively, comments are provided throughout
//the method for clarification.
Exp* createExpression(Exp* exp, char* c) {
	//If null, we are at the end of the string, done!
	if(*c == '\0') return exp;

	//If the current char is a close parenthesis, we have finished whatever subset of
	//the expression we are in, so increment c and return current expression.
	if(*c == ')') {
		c++;
		return exp;
	}

	//If the current char is a space, we will increment c and set the current expression eual to the next
	//actual character.
	if(*c == ' ') {
		c++;
		exp = createExpression(exp, c);

	//It either must be a +/-/*/( or a number one through nine, so allocate space for expression.
	} 
	else if(*c == 'f') { //recognizes it's a subset
		int count = 0;
		char* input = (char*)malloc(100*sizeof(char*));
		while(*c == 'r' || *c == 'f' || *c == '(' || *c == ' ') { // generates an input for subset method
			if(*c == 'r' || *c == 'f') {
				input[count] = *c;
				count++;
				input[count] = ' ';
				count++;
			}
			c++;
		}
		input[count - 1] = '\0';

		char* correctInput = (char*)malloc(100*sizeof(char*)); 
		for(int i = 0; i < strlen(input); i++) { //adjusts for proper input
    		correctInput[i] = input[strlen(input) - 1 - i];
  		}

		char* nameVariable = (char*)malloc(100*sizeof(char*));
		int i = 0;
		while(*c != ')') { //finds variable name
			nameVariable[i] = *c;
			c++;
			i++;
		}
		nameVariable[i] = '\0';
		variable* v = findVariable(nameVariable); //finds variable
		Exp* expT = v->e; //gets expression
		char* tempExp = subset(expT, correctInput); //runs subset on it
		Exp* e;
		exp = createExpression(e, tempExp); //creates new expression

	}
	else if(*c == 'a') { //append function
		c++;
		c++;
		char* v1 = (char*)malloc(100*sizeof(char*)); //first variable
		char* v2 = (char*)malloc(100*sizeof(char*));//second variable
		int i = 0;
		while(*c != ' ' && *c != ')') { //finds name of first variable
			v1[i] = *c;
			c++;
			i++;
		}
		c++;

		i = 0;
		while(*c != ' ' && *c != ')') { //finds name of second variable
			v2[i] = *c;
			c++;
			i++;
		}
		variable* var1 = findVariable(v1); //first variable
		variable* var2 = findVariable(v2); //second variable
		char* appendExp = append(var1->expString, var2->expString); //adds v2 to the end of v1
		Exp* e;
		exp = createExpression(e, appendExp); //creates new expression	
	}
	else {
		exp = initExp1();

		//If it is an open parenthesis, we are starting a subgroup of the expression, so run create expression
		//recursively on exp->first to create the sub expression.
		if(*c == '(') {
			c++;
			exp->first = createExpression(exp->first, c);

			//After sub expresion on exp->first, we need to update *c becuase it does not update while
			//running through the sub expresion. Use a counter starting at one, and while the current character
			//is not a close parenthesis, keep incrementing c. Everytime we ecounter an open parenthesis, increment the counter.
			//This will ensure the we advance c the correct number of spaces to catch up.
			int cnt = 1;
			while(*c != ')') {
				if(*c == '(') {
					cnt++;
				}
				c++;
			}
			
			c += cnt;
			//Now recursively run createExpression on the rest of the exp, and return the final product when done.
			exp->rest = createExpression(exp->rest, c);
			return exp;
		}
		
		//Increment c, allocate space for exp->first and exp->first->symbol
		c++;
		exp->first = (Exp*)malloc(sizeof(Exp));
		exp->first->symbol = (char*)malloc(sizeof(char));

		//Create two variables to help determine how long the exp's symbol is
		int a = 0;
		int cnt = 1;
		while(a == 0) {
			//If it is a space or close parenthesis or NULL, you are at the end of a word, so stop.
			if(*c == ' ' || *c == ')' || *c == '\0') {
				a++;
			//Otherwise, increment counter and c and keep going.
			} else {
				++cnt;
				c++;
			}
			//When a is no longer zero, return c to its previous spot, copy c for cnt number of characters,
			//and save it in exp->first->symbol
			if(a != 0) {
				c -= cnt;
				strncpy(exp->first->symbol, c, cnt);
			}
		}

		//If current character is close parenthesis, stop becuase the subset of the expressio in done,
		//otherwise, run recusively with exp->rest.
		c += cnt;
		if(*c != ')') {
			exp->rest = createExpression(exp->rest, c);
		}
	}
	//RETURN:)
	return exp;
}



//Evaluates current expression
void e() {
	//Creates temp char*, and runs the evaluate helper method
	char* n;
	printf("Name: ");
	scanf(" %[^\n]", &n[0]);
	
	variable* temp = findVariable(n); //retrieves variable from symbol table
	Exp* expression = initExp1(); //allocates it
	expression = temp->e; //sets it

	char* c;
	c = (char*)malloc(sizeof(char));
	int final = evaluate(c, expression, 0);	
	printf("Total: ");
	printf("%d", final);
	printf("\n");
	menu();
}

//Evaluates expression recursively, comment provided throughout function for clarification
int evaluate(char* c, Exp* exp, int total) {
	//int a is basically serving as a bool, while its 0 keep running the loop
	int a = 0;
	//set c to the first symbol and move to the next piece of the expression
	c = exp->first->symbol;
	exp = exp->rest;
	//If current symbol is +, run this:
	char* plus = "+";
	char* minus = "-";
	char* multiplication = "*";
	if(strcmp(c, plus) == 0) {
		while(a == 0) {
			//If there is a subset (aka exp->first->first != NULL), add the subsets
			//total to the current total by recursively calling recursively.
			if(exp->first->first != NULL) {
				total += evaluate(c, exp->first, 0);
				//Once finished, if rest isn't NULL, evaluate the rest, otherwise return total.
				if(exp->rest != NULL) exp = exp->rest;
				else return total;
			//If first's symbol isn't NULL, set c equal to current symbol, convert the char to an int
			//and add it to the total.
			} else if(exp->first->symbol != NULL) {
				c = exp->first->symbol;
				int i = 0;
				if(isdigit(*c) == 0) {
					variable* temp = initVariable();
					temp = findVariable(c);
					char* ch = malloc(2*sizeof(char));
					Exp* expression = initExp1();
					expression = temp->e;
					total += evaluate(ch, expression, 0);
				}
				else {
					i = atoi(c);
					total = add(total, i);
				}
				
				//If exp->rest isn't NULL, keep going!
				if(exp->rest != NULL) exp = exp->rest;
				else a++;
			//If exp->first isn't NULL but the rest is, return total, otherwise evaluate exp->first
			} else if(exp->first != NULL) {
				if(exp->rest == NULL) return total;
				total += evaluate(c, exp->first, 0);
			} else {
				//If nothing above if true, you're done
				a++;
			}
		}
	//If symbol is -:
	} else if (strcmp(c, minus) == 0) {
		//Set c to current first's symbol, set a temporary total, convert current char to int and
		//subtract it from ttotal
		c = exp->first->symbol;
		int ttotal = 0;
		int i = 0;
		if(isdigit(*c) == 0) { //if a variable is found
			variable* temp = initVariable();
			temp = findVariable(c);
			char* ch = malloc(2*sizeof(char));
			Exp* expression = initExp1();
			expression = temp->e;
			i = evaluate(ch, expression, 0);
		}
		else {
			i = atoi(c);
		}
		ttotal = sub(ttotal, i);
		//if rest isn't NULL, keep going 
		if(exp->rest != NULL) exp = exp->rest;
		while(a == 0) {
			//Same as addition, is theres  subset, find its total
			if(exp->first->first != NULL) {
				total -= evaluate(c, exp->first, 0);
				//If rest isn't NULL keep evaluating
				if(exp->rest != NULL) exp = exp->rest;
				else return total;
			//If first has a symbol, set c to that, convert it to integer, add it to ttotal.
			} else if(exp->first->symbol != NULL) {
				c = exp->first->symbol;
				int i = 0;
				if(isdigit(*c) == 0) { //if a variable is found
					variable* temp = initVariable();
					temp = findVariable(c);
					char* ch = malloc(2*sizeof(char));
					Exp* expression = initExp1();
					expression = temp->e;
					i = evaluate(ch, expression, 0);
				}
				else {
					i = atoi(c);
				}
				ttotal = add(ttotal, i);
				//If the rest still isn't NULL, keep going!
				if(exp->rest != NULL) exp = exp->rest;
				else a++;
			//If first isn't NULL, and rest is, stop. Otherwise, run evaluate on the subset.
			} else if (exp->first != NULL) {
				if(exp->rest == NULL) return total;
				total -= evaluate(c, exp->first, 0);
			} else {
				a++;
			}
		}
		//My method of subtracting first number and adding the subsequent numbers require you to
		//multiply total by negative one before adding it to total.
		ttotal *= -1;
		total += ttotal;
	//If symbol is *: ALMOST IDENTICAL TO ADDITION
	} else if (strcmp(c, multiplication) == 0) {
		//Set total to one so multiplication works.
		total = 1;
		while(a == 0) {
			//If there is subset, evaluate it
			if(exp->first->first != NULL) {
				total *= evaluate(c, exp->first, 0);
				//If rest isn't NULL, run on rest
				if(exp->rest != NULL) exp = exp->rest;
				else return total;
			//If first has a symbol, set c to that, convert it to an int, and multiply total
			//by that
			} else if(exp->first->symbol != NULL) {
				c = exp->first->symbol;
				int i = 0;
				if(isdigit(*c) == 0) { //if a variable is found
					variable* temp = initVariable();
					temp = findVariable(c);
					char* ch = malloc(2*sizeof(char));
					Exp* expression = initExp1();
					expression = temp->e;
					i = evaluate(ch, expression, 0);
				}
				else {
					i = atoi(c);
				}
				total = mult(total, i);
				//If there is a rest, go to it to keep evaluating.
				if(exp->rest != NULL) exp = exp->rest;
				else a++;
			//If first isn't NULL, nd rest is, retrun, otherwise evaluate the first's symbols.
			} else if (exp->first != NULL) {
				if(exp->rest == NULL) return total;
				total *= evaluate(c, exp->first, 0);
			} else {
				a++;
			}
		}
	//If none of the above, like just '2', rteurn that number
	} 
	
	else if(isdigit(*c) == 0) { //if there is a parameter
		variable* v = findFunction(c);
		Exp* current = exp;
		int count = 0;
		char paramArray[10][20];
		int w = getWords(v->parameters, paramArray);

		while(current->first != NULL && count != v->numParameters) {
			
			if(findVariable(&paramArray[count][0]) != NULL) { //checks for duplicates
				removeVariable(&paramArray[count][0]);
			}

			Exp* temp = current->first;

			//creates new variable for each parameter
			variable* var = initVariable();
			
			var->name = &paramArray[count][0];
			currName = &paramArray[count][0];
			var->type = 0;
			var->expString = temp->symbol;
			Exp* t;
			currExp = createExpression(t, temp->symbol);
			
			currS->next = var;
			currS = var;		

			var->e = currExp;

			count++;
			
			//if there is another after, continue
			if(current->rest != NULL) { 
				current = current->rest;
			}
		}

		//finds function in symbol table
		variable* v2 = findFunction(c);
		char* ch = malloc(2*sizeof(char));
		Exp* expression = initExp1();
		expression = v2->e;
		total += evaluate(ch, expression, 0);
	}
	else {
		return (*c - '0');
	}
	return total;
}

// helper method for expression
//turns char* into char** seperated by spaces
int getWords(char *base, char target[10][20])
{
	int n = 0;
	int j = 0;
	int i;
	
	for( i= 0; true; i++)
	{
		if(base[i] != ' '){
			target[n][j++]=base[i];
		}
		else{
			target[n][j++] = '\0';//insert NULL
			n++;
			j=0;
		}
		if(base[i] == '\0')
		    break;
	}
	return n;
}

//Helper function to add
int add(int total, int num) {
	return total += num;
}

//Helper function to subtract
int sub(int total, int num) {
	return total -= num;
}

//Helper function to multiply
int mult(int total, int num) {
	return total *= num;
}

//Append function creates a temporary char, asks the user for an input
void a() {
	char* c;
	char* d;
	char* expression;
	c = (char*)malloc(100 * sizeof(char));
	d = (char*)malloc(100 * sizeof(char));
	expression = (char*)malloc(100 * sizeof(char));

	printf("\nPlease insert variable name: ");
	scanf(" %[^\n]", &d[0]);
	printf("\nPlease enter what you would like to append to the current expression: ");
	scanf(" %[^\n]", &c[0]);
	
	variable* temp = findVariable(d);
	expression = temp->expString;
	//Calls append helper function and adds appendage to current global char* variable
	expression = append(expression, c);
	Exp* exp;
	int a = 0;
	if(*expression == '(') {
		expression++;
		a = 1;
	}
	temp->e = createExpression(exp, expression);
	temp->expString = expression;
	if(a = 1) expression--;
	menu();
}

char* append(char* currC, char* c) {
	//Creates three counters, o for open parenthesis, c for close, and a third to increment them
	int ocnt = 1;
	int ccnt = 0;
	int cnt = 1;
	//While the currC is not NULL
	while(*currC != '\0') {
		//increment current, compare ocnt and ccnt, if they're the same, you have reached
		//the point in the char* in which you need to insert.
		currC++;
		if(ocnt == ccnt) {
			//Create a temporary char value, decrement currC by the cnt to restore it to the beginning
			char* new;
			new = (char*)malloc(100 * sizeof(char));
			currC -= cnt;
			//Copy from beginning of currC up to point where appendage will be inserted (--c)
			strncpy(new, currC, --cnt);
			//Add a space in between
			strcat(new, " ");
			//Cancatenate the new char* into currC
			strcat(new, c);
			//restore currC to where it wa using cnt
			currC += (cnt);
			//Cancatenate the rest of the currC value
			strcat(new, currC);
			return new;
		}
		//If open parenthesis, increment its counter
		if(*currC == '(') {
			ocnt++;
			//currC++;
			cnt++;
		//If closed, increment its counter
		} else if (*currC == ')') {
			ccnt++;
			//currC++;
			cnt++;
		//Otherwise neither, just increment the regular counter
		} else {
			//currC++;
			cnt++;
		}
	}	
}

//s creates a temporary char*, allocates space, and asks the user to input a series of
//f's and r's to find the subset they'd like.
void s() {
	char* s;
	s = (char*)malloc(100 * sizeof(char));
	printf("\nPlease enter the subset you would like to see (in terms of 'f' and 'r'): ");
	scanf(" %[^\n]", &s[0]);
	//Prints subset by running helper function
	printf("\nSubset of expression: %s", subset(currExp, s));
	menu();
}

//Subset finds specified subset for user, comments provided througout the method
char* subset(Exp* exp, char* s) {
	//If NULL, you've reached the specified subset, print what it is
	if(*s == '\0') {
		//If exp->symbol is null, you're printing out more than one symbol, so cretae a temporary char*
		//and call a helper method for the hlper method lol.
		if(exp->symbol == NULL) {
			char* new;
			new = (char*)malloc(100 * sizeof(char));
			//Start with an open parethesis to match style
			strcat(new, "(");
			//createSub is helper method
			return createSub(exp, new);
		//If symbol isn't NULL, just return that symbol
		} else {
			return exp->symbol;
		}
	//If we're at a space, increment s and run again
	} else if(*s == ' ') {
		s++;
		subset(exp, s);
	//If it's an f, go to exp->first and run again
	} else if(*s == 'f') {
		if(exp->first == NULL) {
			printf("\nERROR: Subset not found.");
			return NULL;
		}
		exp = exp->first;
		s++;
		subset(exp, s);
	//If it's an r, go to exp->rest and run again
	} else if(*s == 'r') {
		if(exp->rest == NULL) {
			printf("\nERROR: Subset not found.");
			return NULL;
		}
		exp = exp->rest;
		s++;
		subset(exp, s);
	}
}

//Helper to helper, just used when you're printing out more than one symbol
char* createSub(Exp* exp, char* new) {
	//A serves as a true/false
	int a = 0;
	while(a == 0) {
		//If first symbol is NULL, we're in a subset, so add an open parenthesis
		//and recursively call on exp->first
		if(exp->first->symbol == NULL) {
			strcat(new, "(");
			createSub(exp->first, new);
			//Afterwards, if there is more to add to char* new, add a space and run the loop agin
			if(exp->rest != NULL) {
				strcat(new, " ");
				exp = exp->rest;
			}
			//If nothing else to add, drop a close parenthesis and return
			else {
				strcat(new, ")");
				return new;
			}
		}
		//If the first symbol isn't NULL, add it to new
		if(exp->first->symbol != NULL) {
			strcat(new, exp->first->symbol);
			//If there are more symbols to add, add them!
			if(exp->rest != NULL) exp = exp->rest;
			//Otherwise we're done, add a close parenth
			else {
				a = 1;
				strcat(new, ")");
			}
			//If we aren't done, dd a space.
			if (a != 1) strcat(new, " ");
		}
	}
	return new;
}

//allocates space for a new variable
variable* initVariable() {
	variable* v = malloc(sizeof(variable));
	return v;
}

//finds variable in symbol table
variable* findVariable(char* c) {
	variable* current = root; 
    while(current != NULL) { 
        if(strcmp(current->name, c) == 0) {
            return current; 
		}
        current = current->next; 
    } 
    return NULL; 
}

//finds function in symbol table
variable* findFunction(char* c) {
	variable* current = root; 
    while(current != NULL) { 
        if(strcmp(current->name, c) == 0 && current->type == 2) {
            return current; 
		}
        current = current->next; 
    } 
    return NULL; 
}

// removes variable from symbol table and fixes it accordingly
bool removeVariable(char* c) {
	variable* current = root; 
	variable* previous;
	if(current != NULL && current->name == c) {
		root = current->next;
		free(current);
		return true;
	}
    while(current != NULL && strcmp(current->name, c) != 0) { 
        previous = current;
		current = current->next;
    } 

	if(current == NULL) return false;
	previous->next = current->next;
	free(current);
    return true;
}

// ’v’: define a variable. The next line contains the variable and the line after that contains the
// expression. A new data structure is created to contain the expression.
void v() {
	char* v;
	char* c;
	v = (char*)malloc(100 * sizeof(char));
	c = (char*)malloc(100 * sizeof(char));
	printf("\nName: ");
	scanf(" %[^\n]", &v[0]);
	printf("\nPlease input an expression: ");
	scanf(" %[^\n]", &c[0]);
	
	if(findVariable(v) != NULL) {
		removeVariable(v);
	}
	currC = c;
	Exp* exp;
	if(*c == '(') c++;
	variable* var = initVariable();
	var->name = v;
	currName = v;
	var->type = 0;
	var->expString = c;
	if(root == NULL) {
		root = var;
		currS = var;
	}
	else {
		currS->next = var;
		currS = var;
	}
	
	currExp = createExpression(exp, c);
	var->e = currExp;
	menu();
}

// define a function (without parameters). The next line contains the function name and
// the line after that contains the expression. A new data structure is created to contain the
// expression
//SAME AS VARIABLE, user doesn't know that
 void d() {
	 v();
 }

// define a function with parameters. The next line contains the function name, the line
// after that the parameters, and then a line that contains the expression. A new data structure
// is created to contain the expression

//Name of variable cannot start with f, it will get confused with subsets
//also parameters inputs can only be 1 thing, not multible
void l() {
	char* l;
	char* p;
	char* h;
	l = (char*)malloc(100 * sizeof(char));
	p = (char*)malloc(100 * sizeof(char));
	h = (char*)malloc(100 * sizeof(char));
	printf("\nPlease define a function: ");
	scanf(" %[^\n]", &l[0]);
	printf("\nPlease input parameter names: ");
	scanf(" %[^\n]", &p[0]);
	printf("\nPlease input an expression: ");
	scanf(" %[^\n]", &h[0]);

	currC = h;
	currName = l;
	if(*h == '(') h++;
	variable* var = initVariable();
	var->expString = currC;
	var->name = l;
	Exp* exp;
	var->parameters = (char*)malloc(100 * sizeof(char));
	currExp = createExpression(exp, h);
	var->e = currExp;
	var->type = 2;
	if(root == NULL) {
		root = var;
		currS = var;
	}
	else {
		currS->next = var;
		currS = var;
	}

	int count = 0; 
    for (int i = 0; p[i]; i++) {
        if (p[i] != ' ') {
            count++; 
		}	
    }    
	var->parameters = p;
	var->numParameters = count;

	menu();
}

