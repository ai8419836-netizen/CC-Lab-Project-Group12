%{
#include <stdio.h>
int line_number = 1;
%}

%%

"int"           { printf("KEYWORD: %s (line %d)\n", yytext, line_number); }
"float"         { printf("KEYWORD: %s (line %d)\n", yytext, line_number); }
"bool"          { printf("KEYWORD: %s (line %d)\n", yytext, line_number); }
"if"            { printf("KEYWORD: %s (line %d)\n", yytext, line_number); }
"else"          { printf("KEYWORD: %s (line %d)\n", yytext, line_number); }
"while"         { printf("KEYWORD: %s (line %d)\n", yytext, line_number); }
"print"         { printf("KEYWORD: %s (line %d)\n", yytext, line_number); }
"true"          { printf("BOOLEAN: %s (line %d)\n", yytext, line_number); }
"false"         { printf("BOOLEAN: %s (line %d)\n", yytext, line_number); }

[0-9]+\.[0-9]+  { printf("FLOAT: %s (line %d)\n", yytext, line_number); }
[0-9]+          { printf("INTEGER: %s (line %d)\n", yytext, line_number); }

[a-zA-Z_][a-zA-Z0-9_]*    { printf("IDENTIFIER: %s (line %d)\n", yytext, line_number); }

"=="|"<="|">="|"!="|"<"|">"    { printf("RELATIONAL_OP: %s (line %d)\n", yytext, line_number); }

"&&"|"||"|"!"    { printf("LOGICAL_OP: %s (line %d)\n", yytext, line_number); }

"+"|"-"|"*"|"/"|"%"    { printf("ARITHMETIC_OP: %s (line %d)\n", yytext, line_number); }

"="             { printf("ASSIGN: %s (line %d)\n", yytext, line_number); }

";"|"("|")"|"{"|"}"    { printf("DELIMITER: %s (line %d)\n", yytext, line_number); }

"//".*          ;   /* single-line comment: matched and discarded */

"/*"([^*]|\*+[^*/])*\*+"/"    {
                        /* block comment: discarded, but we must still count
                           any newlines inside it so line numbers stay correct */
                        for (int i = 0; yytext[i] != '\0'; i++)
                            if (yytext[i] == '\n') line_number++;
                    }

[ \t]+          ;   /* spaces and tabs: discarded */

\n              { line_number++; }   /* newline: discarded, but increases line counter */

.               { printf("LEXICAL ERROR: invalid character '%s' (line %d)\n", yytext, line_number); }

%%

int yywrap() {
    return 1;
}

int main() {
    yylex();
    return 0;
}
