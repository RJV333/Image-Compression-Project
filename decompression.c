/*     
 *     decompression.c
 *     by Ahmed Aly & Ryan Vasios, 2/28/14
 *     Assignment 4
 *
 *     Functions used to decompress an image in the comp40 image
 *      format back into pnm format.
 */

#include <decompression.h>

/*
 * unpack takes in 8bit characters and combines them using
 * bitpack into one 64bit unsigned int. Then it unpacks the 
 * a,b,c,d,Pravg, and Pbavg from it and it stores this information
 * in a struct and returns a pointer to it.
 */
void unpack(int c1, int c2, int c3, int c4, abcd currentabcd)
{
        uint64_t word = 0;
        word = Bitpack_newu(word, 8, 0, (uint64_t)c4);
        word = Bitpack_newu(word, 8, 8, (uint64_t)c3);
        word = Bitpack_newu(word, 8, 16,  (uint64_t)c2);
        word = Bitpack_newu(word, 8, 24, (uint64_t)c1);       
        currentabcd-> a = Bitpack_getu(word, 6, 26);
        currentabcd-> b = Bitpack_gets(word, 6, 20);
        currentabcd-> c = Bitpack_gets(word, 6, 14);
        currentabcd-> d = Bitpack_gets(word, 6, 8);
        currentabcd-> Pbavg = Bitpack_getu(word, 4, 4);
        currentabcd-> Pravg = Bitpack_getu(word, 4, 0);
}



/*  ABCD_to_XYZ converts a,b,c,d, Pbavg, and Pravg values back to
 *  Y, Pb, and Pr values for each pixel.
 */
A2Methods_UArray2 ABCD_to_XYZ(UArray_T abcds, unsigned width, unsigned height)
{
       A2Methods_T methods = uarray2_methods_plain;
       A2Methods_UArray2 decomp_image = methods -> new (width, height, 
                                                      sizeof(struct XYZvals));
       int index = 0;
       abcd currentABCD;
       XYZvals currentXYZ1, currentXYZ2, currentXYZ3, currentXYZ4;
       
       for (unsigned row = 0; row < height; row = row + 2)
                for (unsigned col = 0 ; col < width; col = col + 2)
        {
                {
     
                currentXYZ1 = (XYZvals)
                                (methods->at(decomp_image, col, row));
                currentXYZ2 = (XYZvals)
                                (methods->at(decomp_image, col+1, row));
                currentXYZ3 = (XYZvals)
                                (methods->at(decomp_image, col, row+1));
                currentXYZ4 = (XYZvals)
                                (methods->at(decomp_image, col+1, row+1));
                
                currentABCD = (abcd)UArray_at(abcds, index);
        
                float a = (float)currentABCD->a/63;
                a = clip(a, 0, 1);
                float b = sint_to_float( currentABCD->b, 0.3, 31);
                b = clip(b, -.5, .5);
                float c = sint_to_float( currentABCD->c, 0.3, 31);
                c = clip(c, -.5, .5);
                float d = sint_to_float( currentABCD->d, 0.3, 31); 
                d = clip(d, -.5, .5);

                currentXYZ1->Y = a - b - c + d;
                currentXYZ1->Y = clip(currentXYZ1->Y, 0, 1); 
                currentXYZ2->Y = a - b + c - d;
                currentXYZ2->Y = clip(currentXYZ2->Y, 0, 1); 
                currentXYZ3->Y = a + b - c - d;
                currentXYZ3->Y = clip(currentXYZ3->Y, 0, 1); 
                currentXYZ4->Y = a + b + c + d;
                currentXYZ4->Y = clip(currentXYZ4->Y, 0, 1); 
                currentXYZ1->Pb = Arith40_chroma_of_index(currentABCD->Pbavg);
                currentXYZ1->Pb = clip(currentXYZ1->Pb, -.5, .5);
                currentXYZ2->Pb = Arith40_chroma_of_index(currentABCD->Pbavg);
                currentXYZ2->Pb = clip(currentXYZ2->Pb, -.5, .5);
                currentXYZ3->Pb = Arith40_chroma_of_index(currentABCD->Pbavg);
                currentXYZ3->Pb = clip(currentXYZ3->Pb, -.5, .5);
                currentXYZ4->Pb = Arith40_chroma_of_index(currentABCD->Pbavg);
                currentXYZ4->Pb = clip(currentXYZ4->Pb, -.5, .5);
                currentXYZ1->Pr = Arith40_chroma_of_index(currentABCD->Pravg);
                currentXYZ1->Pb = clip(currentXYZ1->Pb, -.5, .5);
                currentXYZ2->Pr = Arith40_chroma_of_index(currentABCD->Pravg);
                currentXYZ2->Pb = clip(currentXYZ2->Pb, -.5, .5);
                currentXYZ3->Pr = Arith40_chroma_of_index(currentABCD->Pravg);
                currentXYZ3->Pb = clip(currentXYZ3->Pb, -.5, .5);
                currentXYZ4->Pr = Arith40_chroma_of_index(currentABCD->Pravg);
                currentXYZ4->Pb = clip(currentXYZ4->Pb, -.5, .5);

                index++;
                }
        }
        return decomp_image;
}
/*
 * XYZ_to_RGB converts Y, Pb, and Pr values for each pixel into pnm format.The
 * function returns a pnm_ppm sturct pointer. It takes in an A2Methods_Uarray2
 * of Y, Pb, and Pr values.
 */
Pnm_ppm XYZ_to_RGB(A2Methods_UArray2 decompXYZs)
{
        A2Methods_T methods = uarray2_methods_plain;
        Pnm_ppm decompressedPNM = malloc( sizeof(struct Pnm_ppm) );
        decompressedPNM->width = methods->width(decompXYZs);
        decompressedPNM->height = methods->height(decompXYZs);
        decompressedPNM->denominator = 255;
        decompressedPNM->pixels = methods->new(decompressedPNM->width, 
                           decompressedPNM->height, sizeof(struct Pnm_rgb) );
        
        Pnm_rgb pixelPointer= NULL;
        XYZvals currentXYZ = NULL;
             for (unsigned row = 0; row < decompressedPNM->height; row++)
                for (unsigned col = 0 ; col < decompressedPNM->width; col++)
        {
                {
                       pixelPointer = methods->at
                                        (decompressedPNM->pixels, col, row);
                       currentXYZ = methods->at(decompXYZs, col, row);
                        
                       float r =  ( ( 1 * currentXYZ->Y ) + ( 0.0 * 
                            currentXYZ->Pb ) + (1.402 * currentXYZ->Pr) );
                       r = clip(r, 0, 1);                        
                       float g=  ( ( 1 * currentXYZ->Y ) +  ( -0.344136 * 
                            currentXYZ->Pb )+ (-0.714136 * currentXYZ->Pr));
                       g = clip(g, 0, 1);
                       float b =   ( ( 1 * currentXYZ->Y ) + ( 1.772 * 
                                   currentXYZ->Pb )+ (0 * currentXYZ->Pr));
                       b = clip(b, 0, 1);

                       pixelPointer-> red = (unsigned)(r * decompressedPNM
                                                              ->denominator);
                       pixelPointer->red = clip_unsigned( pixelPointer->
                                                                red, 0, 255);
                       pixelPointer-> green = (unsigned)(g * decompressedPNM
                                                              ->denominator);
                       pixelPointer->green = clip_unsigned( pixelPointer
                                                            ->green, 0, 255);
                       pixelPointer-> blue = (unsigned)(b * decompressedPNM
                                                              ->denominator);
                       pixelPointer->blue = clip_unsigned( pixelPointer
                                                              ->blue, 0, 255);

                }
        }
        
        return decompressedPNM;
}

/* Function sint_to_float converts a quantized signed integer back to
 * to an unquantized float. It take the quantized integer, it's quantized
 * upper bound and the unquantized upper bound.
 */
float sint_to_float(int integer, float fl_upbound, unsigned int_upbound) 
{
        float fl =(float)integer*(fl_upbound/int_upbound);
        return fl;
}

 
/* decompression_caller takes in the a, b, c, d, Pbavg, and Pravg
 * values and calls all the functions necessary to decompress back
 * into pnm format.  
 */
void decompression_caller(UArray_T abcds, unsigned width, unsigned height)
{
       
       A2Methods_UArray2 XYZarray= ABCD_to_XYZ(abcds, width, height);                
        
       Pnm_ppm decomp_pixmap = XYZ_to_RGB(XYZarray);
       

        A2Methods_T methods = uarray2_methods_plain;
        printf("P6\n%u %u\n%u\n", width, height, 255);
        for (unsigned row=0 ; row < height; row++)
        {
                for( unsigned col = 0; col < width; col++)
                {
                Pnm_rgb currentRGB = methods->at(decomp_pixmap->pixels,
                                                              col, row);
                printf("%c%c%c", currentRGB->red, currentRGB->green, 
                                                       currentRGB->blue);
                }
        }
        methods->free(&decomp_pixmap->pixels);
        free(decomp_pixmap);
        methods->free(&XYZarray);
}

