/*     
 *     compression.h
 *     by Ahmed Aly & Ryan Vasios, 2/28/14
 *     Assignment 4
 *
 *     
 */

#ifndef COMPRESSION_INCLUDED
#define COMPRESSION_INCLUDED

#include <a2blocked.h>
#include <a2methods.h>
#include <uarray.h>
#include <a2plain.h>
#include "pnm.h"
#include <math.h>
#include "arith40.h"
#include "stdio.h"
#include "stdlib.h"
#include <bitpack.h>

/*STRUCTS
 * These unique data structures are used in both compressing
 * and decompresing files
 */

/*struct XYZvals
 * Extrapolated from the r,g,b values of each pixel.
 * One struct is extrapolated per each pixel of original image
 */
typedef struct XYZvals {
        float Y, Pb, Pr;
} *XYZvals;

/*closure argument for original RGB to XYZ mapping conversion
 */
typedef struct closure {
        A2Methods_UArray2 cmpimage;
        A2Methods_T method;
        float XYZdenom;
} *closure;

/* struct abcd
 * Extrapolated from the XYZvals of every 4 pixels
 * One sruct is made per 2*2 block of XYZvals
 */
typedef struct abcd {
        int a,b,c,d;
        unsigned Pbavg, Pravg;
} *abcd;



void RGB_to_XYZ( int c, int r, A2Methods_UArray2 pgbArray, 
                 void *pixel,closure cl);
void XYZ_to_ABCD(int row, int col, UArray_T abcds, 
                 A2Methods_UArray2 comp_image, int index, A2Methods_T);
int float_to_sint(float fl, float fl_upbound, float fl_lowbound,
                  unsigned int_upbound);
float clip(float value, float lowbound, float upbound);
unsigned clip_unsigned(unsigned value, unsigned lowbound, unsigned upbound);
uint64_t pack(abcd currentabcd);
void    printBigEndian(uint64_t word);
void printAllBits(UArray_T bitpackArray,unsigned width,unsigned height);
void packAllBits(UArray_T abcds,UArray_T bitpackArray);

#endif
