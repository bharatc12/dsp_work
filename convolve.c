/**
 * @file convolve.c
 * @brief convolution of 2 discrete signals done without parallelization
 */
#include "convolve.h" //defines min and max helper function used for determining window of convolution
#include "read_vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

//symbolic constants because its annoying to dynamically resize more stuff the needed
#define INITIAL_CAPACITY 10
#define MAX_FILES 10

/**
 * @brief Convolves two discrete signals.
 *
 * This function takes two floating point arrays along with their lengths and returns their convolution output.
 *
 * @param h[] First signal
 * @param x[] Second signal
 * @param lenH Length of first signal
 * @param lenX Length of second signal
 * @param lenY Pointer to length of output
 * @return Convolution of h[] and x[]
 */
float* convolve(float h[], float x[], int lenH, int lenX, int* lenY)
{
	int nconv = lenH + lenX - 1; //setting length of convolution output
	(*lenY) = nconv; //assign length to respective variables
	int i, j, h_start, x_start, x_end; //declare variables
	
	float *y = (float*) calloc(nconv, sizeof(float)); //allocate memory space with initalized values of all 0s
	
	for (i=0; i < nconv; i++)
	{
	
	x_start = MAX(0, i-lenH+1); //sliding the window of convolution after
	x_end = MIN(i+1, lenX);	    //each index of the output is calculated
	h_start = MIN(i, lenH-1);
	
		for(j=x_start; j<x_end; j++)
		{
	  	y[i] += h[h_start--]*x[j]; //computing the weighted sum within the window	
		}
	
	}
	
	return y;
}

/**
 * @brief Tests code by running function with signals sythesized in Matlab
 * 
 * In addition it times the preformance as well as writes the output to a txt file for verification externally
 *
 */
int main(int argc, char *argv[])
{
    const char *filenames[] = {
        "signalarray.txt",
        "highpass_filter.txt",
        "bandpass_filter.txt"
    };
    int num_files_to_process = sizeof(filenames) / sizeof(filenames[0]);

    double *all_data_vectors[MAX_FILES];
    int all_vector_sizes[MAX_FILES];
    int files_successfully_loaded = 0;

    for (int i = 0; i < MAX_FILES; ++i) {
        all_data_vectors[i] = NULL;
        all_vector_sizes[i] = 0;
    }

    for (int i = 0; i < num_files_to_process && i < MAX_FILES; i++) {
        const char *current_filename = filenames[i];
        int vector_size = 0;
        double *data_vector = read_vector_from_file(current_filename, &vector_size);

        if (data_vector != NULL) {
            all_data_vectors[files_successfully_loaded] = data_vector;
            all_vector_sizes[files_successfully_loaded] = vector_size;
            files_successfully_loaded++;
        } else {
            printf("Failed to read data from %s\n", current_filename);
        }
    }

    if (files_successfully_loaded < 2) {
        fprintf(stderr, "Need at least signal + one filter loaded.\n");
        return 1;
    }

    // Prepare host signals
    float *signal = (float *)all_data_vectors[0];
    int lenX = all_vector_sizes[0];
    float *filter = (float *)all_data_vectors[1];
    int lenH = all_vector_sizes[1];

    // GPU memory
    float *d_signal, *d_filter, *d_output;
    cudaMalloc((void**)&d_signal, lenX * sizeof(float));
    cudaMalloc((void**)&d_filter, lenH * sizeof(float));
    cudaMalloc((void**)&d_output, lenX * sizeof(float));

    cudaMemcpy(d_signal, signal, lenX * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_filter, filter, lenH * sizeof(float), cudaMemcpyHostToDevice);

    int blockSize = 256;
    int gridSize = (lenX + blockSize - 1) / blockSize;

    int repeat = 10000;  // hidden from output
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start, 0);
    for (int i = 0; i < repeat; i++) {
        convolutionParallel<<<gridSize, blockSize>>>(d_signal, d_filter, d_output, lenX, lenH);
    }
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);

    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop);

    // Only print total runtime in seconds
    printf("Execution time: %f seconds\n", elapsedTime / 1000.0);

    // Copy back one result (optional)
    float *output = (float*) malloc(lenX * sizeof(float));
    cudaMemcpy(output, d_output, lenX * sizeof(float), cudaMemcpyDeviceToHost);

    // Cleanup
    free(output);
    cudaFree(d_signal);
    cudaFree(d_filter);
    cudaFree(d_output);

    for (int i = 0; i < files_successfully_loaded; i++) {
        free(all_data_vectors[i]);
    }

    return 0;
}

