- Autor: @0xGerard
- Plataforma: Linux (ELF 64-bit)
- Dificultad: 2.0
- Protección: Ninguna
- Objetivo: Encontrar una serial válida para pasar el programa.

# Analisis inicial
Ejecutamos el binario, nos pide el serial
```console
 ❯ ./easyjerk
Enter your serial:
```

Si ingresamos algo incorrecto:
```console
Access Denied! Invalid Serial.
```

# Analisis estático
Este es el código desensamblado de la funcion `check_serial` con IDA.
```c
__int64 __fastcall check_serial(const char *a1)
{
  int v2; // eax
  _DWORD v3[10]; // [rsp+10h] [rbp-30h]
  int v4; // [rsp+38h] [rbp-8h]
  int i; // [rsp+3Ch] [rbp-4h]

  v3[0] = 88;
  v3[1] = 110;
  v3[2] = 96;
  v3[3] = 107;
  v3[4] = 123;
  v3[5] = 86;
  v3[6] = 102;
  v3[7] = 117;
  v4 = 8;
  if ( strlen(a1) != 8 )
    return 0;
  for ( i = 0; i < v4; ++i )
  {
    v2 = transform(a1[i], i);
    if ( v2 != v3[i] )
      return 0;
  }
  return 1;
}
```
El código hace lo siguiente:
1. Comprueba que el tamaño del serial sea de 8 caracteres. (linea 17)
2. Itera atravez de cada caracter del serial.
3. Transforma cada caracter aplicando la funcion `transform`. (linea 21)
4. Comprueba que el caracter resultante en la posicion i, sea igual al caracter en la posición i del array de caracteres `v3`. (linea 22)

Para continuar con el analisis necesitamos  el código desensamblado de la funcion `transform`.
```c
__int64 __fastcall transform(unsigned __int8 a1, char a2)
{
  return (((unsigned __int8)(a2 + 7) ^ a1) + 13) & 0x7F;
}
```

La función `transform` recibe un caracter y el indice; aplica varias operaciones al caracter antes de retornarlo.

# Solución: Ataque de fuerza bruta
Para encontrar el serial escribiremos un programa que pruebe con cada caracter del alfabeto, aplicamos las operaciones y lo comparamos con los caracteres de `v3`.

```c
#include <stdio.h>
#include <string.h>

#define SERIAL_LENGTH 8

int transform(int input_char, int position){
    return (((position + 7) ^ input_char) + 13) & 0x7F;
}

int main(){
    char alpha[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char alpha_length = strlen(alpha);
    char serial[SERIAL_LENGTH + 1] = {0};

    char v2[SERIAL_LENGTH];
    v2[0] = 88;
    v2[1] = 110;
    v2[2] = 96;
    v2[3] = 107;
    v2[4] = 123;
    v2[5] = 86;
    v2[6] = 102;
    v2[7] = 117;

    for ( size_t serial_pos = 0; serial_pos < SERIAL_LENGTH; serial_pos++ ){
        for ( size_t char_index = 0; char_index <= alpha_length; char_index++ ){
            if ( transform(alpha[char_index], serial_pos) == v2[serial_pos] ){
                serial[serial_pos] = alpha[char_index];
                break;
            }
        }
    }
    printf("Serial: %s\n", serial);

    return 0;
}
```
