#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H


typedef struct {
    int bits[4];
} s21_decimal;




/*  

    Максимальные и минимальные значения INT32 и s21_decimal:

    INT_MIN = -2 147 483 648 
    INT_MAX = 2 147 483 647 

    UNSIGNED INT_MAX = 4 294 967 295 


    DEC_MIN = - 79 228 162 514 264 337 593 543 950 335
    DEC_MAX = 79 228 162 514 264 337 593 543 950 335

    s21_decimal max 2^96 

*/

/* 


    Битовое представление суммы числа и единицы в различных ситуациях для INT8:

    d - Перенесенная 1 из предыдущего порядка (если такова была)
    
    if a1i + a2i + d == 0 => 0 
    if a1i + a2i + d == 1 => 1
    if a1i + a2i + d == 2 => 0 AND 1 переносим в i+1 = d
    if a1i + a2i + d == 3 => 1 AND 1 переносим в i+1 = d

    unsigned int8:
    ex1:
        00000001 : 1 
       +
        00000001 : 1 
       =
        00000010 : 2 

    ex2:
        00000010 : 2
       +
        00000001 : 1
       =
        00000011 : 3

    ex3:
        01111111 : 127 
       +
        00000001 : 1
       =
        10000000:  128

    ex4:
        11111111 : 255
       +
        00000001 : 1
       =
       100000000 : 0
        overflow
    
*/




/*
    Битовое представление реализуемого числа s21_decimal

    * empty1 биты от 0 до 15;
    * scale биты от 16 до 23;
    * empty2 биты от 24 до 30;
    * sign бит 31

    x = {1,0}

    bits[0]: xxxxxxxx.xxxxxxxx.xxxxxxxx.xxxxxxxx
    bits[1]: xxxxxxxx.xxxxxxxx.xxxxxxxx.xxxxxxxx
    bits[2]: xxxxxxxx.xxxxxxxx.xxxxxxxx.xxxxxxxx
    bits[3]: 00000000.00000000.xxxxxxxx.0000000x


    mantissa: 
        bits[2]                          bits[1]                          bits[0]
        xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx.xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx.xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
        

    bits[3]:
        00000000.00000000.xxxxxxxx.0000000x 
        

*/







/*  

    Битовое представление суммы числа и единицы в различных ситуациях для s21_decimal:

    bits[i] = INT32

    ex1:
            bits[0]:
            00000000000000000000000000000001 : 1
          + 
            00000000000000000000000000000001 : 1
          =
            00000000000000000000000000000010 : 2


    ex2:
            bits[0]:
            01111111111111111111111111111111 : int 2147483647 | unsigned int 2^31 - 1
          + 
            00000000000000000000000000000001 : 1
          =
            10000000000000000000000000000000 : int -2147483648 


    ex3:
            bits[0]:
            1111111111111111111111111111111 : int -1 | unsigned int 2^32 - 1
           +
            0000000000000000000000000000001 : 1
           =
            0000000000000000000000000000000 : 0

    
    ex4:
            bits[1]:                        bits[0]:
            0000000000000000000000000000000.1111111111111111111111111111111 : 2^32 - 1
           +
            0000000000000000000000000000000.0000000000000000000000000000001 : 1
           =
            0000000000000000000000000000001.0000000000000000000000000000000 : 2^32

    ex5:
            bits[2]:                        bits[1]:                        bits[0]:
            0000000000000000000000000000000.1111111111111111111111111111111.1111111111111111111111111111111 : 2^64 - 1
           +
            0000000000000000000000000000000.0000000000000000000000000000000.0000000000000000000000000000001 : 1
           =
            0000000000000000000000000000001.0000000000000000000000000000000.0000000000000000000000000000000 : 2^64 

    ex6:
            bits[2]:                        bits[1]:                        bits[0]:
            1111111111111111111111111111111.1111111111111111111111111111111.1111111111111111111111111111111 : 2^96 - 1
           +
            0000000000000000000000000000000.0000000000000000000000000000000.0000000000000000000000000000001 : 1
          =
            overflow


*/


/*  
    Первый подводный камень или почему нам нужен big_decimal:

    scale - степень числа 10 , на которое будет делится мантиса
    scale - после кого числа ставим запятую, если считаем с права

    ex1:
        s21_decimal dec: mantissa = 123456789  AND scale = 4 => dec = 12345,6789
        
    
    ex2:

        s21_decimal dec1  : mantissa = 2^96 AND scale = 10   2^96 / 10^10
        s21_decimal dec2  : mantissa = 2^96 AND scale = 7    2^96 / 10^7 

    ex3:

        s21_decimal dec1  : mantissa = 2^96 AND scale = 8   2^96 / 10^8
        s21_decimal dec2  : mantissa = 2^94 AND scale = 7    2^94 / 10^7 

        dec1 > dec2? OR dec1 < dec2?

        dec1 - dec2 = ?

    
*/

/*
    ex1:
        12345 scale = 2
        12345 scale = 4

        123,45 + 1,2345  = (1234500 + 12345)/10^4 



*/

typedef struct {
    int bits[8];
} s21_big_decimal;

/*  


    Битовое представление s21_big_decimal:
    Все INT отведены на мантиссу 
    X = {0,1}
    bits[0]: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    bits[1]: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    bits[2]: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    bits[3]: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    bits[4]: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    bits[5]: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    bits[6]: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    bits[7]: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

*/


/*

    scale - степень 10, на которую требуется разделить мантиссу
    scale - кол-во знаков после запятой
    
    При равных мантиссах, то число больше, у которого меньше scale

    ex1:
        mantissa1 = 12345 AND scale = 2 => 123,45
        mantissa2 = 12345 AND scale = 3 => 12,345

        123,45+12,345 = 135,795 => mantissa_result = 134795 AND scale = 3

        123450
       +
        012345


    Алгоритм работы с big decimal:

        ex:
            12345 scale_x = 2 => 123,45 => 12345 / 10^2 => 123450 / 10^3
            12345 scale_y = 3 => 12,345 => 12345 / 10^3 

            dec1 = 123450 / 10^3
            dec2 = 12345 / 10^3

            scale_result = 3

            ИЛИ 

            dec1 = 123450 
            dec2 = 012345 

            dec_result / 10^3 

            scale_result = 3


    s21_decimal dec1  : mantissa1 = 2^96 - 1 AND scale1 = {0,28} 
    s21_decimal dec2  : mantissa2 = 2^95 - 1 AND scale2 = {0,28}  

    1. MAX_SCALE = max({x,y})  // НАХОДИМ МАКСИМАЛЬНЫЙ scale

    2. 
        if MAX_SCALE = scale1 , SCALE_2 = MAX_SCALE - MIN_SCALE, SCALE_1 = 0 // ЕСЛИ dec1 ИМЕЕТ MAX_SCALE

        if MAX_SCALE = scale2 , SCALE_1 = MAX_SCALE - MIN_SCALE, SCALE_2 = 0 // ЕСЛИ dec2 ИМЕЕТ MAX_SCALE

    3.  
        s21_big_decimal big_dec1 : big_mantissa1 = mantissa1 * 10^SCALE_2     
        s21_big_decimal big_dec2 : big_mantissa2 = mantissa2 * 10^SCALE_1

        y/10^2 = x/10 => 
        y * 10 = x * 10^2 : 10^2 => 
        y = x * 10
    
    4.  
        X = (+,-,/,*) if need use sign
        big_mantissa1 X big_mantissa2 = big_mantissa_result

    5.  
        SCALE = max(SCALE_X, SCALE_Y)
        big_mantissa_result / (10^SCALE)

    6.
        big_dec_result => dec_result 

    

    
    ex1:
        1)
            bits[2]:                        bits[1]:                        bits[0]:
            1111111111111111111111111111111.1111111111111111111111111111111.1111111111111111111111111111111 : 2^96 - 1  (dec1) scale_1 = 5
            
            1111111111111111111111111111111.1111111111111111111111111111111.1111111111111111111111111111111 : 2^96 - 1  (dec2) scale_2 = 8

        2)
            MAX_SCALE = scale_2 =>  
            SCALE_1 = scale_2 - scale_1 = 8 - 5 = 3;
            SCALE_2 = 0

        3)  

            big_dec_mantissa1 = dec_mantissa1
            big_dec_mantissa2 = dec_mantissa2 * 10^SCALE_X

            bits[3]:                        bits[2]:                        bits[1]:                        bits[0]:
            XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  : 2^96 - 1  * 10^3 (dec1) 
    Операция (+,-,/,*)
            0000000000000000000000000000000.1111111111111111111111111111111.1111111111111111111111111111111.1111111111111111111111111111111 : 2^96 - 1  (dec2) 
           =
            big_dec_mantissa_result
        
        4)
            big_dec_mantissa_result = dec_mantissa;

            scale = 8
                

*/


/*

    s21_decimal_operation(dec1 , dec2){      
        
        scale1 = s21_get_scale(dec1)
        scale2 = s21_get_scale(dec2)

        sign1 = s21_get_sign(dec1)
        sign2 = s21_get_sign(dec2)

        ... 

        s21_decimal_to_big_decimal(  dec1,
                                     SCALE_1,
                                     &big_dec1)

        s21_decimal_to_bit_decimal(  dec2,
                                     SCALE_2,
                                     &big_dec1)

        ....

        1. - a + b = b - a
        2. |a-b| = |b-a|
        
        ....


        s21_big_decimal_operation(  big_dec1, 
                                    big_dec2, 
                                    &big_dec_result )


        s21_big_decimal_to_decimal( big_dec_result,
                                    &dec_result)


        s21_set_sign(&dec_result)
        s21_set_scale(&dec_result)


        FINISH
        return ...
    }


*/





typedef union decimal_bit3 {

    /*
        Специальный тип данных, который позволяет хранить 
        различные типы данных в одном и том же участке памяти
    */

    int i; 0-31

    struct {
            uint32_t empty1 : 16; // 0 - 15
            uint32_t scale : 8;  // 16 - 23
            uint32_t empty2 : 7; // 24 - 30
            uint32_t sign : 1; // 31 
            } parts;

} decimal_bit3;


/*

    decimal_bit3 bit3;
    bit3.i = 0  => bit3.parts.sing = 0

    bit3.parts.sing = 0 => i = b: 10000000.00000000.00000000.00000000



*/

/*

    Битовая логика:

        0 & 0 = 0 
        1 & 0 = 0
        0 & 1 = 0
        1 & 1 = 1 

        0 | 0 = 0
        1 | 0 = 1
        0 | 1 = 0
        1 | 1 = 1


    Получить значение бита по index

    ex1:

        10101010 : decimal

        00000001 << index = 7
        10000000 
    
        10101010 : decimal
        &
        10000000 
        10000000 

    if 0 => 0 
    if 1 => 1


    Установить значение бита по index 

    ex1: set 0

        10101010 : decimal

        00000001 << index = 7
        10000000
        ~
        01111111

        10101010
       & 
        01111111
       = 
        00101010

    ex2: set 1 

        00101010 : decimal

        00000001 << index = 7
        10000000


        00101010
       |
        10000000
       =
        10101010


*/
#endif

























































