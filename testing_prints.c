#include <stdio.h>
#include <a2methods.h>
#include <uarray.h>
#include <a2plain.h>
#include "compression.h"

/* printABCDS takes in a UArray of abcd values and its length
 * It then iterates through the array clearly printing the values 
 * in each struct abcd
 */
void printABCDS (UArray_T abcds, int Arrlength)
{

        for (int index=0 ; index < Arrlength; index++)
        {
         abcd currentABCD = UArray_at(abcds,index);
         printf("a: %d, b: %d, c: %d, d: %d, Pb: %d, Pr: %d\n", 
                currentABCD-> a, currentABCD->b,  currentABCD-> c,
                 currentABCD-> d, currentABCD-> Pbavg, currentABCD-> Pravg);  

        }

}
/*print_RGBs prints an array of struct RGBS such as that
 * contained in Pnm_ppm.  
 *
 */
void print_RGBs(A2Methods_UArray2 RGBS)
{
        A2Methods_T methods = uarray2_methods_plain;
        int width = methods->width(RGBS);
        int height = methods->height(RGBS);
        printf("RGB ARRAY: FINAL WIDTH: %d  FINAL HEIGHT: %d\n",
                                                 height, width);
        
        for (int row=0 ; row < height; row++)
        {
                for( int col = 0; col < width; col++)
                {
                Pnm_rgb currentRGB = methods->at(RGBS, col, row);
                printf("r: %u, g: %u, b: %u\n", currentRGB->red, 
                                                currentRGB->green, 
                                                currentRGB->blue);     
                }
        }
        
}
/* printXYZ_A2UArray prints all the XYZ values within an A2Methods_Uarray2
 *
 *
 */
void printXYZ_A2UArray (A2Methods_UArray2 XYZ)
{
        A2Methods_T methods = uarray2_methods_plain;
        int width = methods->width(XYZ);
        int height = methods->height(XYZ);
        printf("XYZ ARRAY: FINAL WIDTH: %d  FINAL HEIGHT: %d\n", 
                                                width, height);
        
        for (int row=0 ; row < height; row++)
        {
                for( int col = 0; col < width; col++)
                {
                XYZvals currentXYZ = methods->at(XYZ, col, row);
                printf("Y: %f, Pb: %f, Pr: %f\n", currentXYZ-> Y, 
                                currentXYZ->Pb,  currentXYZ-> Pr);     
                }
        }
        
}
