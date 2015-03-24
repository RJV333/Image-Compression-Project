/*     
 *     compression.c
 *     by Ahmed Aly & Ryan Vasios, 2/28/14
 *     Assignment 4
 *
 *     
 */

#include <compression.h>

/* RGB_to_XYZ examines the rgb values in a pixel of the original image and 
 *from them calculates the XYZ values for the corresponding col and rox in the 2D
 * array of XYZvals, cmpimage, within the closure, cl
 */
void RGB_to_XYZ( int col, int row, A2Methods_UArray2 pgbArray, void *pixel,
                                                                 closure cl)
{
        XYZvals xyzpointer = cl->method-> at (cl->cmpimage, col, row);
        (void)pixel;
        (void) cl;       

        struct Pnm_rgb* rgbVals = (struct Pnm_rgb*)(cl->method->at 
                                                (pgbArray, col, row));
        
        float r = rgbVals->red / cl->XYZdenom;
        float g = rgbVals->green / cl->XYZdenom; 
        float b = rgbVals->blue / cl->XYZdenom;
        
        
        
        xyzpointer->Y = 0.299 * r + 0.587 * g + 0.114 * b;
        xyzpointer->Y = clip (xyzpointer->Y, 0, 1);
        xyzpointer->Pb = -0.168736 * r - 0.331264 * g + 0.5 * b;
        xyzpointer->Pb = clip (xyzpointer->Pb, -.5, .5);
        xyzpointer->Pr = 0.5 * r - 0.418688 *g -0.081312 * b;
        xyzpointer->Pr = clip (xyzpointer->Pr, -.5, .5);
        
        
} 

/* XYZ_to_ABCD calculates the abcd values for every four pixels of XYZvals
 * in a 2*2 block. It places these values within the one dimensions Uarray
 * abcds
 */
void XYZ_to_ABCD(int row, int col, UArray_T abcds, 
                A2Methods_UArray2 comp_image, int index, A2Methods_T methods)
{
        abcd currentABCD = UArray_at(abcds,index);
        XYZvals currentXYZ1 = (XYZvals)(methods->at(comp_image, col, row));
        XYZvals currentXYZ2 = (XYZvals)(methods->at(comp_image, col+1, row));
        XYZvals currentXYZ3 = (XYZvals)(methods->at(comp_image, col, row+1));
        XYZvals currentXYZ4 = (XYZvals)(methods->at(comp_image, col+1, row+1));

        float Y1 = currentXYZ1->Y;
        float Y2 = currentXYZ2->Y;
        float Y3 = currentXYZ3->Y;
        float Y4 = currentXYZ4->Y;
        
        float a = (Y1 + Y2 + Y3 + Y4) / 4;
        float b = (Y4 + Y3 - Y2 - Y1) / 4;
        float c = (Y4 - Y3 + Y2 - Y1) / 4;
        float d = (Y4 - Y3 - Y2 + Y1) / 4;

        currentABCD->a = (int)floor(a*63);
        currentABCD->b = float_to_sint(b, 0.3, -0.3, 31);
        currentABCD->c = float_to_sint(c, 0.3, -0.3, 31); 
        currentABCD->d = float_to_sint(d, 0.3, -0.3, 31);

        
        float pbavg = ((currentXYZ1-> Pb) + (currentXYZ2->Pb) + (currentXYZ3->Pb) + (currentXYZ4->Pb))/4;
        currentABCD->Pbavg = Arith40_index_of_chroma(pbavg);
        float pravg = ((currentXYZ1-> Pr) + (currentXYZ2->Pr) + (currentXYZ3->Pr) + (currentXYZ4->Pr))/4;
        currentABCD->Pravg = Arith40_index_of_chroma(pravg);      
}

/* float_to_sint takes in a float, its lower and upper bounds, and the value
 * of its quanitizied increments.  It returns a signed integer which is the 
 * quantizied value of its original float
 */
int float_to_sint(float fl, float fl_upbound, float fl_lowbound, unsigned int_upbound)
{
        if (fl<fl_lowbound) {fl=fl_lowbound;}
        if (fl>fl_upbound) {fl=fl_upbound;}
        int n = (int)floor(fl*(int_upbound/fl_upbound));
        return n;
}
/* clip performs an operation that is important to repeatedly call thoughout
 * all stages of decompression.  It makes sure that a value is within its 
 * permitted range, assigning it to its upper or lower bound if it is outside
 * this range
 */
float clip(float value, float lowbound, float upbound)
{

        if (value < lowbound)
                value = lowbound;
        if (value > upbound)
                value = upbound;
       
        return value;

}

/* clip_unsigned performs the same operation as clip but handles the 
 * type, unsigned
 *
 */
unsigned clip_unsigned(unsigned value, unsigned lowbound, unsigned upbound)
{

        if (value < lowbound)
                value = lowbound;
        if (value > upbound)
                value = upbound;
       
        return value;

}
/* packAllBits takes in a UArray of abcd values and a UArray of packed bits
 * for every struct of abcd values within the first array it creates a
 * packed bit and places it in the second array
 */
void packAllBits(UArray_T abcds,UArray_T bitpackArray)
{
      int len = UArray_length(abcds); 
      uint64_t* bitpackpointer = NULL;
      abcd currentabcd = NULL;
      
      for (int i=0; i< len; i++)
      {
          bitpackpointer = UArray_at(bitpackArray, i);
          currentabcd = UArray_at(abcds, i);
          *bitpackpointer = pack(currentabcd); 
      }
}
/* pack takes in a pointer to a abcd struct, makes sure every value
 * would fit in its alloted bit field, and then places those values
 * within their appropriate field in the packed bit.
 */
uint64_t pack(abcd currentabcd)
{
        /*check if the values fit in a uint64_t/int64_t*/
        Bitpack_fitsu((uint64_t)currentabcd->a, 6);
        Bitpack_fitss((int64_t)currentabcd->b, 6);
        Bitpack_fitss((int64_t)currentabcd->c, 6);
        Bitpack_fitss((int64_t)currentabcd->d, 6);
        Bitpack_fitsu((uint64_t)currentabcd->Pbavg, 4);
        Bitpack_fitsu((uint64_t)currentabcd->Pravg, 4);
        uint64_t word = 0;
        word = Bitpack_newu(word, 4, 0, (uint64_t)currentabcd->Pravg);
        word = Bitpack_newu(word, 4, 4, (uint64_t)currentabcd->Pbavg);
        word = Bitpack_news(word, 6, 8,  (int64_t)currentabcd->d);
        word = Bitpack_news(word, 6, 14, (int64_t)currentabcd->c);
        word = Bitpack_news(word, 6, 20, (int64_t)currentabcd->b);
        word = Bitpack_newu(word, 6, 26, (uint64_t)currentabcd->a);
        
        return word;

}
/* printAllBits takes in the bitpackArray and the width and height of the
 * image its decompression would create.  It prints the header detailed in
 * the assignment and then prints each packed bit in the bitpackarry in
 * BigEndian order
 */
void printAllBits(UArray_T bitpackArray,unsigned width,unsigned height)
{ 
        printf("COMP40 Compressed image format 2\n%u %u", width, height);
        printf("\n");       
        int len = UArray_length(bitpackArray);
        uint64_t* bitpackpointer;
        
        for (int i = 0; i < len; i++)
        {
                bitpackpointer = UArray_at(bitpackArray, i);
                
                printBigEndian(*bitpackpointer);
        
        }
}
/* printBigEndian takes in a uint64_t word and then prints it in BigEndian
 * order.  It does this through a for loop the decrements the lsb of the byte
 * being printed, working back through higher order bits to the lower
 */
void    printBigEndian(uint64_t word)
{
        char tempchar;      
        uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb);
        for(int i=24; i >=0; i=i-8)
        {
                tempchar= (char)Bitpack_getu(word, 8, i);
                putchar(tempchar);
        }       
}