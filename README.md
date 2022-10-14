# EasyScript Documentation

Welcome to the EasyScript programming language documentation.
Glad to see you have chosen this brand new and EASY language with absolute simple syntax and without unneesary and annoying errors.
You can declare variables without declaring any type and do arithmetic with them without any necessity of matching their types.
You can print the variable simply by typing it's name without any 'print' keyword. Anyway if you like the 'print' keyword or have
always had used to it in the experience of other programming languages you can still apply it here.
So here we go.


//Tokens
You must seperate any token with spaces or new line characters otherwise they won't be recognized by the EasyScript interpretator.
a+b is not valid. Instead write a + b. This is done for code readability.



//Token types
Like any other programming languages you write a number with '0-9' symbols, a '.'(dot) for real numbers and a '-'(minus) sign
for negative numbers.

To write a string you open the token with <'> or <"> symbols and close it. Everything between them will be the string characters irrespective to their similarity with token types.

You can start with or use any symbol in the variable name untill the name is not consisted of an only symbol that is an operator symbol.

Operator symbols are: = + - * / ;



//Declaring variables
To declare a variable you just need to type a valid variable name. Once you do it the variable is automatically declared.
The default type of the variable is 'undefined'. When you assign it a value the variable type is automatically set
depending on the assigned value type or the returned value type resolved by the expression.
Assigned value type can be any type: string, number or a variable itself.

age = 18 // variable type set to number
name = "John" // variable type set to string
surname = "Doe"
fullname = name + surname // variable type set to string (the type of the name and surname)
fullname = name + midname // variable type set to undefined. 

The variable 'midname' actually exists because once you type a valid name it is automatically declared
but as it's type is yet undefined (default) it is passed to the 'fullname'



//Assignment
You can assign a value only to a variable so the token type before "=" operator must be an identifier otherwise you will get error
5 = 6 // Error: Can't assign value to a constant literal
'string' = 'str' // Error: Can't assign value to a constant literal

You can also organize a sequential assignment. Like // a = b = c = 5



//Statement section
In EasyScript the code is divided into statments. The statement is terminated with semicolon ";". You can implement several assignments, arithmetic and print operations within a single statement but you must consider that a declared variable is not
accessible after its declaration unless you set the termination symbol.
But the interesting fact here is that you can access the declared variable before* it's declaration but not earlier then the statement start point. The operations inside a statement are organized backwise.



//Mathematical operations

//Addition
In EasyScript you can do mathematical operations with all any operands regardless of their type. Any relationship of a particular type pair has its meaning.
For example, you may wonder why should we need an expression such as ( 5 - "my string" ) and what should be its result. So here we go.

If we do the addition of a string to a number, like

( "my sting " + 123 )

it converts the number to a string and then concatenates them. In general, an expression containing a string operand converts the other number operands to string except for one case. When we do chain arithmetic with strings and numbers such as 

( 4 + 5 + “my string” + 6 + 7 )

the expression returns

( 45mystring13 ).

That might seem a little odd but it has its meaning.

As a statement starts its operation backwise, it takes its first pair of operands from the end: 6 and 7 and returns the result 13, and then adds the string to 13. After the first appearance of a string from the end, the interpreter starts treating any occurring number as a string, and then it concatenates 5 and 4 to the string.

//Subtraction
In EasyScript you can do some operations with simple arithmetic that you would need some libraries in any other programming languages.

"My string" - 3

this expression subtracts 3 characters from the end of the script. The result is: "My str"

3 - "My string"

this expression subtracts 3 characters from the start of the string. The result is: "string"

//multiplication and division

Respectively 

3 * "String " returns:
"String String String"

"String" / 2 returns:
"Str" (Divides the string's length)

Only

3 / "string" has no logic so it converts the string to a namber with value of the string's length (3 / 6 = 0.5)

Also you can do assignments simulteniously while doing arithetic:

born_year = 1995 ;
this_year = born_year + age = 35 ;

The "age" is assigned 35 and then added to "born_year" and finally the result assigned to "this_year"



// Boolean operations
EasyScript supports the following logic operators ">, <, ==, !=, >=, <=, &&, ||, ===, !=="
The last two are strict equality operator.

12 == "12" returns 1
12 === "12" returns 0  ( types must match too )



//Print

Finally the most beautiful part of this language is the printing process. You don't have to type any keyword or function to print a value.
You just type the expression.
age = 25 ;
this_year = 2022 ;
"You were born in" + this_year - age ;

Notice that the third line has no assignment, so that meens it must be printed. Any expression that is not assigned to a variable is printed.
Also if you want you can assign the result of the expression to a variable and the print it:

age = 25 ;
print = "I am " + age + "year old." ;
print ;
