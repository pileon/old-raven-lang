# Formal grammar

This file contains the formal grammar for the language.

    program
        : statement_list
        ;
        
    statement_list
        : statement
        | statement statement_list
        ;
        
    statement:
        : declaration_stmt
        | assignment_stmt
        | return_stmt
        ;
    
    declaration_stmt
        : VAR IDENT
        | VAR IDENT '=' expression
        | CONST IDENT '=' expression
        ;
        
    assignment_stmt
        : expression '=' expression
        ;
        
    return_stmt
        : RETURN
        | RETURN expression
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
        | IF '(' expression ')' expression ELSE expression
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
        : add_sub_expr
        | add_sub_expr comparision_op add_sub_expr
        ;
        
    comparison_op
        : '=='
        | "!="
        | "<="
        | ">="
        | '<'
        | '>'
        | "~="
        | "!~="
        ;
        
    add_sub_expr
        : mul_div_expr
        | mul_div_expr '+' add_sub_expr
        | mul_div_expr '-' add_sub_expr
        ;
        
    mul_div_expr
        : bitwise_or_expr
        | bitwise_or_expr '*' mul_div_expr
        | bitwise_or_expr "**" mul_div_expr
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
