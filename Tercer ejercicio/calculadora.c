#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINEA 1000

double newton_raphson(double x);
double evaluar_expresion(char *expr, int *pos, int *error);
double evaluar_termino(char *expr, int *pos, int *error);
double evaluar_factor(char *expr, int *pos, int *error);
double evaluar_potencia(char *expr, int *pos, int *error);
double obtener_numero(char *expr, int *pos, int *error);

double newton_raphson(double x) {
    if(x < 0) {
        printf("Error: Raiz cuadrada de numero negativo\n");
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

double obtener_numero(char *expr, int *pos, int *error) {
    char buffer[100];
    int i = 0;
    
    while(isdigit(expr[*pos]) || expr[*pos] == '.') {
        buffer[i++] = expr[*pos];
        (*pos)++;
    }
    buffer[i] = '\0';
    
    if(i == 0) {
        *error = 1;
        return 0;
    }
    
    return atof(buffer);
}

double evaluar_factor(char *expr, int *pos, int *error) {
    double resultado = 0;
    
    // Saltar espacios
    while(expr[*pos] == ' ') (*pos)++;
    
    if(expr[*pos] == '(') {
        (*pos)++;
        resultado = evaluar_expresion(expr, pos, error);
        while(expr[*pos] == ' ') (*pos)++;
        if(expr[*pos] == ')') {
            (*pos)++;
        } else {
            *error = 1;
        }
    }
    else if(strncmp(&expr[*pos], "sqrt", 4) == 0) {
        (*pos) += 4;
        while(expr[*pos] == ' ') (*pos)++;
        if(expr[*pos] == '(') {
            (*pos)++;
            double val = evaluar_expresion(expr, pos, error);
            while(expr[*pos] == ' ') (*pos)++;
            if(expr[*pos] == ')') {
                (*pos)++;
                resultado = newton_raphson(val);
            } else {
                *error = 1;
            }
        } else {
            *error = 1;
        }
    }
    else if(expr[*pos] == '-') {
        (*pos)++;
        resultado = -evaluar_factor(expr, pos, error);
    }
    else if(isdigit(expr[*pos]) || expr[*pos] == '.') {
        resultado = obtener_numero(expr, pos, error);
    }
    else {
        *error = 1;
    }
    
    return resultado;
}

double evaluar_potencia(char *expr, int *pos, int *error) {
    double resultado = evaluar_factor(expr, pos, error);
    
    while(!*error) {
        while(expr[*pos] == ' ') (*pos)++;
        
        if(expr[*pos] == '^') {
            (*pos)++;
            double exp = evaluar_factor(expr, pos, error);
            resultado = pow(resultado, exp);
        } else {
            break;
        }
    }
    
    return resultado;
}

double evaluar_termino(char *expr, int *pos, int *error) {
    double resultado = evaluar_potencia(expr, pos, error);
    
    while(!*error) {
        while(expr[*pos] == ' ') (*pos)++;
        
        if(expr[*pos] == '*') {
            (*pos)++;
            resultado = resultado * evaluar_potencia(expr, pos, error);
        }
        else if(expr[*pos] == '/') {
            (*pos)++;
            double divisor = evaluar_potencia(expr, pos, error);
            if(divisor == 0) {
                printf("Error: Division por cero\n");
                resultado = 0;
            } else {
                resultado = resultado / divisor;
            }
        }
        else {
            break;
        }
    }
    
    return resultado;
}

double evaluar_expresion(char *expr, int *pos, int *error) {
    double resultado = evaluar_termino(expr, pos, error);
    
    while(!*error) {
        while(expr[*pos] == ' ') (*pos)++;
        
        if(expr[*pos] == '+') {
            (*pos)++;
            resultado = resultado + evaluar_termino(expr, pos, error);
        }
        else if(expr[*pos] == '-') {
            (*pos)++;
            resultado = resultado - evaluar_termino(expr, pos, error);
        }
        else {
            break;
        }
    }
    
    return resultado;
}

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("Uso: %s archivo_entrada.txt\n", argv[0]);
        return 1;
    }
    
    FILE *archivo = fopen(argv[1], "r");
    if(!archivo) {
        printf("Error: No se puede abrir %s\n", argv[1]);
        return 1;
    }
    
    char linea[MAX_LINEA];
    int num_linea = 0;
    
    printf("Procesando: %s\n", argv[1]);
    printf("------------------------------------\n");
    
    while(fgets(linea, MAX_LINEA, archivo)) {
        num_linea++;
        
        // Eliminar salto de línea
        linea[strcspn(linea, "\n")] = '\0';
        
        // Saltar líneas vacías
        if(strlen(linea) == 0) continue;
        
        int pos = 0;
        int error = 0;
        
        double resultado = evaluar_expresion(linea, &pos, &error);
        
        // Verificar que no queden caracteres sin procesar
        while(linea[pos] == ' ') pos++;
        
        if(!error && linea[pos] == '\0') {
            printf("Linea %d: %s = %g\n", num_linea, linea, resultado);
        } else {
            printf("Linea %d: %s = Error de sintaxis\n", num_linea, linea);
        }
    }
    
    fclose(archivo);
    printf("------------------------------------\n");
    printf("Procesamiento completado\n");
    
    return 0;
}
