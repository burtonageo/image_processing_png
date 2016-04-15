#ifndef IMAGE_PROCESSING_PNG_H
#define IMAGE_PROCESSING_PNG_H

#include <memory>
#include <stdlib.h>
#include <stdint.h>

namespace png {
    /// A chunk of a png image
    struct Chunk {
        uint32_t length;
        uint32_t chunk_type;
        uint8_t *data;
        uint32_t checksum;
    };

    /// A complete png image
    struct Image {
        Chunk* chunks;
    };

    /// Constants for the critical chunk types of a png image
    enum CriticalChunks: uint32_t {
        IHDR = 'RDHI',
        PLTE = 'ETLP',
        IDAT = 'TADI',
        IEND = 'DNEI'
    };

    /// Constants for the optional chunk types of a png image
    enum AnciliaryChunks: uint32_t {
        bKGD = 'DGKb',
        cHRM = 'MRHc',
        gAMA = 'AMAg',
        hIST = 'TSIh',
        iCCP = 'PCCi',
        iTXt = 'tXTi',
        pHYs = 'sYHp',
        sBIT = 'TIBs',
        sPLT = 'TLPs',
        sRGB = 'BGRs',
        sTER = 'RETs',
        tEXt = 'tXEt',
        tIME = 'EMIt',
        tRNS = 'SNRt',
        zTXt = 'tXTz'
    };

    /// Describes the layout of a png image
    struct Dimensions {
        uint32_t width, height, bit_depth, color_type;
    };

    enum ColorOptions: uint32_t {
        Indexed,
        Grayscale,
        GrayscaleAlpha,
        Truecolor,
        TruecolorAlpha
    };

    enum ParseError {
        NoError = 0,
        FileError,
        MalformedHeader,
        BadChunk
    };

    struct ParseResult {
        static ParseResult error(ParseError error_type) {
            return ParseResult { Image { nullptr}, error_type };
        }

        static ParseResult ok(Image image) {
            return ParseResult { image, NoError };
        }

        Image image;
        ParseError err;
    };

    ParseResult parse_file(std::unique_ptr<FILE, int(*)(FILE*)>&& file);
}

#endif // IMAGE_PROCESSING_PNG_H
