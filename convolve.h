/**
 * @file convolve.h
 * @brief min and max helper functions in addition to function prototype.
 */
#include <stdlib.h>
#include <stdio.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y)) 
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))


float* convolve(float h[], float x[], int lenH, int lenX, int* lenY);
