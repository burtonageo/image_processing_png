#include "image_processing_png.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

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

size_t get_file_size(const std::unique_ptr<FILE, int(*)(FILE*)>& file) {
    fseek(&*file, 0, SEEK_END);
    size_t len = ftell(&*file);
    rewind(&*file);
    return len;
}

}

namespace png {

ParseResult parse_file(std::unique_ptr<FILE, int(*)(FILE*)>&& file) {
    assert(file != nullptr);
    if (file == nullptr) {
        return ParseResult { Image { nullptr }, FileError };
    }

    const size_t file_size = get_file_size(file);
    uint8_t* buffer = (uint8_t*)malloc(file_size);

    // Parse header
    constexpr uint32_t PNG_HEADER_LENGTH = 8;
    constexpr uint32_t HEADER_SIZE = PNG_HEADER_LENGTH * sizeof(uint8_t);
    {
        // Check if there is enough room in the file for a complete header
        if (file_size < HEADER_SIZE) {
            return ParseResult::error(MalformedHeader);
        }

        size_t result = fread(buffer, 1, HEADER_SIZE, &*file);
        if (result != HEADER_SIZE) {
            return ParseResult::error(MalformedHeader);
        }

        // Check the high bit
        if (buffer[0] != 0x89) {
            return ParseResult::error(MalformedHeader);
        }

        // Check the 'PNG' ascii marker
        if (!match_range((const char*)&buffer[1], "PNG", 3)) {
            return ParseResult::error(MalformedHeader);
        }

        // Check the line endings
        static const char LINE_ENDINGS[] = {0x0D, 0x0A, 0x1A, 0x0A};
        if (!match_range((const char*)&buffer[4], (const char*)LINE_ENDINGS, 4)) {
            return ParseResult::error(MalformedHeader);
        }
    }

    // Parse chunks
    {
        uint32_t current_chunk = 0;
        while (!feof(&*file)) {
            struct ChunkHeader { uint32_t len, ty; };
            size_t result = fread(buffer, 1, sizeof(ChunkHeader), &*file);
            if (result != 0) {
                return ParseResult::error(BadChunk);
            }
            ChunkHeader* header = (ChunkHeader*)buffer;
            // uint8_t* chunk_data = (uint8_t*)malloc(header->len);
            switch (header->ty) {
                case IHDR:
                    // Dimensions dimensions;
                    break;
                default:
                    break;
            }
            current_chunk += 1;
        }
    }

    Chunk* chunks = (Chunk*)buffer;
    return ParseResult::ok(Image { chunks });
}

}
