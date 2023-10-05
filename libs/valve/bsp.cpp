//
// Created by valera on 27.09.23.
//

#include "bsp.h"

namespace valve
{
    BSP::BSP(const std::string &filepath)
    {
        std::cout << "[BSP]: Loading file (" << filepath << ")" << std::endl;
        std::ifstream file(filepath, std::ios::binary);

        if (file.fail())
        {
            std::cout << "[BSP] Error: Fail to read file " << filepath << std::endl;
            return;
        }

        file.seekg(0, std::ios::end);
        uint64_t fileSize = file.tellg();

        rawMapData = new char[fileSize];

        file.seekg(0, std::ios::beg);
        file.read(rawMapData, fileSize);

        getDataFromMap();

        std::cout << "[BSP]: File loaded" << std::endl;
    }

    BSP::~BSP()
    {
        delete[] planes;
        delete[] nodes;
        delete[] leafs;
        delete[] brushes;
        delete[] faces;
        delete[] rawMapData;
        delete[] vertices;
    }

    void BSP::getDataFromMap()
    {
        memcpy(&header, rawMapData, sizeof(bsp::header_t));

        if (!isHl2Version())
        {
            std::cout << "[BSP] Error: Invalid map version (" << header.version << "). Expected " << bsp::MAPVERSION_HL2_MIN << "-" << bsp::MAPVERSION_HL2_MAX << std::endl;
            return;
        }

        std::cout << "[BSP]: Loading lumps..." << std::endl;

        getLump(bsp::Lumps::LUMP_PLANES);
        getLump(bsp::Lumps::LUMP_NODES);
        getLump(bsp::Lumps::LUMP_LEAFS);
        getLump(bsp::Lumps::LUMP_BRUSHES);
        getLump(bsp::Lumps::LUMP_FACES);
        getLump(bsp::Lumps::LUMP_VERTEXES);

        std::cout << "[BSP]: Lumps loaded" << std::endl;
    }

    void BSP::getLump(bsp::Lumps lumpType)
    {
        using namespace bsp;
        switch(lumpType)
        {
            case Lumps::LUMP_PLANES:
            {
                lump_t currentLump = header.lumps[Lumps::LUMP_PLANES];

                numMapPlanes = currentLump.dataLength / sizeof(plane_t);
                planes = new plane_t[numMapPlanes];

                memcpy(planes, &rawMapData[currentLump.dataOffset], currentLump.dataLength);
                break;
            }
            case Lumps::LUMP_NODES:
            {
                lump_t currentLump = header.lumps[LUMP_NODES];

                numMapNodes = currentLump.dataLength / sizeof(node_t);
                nodes = new node_t[numMapNodes];

                memcpy(nodes, &rawMapData[currentLump.dataOffset], currentLump.dataLength);
                break;
            }
            case Lumps::LUMP_LEAFS:
            {
                lump_t currentLump = header.lumps[LUMP_LEAFS];

                numMapLeafs = currentLump.dataLength / sizeof(leaf_t);
                leafs = new leaf_t[numMapLeafs];

                memcpy(leafs, &rawMapData[currentLump.dataOffset], currentLump.dataLength);
                break;
            }
            case Lumps::LUMP_BRUSHES:
            {
                lump_t currentLump = header.lumps[LUMP_BRUSHES];

                numMapBrushes = currentLump.dataLength / sizeof(brush_t);
                brushes = new brush_t[numMapBrushes];

                memcpy(brushes, &rawMapData[currentLump.dataOffset], currentLump.dataLength);
                break;
            }
            case Lumps::LUMP_FACES:
            {
                lump_t currentLump = header.lumps[Lumps::LUMP_FACES];

                numMapFaces = currentLump.dataLength / sizeof(face_t);
                faces = new face_t[numMapFaces];

                memcpy(faces, &rawMapData[currentLump.dataOffset], currentLump.dataLength);
                break;
            }
            case Lumps::LUMP_VERTEXES:
            {
                lump_t currentLump = header.lumps[Lumps::LUMP_VERTEXES];

                numMapVertices = currentLump.dataLength / sizeof(vertex_t);
                verticesArraySize = numMapVertices * 3;

                vertices = new vector_t[numMapVertices];

                memcpy(vertices, &rawMapData[currentLump.dataOffset], currentLump.dataLength);
                break;
            }
            default:
            {
                std::cout << "[BSP] Error: Unknown lump (" << lumpType << "). Check if ID exists in func" << std::endl;
                break;
            }
        }
    }

    int BSP::getVerticesArraySize()
    {
        return verticesArraySize;
    }

    bool BSP::isVbsp(int32_t id)
    {
        return id == ( 'P' << 24 ) + ( 'S' << 16 ) + ( 'B' << 8 ) + 'V';
    }

    bool BSP::isHl2Version()
    {
        if (!isVbsp(header.ident))
        {
            std::cout << "[BSP] Error: Map ID isn't equal to VBSP" << std::endl;
            return false;
        }

        return header.version >= bsp::MAPVERSION_HL2_MIN && header.version <= bsp::MAPVERSION_HL2_MAX;
    }

} // vbsp