#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

FILE *archivo_salida;
char *texto_entrada;
int pos_actual = 0;
double yylval_real;
int linea_actual = 1;

enum {
    NUMERO = 258,
    SQRT = 259,
    FIN_LINEA = 260
};

int token_actual;
double valor_token;

int yylex();
void yyerror(const char *s);
int yyparse();
double newton_raphson(double x);

int yylex() {
    while(1) {
        char c = texto_entrada[pos_actual];
        
        if(c == '\0') return 0;
        
        if(c == ' ' || c == '\t') {
            pos_actual++;
            continue;
        }
        
        if(c == '\n') {
            pos_actual++;
            linea_actual++;
            return FIN_LINEA;
        }
        
        if((c >= '0' && c <= '9')) {
            char buffer[100];
            int i = 0;
            while((texto_entrada[pos_actual] >= '0' && texto_entrada[pos_actual] <= '9') || 
                   texto_entrada[pos_actual] == '.') {
                buffer[i++] = texto_entrada[pos_actual++];
            }
            buffer[i] = '\0';
            yylval_real = atof(buffer);
            return NUMERO;
        }
        
        if(c == 's') {
            if(strncmp(&texto_entrada[pos_actual], "sqrt", 4) == 0) {
                pos_actual += 4;
                return SQRT;
            }
        }
        
        pos_actual++;
        return c;
    }
}

double expr();

double expr() {
    double resultado = 0;
    
    if(token_actual == NUMERO) {
        resultado = valor_token;
        token_actual = yylex();
    }
    else if(token_actual == SQRT) {
        token_actual = yylex();
        if(token_actual == '(') {
            token_actual = yylex();
            double val = expr();
            if(token_actual == ')') {
                token_actual = yylex();
                resultado = newton_raphson(val);
            } else {
                yyerror("Esperaba ')'");
            }
        } else {
            yyerror("Esperaba '('");
        }
    }
    else if(token_actual == '(') {
        token_actual = yylex();
        resultado = expr();
        if(token_actual == ')') {
            token_actual = yylex();
        } else {
            yyerror("Esperaba ')'");
        }
    }
    else if(token_actual == '-') {
        token_actual = yylex();
        resultado = -expr();
    }
    else {
        yyerror("Expresion invalida");
    }
    
    while(1) {
        if(token_actual == '+') {
            token_actual = yylex();
            resultado = resultado + expr();
        }
        else if(token_actual == '-') {
            token_actual = yylex();
            resultado = resultado - expr();
        }
        else if(token_actual == '*') {
            token_actual = yylex();
            resultado = resultado * expr();
        }
        else if(token_actual == '/') {
            token_actual = yylex();
            double divisor = expr();
            if(divisor == 0) {
                fprintf(archivo_salida, "Error: Division por cero\n");
                resultado = 0;
            } else {
                resultado = resultado / divisor;
            }
        }
        else if(token_actual == '^') {
            token_actual = yylex();
            resultado = pow(resultado, expr());
        }
        else {
            break;
        }
    }
    
    return resultado;
}

int yyparse() {
    token_actual = yylex();
    while(token_actual != 0) {
        if(token_actual == FIN_LINEA) {
            token_actual = yylex();
            continue;
        }
        
        double res = expr();
        if(token_actual == FIN_LINEA || token_actual == 0) {
            fprintf(archivo_salida, "Resultado: %g\n", res);
            token_actual = yylex();
        } else {
            yyerror("Error de sintaxis");
            while(token_actual != 0 && token_actual != FIN_LINEA) {
                token_actual = yylex();
            }
            if(token_actual == FIN_LINEA) token_actual = yylex();
        }
    }
    return 0;
}

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
    fprintf(archivo_salida, "Error linea %d: %s\n", linea_actual, s);
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
    
    fseek(entrada, 0, SEEK_END);
    long tamano = ftell(entrada);
    fseek(entrada, 0, SEEK_SET);
    
    texto_entrada = malloc(tamano + 1);
    fread(texto_entrada, 1, tamano, entrada);
    texto_entrada[tamano] = '\0';
    
    fclose(entrada);
    
    archivo_salida = stdout;
    pos_actual = 0;
    linea_actual = 1;
    
    printf("Procesando: %s\n", argv[1]);
    printf("------------------------------------\n");
    
    yyparse();
    
    printf("------------------------------------\n");
    printf("Procesamiento completado\n");
    
    free(texto_entrada);
    return 0;
}