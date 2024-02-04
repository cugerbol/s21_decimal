#include <stdio.h>
#include "s21_decimal.h"

int get_bit(s21_decimal decimal, int index) {
/*

    10101010
    00000001 << index = 7
    10000000 
    
    10101010
    &
    10000000 
    10000000 

    >0 | =0

    (xi+yi)/2


*/
    int mask = 1u << (index % 32);
    return (decimal.bits[index / 32] & mask) != 0;
}

int main(){
    s21_decimal dec;
    dec.bits[0] = 0b10000000;
    printf("%d\n",dec.bits[0]);
    printf("%d\n",get_bit(dec,8));
}