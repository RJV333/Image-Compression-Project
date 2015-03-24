#ifndef TESTING_PRINTS_INCLUDED
#define TESTING_PRINTS_INCLUDED
#include <stdbool.h>
#include <stdio.h>

/* These three printing functions have been invaluable for testing
 * both compression and decompression at all levels
 */

extern void printABCDS (UArray_T abcds, int index);
void printXYZ_A2UArray (A2Methods_UArray2 XYZ);
void print_RGBs(A2Methods_UArray2 RGBS);


#endif