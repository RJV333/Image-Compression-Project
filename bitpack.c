/*     
 *     bitpack.c
 *     by Ahmed Aly & Ryan Vasios, 2/28/14
 *     Assignment 4
 *
 *     Interface for bitpack
 */
#include <bitpack.h>
#include <math.h>
#include <stdbool.h>
#include "stdio.h"
#include <except.h>
#include <assert.h>

Except_T Bitpack_Overflow = { "Overflow packing bits"};

/* Bitpack_fitsu take in a unsigned 64 bit integer and a width 
 * and returns either true if the integer could fit in a bit
 * field of width bits or false if the integer can't fit.
 */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        uint64_t UpBound = pow(2, width);
        if (width >= 1 && n < UpBound)
        {
                return true;
        }
        else {
                RAISE(Bitpack_Overflow);
                return false;
        }
}

/* Bitpack_fitss take in a signed 64 bit integer and a width 
 * and returns either true if the integer could fit in a bit
 * field of width bits or false if the integer can't fit.
 */
bool Bitpack_fitss(int64_t n, unsigned width)
{
        int64_t LowBound = -1 * pow(2, width-1);
        int64_t UpBound = pow(2, width-1) - 1;
        if ( n >= LowBound && n <= UpBound)
        {
                return true;
        }
        else 
        {
                RAISE(Bitpack_Overflow);
                return false;
        }
}


/* Bitpack_getu takes in an unsigned 64 bit word and extracts a 
 * field from a word given the width of the field and the location
 * of the field’s least significant bit. The extracted field is 
 * returned in an unsigned 64 bit integer.
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        if (width==0)
        {
                return 0; 
        }
        else 
        {
                uint64_t mask = (pow(2, width) - 1); 
                mask = mask << lsb;
                uint64_t output = (mask & word) ;
                output = output >> lsb; 
                return output;
        }
}

/* Bitpack_gets takes in a signed 64 bit word and extracts a 
 * field from a word given the width of the field and the location
 * of the field’s least significant bit. The extracted field is 
 * returned in an signed 64 bit integer.
 */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        if (width==0)
        {
                return 0; 
        }
        if(Bitpack_getu(word, width, lsb) >= pow(2,width-1))
        {
                word = word >> lsb;
                uint64_t mask = ~0;
                mask = mask << width;
                int64_t output = (mask | word); 
                return output;
        }
        else 
                return Bitpack_getu(word, width, lsb);
}

/* Bitpack_newu return a new word which is identical to the original
 * word, except the field of width width with least significant
 * bit at lsb will have been replaced by a width-bit representation of
 * value. An exception is raised if any of the algebraic rules for width
 * or lsb are not followed.
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
                      uint64_t value)
{
        TRY
               Bitpack_fitsu(value,width);
                if (width > (64-lsb))
                {RAISE(Bitpack_Overflow);}
                if (lsb >64)
                {RAISE(Bitpack_Overflow);}
                if (width > 64)
                {RAISE(Bitpack_Overflow);}  
        EXCEPT(Bitpack_Overflow)
                RAISE(Bitpack_Overflow);
        END_TRY;  
        uint64_t mask = ~0;
        mask = mask << (width + lsb);
        uint64_t maskmerge = (pow(2, lsb) - 1);
        mask = (mask | maskmerge);   
        word = ( word & mask );
        word = ( word | (value << lsb));
        return word;
        
        
}

/* Bitpack_news return a new word which is identical to the original
 * word, except the field of width width with least significant
 * bit at lsb will have been replaced by a width-bit representation of
 * value. An exception is raised if any of the algebraic rules for width
 * or lsb are not followed.
 */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
                      int64_t value)
{
        TRY
                Bitpack_fitss(value,width);
                if (width > (64-lsb))
                {RAISE(Bitpack_Overflow);}
                if (lsb >64)
                {RAISE(Bitpack_Overflow);}
                if (width > 64)
                {RAISE(Bitpack_Overflow);}
                
        EXCEPT(Bitpack_Overflow)
                RAISE(Bitpack_Overflow);
        END_TRY;
        uint64_t mask = ~0;
        mask = mask << (width + lsb);
        uint64_t maskmerge = (pow(2, lsb) - 1);
        mask = (mask | maskmerge);
        word = ( word & mask );
        word = ( word | (value << lsb));
        return word;
}


