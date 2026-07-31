%{
#include <stdio.h>
%}

%%
"int"                     { printf("KEYWORD: int\n"); }
[0-9]+                    { printf("INTEGER: %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*    { printf("IDENTIFIER: %s\n", yytext); }
[ \t\n]+                  ;
.                         { printf("UNKNOWN: %s\n", yytext); }
%%

int main() {
    yylex();
    return 0;
}
