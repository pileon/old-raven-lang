# Formal grammar

This file contains the formal grammar for the language.

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
        : '{' statement_list '}'
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
        ;
        
    function
        : FUNCTION opt_arguments '{' statement_list '}'
        ;
