/*
 * HW4: ARITH README.txt
 *
 * submitted by Ahmed Aly and Ryan Vasios
 *
 * logins: aaly01 and rvasio01
 *
 * 
 * To our knowledge all portions of the assignment have been implemented
 *   correctly
 *
 *ACKNOWLEDGEMENTS
 *  As always, we wish to acknowledge and thank the course TAs for their 
 *  assistance on the project. Their advice on "clipping" values throughout
 *  decompression was especially useful in creating useful output
 *
 * PAIR PROGRAMMING 
 *    All portions of the assignment were completed using pair programming.
 *   
 *TIME SPENT
 *    Approximately 25 hours were spent on the assignment with several hours
 *  of independent thought and research as well for each of us
 *
 *
 *
 *
 * .H FILES INCLUDED AND THEIR ROLES 
 *
 * Our program contains the following pertinent .h files
 *
 * compression.h - functions used for the process of compression, as 
 *      well as a few minor functions useful for both compression and 
 *      decompression such as "clipping"
 *
 * decompression.h - functions used for the process of decompression
 *
 * bitpack.h - functions used for the packing of bits
 *
 * testing_prints.h - functions used to print the values of an image in the 
 *      process of compressing or decompressing. This enabled us to compare
 *      parallel values from an image getting decompressed to those of the'same'
 *      image being initially compressed
 *
 *
 *
 *  BRIEF DIAGRAM:
 *
 * IMAGE TO COMPRESS.PNM                     DECOMPRESSED IMAGE.PNM
 *      |                                       ^
 *      V                                       |   
 *    RGB values (1 per img pixel)          RGB values
 *      |                                       ^
 *      V                                       |  
 *    XYZ values (1 per img pixel)           XYZ Values 
 *      |                                       ^
 *      V                                       |   
 *   abcd values (1 per every 4 img pixels) abcd values
 *      |                                       ^
 *      v                                       |
 *   COMPRESSEDIMAGE.TXT                    COMPRESSEDIMAGE.TXT
 *
 *
 *
 *
 *
 * BE SURE TO CHECK THE SAMPLE INPUT/OUT FILES OF 
 *
 * BOSCH.PNM (ORIGINAL IMAGE)
 * BOSCH.TXT (COMPRESSED IMAGE TEXT)
 * BOSCH2.PNM (DECOMPRESSED IMAGE)
 */
