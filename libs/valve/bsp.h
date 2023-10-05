//
// Created by valera on 27.09.23.
//

#ifndef SRC_RENDERER_BSP_H
#define SRC_RENDERER_BSP_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory.h>

#include "bspstructs.h"

namespace valve
{
    // Reference: https://developer.valvesoftware.com/wiki/BSP_(Source)

    class BSP
    {
    private:
        char *rawMapData = nullptr;
        int verticesArraySize = 0;

        void getDataFromMap();
        void getLump(bsp::Lumps lumpType);

    public:
        explicit BSP(const std::string &filepath);
        ~BSP();

        bsp::header_t header;

        bsp::vector_t *vertices = nullptr;
        bsp::plane_t *planes = nullptr;
        bsp::node_t *nodes = nullptr;
        bsp::leaf_t *leafs = nullptr;
        bsp::brush_t *brushes = nullptr;
        bsp::face_t *faces = nullptr;

        int numMapVertices = 0;
        int numMapPlanes = 0;
        int numMapNodes = 0;
        int numMapLeafs = 0;
        int numMapBrushes = 0;
        int numMapFaces = 0;

        int getVerticesArraySize();
        bool isVbsp(int32_t id);
        bool isHl2Version();
    };

} // vbsp

#endif //SRC_RENDERER_BSP_H
