//
// Created by valera on 05.10.23.
//

#ifndef SRC_RENDERER_BSPSTRUCTS_H
#define SRC_RENDERER_BSPSTRUCTS_H

#include <iostream>

#include "glm.hpp"

// Reference: https://developer.valvesoftware.com/wiki/BSP_(Source)
namespace bsp
{
    using vector_t = glm::vec3;
    using vertex_t = glm::vec3;
    using surfedge_t = int32_t;

    constexpr int MAPVERSION_HL2_MIN = 19;
    constexpr int MAPVERSION_HL2_MAX = 20;

    constexpr int HEADER_LUMPS = 64;

    // Reference: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/bspfile.h#L56
    constexpr int	MAX_MAP_MODELS =				1024;
    constexpr int	MAX_MAP_BRUSHES =				8192;
    constexpr int	MAX_MAP_ENTITIES =				8192;
    constexpr int	MAX_MAP_TEXINFO =		        12288;
    constexpr int   MAX_MAP_TEXDATA =		        2048;
    constexpr int   MAX_MAP_DISPINFO =		        2048;
    constexpr int	MAX_MAP_AREAS =		            256;
    constexpr int   MAX_MAP_AREA_BYTES =		    (MAX_MAP_AREAS/8);
    constexpr int	MAX_MAP_AREAPORTALS =		    1024;
    constexpr int	MAX_MAP_PLANES =		        65536;
    constexpr int	MAX_MAP_NODES =		            65536;
    constexpr int	MAX_MAP_BRUSHSIDES =		    65536;
    constexpr int	MAX_MAP_LEAFS =		            65536;
    constexpr int	MAX_MAP_VERTS =		            65536;
    constexpr int   MAX_MAP_VERTNORMALS =		    256000;
    constexpr int   MAX_MAP_VERTNORMALINDICES =		256000;
    constexpr int	MAX_MAP_FACES =			        65536;
    constexpr int	MAX_MAP_LEAFFACES =			    65536;
    constexpr int	MAX_MAP_LEAFBRUSHES =			65536;
    constexpr int	MAX_MAP_PORTALS =			    65536;
    constexpr int   MAX_MAP_CLUSTERS =			    65536;
    constexpr int   MAX_MAP_LEAFWATERDATA =			32768;
    constexpr int   MAX_MAP_PORTALVERTS =			128000;
    constexpr int	MAX_MAP_EDGES =				    256000;
    constexpr int	MAX_MAP_SURFEDGES =             512000;
    constexpr int	MAX_MAP_LIGHTING =              0x1000000;
    constexpr int	MAX_MAP_VISIBILITY =		    0x1000000;			// increased BSPVERSION 7
    constexpr int	MAX_MAP_TEXTURES =		        1024;
    constexpr int   MAX_MAP_WORLDLIGHTS =		    8192;
    constexpr int   MAX_MAP_CUBEMAPSAMPLES =		1024;
    constexpr int   MAX_MAP_OVERLAYS =			    512;
    constexpr int   MAX_MAP_WATEROVERLAYS =	        16384;
    constexpr int   MAX_MAP_TEXDATA_STRING_DATA =   256000;
    constexpr int   MAX_MAP_TEXDATA_STRING_TABLE =  65536;


    enum Lumps
    {
	    LUMP_ENTITIES = 0, // *
	    LUMP_PLANES = 1, // *
	    LUMP_TEXDATA = 2, // *
	    LUMP_VERTEXES = 3, // *
	    LUMP_VISIBILITY = 4, // *
	    LUMP_NODES = 5, // *
	    LUMP_TEXINFO = 6, // *
	    LUMP_FACES = 7, // *
	    LUMP_LIGHTING = 8, // *
	    LUMP_OCCLUSION = 9,
	    LUMP_LEAFS = 10, // *
	    LUMP_FACEIDS = 11,
	    LUMP_EDGES = 12, // *
	    LUMP_SURFEDGES = 13, // *
	    LUMP_MODELS = 14, // *
	    LUMP_WORLDLIGHTS = 15, //
	    LUMP_LEAFFACES = 16, // *
	    LUMP_LEAFBRUSHES = 17, // *
	    LUMP_BRUSHES = 18, // *
	    LUMP_BRUSHSIDES = 19, // *
	    LUMP_AREAS = 20, // *
	    LUMP_AREAPORTALS = 21, // *
	    LUMP_UNUSED0 = 22,
	    LUMP_UNUSED1 = 23,
	    LUMP_UNUSED2 = 24,
	    LUMP_UNUSED3 = 25,
	    LUMP_DISPINFO = 26,
	    LUMP_ORIGINALFACES = 27,
	    LUMP_PHYSDISP = 28,
	    LUMP_PHYSCOLLIDE = 29,
	    LUMP_VERTNORMALS = 30,
	    LUMP_VERTNORMALINDICES = 31,
	    LUMP_DISP_LIGHTMAP_ALPHAS = 32,
	    LUMP_DISP_VERTS = 33, // CDispVerts
	    LUMP_DISP_LIGHTMAP_SAMPLE_POSITIONS = 34, // For each displacement
	    //     For each lightmap sample
	    //         byte for index
	    //         if 255, then index = next byte + 255
	    //         3 bytes for barycentric coordinates
	    // The game lump is a method of adding game-specific lumps
	    // FIXME: Eventually, all lumps could use the game lump system
	    LUMP_GAME_LUMP = 35,
	    LUMP_LEAFWATERDATA = 36,
	    LUMP_PRIMITIVES = 37,
	    LUMP_PRIMVERTS = 38,
	    LUMP_PRIMINDICES = 39,
	    // A pak file can be embedded in a .bsp now, and the file system will search the pak
	    //  file first for any referenced names, before deferring to the game directory
	    //  file system/pak files and finally the base directory file system/pak files.
	    LUMP_PAKFILE = 40,
	    LUMP_CLIPPORTALVERTS = 41,
	    // A map can have a number of cubemap entities in it which cause cubemap renders
	    // to be taken after running vrad.
	    LUMP_CUBEMAPS = 42,
	    LUMP_TEXDATA_STRING_DATA = 43,
	    LUMP_TEXDATA_STRING_TABLE = 44,
	    LUMP_OVERLAYS = 45,
	    LUMP_LEAFMINDISTTOWATER = 46,
	    LUMP_FACE_MACRO_TEXTURE_INFO = 47,
	    LUMP_DISP_TRIS = 48,
	    LUMP_PHYSCOLLIDESURFACE = 49, // deprecated.  We no longer use win32-specific havok compression on terrain
	    LUMP_WATEROVERLAYS = 50,
	    LUMP_LEAF_AMBIENT_INDEX_HDR = 51, // index of LUMP_LEAF_AMBIENT_LIGHTING_HDR
	    LUMP_LEAF_AMBIENT_INDEX = 52, // index of LUMP_LEAF_AMBIENT_LIGHTING

	    // optional lumps for HDR
	    LUMP_LIGHTING_HDR = 53,
	    LUMP_WORLDLIGHTS_HDR = 54,
	    LUMP_LEAF_AMBIENT_LIGHTING_HDR = 55, // NOTE: this data overrides part of the data stored in LUMP_LEAFS.
	    LUMP_LEAF_AMBIENT_LIGHTING = 56, // NOTE: this data overrides part of the data stored in LUMP_LEAFS.

	    LUMP_XZIPPAKFILE = 57, // deprecated. xbox 1: xzip version of pak file
	    LUMP_FACES_HDR = 58, // HDR maps may have different face data.
	    LUMP_MAP_FLAGS = 59, // extended level-wide flags. not present in all levels
	    LUMP_OVERLAY_FADES = 60, // Fade distances for overlays
    };

    enum ContentsFlags: int
    {
        CONTENTS_EMPTY,
        CONTENTS_SOLID,
        CONTENTS_WINDOW,
        CONTENTS_AUX,
        CONTENTS_GRATE,
        CONTENTS_SLIME,
        CONTENTS_WATER,
        CONTENTS_MIST,
        CONTENTS_OPAQUE,
        CONTENTS_TESTFOGVOLUME,
        CONTENTS_UNUSED,
        CONTENTS_UNUSED6,
        CONTENTS_TEAM1,
        CONTENTS_TEAM2,
        CONTENTS_IGNORE_NODRAW_OPAQUE,
        CONTENTS_MOVEABLE,
        CONTENTS_AREAPORTAL,
        CONTENTS_PLAYERCLIP,
        CONTENTS_MONSTERCLIP,
        CONTENTS_CURRENT_0,
        CONTENTS_CURRENT_90,
        CONTENTS_CURRENT_180,
        CONTENTS_CURRENT_270,
        CONTENTS_CURRENT_UP,
        CONTENTS_CURRENT_DOWN,
        CONTENTS_ORIGIN,
        CONTENTS_MONSTER,
        CONTENTS_DEBRIS,
        CONTENTS_DETAIL,
        CONTENTS_TRANSLUCENT,
        CONTENTS_LADDER,
        CONTENTS_HITBOX
    };

    enum PlaneType: int
    {
        PlaneX,
        PlaneY,
        PlaneZ,
        PlaneAnyX,
        PlaneAnyY,
        PlaneAnyZ,
        PlaneNonAxial
    };

    struct lump_t
    {
        int32_t dataOffset = 0;         // offset into file (bytes)
        int32_t dataLength = 0;         // length of lump (bytes)
        int32_t version = 0;            // lump format version
        char magic[4];                  // lump ident code
    };

    struct header_t
    {
        int32_t ident = 0;
        int32_t version = 0;
        lump_t lumps[HEADER_LUMPS];
        int32_t mapRevision = 0;
    };

    struct plane_t
    {
        vector_t normal;
        float dist;
        PlaneType type;
    };

    struct node_t
    {
        int             planenum;       // index into plane array
        int             children[2];    // negative numbers are -(leafs + 1), not nodes
        short           mins[3];        // for frustum culling
        short           maxs[3];
        unsigned short  firstface;      // index into face array
        unsigned short  numfaces;       // counting both sides
        short           area;           // If all leaves below this node are in the same area, then
        // this is the area index. If not, this is -1.
        short           paddding;       // pad to 32 bytes length
    };

    struct leaf_t
    {
        int             contents;             // OR of all brushes (not needed?)
        short           cluster;              // cluster this leaf is in
        short           area:9;               // area this leaf is in
        short           flags:7;              // flags
        short           mins[3];              // for frustum culling
        short           maxs[3];
        unsigned short  firstleafface;        // index into leaffaces
        unsigned short  numleaffaces;
        unsigned short  firstleafbrush;       // index into leafbrushes
        unsigned short  numleafbrushes;
        short           leafWaterDataID;      // -1 for not in water

        //!!! NOTE: for lump version 0 (usually in maps of version 19 or lower) uncomment the next line
        //CompressedLightCube   ambientLighting;      // Precaculated light info for entities.
        short                 padding;              // padding to 4-byte boundary
    };

    struct brush_t
    {
        int    firstside;     // first brushside
        int    numsides;      // number of brushsides
        ContentsFlags    contents;      // contents flags
    };

    struct face_t
    {
        unsigned short      planenum;               // the plane number
        uint8_t             side;                   // faces opposite to the node's plane direction
        uint8_t             onNode;                 // 1 of on node, 0 if in leaf
        int                 firstedge;              // index into surfedges
        short               numedges;               // number of surfedges
        short               texinfo;                // texture info
        short               dispinfo;               // displacement info
        short               surfaceFogVolumeID;     // ?
        uint8_t             styles[4];              // switchable lighting info
        int                 lightofs;               // offset into lightmap lump
        float               area;                   // face area in units^2
        int                 LightmapTextureMinsInLuxels[2]; // texture lighting info
        int                 LightmapTextureSizeInLuxels[2]; // texture lighting info
        int                 origFace;               // original face this was split from
        unsigned short      numPrims;               // primitives
        unsigned short      firstPrimID;
        unsigned int        smoothingGroups;        // lightmap smoothing group
    };

    struct texinfo_t
    {
        float   textureVecs[2][4];    // [s/t][xyz offset]
        float   lightmapVecs[2][4];   // [s/t][xyz offset] - length is in units of texels/area
        int     flags;                // miptex flags overrides
        int     texdata;              // Pointer to texture name, size, etc.
    };

    struct texdata_t
    {
        vector_t  reflectivity;            // RGB reflectivity
        int     nameStringTableID;       // index into TexdataStringTable
        int     width, height;           // source image
        int     view_width, view_height;
    };

    struct edge_t
    {
        unsigned short	v[2];
    };

}

#endif //SRC_RENDERER_BSPSTRUCTS_H
