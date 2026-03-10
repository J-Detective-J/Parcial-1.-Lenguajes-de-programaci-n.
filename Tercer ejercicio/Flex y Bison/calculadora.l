%{
#include "calculadora.tab.h"
#include <stdlib.h>
%}

%%
[0-9]+(\.[0-9]+)?  { yylval.real = atof(yytext); return NUMERO; }
"sqrt"             { return SQRT; }
\n                 { return FIN_LINEA; }
[ \t]              ; /* ignorar espacios */
.                  { return yytext[0]; }
%%

int yywrap() {
    return 1;
}