class ValidadorAjedrez:
    def __init__(self):
        # Estados del autómata
        self.estados = {
            "INICIO": 0,
            "ORIGEN": 1,
            "SEPARADOR": 2,
            "DESTINO": 3,
            "ACEPTADO": 4,
            "ERROR": -1
        }
        self.estado_actual = self.estados["INICIO"]
        self.piezas = "KRTACP"
        self.coordenadas = "abcdefgh"
    
    def reiniciar(self):
        """Vuelve al estado inicial"""
        self.estado_actual = self.estados["INICIO"]
    
    def procesar_caracter(self, caracter):
        """Procesa un caracter y cambia de estado"""
        
        # Estado INICIO: esperando pieza
        if self.estado_actual == self.estados["INICIO"]:
            if caracter in self.piezas:
                self.estado_actual = self.estados["ORIGEN"]
            else:
                self.estado_actual = self.estados["ERROR"]
        
        # Estado ORIGEN: leyendo piezas
        elif self.estado_actual == self.estados["ORIGEN"]:
            if caracter in self.piezas:
                # Sigue siendo pieza, nos quedamos aquí
                pass
            elif caracter == "-":
                self.estado_actual = self.estados["SEPARADOR"]
            elif caracter == "X":
                self.estado_actual = self.estados["SEPARADOR"]
            else:
                self.estado_actual = self.estados["ERROR"]
        
        # Estado SEPARADOR: leyendo -> o X
        elif self.estado_actual == self.estados["SEPARADOR"]:
            if caracter == ">":
                # Parte del separador ->
                pass
            elif caracter in self.coordenadas or caracter in self.piezas:
                self.estado_actual = self.estados["DESTINO"]
            elif caracter == " ":
                # Espacio
                pass
            else:
                self.estado_actual = self.estados["ERROR"]
        
        # Estado DESTINO
        elif self.estado_actual == self.estados["DESTINO"]:
            if caracter.isdigit() and "1" <= caracter <= "8":
                # Número válido de fila
                pass
            elif caracter in self.piezas or caracter in self.coordenadas:
                # Pieza o coordenada
                pass
            else:
                self.estado_actual = self.estados["ERROR"]
    
    def validar(self, movimiento):
        # Valida un movimiento completo
        self.reiniciar()
        
        for caracter in movimiento:
            self.procesar_caracter(caracter)
            if self.estado_actual == self.estados["ERROR"]:
                return False
        
        # Si terminamos en DESTINO, es válido
        return self.estado_actual == self.estados["DESTINO"]
    
    def validar_archivo(self, movimientos):
        # Valida todos los movimientos en un archivo
        try:
            with open("movimientos.txt") as archivo:
                lineas = archivo.readlines()
            
            print("=" * 50)
            print("VALIDACIÓN DE MOVIMIENTOS DE AJEDREZ")
            print("=" * 50)
            print("Archivo: movimiento")
            print("Piezas: K=Rey, R=Reina, T=Torre, A=Alfil, C=Caballo, P=Peón")
            print("-" * 50)
            
            validos = 0
            invalidos = 0
            
            for i, linea in enumerate(lineas, 1):
                movimiento = linea.strip()
                if movimiento and not movimiento.startswith("#"):  # Ignorar líneas vacías y comentarios
                    es_valido = self.validar(movimiento)
                    if es_valido:
                        print("Línea {i:2d}: {movimiento}")
                        validos += 1
                    else:
                        print("Línea {i:2d}: {movimiento}")
                        invalidos += 1
            
            print("-" * 50)
            print("RESUMEN:")
            print("  Total movimientos: {validos + invalidos}")
            print("  Válidos: {validos}")
            print("  Inválidos: {invalidos}")
            print("=" * 50)
            
        except FileNotFoundError:
            print("Error: No se encontró el archivo 'movimiento'")
        except Exception as e:
            print("Error al leer el archivo: {e}")


# Programa
if __name__ == "__main__":
    import sys
    
    # Crear validador y procesar archivo
    validador = ValidadorAjedrez()
    validador.validar_archivo(movimientos)