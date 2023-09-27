//
// Created by valera on 27.09.23.
//

#include "bsp.h"

namespace vbsp
{
    BSP::BSP(std::string filepath)
    {
        std::cout << "[BSP]: Loading file (" << filepath << ")" << std::endl;
        std::ifstream file(filepath, std::ios::binary);

        if (file.fail())
        {
            std::cout << "[BSP]: Fail to read file " << filepath << std::endl;
            return;
        }

        file.seekg(0, std::ios::end);
        uint64_t fileSize = file.tellg();
        char *mapData = new char[fileSize];
        file.seekg(0, std::ios::beg);

        file.read(mapData, fileSize);
        file.close();

        std::cout << "[BSP]: File loaded" << std::endl;

        memcpy(&header, &mapData[0], sizeof(header));

        std::string identStatus = isVbsp(header.ident) ? "true" : "false";

        std::cout << "[BSP]: Is VBSP: " << identStatus << std::endl;
        std::cout << "[BSP]: Map version: " << header.version << std::endl;
        std::cout << "[BSP]: Map revision: " << header.mapRevision << std::endl;
    }

    bool BSP::isVbsp(int32_t id)
    {
        return id == ( 'P' << 24 ) + ( 'S' << 16 ) + ( 'B' << 8 ) + 'V';
    }

} // vbsp