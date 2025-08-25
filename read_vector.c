// read_vector.c
#include "read_vector.h"
#include <stdio.h>
#include <stdlib.h> // For malloc, realloc, free
#include <errno.h>  // For perror

/**
 * @brief Reads a vector of floats from a file.
 *
 * This function reads floating-point numbers from a specified file,
 * dynamically allocates memory for them, and returns a pointer to the array.
 *
 * @param filename The path to the input file.
 * @param size Pointer to an integer where the number of elements read will be stored.
 * @return A pointer to the dynamically allocated array of floats, or NULL on failure.
 */
float* read_vector_from_file(const char *filename, int *size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        *size = 0;
        return NULL;
    }

    float *data = NULL;
    int capacity = 10; // Initial capacity
    int count = 0;
    float value;

    // Allocate initial memory
    data = (float*)malloc(capacity * sizeof(float));
    if (data == NULL) {
        perror("Error allocating initial memory");
        fclose(file);
        *size = 0;
        return NULL;
    }

    while (fscanf(file, "%f", &value) == 1) { // Read floats
        if (count == capacity) {
            capacity *= 2; // Double the capacity
            float *temp = (float*)realloc(data, capacity * sizeof(float));
            if (temp == NULL) {
                perror("Error reallocating memory");
                free(data);
                fclose(file);
                *size = 0;
                return NULL;
            }
            data = temp;
        }
        data[count++] = value;
    }

    fclose(file);

    // Reallocate to exact size if needed, or just set the size
    if (count > 0) {
        float *temp = (float*)realloc(data, count * sizeof(float));
        if (temp == NULL) {
            // Realloc failed, but data is still valid at original size
            fprintf(stderr, "Warning: Failed to reallocate to exact size. Using current capacity.\n");
            *size = count;
            return data;
        }
        data = temp;
    } else {
        // No data read, free allocated memory
        free(data);
        data = NULL;
    }

    *size = count;
    return data;
}
