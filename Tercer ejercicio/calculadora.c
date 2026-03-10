#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINEA 1000

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
        if(fabs(sig - aprox) < tolerancia) return sig;
        aprox = sig;
    }
    return aprox;
}

double procesar_expresion(char *expr) {
    char *ptr = expr;
    double numeros[100];
    char operadores[100];
    int num_count = 0;
    int op_count = 0;
    int i = 0;
    
    // Eliminar todos los espacios
    char limpia[MAX_LINEA];
    int j = 0;
    for(i = 0; expr[i]; i++) {
        if(expr[i] != ' ') limpia[j++] = expr[i];
    }
    limpia[j] = '\0';
    
    // Procesar sqrt primero
    if(strncmp(limpia, "sqrt", 4) == 0) {
        int pos = 4;
        if(limpia[pos] == '(') {
            pos++;
            char subexpr[MAX_LINEA];
            int k = 0;
            int paren = 1;
            
            while(limpia[pos] && paren > 0) {
                if(limpia[pos] == '(') paren++;
                if(limpia[pos] == ')') paren--;
                if(paren > 0) subexpr[k++] = limpia[pos++];
            }
            subexpr[k] = '\0';
            
            double val = procesar_expresion(subexpr);
            return newton_raphson(val);
        }
    }
    
    // Procesar paréntesis
    for(i = 0; limpia[i]; i++) {
        if(limpia[i] == '(') {
            int inicio = i + 1;
            int paren = 1;
            int j = inicio;
            
            while(limpia[j] && paren > 0) {
                if(limpia[j] == '(') paren++;
                if(limpia[j] == ')') paren--;
                j++;
            }
            
            char subexpr[MAX_LINEA];
            strncpy(subexpr, limpia + inicio, j - inicio - 1);
            subexpr[j - inicio - 1] = '\0';
            
            double val = procesar_expresion(subexpr);
            
            char nueva[MAX_LINEA];
            snprintf(nueva, sizeof(nueva), "%.15g%s", val, limpia + j);
            return procesar_expresion(nueva);
        }
    }
    
    // Buscar operadores por precedencia
    for(i = 0; limpia[i]; i++) {
        if(limpia[i] == '+' || limpia[i] == '-') {
            if(i > 0 && limpia[i-1] != 'e' && limpia[i-1] != 'E') {
                char izq[MAX_LINEA], der[MAX_LINEA];
                strncpy(izq, limpia, i);
                izq[i] = '\0';
                strcpy(der, limpia + i + 1);
                
                double a = procesar_expresion(izq);
                double b = procesar_expresion(der);
                
                if(limpia[i] == '+') return a + b;
                else return a - b;
            }
        }
    }
    
    for(i = 0; limpia[i]; i++) {
        if(limpia[i] == '*' || limpia[i] == '/') {
            char izq[MAX_LINEA], der[MAX_LINEA];
            strncpy(izq, limpia, i);
            izq[i] = '\0';
            strcpy(der, limpia + i + 1);
            
            double a = procesar_expresion(izq);
            double b = procesar_expresion(der);
            
            if(limpia[i] == '*') return a * b;
            else {
                if(b == 0) {
                    printf("Error: Division por cero\n");
                    return 0;
                }
                return a / b;
            }
        }
    }
    
    for(i = 0; limpia[i]; i++) {
        if(limpia[i] == '^') {
            char izq[MAX_LINEA], der[MAX_LINEA];
            strncpy(izq, limpia, i);
            izq[i] = '\0';
            strcpy(der, limpia + i + 1);
            
            double a = procesar_expresion(izq);
            double b = procesar_expresion(der);
            
            return pow(a, b);
        }
    }
    
    // Si es un número
    return atof(limpia);
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
        
        linea[strcspn(linea, "\n")] = '\0';
        
        if(strlen(linea) == 0) continue;
        
        printf("Linea %d: %s = ", num_linea, linea);
        
        double resultado = procesar_expresion(linea);
        printf("%g\n", resultado);
    }
    
    fclose(archivo);
    printf("------------------------------------\n");
    printf("Procesamiento completado\n");
    
    return 0;
}
