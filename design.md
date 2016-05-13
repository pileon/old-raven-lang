# Design

This file is used for the design of the language. It contains the
definition of the language, including grammar rules, symbol naming
scheme, etc.

## Grammar

    program
        : statement_list
        ;
        
    statement_list
        : statement opt_separator
        | statement separator statement_list
        ;
        
    opt_separator
        : /* empty */
        | separator
        ;
        
    separator
        : NEWLINE
        | ';'
        ;
        
    statement:
        : declaration_stmt
        | assignment_stmt
        | return_stmt
        | if_stmt
        | loop_stmt
        | jump_stmt
        | block_stmt
        | switch_stmt
        | use_stmt
        ;
    
    declaration_stmt
        : VAR IDENT
        | VAR IDENT '=' expression
        | CONST IDENT '=' expression
        ;
        
    assignment_stmt
        : selection_expr assignment_op expression
        ;
        
    assignment_op
        : '='
        | "+="
        | "-="
        | "*="
        | "/="
        | "%="
        | "|="
        | "&="
        | "^="
        | "<<="
        | ">>="
        ;

    return_stmt
        : RETURN
        | RETURN expression
        ;
        
    if_stmt
        : IF '(' logical_or_expr ')' statement
        | IF '(' logical_or_expr ')' statement ELSE statement
        ;
        
    loop_expr
        : WHILE '(' expression_list ')' statement
        | DO statement WHILE '(' expression_list ')'
        | for_stmt
        ;
        
    for_stmp
        : FOR '(' for_init_expr ';' logical_or_expr ';' for_incr_expr ')' statement
        | FOR '(' range_for_init_expr ':' expression ')' statement
        ;
        
    for_init_expr
        : selection_expr '=' expression
        | VAR IDENT '=' expression
        ;
        
    for_incr_expr
        : expression
        | selection_expr assignment_op expression
        ;
        
    range_for_init_expr
        : selection_expr
        | VAR IDENT
        | CONST IDENT
        ;
        
    jump_stmt
        : BREAK
        | CONTINUE
        ;
        
    block_stmt
        : '{' /* empty */ '}'
        | '{' statement_list '}'
        ;
        
    switch_stmt
        : SWITCH '(' expression ')' '{' switch_body '}'
        ;
        
    switch_body
        : switch_case
        | switch_case separator switch_body
        ;
        
    switch_case
        : cases statement_list
        ;
        
    cases:
        : single_case
        | single_case cases
        ;
        
    single_case
        : CASE expression ':'
        ;
        
    use_stmt
        : USE use_identifier
        ;
        
    use_identifier
        : IDENT
        | IDENT '.' use_identifier
        ;
        
    expression_list
        : expression
        | expression ',' expression_list
        ;
        
    expression:
        : conditional_expr
        ;
        
    conditional_expr
        : logical_or_expr
        | IF '(' logical_or_expr ')' expression ELSE expression
        ;
        
    logical_or_expr
        : logical_and_expr
        | logical_and_expr "||" logical_or_expr
        ;
        
    logical_and_expr
        : logical_not_expr
        | logical_not_expr "&&" logical_and_expr
        ;
        
    logical_not_expr
        : comparision_expr
        | '!' logical_not_expr
        ;
        
    comparison_expr
        : range_expr
        | range_expr comparision_op add_sub_expr
        ;
        
    comparison_op
        : '=='
        | "!="
        | "<="
        | ">="
        | '<'
        | '>'
        | "~="      /* aproximately equal to, usefull for case-insesitive comparisons, or floating point comparisons */
        | "!~="     /* aproximately not equal to */
        ;
        
    range_expr
        : add_sub_expr
        | add_sub_expr "..." add_sub_expr
        ;
        
    add_sub_expr
        : mul_div_expr
        | mul_div_expr '+' add_sub_expr
        | mul_div_expr '-' add_sub_expr
        ;
        
    mul_div_expr
        : bitwise_or_expr
        | bitwise_or_expr '*' mul_div_expr
        | bitwise_or_expr "**" mul_div_expr     /* exponentiation */
        | bitwise_or_expr '/' mul_div_expr
        | bitwise_or_expr '%' mul_div_expr
        ;
        
    bitwise_or_expr
        : bitwise_xor_expr
        | bitwise_xor_expr '|' bitwise_or_expr
        ;
        
    bitwise_xor_expr
        : bitwise_and_expr
        | bitwise_and_expr '^' bitwise_xor_expr
        ;
        
    bitwise_and_expr
        : bitwise_shift_expr
        | bitwise_shift_expr '&' bitwise_and_expr
        ;
        
    bitwise_shift_expr
        : prefix_expr
        | prefix_expr "<<" bitwise_shift_expr
        | prefix_expr ">>" bitwise_shift_expr
        ;
        
    prefix_expr
        : selection_expr
        | '|' prefix_expr
        | '~' prefix_expr
        | '+' prefix_expr
        | '-' prefix_expr
        ;
        
    selection_expr
        : suffix_expr
        | suffix_expr '.' selection_expr
        ;
                
    suffix_expr
        : primary_expr
        | suffix_expr '(' /* empty */ ')'
        | suffix_expr '(' expression_list ')'
        | suffix_expr '[' expression ']'
        ;
        
    primary_expr
        : literal_expr
        | '(' expression ')'
        ;
        
    literal_expr
        : NUMBER
        | STRING
        | function
        | class
        | object
        | list
        | map
        ;
        
    function
        : FUNCTION opt_arguments statement
        | FUNCTION opt_arguments expression
        ;
        
    opt_arguments
        : /* empty */
        | '(' /* empty */ ')'
        | '(' argument_list ')'
        ;
        
    argument_list
        : argument
        | argument ',' argument_list
        ;
        
    argument
        : IDENT
        | CONST IDENT
        ;
        
    class
        : property_visibility CLASS opt_inheritance '{' /* empty */ '}'
        | property_visibility CLASS opt_inheritance '{' property_list '}'
        ;
        
    opt_inheritance
        : /* empty */
        | '(' /* empty */ ')'
        | '(' inheritance_list ')'
        ;
        
    inheritance_list
        : inheritance
        | inheritance ',' inheritance_list
        ;
        
    inheritance
        : IDENT
        | IDENT '.' inheritance
        ;
        
    property_list
        : property opt_separator
        | property separator property_list
        ;
        
    property
        : property_visibility declaration_stmt
        | property_visibility assignment_stmt
        ;
        
    property_visibility
        : /* nothing */
        | PRIVATE
        | PUBLIC
        ;
        
    class
        : property_visibility OBJECT opt_inheritance '{' /* empty */ '}'
        | property_visibility OBJECT opt_inheritance '{' property_list '}'
        ;
        
    list
        : '[' /* empty */ ']'
        | '[' expression_list ']'
        ;
        
    map
        : '{' /* empty */ '}'
        | '{' map_list '}'
        ;
        
    map_list
        : map_element
        | map_element ',' map_list
        ;
        
    map_element
        : expression ':' expression
        ;

## Constant string literals

Constant string literals are enclosed in double quotes (`"`).

Constant string literals might contain embedded expressions. They start
with a percentage (`%`) and the expression is enclosed in curly-braces
(`{}`). Embedded expressions are evaluated lazily, when the constant
string literal is used and not at the time of compilation. Expressions
must be valid or a run-time error will be thrown.

There is no limit on the length of strings, or the number of embedded
expressions.

### Special characters

To include special characters, like newlines or actual double-quote,
special *escape* sequences are used. All start with a back-slash.

Special characters

* Newline `\n`
* carriage return `\r`
* Tab `\t`
* Alert (or bell) `\a`
* Nackspace `\b`
* The escape character `\e`
* Double-quote `\"`
* Literal percentage `\%`

Besides the  special characters listed above, octal, hexadecimal and
Unicode characters are allowed. Octal values begins with a backslash
followed by a zero and the octal digits. Hexadecimal numbers start
with a backslash followed by an upper- or lower-case `x` and then the
hexadecimal digits. Unicode characters start with a backslash followed
by an upper- or lower-case `u` and then up to 8 hexadecimal digits.

Both octal and hexadecimal numbers are signed 64 bit values, and can
include the whole range from zero to 9223372036854775807 (inclusive).
Negative numbers are not allowed. Unicode character numbers are 32 bit
unsigned integers.

## Comments

There are three types of comments: Two line comments and one block
comment.

Line comments start with either a single *square* or *hash* (`#`) or
two forward slashes (`//`). These comments go to the end of the line.

Block comments start with a forward slash followed by an asterisk, and
end with an asterisk followed by a forward slash (`/* */`). Block
comments can span multiple lines, and they can also be nested. The
nesting must have matching start and end delimiters.
 
## Identifiers

Valid characters in identifier names are

    ABCDEFGHIJKLMNOPQRSTUVWXYZ
    abcdefghijklmnopqrstuvwxyz
    1234567890
    §@$£¤?_

Identifiers may not start with a digit.

When parsing identifier names the standard
[`isalpha`](http://en.cppreference.com/w/cpp/string/byte/isalpha) and
[`isalnum`](http://en.cppreference.com/w/cpp/string/byte/isalnum)
functions are used, which may extend the valid letters depending on
locale settings of the system. The "standard" 26 English letters will
always be available.

There is no limit on the length of an identifier.

## Numbers

Raven script only support 64-bit signed integers. 

Since Raven script uses the platform native 64-bit integers the range
depends on the underlying system. Using the most common
[Two's complement](https://en.wikipedia.org/wiki/Two's_complement)
the range is -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807.
With the less common
[one's complement](https://en.wikipedia.org/wiki/Ones'_complement)
the range is -9,223,372,036,854,775,807 to 9,223,372,036,854,775,807.

It should be noted that like C and C++, Raven script doesn't really
have negative number literals. Instead to get a negative number one
must use the unary negation operator.

### Other bases

Raven script can have hexadecimal, octal and binary numbers.

Octal numbers start with a leading zero, followed by the octal number.
Only digits `0` to `7` (inclusive) are allowed.

Binary numbers start with a leading zero followed by a `b` (or `B`)
followed by the binary number. Only the binary digits `0` and `1` are
allowed.

Hexadecimal numbers start with a leading zero followed by a `x`
(or `X`) followed by the hexadecimal number. For the high digits,
value `10` to `15`, the letters `a` to `f` are used, with `a` being
`10` and `f` being `15`. Both upper- and lower-case letters are
allowed.

## Scoping and definition order

Raven script uses strict scoping of symbols. A symbol defined in an
inner scope will no longer exist in an outer scope.

A symbol defined in an outer scope may be redefined (shadowed) in an
inner scope. There is no scoping operator in Raven script, which means
that there is no way to access the symbol from the outer scope if it
has been shadowed in an inner scope.

Symbols defined in an outer scope are available in an inner scope, if
it haven't been shadowed by an inner definition.

Class inheritance brings scoping. The parent class is an outer scope
while the child class is an inner scope. However for classes and
objects it is possible to access variables in the parent classes by
using the `super()` built-in function.

Definition order of symbols is strictly from top to botton in the
source file. Symbols defined at the top are defined before symbols
defined at the bottom.

Symbols must be defined before they are referenced. The only exception
is expressions embedded in strings: As these expressions are lazily
evaluated symbols must have been defined before the expression is
evaluated by the interpreter.
