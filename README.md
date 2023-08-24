# Multithreaded Image Box Blur with lodepng**

This program performs a box blur operation on an image using multiple threads and the lodepng library for image manipulation. The blur is applied to every 3x3 patch of pixels in the image, and the results are saved to an output image.

**Program Overview:**

1. Include necessary libraries: lodepng.h, pthread.h, stdio.h, stdlib.h.

2. Define a struct `thread_params` to hold parameters for each thread's execution.

3. Implement the threaded function `apply_box_blur` that calculates the box blur for a specific portion of the image.

4. In the `main` function:

   - Define the number of threads (NUM_THREADS).
   - Load the image using lodepng.
   - Calculate the number of 3x3 blocks for blur.
   - Divide the blocks into chunks for parallel processing.
   - Create threads for each chunk, passing the required parameters.
   - Wait for all threads to finish.
   - Save the resulting image.
   - Free allocated memory.

**Usage:**

1. Compile the program:
   ```
   gcc boxblur.c lodepng.c -o boxblur -lpthread
   ```

2. Run the compiled program:
   ```
   ./boxblur
   ```

**Program Explanation:**

1. The program begins by including the necessary headers and defining a `thread_params` struct to hold thread-specific data.

2. The `apply_box_blur` function takes a struct of parameters as input and calculates the average color values for each 3x3 patch in a specific portion of the image. The calculated averages replace the original color values for each pixel in the patch.

3. In the `main` function:

   - The number of threads (NUM_THREADS) is defined.
   - The input image ("sample.png") is loaded using the lodepng library.
   - The number of 3x3 blocks in the image is calculated.
   - The image blocks are divided into chunks for parallel processing.
   - Threads are created to apply the box blur to each chunk.
   - The program waits for all threads to complete.
   - The resulting image is saved as "out.png".

4. The program utilizes the `pthread` library for multithreading and the `lodepng` library for image I/O.

**Note:**

- Ensure that the "sample.png" image file is present in the same directory as the program.
- The `lodepng` library must be available and properly linked during compilation (`-lpthread` for pthread support).

This program demonstrates a multithreaded approach to applying a box blur effect to an image, enhancing performance by parallelizing the computation of color averages.
