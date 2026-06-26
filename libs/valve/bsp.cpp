#include "bsp.h"

namespace valve
{
    BSP::BSP(const std::string& filepath)
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
        std::cout << "[BSP]: Loading rendering data" << std::endl;

        generateTris();
        getEntitesData();
    }

    BSP::~BSP()
    {
        delete[] rawMapData;
        delete[] vertices;
        delete[] planes;
        delete[] nodes;
        delete[] leafs;
        delete[] brushes;
        delete[] faces;
        delete[] edges;
        delete[] surfedges;
        delete[] texinfos;
        delete[] texdatas;
        delete[] textable;
    }

    void BSP::generateTris()
    {
        int color = -1;
        for (int i = 0; i < numMapFaces; i++)
        {
            const bsp::face_t &currentFace = faces[i];

            const bsp::texinfo_t currentTextureInfo = texinfos[currentFace.texinfo];
            const bsp::texdata_t currentTextureData = texdatas[currentTextureInfo.texdata];
            const int32_t textureDataOffset = currentTextureData.nameStringTableID;

            const char *textureNamePtr = &texStringData.at(textureDataOffset);
            std::string textureName(textureNamePtr);

            if (textureName.find("AREAPORTAL") != std::string::npos)
                continue;

            if (textureName.find("SKY") != std::string::npos)
                continue;

            glm::vec3 currentFaceColor;

            if (color == -1)
                currentFaceColor = glm::vec3(1.f, 0.f, 0.f);
            else if (color == 0)
                currentFaceColor = glm::vec3(0.f, 1.f, 0.f);
            else if (color == 1)
                currentFaceColor = glm::vec3(0.f, 0.f, 1.f);

            if (currentFace.numedges < 3) continue;

            std::vector<glm::vec3> polygons;

            for (int j = 0; j < currentFace.numedges; j++)
            {
                const bsp::surfedge_t& currentSurfedge = surfedges[currentFace.firstedge + j];
                const bsp::edge_t& currentEdge = edges[std::abs(currentSurfedge)];

                const int32_t vertIndex = currentSurfedge < 0 ? currentEdge.v[1] : currentEdge.v[0];

                const bsp::vertex_t& vertex = vertices[vertIndex];
                polygons.push_back({vertex.x, vertex.y, vertex.z});
            }

            if (polygons.size() < 3) return;

            const auto& v0 = polygons[0];

            for (int j = 1; j < polygons.size() - 1; j++)
            {
                const auto& v1 = polygons[j];
                const auto& v2 = polygons[j + 1];

                glm::vec3 edge1 = v1 - v0;
                glm::vec3 edge2 = v2 - v0;
                glm::vec3 normal = glm::cross(edge1, edge2);

                if (glm::length(normal) < 0.00001f) continue;

                verticesToDraw.push_back(v0.x);
                verticesToDraw.push_back(v0.y);
                verticesToDraw.push_back(v0.z);

                verticesToDraw.push_back(normal.x);
                verticesToDraw.push_back(normal.y);
                verticesToDraw.push_back(normal.z);

                verticesToDraw.push_back(v1.x);
                verticesToDraw.push_back(v1.y);
                verticesToDraw.push_back(v1.z);

                verticesToDraw.push_back(normal.x);
                verticesToDraw.push_back(normal.y);
                verticesToDraw.push_back(normal.z);

                verticesToDraw.push_back(v2.x);
                verticesToDraw.push_back(v2.y);
                verticesToDraw.push_back(v2.z);

                verticesToDraw.push_back(currentFaceColor.r);
                verticesToDraw.push_back(currentFaceColor.g);
                verticesToDraw.push_back(currentFaceColor.b);
            }

            color++;

            if (color == 2)
                color = -1;
        }
    }

    void BSP::getDataFromMap()
    {
        memcpy(&header, rawMapData, sizeof(bsp::header_t));

        if (!isHl2Version())
        {
            std::cout << "[BSP] Error: Invalid map version (" << header.version << "). Expected " <<
                bsp::MAPVERSION_HL2_MIN << "-" << bsp::MAPVERSION_HL2_MAX << std::endl;
            return;
        }

        std::cout << "\t[BSP]: Loading lumps..." << std::endl;

        getLump(bsp::Lumps::LUMP_ENTITIES);
        getLump(bsp::Lumps::LUMP_TEXINFO);
        getLump(bsp::Lumps::LUMP_TEXDATA);
        getLump(bsp::Lumps::LUMP_TEXDATA_STRING_TABLE);
        getLump(bsp::Lumps::LUMP_TEXDATA_STRING_DATA);
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
    }

    void BSP::getEntitesData()
    {
        std::regex entitiesRegex(R"(\{[^{}]*\})");

        auto begin = std::sregex_iterator(rawEntitiesTextBuffer.begin(), rawEntitiesTextBuffer.end(),
            entitiesRegex);
        auto end = std::sregex_iterator();

        for (auto it = begin; it != end; ++it) {
            entitiesStrs.push_back(it->str());
        }

        const auto& playerSpawnIt =
            std::find_if(entitiesStrs.begin(), entitiesStrs.end(),
                         [](const std::string& str)
                         {
                             return str.find("info_player_start") != std::string::npos;
                         });

        if (playerSpawnIt != entitiesStrs.end())
        {
            const std::string data = playerSpawnIt->data();
            const std::string::size_type originPos = data.find("\"origin\" ");
            std::string posData = data.substr(originPos);
            posData.erase(0, 10);

            const std::string::size_type originEndPos = posData.find('\"');
            posData.erase(originEndPos, posData.length());

            std::stringstream ss(posData);
            std::string substring;
            std::vector<float> positions;

            while (std::getline(ss, substring, ' ')) {
                positions.push_back(std::stof(substring) * 0.0254f);
            }

            playerSpawn = glm::vec3(positions[0], positions[2], -positions[1]);
        }
    }

    void BSP::getLump(bsp::Lumps lumpType)
    {
        using namespace bsp;
        switch (lumpType)
        {
        case Lumps::LUMP_ENTITIES:
            {
                lump_t currentLump = header.lumps[Lumps::LUMP_ENTITIES];

                char *rawText = new char[currentLump.dataLength];
                memcpy(rawText, &rawMapData[currentLump.dataOffset], currentLump.dataLength);

                rawEntitiesTextBuffer = rawText;
                break;
            }
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
        case Lumps::LUMP_TEXINFO:
            {
                lump_t currentLump = header.lumps[LUMP_TEXINFO];
                numMapTexInfo = currentLump.dataLength / sizeof(texinfo_t);

                texinfos = new texinfo_t[numMapTexInfo];

                memcpy(texinfos, &rawMapData[currentLump.dataOffset], currentLump.dataLength);
                break;
            }
        case Lumps::LUMP_TEXDATA:
            {
                lump_t currentLump = header.lumps[LUMP_TEXDATA];
                numMapTexData = currentLump.dataLength / sizeof(texdata_t);

                texdatas = new texdata_t[numMapTexData];

                memcpy(texdatas, &rawMapData[currentLump.dataOffset], currentLump.dataLength);
                break;
            }
        case Lumps::LUMP_TEXDATA_STRING_TABLE:
            {
                lump_t currentLump = header.lumps[LUMP_TEXDATA_STRING_TABLE];
                numMapTexTable = currentLump.dataLength / sizeof(int32_t);

                textable = new int32_t[numMapTexTable];

                memcpy(textable, &rawMapData[currentLump.dataOffset], currentLump.dataLength);
                break;
            }
            case Lumps::LUMP_TEXDATA_STRING_DATA:
            {
                lump_t currentLump = header.lumps[LUMP_TEXDATA_STRING_DATA];
                numMapTexStringData = currentLump.dataLength / sizeof(char);

                texStringDatas = new char[numMapTexStringData];

                memcpy(texStringDatas, &rawMapData[currentLump.dataOffset], numMapTexStringData);

                for (int i = 0; i < numMapTexStringData; i++) {
                    texStringData += texStringDatas[i];
                }

                break;
            }
        default:
            {
                std::cout   << "\t[BSP] Error: Unknown lump (" << lumpType
                            << "). Check if ID exists in func" << std::endl;
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
        return id == ('P' << 24) + ('S' << 16) + ('B' << 8) + 'V';
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
