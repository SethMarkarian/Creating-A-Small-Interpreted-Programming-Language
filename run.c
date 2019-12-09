#include "run.h"
#include "exp.h"
// #include "st.h"

//Main function, declares space for global variables and calls menu function.
int main() {
	currExp = (Exp*)malloc(sizeof(Exp));
	currC = (char*)malloc(100* sizeof(char));
	currName = (char*)malloc(100* sizeof(char));
	menu();
	return 0;
}

//Provides the user a prompt to enter which command they'd like to use,
//or can provide a list of commands to choose from, reports error if command is unrecognized..
void menu() {
	printf("\nPlease enter a command, or type 'h' to see list of available commands: ");
	char ch;
	scanf(" %c", &ch);
	if (ch == 'h') h();
	else if (ch == 'c') c();
	else if (ch == 'p') p();
	else if (ch == 'e') e();
	else if (ch == 'a') a();
	else if (ch == 's') s();
	else if (ch == 'x') x();
	else if (ch == 'v') v();
	else if (ch == 'd') d();
	else if (ch == 'l') l();
	else error();
}

//Basic help menu telling user what inputs they can choose from.
void h() {
	printf("\nType 'c' to create a new expression.");
	printf("\nType 'p' to print the current expression.");
	printf("\nType 'e' to evaluate the current expression.");
	printf("\nType 'a' to append a new expression to the current.");
	printf("\nType 's' to print a subset of the expression.");
	printf("\nType 'v' to define a variable.");
	printf("\nType 'd' to define a function w/o parameters.");
	printf("\nType 'l' to define a function w/ parameters.");
	printf("\nType 'x' to exit the program.");
	printf("\n");
	menu();
}

//Prints the current expressio from the global variable.
void p() {
	char* c = (char*)malloc(100 * sizeof(char));
	printf("\nPlease insert variable name: ");
	scanf(" %[^\n]", &c[0]);
	variable* temp = findVariable(c);
	char* d = (char*)malloc(100 * sizeof(char));
	d = temp->expString;
	d--;
	printf("Expression: ");
	printf("%s \n", d);
	menu();
}

//Frees the memory used for global variables and exits program.
void x() {
	free(currC);
	free(currExp);
	exit;
}

//An unrecognized comman was entered, return user to menu.
void error() {
	printf("\nWhat you entered is not a recognized command, please try again.");
	menu();
}
