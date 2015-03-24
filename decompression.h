/*     
 *     decompression.h
 *     by Ahmed Aly & Ryan Vasios, 2/28/14
 *     Assignment 4
 *
 *     Functions used for the decompression of image in comp40 image
 *     format into pnm format 
 */

#ifndef DECOMPRESSION_INCLUDED
#define DECOMPRESSION_INCLUDED

#include <compression.h>



float sint_to_float(int integer, float fl_upbound, unsigned int_upbound);
A2Methods_UArray2 ABCD_to_XYZ(UArray_T abcds, unsigned width, unsigned height);
Pnm_ppm XYZ_to_RGB(A2Methods_UArray2 decompXYZs);
void decompression_caller(UArray_T abcds, unsigned width, unsigned height);
void unpack(int c1, int c2, int c3, int c4, abcd currentabcd);


#endif