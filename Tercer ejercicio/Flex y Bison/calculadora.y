%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void yyerror(const char *s);
int yylex(void);
double newton_raphson(double x);

FILE *archivo_salida;
%}

%union {
    double real;
}

%token <real> NUMERO
%token SQRT
%token FIN_LINEA

%type <real> expresion

%left '+' '-'
%left '*' '/'
%right '^'
%left SQRT

%%

entrada: /* vacio */
       | entrada linea
       ;

linea: expresion FIN_LINEA {
    fprintf(archivo_salida, "Resultado: %g\n", $1);
}
     | FIN_LINEA
     | error FIN_LINEA { 
         fprintf(archivo_salida, "Error en la expresion\n");
         yyerrok;
     }
     ;

expresion: NUMERO { $$ = $1; }
         | expresion '+' expresion { $$ = $1 + $3; }
         | expresion '-' expresion { $$ = $1 - $3; }
         | expresion '*' expresion { $$ = $1 * $3; }
         | expresion '/' expresion { 
             if($3 == 0) {
                 fprintf(archivo_salida, "Error: Division por cero\n");
                 $$ = 0;
             } else {
                 $$ = $1 / $3;
             }
         }
         | expresion '^' expresion { $$ = pow($1, $3); }
         | SQRT expresion { $$ = newton_raphson($2); }
         | '(' expresion ')' { $$ = $2; }
         | '-' expresion %prec '*' { $$ = -$2; }
         ;

%%

double newton_raphson(double x) {
    if(x < 0) {
        fprintf(archivo_salida, "Error: Raiz cuadrada de numero negativo\n");
        return 0;
    }
    if(x == 0) return 0;
    
    double aprox = x / 2.0;
    double tolerancia = 1e-10;
    
    for(int i = 0; i < 100; i++) {
        double sig = 0.5 * (aprox + x / aprox);
        if(fabs(sig - aprox) < tolerancia) {
            return sig;
        }
        aprox = sig;
    }
    return aprox;
}

void yyerror(const char *s) {
    fprintf(archivo_salida, "Error: %s\n", s);
}

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("Uso: %s archivo_entrada.txt\n", argv[0]);
        return 1;
    }
    
    FILE *entrada = fopen(argv[1], "r");
    if(!entrada) {
        printf("Error: No se puede abrir %s\n", argv[1]);
        return 1;
    }
    
    archivo_salida = stdout;
    yyin = entrada;
    
    printf("Procesando: %s\n", argv[1]);
    printf("------------------------------------\n");
    
    yyparse();
    
    fclose(entrada);
    printf("------------------------------------\n");
    printf("Procesamiento completado\n");
    
    return 0;
}