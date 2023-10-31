#include <stdio.h>
#include <math.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH UCHAR_MAX
#define HEIGHT WIDTH
#define CHANNELS 4
#define STRIDE WIDTH * CHANNELS
#define SIZE STRIDE * HEIGHT

struct Color {
    unsigned char r, g, b, a;
};

// Array to store pixel data
unsigned int data[SIZE] = {0};

/**
 * Main function to process the file and generate an image.
 * 
 * @param argc The number of command line arguments.
 * @param argv An array of command line arguments.
 * @return 0 if successful, 1 otherwise.
 */
int main(int argc, char** argv) {
    // Check if a file path is provided
    if (argc < 2) {
        printf("Usage: %s <path>\n", argv[0]);
        return 1;
    }

    // Open the file for reading
    FILE* fp = fopen(argv[1], "rb");
    if (!fp) {
        printf("Failed to open file: %s\n", argv[1]);
        return 1;
    }
    
    unsigned int max_value = 0;
    unsigned char x = 0, y = 0;
    
    // Read the file and update pixel data
    while (fread(&x, sizeof(char), 1, fp) && fread(&y, sizeof(char), 1, fp)) {
        ++(data)[y * WIDTH + x];
        max_value = data[y * WIDTH + x] > max_value ? data[y * WIDTH + x] : max_value;
    }
    fclose(fp);

    // Normalize pixel data and convert to RGBA format
    for (int i = 0; i < SIZE; ++i) {
        signed int value = logl(data[i] + 1) / logl(max_value + 1) * UCHAR_MAX;
        data[i] = *(int*)&(struct Color){
            .r = value,
            .g = value,
            .b = value,
            .a = 0xFF
        };
    }

    // Write the image to a PNG file
    if (!stbi_write_png("output.png", WIDTH, HEIGHT, CHANNELS, data, STRIDE)) {
        printf("Failed to write image\n");
        return 1;
    }

    printf("Generated image: output.png\n");
    return 0;
}