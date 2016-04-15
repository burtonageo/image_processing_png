#include <memory>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_processing_png.h"

int main(int argc, const char* const* argv) {
    if (argc < 2) {
        printf("Usage: image_processing [FILE]\n");
        return EXIT_FAILURE;
    }

    const char* filename = argv[1];
    uintptr_t len = strlen(filename);
    if (strcmp(&filename[len - 5], ".png") == 0) {
        printf("Filepath %s must point to a png file\n", filename);
        return EXIT_FAILURE;
    }

    std::unique_ptr<FILE, int(*)(FILE*)> file(fopen(filename, "r"), &fclose);
    if (file == nullptr) {
        printf("File does not exist!\n");
        return EXIT_FAILURE;
    }

    auto png = png::parse_file(std::move(file));

    if (png.err != png::NoError) {
        printf("Error parsing file %s\n", filename);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
