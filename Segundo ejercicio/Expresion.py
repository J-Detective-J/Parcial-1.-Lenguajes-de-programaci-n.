class ValidadorID:
    def __init__(self):
        # Estados del autómata
        self.estados = {
            "INICIO": 0,
            "ID_VALIDO": 1,
            "ACEPTADO": 2,
            "ERROR": -1
        }
        self.estado_actual = self.estados["INICIO"]
        self.letras = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
        self.digitos = "0123456789"
    
    def reiniciar(self):
        """Vuelve al estado inicial"""
        self.estado_actual = self.estados["INICIO"]
    
    def procesar_caracter(self, caracter):
        """Procesa un caracter y cambia de estado"""
        
        # Estado INICIO: esperando letra mayúscula o minúscula
        if self.estado_actual == self.estados["INICIO"]:
            if caracter in self.letras:
                self.estado_actual = self.estados["ID_VALIDO"]
            else:
                self.estado_actual = self.estados["ERROR"]
        
        # Estado ID_VALIDO: leyendo letras o dígitos
        elif self.estado_actual == self.estados["ID_VALIDO"]:
            if caracter in self.letras or caracter in self.digitos:
                # Sigue siendo válido, nos quedamos aquí
                pass
            else:
                self.estado_actual = self.estados["ERROR"]
    
    def validar(self, identificador):
        """Valida un identificador completo"""
        self.reiniciar()
        
        for caracter in identificador:
            self.procesar_caracter(caracter)
            if self.estado_actual == self.estados["ERROR"]:
                return False
        
        # Si terminamos en ID_VALIDO, es aceptado
        return self.estado_actual == self.estados["ID_VALIDO"]
    
    def validar_archivo(self):
        """Valida todos los identificadores en un archivo"""
        try:
            with open("pruebas.txt", "r", encoding="utf-8") as archivo:
                lineas = archivo.readlines()
            
            print("=" * 60)
            print("VALIDACIÓN DE IDENTIFICADORES")
            print("=" * 60)
            print("Archivo: pruebas.txt")
            print("Formato: [A-Za-z][A-Za-z0-9]*")
            print("Debe comenzar con letra (mayúscula o minúscula)")
            print("Puede continuar con letras o números")
            print("-" * 60)
            
            aceptados = 0
            rechazados = 0
            
            for i, linea in enumerate(lineas, 1):
                identificador = linea.strip()
                if identificador and not identificador.startswith("#"):  # Ignorar líneas vacías y comentarios
                    es_valido = self.validar(identificador)
                    if es_valido:
                        print(f"Línea {i:2d}: {identificador} -> ACEPTADO")
                        aceptados += 1
                    else:
                        print(f"Línea {i:2d}: {identificador} -> NO ACEPTADO")
                        rechazados += 1
            
            print("-" * 60)
            print("RESUMEN:")
            print(f"  Total identificadores: {aceptados + rechazados}")
            print(f"  ACEPTADOS: {aceptados}")
            print(f"  RECHAZADOS: {rechazados}")
            print("=" * 60)
            
        except FileNotFoundError:
            print("Error: No se encontró el archivo 'pruebas.txt'")
        except Exception as e:
            print(f"Error al leer el archivo: {e}")


# Programa principal
if __name__ == "__main__":
    # Crear validador y procesar archivo
    validador = ValidadorID()
    validador.validar_archivo()