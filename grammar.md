# Formal grammar

This file contains the formal grammar for the language.

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

There is no limit on the length of an identifier.

## Numbers

Raven Script only support 64-bit signed integers. 

It should be noted that like C and C++, Raven script doesn't really
have negative number literals. Instead to get a negative number one
must use the unary negation operator.
