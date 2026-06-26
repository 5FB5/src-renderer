//
// Created by valera on 27.09.23.
//

#ifndef SRC_RENDERER_BSP_H
#define SRC_RENDERER_BSP_H

#include <regex>
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
    public:
        explicit BSP(const std::string &filepath);
        ~BSP();
        void generateTris();

        bsp::header_t header;

        glm::vec3 playerSpawn = glm::vec3(0.0f, 0.0f, 0.0f);

        bsp::vertex_t *vertices = nullptr;
        bsp::plane_t *planes = nullptr;
        bsp::node_t *nodes = nullptr;
        bsp::leaf_t *leafs = nullptr;
        bsp::brush_t *brushes = nullptr;
        bsp::face_t *faces = nullptr;
        bsp::edge_t *edges = nullptr;
        bsp::surfedge_t *surfedges = nullptr;
        bsp::texinfo_t *texinfos = nullptr;
        bsp::texdata_t *texdatas = nullptr;
        int32_t *textable = nullptr;
        char *texStringDatas = nullptr;
        std::string texStringData = "";

        std::vector<float> verticesToDraw;
        std::vector<std::string> entitiesStrs;

        int numMapVertices = 0;
        int numMapPlanes = 0;
        int numMapNodes = 0;
        int numMapLeafs = 0;
        int numMapBrushes = 0;
        int numMapFaces = 0;
        int numMapEdges = 0;
        int numMapSurfEdges = 0;
        int numMapTexInfo = 0;
        int numMapTexData = 0;
        int numMapTexTable = 0;
        int numMapTexStringData = 0;

        int getVerticesArraySize();
        bool isVbsp(int32_t id);
        bool isHl2Version();

    private:
        std::string rawEntitiesTextBuffer = "";
        char *rawMapData = nullptr;
        int verticesArraySize = 0;

        void getDataFromMap();
        void getEntitesData();
        void getLump(bsp::Lumps lumpType);
    };

} // vbsp

#endif //SRC_RENDERER_BSP_H
