#include "lodepng.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// compile with: 
// gcc boxblur.c lodepng.c

/*
    @author shawkim@stcmalta.edu.mt
    
*/

struct thread_params {
    unsigned char* image;
    unsigned width;
    unsigned height;
    int chunk_start;
    int chunk_end;
};
// threaded function that computes the box blur on a specific part of the image
void* apply_box_blur(void* args) {

    // extract parameters
    struct thread_params* params = (struct thread_params*) args;
    unsigned char* image = params->image;
    unsigned width = params->width;
    unsigned height = params->height;
    int chunk_start = params->chunk_start;
    int chunk_end = params->chunk_end;


    // extract 3x3 patches for every chunk
    for (int i = chunk_start; i < chunk_end; i++) {
        int x = i % width;
        int y = i / width;

        // skip the edges
        if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
            continue;
        }

        // compute the average of the 3x3 patch
        int sum_r = 0;
        int sum_g = 0;
        int sum_b = 0;
        for (int j = -1; j <= 1; j++) {
            for (int k = -1; k <= 1; k++) {
                int index = (y + j) * width * 4 + (x + k) * 4;
                sum_r += image[index];
                sum_g += image[index + 1];
                sum_b += image[index + 2];
            }
        }
        int avg_r = sum_r / 9;
        int avg_g = sum_g / 9;
        int avg_b = sum_b / 9;

        // replace the 3x3 patch with the average
        for (int j = -1; j <= 1; j++) {
            for (int k = -1; k <= 1; k++) {
                int index = (y + j) * width * 4 + (x + k) * 4;
                image[index] = avg_r;
                image[index + 1] = avg_g;
                image[index + 2] = avg_b;
            }
        }
    }

    return NULL;
}

int main() {

    // define the number of threads
    int NUM_THREADS = 4;

    // load image
    const char* filename = "sample.png";

    // use lodepng to load the image
    unsigned error;
    unsigned char* image;
    unsigned width, height;
    error = lodepng_decode32_file(&image, &width, &height, "sample.png");
    if(error) printf("error %u: %s", error, lodepng_error_text(error));

    // compute how many 3x3 blocks there are with stride 1
    int num_blocks = (width - 2) * (height - 2);

    // divide the blocks into chunks
    int chunk_size = num_blocks / NUM_THREADS;

    // if chunk_size is 0, then NUM_THREADS > num_blocks
    // in this case, we just use 1 thread
    if (chunk_size == 0) {
        NUM_THREADS = 1;
        chunk_size = num_blocks;
    }

    // for every 3x3 block, create a thread and apply the box blur
    pthread_t* threads = malloc(NUM_THREADS * sizeof(pthread_t));
    struct thread_params* params = malloc(NUM_THREADS * sizeof(struct thread_params));
    for (int i = 0; i < NUM_THREADS; i++) {
        params[i].image = image;
        params[i].width = width;
        params[i].height = height;
        params[i].chunk_start = i * chunk_size;
        params[i].chunk_end = (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, apply_box_blur, &params[i]);
    }

    // wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // save the image
    lodepng_encode32_file("out.png", image, width, height);

    // free memory
    free(image);
    free(threads);
    free(params);

    return 0;
}
// gcc boxblur.c loadepng.c >> result , ./a.out 
//after the png blur, result, > out.png