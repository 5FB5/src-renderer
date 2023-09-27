//
// Created by valera on 27.09.23.
//

#ifndef SRC_RENDERER_BSP_H
#define SRC_RENDERER_BSP_H

#include <filesystem>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <memory.h>

namespace vbsp
{
    // Reference: https://developer.valvesoftware.com/wiki/BSP_(Source)
    constexpr int HEADER_LUMPS = 64;

    struct lump_t
    {
        int32_t fileofs = 0;      // offset into file (bytes)
        int32_t filelen = 0;      // length of lump (bytes)
        int32_t version = 0;      // lump format version
        char fourCC[4];    // lump ident code
    };

    struct header_t
    {
        int32_t ident = 0;
        int32_t version = 0;
        lump_t lumps[HEADER_LUMPS];
        int32_t mapRevision = 0;
    };

    class BSP
    {
    public:
        explicit BSP(std::string filepath);

        header_t header;

        bool isVbsp(int32_t id);
    };

} // vbsp

#endif //SRC_RENDERER_BSP_H
