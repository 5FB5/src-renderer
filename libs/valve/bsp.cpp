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
        delete[] rawMapData;
        delete[] planes;
        delete[] nodes;
        delete[] leafs;
        delete[] brushes;
        delete[] faces;
        delete[] vertices;
        delete[] edges;
        delete[] surfedges;
    }

    void BSP::getDataFromMap()
    {
        memcpy(&header, rawMapData, sizeof(bsp::header_t));

        if (!isHl2Version())
        {
            std::cout << "[BSP] Error: Invalid map version (" << header.version << "). Expected " << bsp::MAPVERSION_HL2_MIN << "-" << bsp::MAPVERSION_HL2_MAX << std::endl;
            return;
        }

        std::cout << "\t[BSP]: Loading lumps..." << std::endl;

        getLump(bsp::Lumps::LUMP_PLANES);
        getLump(bsp::Lumps::LUMP_NODES);
        getLump(bsp::Lumps::LUMP_LEAFS);
        getLump(bsp::Lumps::LUMP_BRUSHES);
        getLump(bsp::Lumps::LUMP_FACES);
        getLump(bsp::Lumps::LUMP_EDGES);
        getLump(bsp::Lumps::LUMP_SURFEDGES);
        getLump(bsp::Lumps::LUMP_VERTEXES);

        std::cout << "\t[BSP]: Lumps loaded" << std::endl;

        std::cout << "\t\tbrushes: " << numMapBrushes << std::endl;
        std::cout << "\t\tfaces: " << numMapFaces << std::endl;
        std::cout << "\t\tedges: " << numMapEdges << std::endl;
        std::cout << "\t\tsurfedges: " << numMapSurfEdges << std::endl;
        std::cout << "\t\tvertices: " << numMapVertices << std::endl;
        std::cout << "\t\tplanes: " << numMapPlanes << std::endl;
        std::cout << "\t\tnodes: " << numMapNodes << std::endl;
        std::cout << "\t\tleafs: " << numMapLeafs << std::endl;

        for (int i = 0; i < numMapFaces; i++)
        {
            const bsp::face_t &currentFace = faces[i];

            if (currentFace.numedges < 3) continue;
            std::vector<glm::vec3> polygons;

            for (int j = 0; j < currentFace.numedges; j++)
            {
                const bsp::surfedge_t &currentSurfedge = surfedges[currentFace.firstedge + j];
                const bsp::edge_t &currentEdge = edges[std::abs(currentSurfedge)];

                const int32_t vertIndex = currentSurfedge < 0 ? currentEdge.v[1] : currentEdge.v[0];

                const bsp::vertex_t &vertex = vertices[vertIndex];
                polygons.push_back( { vertex.x, vertex.y, vertex.z } );
            }

            if (polygons.size() < 3) return;

            const auto &v0 = polygons[0];
            for (int j = 1; j < polygons.size() - 1; j++)
            {
                const auto &v1 = polygons[j];
                const auto &v2 = polygons[j + 1];

                glm::vec3 edge1 = v1 - v0;
                glm::vec3 edge2 = v2 - v0;
                glm::vec3 normal = glm::cross(edge1, edge2);

                if (glm::length(normal) < 0.00001f) continue;

                verticesToDraw.push_back(v0.x);
                verticesToDraw.push_back(v0.y);
                verticesToDraw.push_back(v0.z);

                verticesToDraw.push_back(v1.x);
                verticesToDraw.push_back(v1.y);
                verticesToDraw.push_back(v1.z);

                verticesToDraw.push_back(v2.x);
                verticesToDraw.push_back(v2.y);
                verticesToDraw.push_back(v2.z);
            }
        }
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
                verticesArraySize = numMapVertices;

                vertices = new vertex_t[verticesArraySize];

                memcpy(vertices, &rawMapData[currentLump.dataOffset], currentLump.dataLength);
                break;
            }
            case Lumps::LUMP_EDGES:
            {
                lump_t currentLump = header.lumps[LUMP_EDGES];
                numMapEdges = currentLump.dataLength / sizeof(edge_t);

                edges = new edge_t[numMapEdges];

                memcpy(edges, &rawMapData[currentLump.dataOffset], currentLump.dataLength);
                break;
            }
            case Lumps::LUMP_SURFEDGES:
            {
                    lump_t currentLump = header.lumps[LUMP_SURFEDGES];
                    numMapSurfEdges = currentLump.dataLength / sizeof(surfedge_t);

                    surfedges = new surfedge_t[numMapSurfEdges];

                    memcpy(surfedges, &rawMapData[currentLump.dataOffset], currentLump.dataLength);
                    break;
            }
            default:
            {
                std::cout << "\t[BSP] Error: Unknown lump (" << lumpType << "). Check if ID exists in func" << std::endl;
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
            std::cout << "\t[BSP] Error: Map ID isn't equal to VBSP" << std::endl;
            return false;
        }

        return header.version >= bsp::MAPVERSION_HL2_MIN && header.version <= bsp::MAPVERSION_HL2_MAX;
    }

} // vbsp