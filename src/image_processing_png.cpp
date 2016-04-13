#include "image_processing_png.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace {

bool match_range(const char* __restrict first_text, const char* __restrict second_text, uint32_t range) {
    for (uint32_t i = 0; i < range; i++) {
        char c0 = first_text[i], c1 = second_text[i];
        if (c0 == c1) {
            continue;
        }
        return false;
    }
    return true;
}

}

namespace png {

ParseResult parse_file(std::unique_ptr<FILE, int(*)(FILE*)>&& file) {
    assert(file != nullptr);

    const uint32_t file_size = [&]() {
        fseek(&*file, 0, SEEK_END);
        uint32_t len = ftell(&*file);
        rewind(&*file);
        return len;
    }();

    // Parse header
    {
        static const uint32_t PNG_HEADER_LENGTH = 8;
        static const uint32_t HEADER_SIZE = PNG_HEADER_LENGTH * sizeof(uint8_t);

        // Not enough room in the file for a complete header
        if (file_size < PNG_HEADER_LENGTH) {
            return ParseResult { Image { nullptr}, MalformedHeader };
        }

        uint8_t* buffer = (uint8_t*)malloc(HEADER_SIZE);
        size_t result = fread(buffer, 1, HEADER_SIZE, &*file);
        if (result != HEADER_SIZE) {
            return ParseResult { Image { nullptr }, FileError };
        }

        // Check the high bit
        if (buffer[0] != 0x89) {
            return ParseResult { Image { nullptr}, MalformedHeader };
        }

        // Check the 'PNG' ascii marker
        if (!match_range((const char*)&buffer[1], "PNG", 3)) {
            return ParseResult { Image { nullptr}, MalformedHeader };
        }

        // Check the line endings
        static const char LINE_ENDINGS[] = {0x0D, 0x0A, 0x1A, 0x0A};
        if (!match_range((const char*)&buffer[4], (const char*)LINE_ENDINGS, 4)) {
            return ParseResult { Image { nullptr}, MalformedHeader };
        }
    }

    return ParseResult { Image { nullptr}, NoError };
}

}
