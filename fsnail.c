#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

/*
List of operations:

Arithmetic and Stack operations:
    - push value: Adds an element on top of the stack
    - pop: Deletes the element on top of the stack
    - dup: Duplicates the element on top of the stack
    - clear: Clears the stack content
    - swap: Swaps the two elements on top
    - sum: Sums the first two elements of the stack
    - sub: Same as sum but with the subtraction
    - mult: Same but with multiplication
    - div: Same but with division
    - rem: Gets the remainder of the division between the two elements on top (treats the elements as integers)
    - toint: Converts the top element to an integer by rounding it down
    - inc name: Increments the top element value
    - dec name: Decrements the top element value

    The operations are always:
        second_element # first_element = result

    Where # is the generic representation of an operator

Binary operations:
    - and: Performs the AND operation between the top two elements
    - or: Same as or but with OR
    - not: Same but with NOT
    - xor: Same but with XOR
    - lshift: Single left shift
    - rshift: Single right shift

    IMPORTANT: the NOT and the two shift operations only affect the element on top of the stack.
    With thess functions all the elements are treated as integers

Logical operations:
    - ifeq: Checks if the top element is equal to the second one
    - ifdif: Checks if the top element is different from the second one
    - ifgr: Checks if the top element is greater than the second one
    - iflw: Checks if the top element is lower than the second one
    - iftrue: Checks if the first element is equal to one
    - iffalse: Checks if the first element is equal to zero
    - endif: Ends the series of if instructions

    If one of these operations results as true, the program will execute the code contained between if and endif.
    Nested if are supported

Jumps and labels:
    - label name: Creates a new label
    - goto name: Jumps to the given label

Input and output:
    - print "string": Prints the string X
    - printnl "string": Prints a string and goes to a new line
    - in: Gets the value as an integer, the value is stored in a new element on top of the stack
    - inchar: Gets the value as a char, the value is stored in a new element on top of the stack
    - out: Prints the element on top of the stack
    - outint: Prints the element on top of the stack as an integer
    - outchar: Prints the element on top of the stack as a character (treats the element as integers)
    - sclear: Clears the screen X

Variabiles:
    - var name: Creates a variable
    - del name: Deletes a variable
    - store name: Saves the content of the stack top into the variable
    - pstore name: Saves the content of the stack top into the variable and pops it
    - load name: Loads the content of the variable on top of the stack
    - vclear: Clears variable stack content

Comments and extra instructions:
    - -->: Starts the comment
    - <--: Ends the comment
    - stack: Prints out the current stack
    - halt: Terminates the program
    - randint value: Generates a number 0 < x <= value and pushes it on top of the stack

    The comment delimiters need to be separated as individual tokens

Advanced Math:
    - abs: Returns the absolute value of the element on top of the stack
    - pow: Elevates the second to last number to the power of the top element (it acts exactly as the aritmethic operators)
    - ln: Calculates the natural logarithm of the top element
    - log: Calculates the common (base 10) logarithm of the top element
    - logtwo: Calculates the base 2 logarithm of the top element
    - ceil: Rounds up the top element (toint can be used as floor)
    - sqrt: Calculates the square root of the top element
    - sin: Calculates the sine of the top element (result in radians)
    - cos: Calculates the cosine of the top element (result in radians)
    - tan: Calculates the tangent of the top element (result in radians)

*/

//################################# - Arithmetic and stack operations - #################################################

#define D 1024 //Defines the maximun length of a line
#define NAME_SIZE 32

//Creates the structure of the linked list
struct link{
    char name[NAME_SIZE]; //Used to save the variable name
    float value;
    struct link *next;
};

typedef struct link l;
typedef l *node;

typedef struct{
    char string[D];
    int line; //Contains the position of the token in the file
}token;

node new_node(){
    return malloc(sizeof(l));
}

//Serves as a push
void push(node *head, float v, char name[]){
    node temp = *head;
    
    node new = new_node();
    new->value = v;
    strncpy(new->name, name, NAME_SIZE);
    if(temp == NULL){ //If the stack is empty
        new->next = *head; //Adds the new node before the current one
        *head = new; //Updates the head node
        return;
    }

    while(temp->next != NULL) temp = temp->next; //Stops at the last element
    new->next = temp->next; //Inserts the new node before the NULL one
    temp->next = new; //Inserts the new node after the last one, effectively pushing the given value
}

//Acts as a pop
int pop(node *head){
    node temp = *head;

    if(*head == NULL) return 0; //If the stack is already empty, so there's nothing to remove

    if((*head)->next == NULL){ //If there's only one element, it gets deleted and then the head is set to NULL
        node temp = *head;
        *head = temp->next;
        free(temp);
        return 1;
    }

    while(temp->next->next != NULL) temp = temp->next; //Stops at the second to last element
    node n = temp->next; //Saves the last node in the stack
    temp->next = n->next; //Disconnects the last element from the stack
    free(n); //Removes it
    return 1;
}

int dup(node head){
    if(head == NULL) return 0;

    while(head->next != NULL) head = head->next; //Stops at the top element

    //Creates and inserts the duplicated node between the top and the null one
    node n = new_node();
    n->value = head->value;
    n->next = head->next;
    head->next = n;

    return 1;
}

void clear(node *head){
    while(*head != NULL) pop(head);
}

int swap(node *head){
    int swap; //Serves as a support for the value's switch
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return 0; //If the stack is not composed of at least two elements the function returns 0

    while(temp->next->next) temp = temp->next; //Stops at the second to last element

    //Swaps the elements' values
    swap = temp->value;
    temp->value = temp->next->value;
    temp->next->value = swap;

    return 1;
}

int sum(node *head){
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return 0; //If the stack is not composed of at least two elements the function returns 0

    while(temp->next->next) temp = temp->next; //Stops at the second to last element

    temp->value = temp->value + temp->next->value; //Sums the two values

    node n = temp->next; //Store the node to delete
    temp->next = temp->next->next; //Removes the n node
    free(n); //Deletes the node

    return 1;
}

int sub(node *head){
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return 0; 

    while(temp->next->next) temp = temp->next; 

    temp->value = temp->value - temp->next->value;

    node n = temp->next; 
    temp->next = temp->next->next; 
    free(n); 

    return 1;
}

int mult(node *head){
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return 0;

    while(temp->next->next) temp = temp->next;

    temp->value = temp->value * temp->next->value;

    node n = temp->next;
    temp->next = temp->next->next;
    free(n);
    
    return 1;
}

int my_div(node *head){ //Called like this to avoid conflicts with the C function div
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return 0;

    while(temp->next->next) temp = temp->next;

    if(temp->next->value == 0) return 0; //You can't divide a number by 0

    temp->value = temp->value / temp->next->value;

    node n = temp->next;
    temp->next = temp->next->next;
    free(n);

    return 1;
}

int rem(node *head){
    int first, second;
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return 0;

    while(temp->next->next) temp = temp->next;

    if(temp->next->value == 0) return 0; //You can't divide a number by 0

    first = temp->value;
    second = temp->next->value;

    temp->value = first % second;

    node n = temp->next;
    temp->next = temp->next->next;
    free(n);

    return 1;
}

int toint(node head){ //Lowers down the number value
    int intvalue;

    if(head == NULL) return 0;

    while(head->next != NULL) head = head->next;

    intvalue = head->value;
    head->value = intvalue;

    return 1;
}

int inc(node head){
    if(head == NULL) return 0;

    while(head->next != NULL) head = head->next;
    head->value = head->value + 1;

    return 1;
}

int dec(node head){
    if(head == NULL) return 0;

    while(head->next != NULL) head = head->next;
    head->value = head->value - 1;

    return 1;
}

//################################# - end of the section - #################################################




//################################# - binary operations - ##################################################


int and(node *head){
    int first, second;
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return 0; 

    while(temp->next->next) temp = temp->next; 

    first = temp->value;
    second = temp->next->value;

    temp->value = first & second;

    node n = temp->next; 
    temp->next = temp->next->next; 
    free(n); 

    return 1;
}

int or(node *head){
    int first, second;
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return 0; 

    while(temp->next->next) temp = temp->next;

    first = temp->value;
    second = temp->next->value;

    temp->value = first | second;

    node n = temp->next; 
    temp->next = temp->next->next; 
    free(n); 

    return 1;
}

int xor(node *head){
    int first, second;
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return 0; 

    while(temp->next->next) temp = temp->next;

    first = temp->value;
    second = temp->next->value;

    temp->value = first ^ second;

    node n = temp->next; 
    temp->next = temp->next->next; 
    free(n); 

    return 1;
}

int not(node *head){
    int first;
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return 0; 

    while(temp->next) temp = temp->next; //Stops at the last element

    first = temp->value;

    temp->value = ~first;

    return 1;
}

int lshift(node *head){
    int first;
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return 0; 

    while(temp->next) temp = temp->next;

    first = temp->value;

    temp->value = first << 1;

    return 1;
}

int rshift(node *head){
    int first;
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return 0;

    while(temp->next) temp = temp->next;

    first = temp->value;

    temp->value = first >> 1;

    return 1;
}


//################################# - end of the section - #################################################




//################################# - logical operations - #################################################

int next_valid_instruction(token code[], int elements, int current_pos){ //It returns the next valid position in the code (it's used with false ifs)
    int valid = 0;
    int i;

    for(i = current_pos; i < elements; i++){
        
        if(strncmp(code[i].string, "endif", D) == 0){
            if(valid == 1)
                return i;
            else
                valid--;
        }
        
        if(strncmp(code[i].string, "ifeq", D) == 0 || strncmp(code[i].string, "ifdif", D) == 0 || strncmp(code[i].string, "ifgr", D) == 0 || strncmp(code[i].string, "iflw", D) == 0 || strncmp(code[i].string, "iftrue", D) == 0 || strncmp(code[i].string, "iffalse", D) == 0){
            valid++;
        }
    }
    return 0;
}

int if_eq(node *head){
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return -1; /*The functions returns -1 to communicate
                                                           that an error is occuring because the elements on the stack are insufficient*/

    while(temp->next->next) temp = temp->next;

    if(temp->next->value == temp->value) return 1;
    return 0;
}

int if_dif(node *head){
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return -1;

    while(temp->next->next) temp = temp->next;

    if(temp->next->value != temp->value) return 1;
    return 0;
}

int if_gr(node *head){
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return -1;

    while(temp->next->next) temp = temp->next;

    if(temp->value > temp->next->value) return 1;
    return 0;
}

int if_lw(node *head){
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return -1;

    while(temp->next->next) temp = temp->next;

    if(temp->value < temp->next->value) return 1;
    return 0;
}

int if_true(node *head){
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return -1;

    while(temp->next) temp = temp->next;

    if(temp->next->value == 1) return 1;
    return 0;
}

int if_false(node *head){
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return -1;

    while(temp->next) temp = temp->next;

    if(temp->next->value == 0) return 1;
    return 0;
}

//################################# - end of the section - #################################################



//################################# -   I/0 operations   - #################################################

//Removes the " from the string
void prepare_string(char string[], int l, char temp[]){
    int i;

    for(i = 0; i < l-2; i++){
        temp[i] = string[i + 1];
    }
    temp[i] = '\0';
}

int out(node head, int code){
    int c;

    if(head == NULL) return 0; //If the stack is empty the function fails

    while(head->next != NULL) head = head->next;

    switch (code)
    {
    case 0: //Prints as a float
        printf("%.3f", head->value);
        break;
    case 1: //Prints as an integer
        c = head->value;
        printf("%d", c);
        break;
    case 2: //Prints as a char
        c = head->value;
        printf("%c", c);
        break;
    }

    return 1;
}

void in(node *head, int code){
    float input;
    char c, character;
    int valid;
    node temp = *head;

    while(1){//Keeps asking for a value if the given one is not correct

        if(code == 0) //If code is 0, asks for an integer
            valid = scanf("%f", &input);
        else if(code == 1){ //If code is 1, asks for a char

            valid = scanf(" %c", &character); //Typecasting
            input = character;
        }

        if(valid == 1) break; //If the input is valid the loop ends to continue the function
    }
    
    node n = new_node();
    n->value = input;
    

    if(*head == NULL){ //If the stack is empty, the node is inserted berfore the null node
        n->next = *head;
        *head = n;
        return;
    }

    while(temp->next != NULL) temp = temp->next; //Stops at the last non null element
    n->next = temp->next; //Connects the new element to the null one
    temp->next = n; //Connects the new element to the last non null element

    while((c = getchar()) != '\n' && c != EOF); //Clears the input buffer
}

void sclear(){
    printf("\e[1;1H\e[2J");
}


//################################# - end of the section - #################################################



//################################# - Goto secotion - ######################################################

int jump(token code[], int elements, char label[D]){ //Jumps to the given label
    int i;

    for(i = 0; i < elements; i++){ //Returns, if present, the index of the given label
        if(i + 1 < elements && strncmp(code[i].string, "label", D) == 0){
            if(strncmp(code[i + 1].string, label, D) == 0)
                return i+1;
        }

    }

    return -1; //Returns the code for the non existing label
}

//################################# - end of the section - #################################################

//################################# - Variables section - #################################################

int store(node varstack, node stack, char name[]){ //Loads the variable content on top of the stack
    if(varstack == NULL) return 0; //If the variable stack is empty

    while(varstack != NULL){
        if(strncmp(varstack->name, name, NAME_SIZE) == 0){
            while(stack->next != NULL) stack = stack->next;
            varstack->value = stack->value;
            return 1;
        }
        varstack = varstack->next;
    }

    return 0;
}

int load(node varstack, node *stack, char name[]){ //Loads the variable content on top of the stack
    if(varstack == NULL) return 0; //If the variable stack is empty

    while(varstack != NULL){
        if(strncmp(varstack->name, name, NAME_SIZE) == 0){
            push(stack, varstack->value, "");
            return 1;
        }
        varstack = varstack->next;
    }

    return 0;
}

int delete_var(node *varstack, char name[]){ //Deletes the variable with the specified name
    node temp = *varstack;

    if(*varstack == NULL) return 0;

    if(strncmp((*varstack)->name, name, NAME_SIZE) == 0){ //Checks if the variable to delete is the first one
        *varstack = temp->next;
        free(temp);
        return 1;
    }

    while(temp->next != NULL){ //Checks for the presence of the node and deletes it
        if(strncmp(temp->next->name, name, NAME_SIZE) == 0){
            node n = temp->next;
            temp->next = n->next;
            free(n);
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

//################################# - end of the section - #################################################



//################################# - Advanced math section - ##############################################

int my_abs(node *stack){ //I decided to avoid using the library for this simple function
    node temp = *stack;

    if(*stack == NULL) return 0; //Checks if the stack is empty
    while(temp->next != NULL) temp = temp->next;
    
    if(temp->value < 0) temp->value *= (-1);

    return 1;
}

int my_pow(node *head){
    node temp = *head;

    if(*head == NULL || (*head)->next == NULL) return 0; 

    while(temp->next->next) temp = temp->next;

    temp->value = powf(temp->value, temp->next->value);

    node n = temp->next; 
    temp->next = temp->next->next; 
    free(n); 

    return 1;
}

int ln(node *head){
    node temp = *head;

    if(*head == NULL) return 0; 

    while(temp->next) temp = temp->next;

    temp->value = logf(temp->value);

    return 1;
}

int my_log(node *head){
    node temp = *head;

    if(*head == NULL) return 0; 

    while(temp->next) temp = temp->next;

    temp->value = log10f(temp->value);

    return 1;
}

int logtw(node *head){
    node temp = *head;

    if(*head == NULL) return 0; 

    while(temp->next) temp = temp->next;

    temp->value = log2f(temp->value);

    return 1;
}

int my_ceil(node *head){
    node temp = *head;

    if(*head == NULL) return 0; 

    while(temp->next) temp = temp->next;

    temp->value = ceilf(temp->value);

    return 1;
}

int my_sqrt(node *head){
    node temp = *head;

    if(*head == NULL) return 0; 

    while(temp->next) temp = temp->next;

    temp->value = sqrtf(temp->value);

    return 1;
}

int my_sin(node *head){
    node temp = *head;

    if(*head == NULL) return 0; 

    while(temp->next) temp = temp->next;

    temp->value = sinf(temp->value);

    return 1;
}

int my_cos(node *head){
    node temp = *head;

    if(*head == NULL) return 0; 

    while(temp->next) temp = temp->next;

    temp->value = cosf(temp->value);

    return 1;
}

int my_tan(node *head){
    node temp = *head;

    if(*head == NULL) return 0; 

    while(temp->next) temp = temp->next;

    temp->value = tanf(temp->value);

    return 1;
}

//################################# - end of the section - #################################################



//################################# - Random section - #####################################################

void randint(node *head, float n){
    float number;
    int limit = n;

    srand(time(0));
    number = (rand() % limit) + 1;
    push(head, number, "");
}

//################################# - end of the section - #################################################


void printlist(node head){
    if(head == NULL){
        printf("\nEMPTY\n");
        return;
    }

    printf("\n|");
    for(;head != NULL; head = head->next) printf("%.3f|", head->value);
    printf("<-top\n");
}

/*This function is a secure version of fscanf that truncates the string by the length of the given string. This approach serves to avoid buffer overflow*/
int sfscanf(FILE *fp, char string[], int l, int *line, char *carriage){
    char c;
    int is_string = 0;
    int i = 0;

    if(fp == NULL || string == NULL || l <= 0) return EOF;

    if(*carriage == '\n') (*line)++; //Is necessary to avoid missing a \n character

    /*Scrolls the contents of the file until the character c is a valid character. This cycle is important because it skips all the indentation
    and space charaters, leaving only the right characters*/
    while((c = fgetc(fp)) != EOF && isspace(c)){
        if(c == '\n'){
            (*line)++;
        }
    }

    if(c == EOF){ //Covers the case where in the file there's nothing valid. For example the last portion of the file could only consist in space characters
        string[0] = '\0';
        return EOF;
    }

    
    if(c == '\"') is_string = 1;//Avoid the first character because it is the beginnning of a string
    string[i++] = c; //This line of code is necesarry to avoid skipping the first charcter of the string which is a valid one

    while((c = fgetc(fp)) != EOF && i < l - 1){ //Loads the string if the character c or the length are valid
        if(is_string){
            string[i++] = c;
            if(c == '\"') break;
        }
        else{
            if(isspace(c)){
                if(c == '\n'){
                    (*carriage) = '\n';
                    
                }
                break;
            }
            else{
                string[i++] = c;
                (*carriage) = '\0';
            }
        }
        
    }

    string[i] = '\0'; //Terminates the string

    return 1;
}


int real_number(char s[]){ //Checks if the given number is a float
    for(int i = 0; s[i] != '\0'; i++)
        if((isalpha(s[i]) || ispunct(s[i])) && s[i] != '.' && s[i] != '-')
            return 0;
    
    return 1;
}

int initial_debug(token code[], int elements){ //Checks if the if are declared correctly
    int if_stack[elements];
    int endif_stack[elements];

    int if_top = 0;
    for(int i = 0; i < elements; i++){
        if(strncmp(code[i].string, "ifeq", D) == 0 || strncmp(code[i].string, "ifdif", D) == 0 || strncmp(code[i].string, "ifgr", D) == 0 || strncmp(code[i].string, "iflw", D) == 0 || strncmp(code[i].string, "iftrue", D) == 0 || strncmp(code[i].string, "iffalse", D) == 0){
            if_stack[if_top++] = code[i].line;
        }
        if(strncmp(code[i].string, "endif", D) == 0 && if_top >= 0){
            if_top--;
        }
    }
    int endif_top = 0;
    for(int i = elements; i > 0; i--){ //The tokens are scrolled from the end to obtain the same result as for the if cycles
        if(strncmp(code[i].string, "endif", D) == 0){
            endif_stack[endif_top++] = code[i].line;
        }

        if((strncmp(code[i].string, "ifeq", D) == 0 || strncmp(code[i].string, "ifdif", D) == 0 || strncmp(code[i].string, "ifgr", D) == 0 || strncmp(code[i].string, "iflw", D) == 0 || strncmp(code[i].string, "iftrue", D) == 0 || strncmp(code[i].string, "iffalse", D) == 0) && endif_top > -1){
            endif_top--;
        }
    }

    for(int i = 0; i < if_top; i++){
        printf("ERROR 9: The if at line %d is missing its counter part\n", if_stack[i]);
    }

    for(int i = endif_top - 1; i >= 0; i--){ //To print the errors in the correct order it's necessary to navigate the endif_stack backwards
        printf("ERROR 9: The endif at line %d is missing its counter part\n", endif_stack[i]);
    }

    if(if_top < 0 || endif_top < 0) return 0;
    return 1;
}

int valid_extension(char filename[]){ //Checks if the file extension is valid
    char extension[5];

    int len = strlen(filename);
    if(len <= 4) return 0;

    int j = 0;
    for(int i = len - 4; i < len; i++)
        extension[j++] = filename[i];
    extension[j] = '\0';

    if(strncmp(extension, ".fsn", 5) == 0) return 1;

    return 0; 
}

int main(int argc, char *argv[])
{
    FILE *fp;
    char line[D], carriage;
    int elements = 0, pos = 0;

    node stack = NULL; //Creates the head of the stack
    node varstack = NULL; //Creates the head of the varstack

    if(argc != 2){
        printf("ERROR 1: Invalid number of parameters\n");
        return 1;
    }

    if(!valid_extension(argv[1])){
        printf("ERROR 10: The file extension is not valid\n");
        return 10;
    }

    fp = fopen(argv[1], "r");

    if(fp == NULL){
        printf("ERROR 2: The file does not exist\n");
        return 2;
    }


    //This cycle counts the number of tokens that will compose the array of strings
    while(sfscanf(fp, line, D, &pos, &carriage) != EOF)
        elements++;
    pos = 1;

    token code[elements];
    fseek(fp, 0, SEEK_SET); //Restores the original file pointer's position

    
    int comment = 0, i = 0;
    while(sfscanf(fp, line, D, &pos, &carriage) && i < elements){ //Copies all of the tokens inside the array of strings

        if(strncmp(line, "-->", D) == 0) comment = 1;
        if(strncmp(line, "<--", D) == 0){
            comment = 0;
            continue;
        }
        if(!comment){
            if(strncmp(line, "", D) == 0) break;
            strncpy(code[i].string, line, D);
            code[i].line = pos;

            i++;
        }
    }



    strncpy(code[i].string, "halt", D);
    fclose(fp);

    if(!initial_debug(code, elements)) return 9;

    //This cycle contains the actual interpretation of the given code
    for(int i = 0; i < elements; i++){

        if(strncmp(code[i].string, "push", D) == 0){
            if(i + 1 < elements){
                if(real_number(code[i + 1].string)){ /*This check is needed because if the given string can't be turned in a number, the atoi functions returns 0
                                                but the user would like to insert 0, so if the argument of add is not a valid number, the interpreter will
                                                return an error.*/
                    push(&stack, atof(code[i + 1].string), "");
                }
                else{
                    printf("ERROR 3: The argument at line %d is not a number\n", code[i].line);
                    return 3;
                }
            }
            i++;
        }

        else if(strncmp(code[i].string, "pop", D) == 0){
            if(!pop(&stack)){
                printf("ERROR 4: The stack is empty, line %d\n", code[i].line);
                return 4;
            }
        }

        else if(strncmp(code[i].string, "dup", D) == 0){
            if(!dup(stack)){
                printf("ERROR 4: The stack is empty, line %d\n", code[i].line);
                return 4;
            }
        }

        else if(strncmp(code[i].string, "clear", D) == 0)
            clear(&stack);
        
        else if(strncmp(code[i].string, "swap", D) == 0){
            if(!swap(&stack)){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "sum", D) == 0){
            if(!sum(&stack)){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "sub", D) == 0){
            if(!sub(&stack)){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "mult", D) == 0){
            if(!mult(&stack)){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "div", D) == 0){
            if(!my_div(&stack)){
                printf("ERROR 5: Invalid Operation. The stack is either composed of less than 2 elements or the top element has a value of zero, line %d\n", code[i].line);
                return 5;
            }
        }
        
        else if(strncmp(code[i].string, "rem", D) == 0){
            if(!rem(&stack)){
                printf("ERROR 5: Invalid Operation. The stack is either composed of less than 2 elements or the top element has a value of zero, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "toint", D) == 0){
            if(!toint(stack)){
                printf("ERROR 5: Invalid Operation. The stack is either composed of less than 2 elements or the top element has a value of zero, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "inc", D) == 0){
            if(!inc(stack)){
                printf("ERROR 5: Invalid Operation. The stack is either composed of less than 2 elements or the top element has a value of zero, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "dec", D) == 0){
            if(!dec(stack)){
                printf("ERROR 5: Invalid Operation. The stack is either composed of less than 2 elements or the top element has a value of zero, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "and", D) == 0){
            if(!and(&stack)){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "or", D) == 0){
            if(!or(&stack)){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "not", D) == 0){
            if(!not(&stack)){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "xor", D) == 0){
            if(!xor(&stack)){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "lshift", D) == 0){
            if(!lshift(&stack)){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "rshift", D) == 0){
            if(!rshift(&stack)){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "print", D) == 0){
            if(i + 1 < elements){
                int len = strlen(code[i + 1].string);
                char temp[len];
                if(i + 1 < elements && code[i + 1].string[0] == '\"' && code[i + 1].string[len - 1] == '\"'){
                    prepare_string(code[i + 1].string, len, temp);
                    printf("%s", temp);
                }
                else{
                    printf("ERROR 6: The argument at line %d is not a string\n", code[i].line);
                    return 6;
                }
            }
            i++; //Skips to the next instruction
            continue;
        }

        else if(strncmp(code[i].string, "printnl", D) == 0){
            if(i + 1 < elements){
                int len = strlen(code[i + 1].string);
                char temp[len];
                if(i + 1 < elements && code[i + 1].string[0] == '\"' && code[i + 1].string[len - 1] == '\"'){
                    prepare_string(code[i + 1].string, len, temp);
                    printf("%s\n", temp);
                }
                else{
                    printf("ERROR 6: The argument at line %d is not a string\n", code[i].line);
                    return 6;
                }
            }
            i++; //Skips to the next instruction, avoiding the argument
            continue;
        }

        else if(strncmp(code[i].string, "in", D) == 0){
            in(&stack, 0);
        }

        else if(strncmp(code[i].string, "inchar", D) == 0){
            in(&stack, 1);
        }

        else if(strncmp(code[i].string, "out", D) == 0){
            if(!out(stack, 0)){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "outint", D) == 0){
            if(!out(stack, 1)){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "outchar", D) == 0){
            if(!out(stack, 2)){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "sclear", D) == 0){
            sclear();
        }

        else if(strncmp(code[i].string, "ifeq", D) == 0){
            int result = if_eq(&stack);
            
            if(result == -1){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }

            if(result == 0){
                int valid = next_valid_instruction(code, elements, i);
                i = valid;
            }
        }

        else if(strncmp(code[i].string, "ifdif", D) == 0){
            int result = if_dif(&stack);
            
            if(result == -1){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }

            if(result == 0){
                int valid = next_valid_instruction(code, elements, i);
                i = valid;
            }
        }

        else if(strncmp(code[i].string, "ifgr", D) == 0){
            int result = if_gr(&stack);
            
            if(result == -1){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }

            if(result == 0){
                int valid = next_valid_instruction(code, elements, i);
                i = valid;
            }
        }

        else if(strncmp(code[i].string, "iflw", D) == 0){
            int result = if_lw(&stack);
            
            if(result == -1){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }

            if(result == 0){
                int valid = next_valid_instruction(code, elements, i);
                i = valid;
            }
        }

        else if(strncmp(code[i].string, "iftrue", D) == 0){
            int result = if_true(&stack);
            
            if(result == -1){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }

            if(result == 0){
                int valid = next_valid_instruction(code, elements, i);
                i = valid;
            }
        }

        else if(strncmp(code[i].string, "iffalse", D) == 0){
            int result = if_false(&stack);
            
            if(result == -1){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }

            if(result == 0){
                int valid = next_valid_instruction(code, elements, i);
                i = valid;
            }
        }

        else if(strncmp(code[i].string, "endif", D) == 0){
            continue; //Does nothing
        }

        else if(strncmp(code[i].string, "goto", D) == 0){
            int j;

            if((i + 1) >= elements){ //Checks for the existence of the label
                printf("ERROR 6: The label at line %d doesn't exist\n", code[i].line);
                return 6;
            }


            if((j = jump(code, elements, code[i + 1].string)) == -1){
                printf("ERROR 6: The label at line %d doesn't exist\n", code[i].line);
                return 6;
            }
            i = j;
        }

        else if(strncmp(code[i].string, "var", D) == 0){
            if(i + 1 < elements){
                push(&varstack, 0, code[i + 1].string); //Sets the default variable value to 0
                i++;
            }
        }

        else if(strncmp(code[i].string, "store", D) == 0){

            if(i + 1 < elements){
                if(!store(varstack, stack, code[i + 1].string)){
                    printf("ERROR 7: The variable at line %d doesn't exists\n", code[i].line);
                    return 7;
                }
            }
            i++;
        }

        else if(strncmp(code[i].string, "pstore", D) == 0){

            if(i + 1 < elements){
                if(stack == NULL){
                    printf("ERROR 4: The stack is empty, line %d\n", code[i].line);
                    return 4;
                }

                if(!store(varstack, stack, code[i + 1].string)){
                    printf("ERROR 7: The variable at line %d doesn't exists\n", code[i].line);
                    return 7;
                }
            }
            pop(&stack);
            i++;
        }

        else if(strncmp(code[i].string, "load", D) == 0){

            if(i + 1 < elements){
                if(!load(varstack, &stack, code[i + 1].string)){
                    printf("ERROR 7: The variable at line %d doesn't exists\n", code[i].line);
                    return 7;
                }
            }
            i++;
        }

        else if(strncmp(code[i].string, "del", D) == 0){

            if(i + 1 < elements){
                if(!delete_var(&varstack, code[i + 1].string)){
                    printf("ERROR 7: The variable at line %d doesn't exists\n", code[i].line);
                    return 7;
                }
            }
            i++;
        }

        else if(strncmp(code[i].string, "randint", D) == 0){
            if(i + 1 < elements){
                if(real_number(code[i + 1].string)){
                    randint(&stack, atof(code[i + 1].string));
                }
                else{
                    printf("ERROR 3: The argument at line %d is not a number\n", code[i].line);
                    return 3;
                }
            }
            i++;
        }

        else if(strncmp(code[i].string, "vclear", D) == 0)
            clear(&stack);

        else if(strncmp(code[i].string, "abs", D) == 0){
            if(!my_abs(&stack)){
                printf("ERROR 4: The stack is empty, line %d\n", code[i].line);
                return 4;
            }
        }

        else if(strncmp(code[i].string, "pow", D) == 0){
            if(!my_pow(&stack)){
                printf("ERROR 5: The stack is composed of less than 2 elements, line %d\n", code[i].line);
                return 5;
            }
        }

        else if(strncmp(code[i].string, "ln", D) == 0){
            if(!ln(&stack)){
                printf("ERROR 4: The stack is empty, line %d\n", code[i].line);
                return 4;
            }
        }

        else if(strncmp(code[i].string, "log", D) == 0){
            if(!my_log(&stack)){
                printf("ERROR 4: The stack is empty, line %d\n", code[i].line);
                return 4;
            }
        }

        else if(strncmp(code[i].string, "ceil", D) == 0){
            if(!my_ceil(&stack)){
                printf("ERROR 4: The stack is empty, line %d\n", code[i].line);
                return 4;
            }
        }

        else if(strncmp(code[i].string, "logtwo", D) == 0){
            if(!logtw(&stack)){
                printf("ERROR 4: The stack is empty, line %d\n", code[i].line);
                return 4;
            }
        }

        else if(strncmp(code[i].string, "sqrt", D) == 0){
            if(!my_sqrt(&stack)){
                printf("ERROR 4: The stack is empty, line %d\n", code[i].line);
                return 4;
            }
        }

        else if(strncmp(code[i].string, "sin", D) == 0){
            if(!my_sin(&stack)){
                printf("ERROR 4: The stack is empty, line %d\n", code[i].line);
                return 4;
            }
        }

        else if(strncmp(code[i].string, "cos", D) == 0){
            if(!my_cos(&stack)){
                printf("ERROR 4: The stack is empty, line %d\n", code[i].line);
                return 4;
            }
        }

        else if(strncmp(code[i].string, "tan", D) == 0){
            if(!my_tan(&stack)){
                printf("ERROR 4: The stack is empty, line %d\n", code[i].line);
                return 4;
            }
        }

        else if(strncmp(code[i].string, "stack", D) == 0){
            printlist(stack);
        }
        else if(strncmp(code[i].string, "label", D) == 0){
            if((i + 1) > elements){
                printf("ERROR 6: The label at line %d doesn't exist\n", code[i].line);
                return 6;
            }
            i++;
        }

        else if(strncmp(code[i].string, "halt", D) == 0) break;

        else{
            printf("ERROR 8: Unknown token in line %d\n", code[i].line);
            return 8;
        }

    }

    clear(&stack); //Clears the stack
    clear(&varstack); //Same but for the var stack
    free(stack);
    free(varstack);


    return 0;
}