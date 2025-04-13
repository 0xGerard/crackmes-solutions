#include <stdio.h>
#include <string.h>

#define SERIAL_LENGTH 8

int transform(int input_char, int position){
    return (((position + 7) ^ input_char) + 13) & 0x7F;
}

int main(){

    char alpha[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTVWXYZ";
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
