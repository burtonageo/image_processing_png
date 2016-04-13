#ifndef IMAGE_PROCESSING_PNG_H
#define IMAGE_PROCESSING_PNG_H

#include <memory>
#include <stdlib.h>
#include <stdint.h>

namespace png {
    struct Chunk {
        uint32_t length;
        uint32_t chunk_type;
        uint8_t *data;
        uint32_t checksum;
    };

    struct Image {
        Chunk* chunks;
    };

    enum ParseError {
        NoError = 0,
        FileError,
        MalformedHeader
    };

    struct ParseResult {
        Image image;
        ParseError error;
    };

    ParseResult parse_file(std::unique_ptr<FILE, int(*)(FILE*)>&& file);
}

#endif // IMAGE_PROCESSING_PNG_H
