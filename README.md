# fsnail
An interpreter made in C for a custom programming language
# About
fsnail is a simple, interpreted and stack based programming language. I wanted to make this interpreter to test my skills and to make a project that was not too hard but also not too easy. The choice of a not so complicated project has influenced the syntax of fsnail that took inspiration from Tanenbaum's MAL and tinybasic.
# Installation
1. First compile the file: `gcc -o fsnail fsnail.c -lm`
2. Move it to the bin folder: `sudo mv fsnail /usr/local/bin/`
3. Use it: `fsnail file.fsn`
# Instructions
Here's the list of all the operations:
**Arithmetic and Stack operations**:
- `push value`: Adds an element on top of the stack
- `pop:` Deletes the element on top of the stack
- `dup`: Duplicates the element on top of the stack
- `clear`: Clears the stack content
- `swap`: Swaps the two elements on top
- `sum`: Sums the first two element of the stack
- `sub`: Same as sum but with the subtraction
- `mult`: Same but with multiplication
- `div`: Same but with division
- `rem`: Gets the remainder of the division between the two elements on top (treats the elements integer
- `toint`: Converts the top element to an integer by rounding it down
- `inc name`: Increments the top element value
- `dec name`: Decrements the top element value
  
>The operations are always: `second_element # first_element = result`, where `#` is the generic representation of an operator

**Binary operations:**
- `and`: Actuate the AND operation between the top two elements
- `or`: Same as or but with OR
- `not`: Same but with NOT
- `xor`: Same but with XOR
- `lshift`: Single left shift
- `rshift`: Single right shift

>IMPORTANT: the NOT and the two shift operations only affects the element on top of the stack and with this functions all the elements are treated as integer

**Logical operations:**
- `ifeq`: Checks if the top element is equal to the second one
- `ifdif`: Checks if the top element is different to the second one
- `ifgr`: Checks if the top element is greater than the second one
- `iflw`: Checks if the top element is lower than the second one
- `iftrue`: Checks if the first element is equal to one
- `iffalse`: Checks if the first element is equal to zero
- `endif`: Ends the series of if instructions

>If the one of these operations results as true, the program will execute the code contained between if and endif. Nested if are also supported

**Jumps and labels:**
- `label` name: Creates a new label
- `goto name`: Jumps to the given label

**Input and output:**
- `print "string"`: Prints the string
- `printnl "string"`: Prints a string and goes to a new line
- `in`: Gets the value as an integer, the value is stored in a new element on top of the stack
- `inchar`: Gets the value a char, the value is stored in a new element on top of the stack 
- `out`: Prints the element on top of the stack 
- `outint`: Prints the element on top of the stack as an integer 
- `outchar`: Prints the element on top of the stack as a characteracter (treats the element as an integer) 
- `sclear`: Clears the screen

**Variables:**
- `var name`: Creates a variable
- `del name`: Deletes a variable
- `store name`: Saves the content on the stack top into the variable
- `pstore name:` Saves the content on the stack top into the variable and pops it
- `load name`: Loads the content of the variable on top of the stack
- `vclear`: Clears variable stack content

**Comments and extra instructions:**
- `-->`: Starts the the comment
- `<--`: Ends the comment
- `stack`: Prints out the current stack 
- `halt`: Terminates the program 
- `randint value`: Generates a number 0 < x <= value and pushes it on top of the stack

>The comment delimiters need to be separated to the comment's content like any other token

**Advanced Math:**
- `abs`: Returns the absolute value of the element on top of the stack
- `pow`: Elevates the second to last number to the power of the top element (it acts exactly as the arithmetical operators)
- `ln`: Calculates the natural logarithm of the top element
- `log`: Calculates the common (base 10) logarithm of the top element
- `logtwo`: Calculates the base 2 logarithm of the top element
- `floor`: Rounds up the top element (toint can be used as ceil)
- `sqrt`: Calculates the square root of the top element
- `sin`: Calculates the sine of the top element (result in radians)
- `cos`: Calculates the cosine of the top element (result in radians)
- `tan`: Calculates the tangent of the top element (result in radians)
# Debugging
The interpreter comes with some debugging features, it checks if an `if` and misses its `endif` and viceversa.  It also applies checks to the types of data (invalid string, invalid number), to the stack (the stack is empty, the stack is composed of less than two elements), to the variable section (the variable doesn't exist), if a token is invalid or if a file exists and its extension is correct.
# Code examples
## Trapezoid area
```
var lbase
var gbase
var height

printnl "Calculating the area of a trapezoid"
printnl "Insert the smaller base:"
in
pstore lbase

printnl "Insert the bigger base:"
in
pstore gbase

printnl "Insert the height"
in
pstore height

load lbase
load gbase
sum

load height
mult

push 2
div

print "The area is: "
out
printnl ""
```
## Infinite loop
```
label while
    printnl "Insert a number:"
    in
    printnl "Insert a number:"
    in

    ifeq
        printnl "The numbers are equal"
        goto while
    endif

    printnl "The numbers are different"

goto while

label and
    printnl ""
```
## For cycle 1
```
printnl "Insert a number:" --> Asks for the limit number <--
in
toint
dec --> Necessary to avoid an extra iteration <--

push 0 --> Acts as a counter <--

label for
    outint
    print ", "

    ifeq
        goto end
    endif

    inc
    goto for


label end
    printnl ""
```
## For cycle 2
```
printnl "Insert a number:" --> Asks for the limit number <--
in
toint

push 0 --> Acts as a counter <--

label for
    ifdif
        outint
        print ", "
        inc
        goto for
    endif
printnl ""
```
## Second grade equation solver
```
var a
var b
var c
var squareroot

printnl "Second grade equation solver. Its type must be: ax^2+by+c = 0"

printnl "Insert a:"
in
pstore a

printnl "Insert b:"
in
pstore b

printnl "Insert c:"
in
pstore c

--> Now we'll apply the standard formula for resolving second grade equations <--

load b --> Calculates b^2 <--
dup
mult

push -4 --> Calculates -4ac <--
load a mult
load c mult
sum

--> Now we have the delta on the top of the stack <--

push 0

iflw
    goto invalid
endif

pop
sqrt

store squareroot

load b --> Calculates -b <--
push -1 mult
sum

load a --> Adds 2a on top of the stack and calculates x1 <--
push 2 mult
div

print "x1 = "
out printnl ""

load b --> Calculates the second solution <--
push -1 mult
load squareroot sub
load a
push 2 mult
div

print "x2 = "
out printnl ""

halt

label invalid
    printnl "The equation has no solution"
```
