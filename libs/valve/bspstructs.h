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

    enum Lumps: int
    {
        LUMP_ENTITIES,
        LUMP_PLANES,
        LUMP_TEXDATA,
        LUMP_VERTEXES,
        LUMP_VISIBILITY,
        LUMP_NODES,
        LUMP_TEXINFO,
        LUMP_FACES,
        LUMP_LIGHTING,
        LUMP_OCCLUSION,
        LUMP_LEAFS,
        LUMP_FACEIDS,
        LUMP_EDGES,
        LUMP_SURFEDGES,
        LUMP_MODELS,
        LUMP_WORLDLIGHTS,
        LUMP_LEAFFACES,
        LUMP_LEAFBRUSHES,
        LUMP_BRUSHES,
        LUMP_BRUSHSIDES,
        LUMP_AREAS,
        LUMP_AREAPORTALS,
        LUMP_PORTALS,
        LUMP_UNUSED0,
        LUMP_PROPCOLLISION,
        LUMP_CLUSTERS,
        LUMP_UNUSED1,
        LUMP_PROPHULLS,
        LUMP_PORTALVERTS,
        LUMP_UNUSED2,
        LUMP_PROPHULLVERTS,
        LUMP_CLUSTERPORTALS,
        LUMP_UNUSED3,
        LUMP_PROPTRIS,
        LUMP_DISPINFO,
        LUMP_ORIGINALFACES,
        LUMP_PHYSDISP,
        LUMP_PHYSCOLLIDE,
        LUMP_VERTNORMALS,
        LUMP_VERTNORMALINDICES,
        LUMP_DISP_LIGHTMAP_ALPHAS,
        LUMP_DISP_VERTS,
        LUMP_DISP_LIGHTMAP_SAMPLE_POSITIONS,
        LUMP_GAME_LUMP,
        LUMP_LEAFWATERDATA,
        LUMP_PRIMITIVES,
        LUMP_PRIMVERTS,
        LUMP_PRIMINDICES,
        LUMP_PAKFILE,
        LUMP_CLIPPORTALVERTS,
        LUMP_CUBEMAPS,
        LUMP_TEXDATA_STRING_DATA,
        LUMP_TEXDATA_STRING_TABLE,
        LUMP_OVERLAYS,
        LUMP_LEAFMINDISTTOWATER,
        LUMP_FACE_MACRO_TEXTURE_INFO,
        LUMP_DISP_TRIS,
        LUMP_PHYSCOLLIDESURFACE,
        LUMP_PROP_BLOB,
        LUMP_WATEROVERLAYS,
        LUMP_LIGHTMAPPAGES,
        LUMP_LEAF_AMBIENT_INDEX_HDR,
        LUMP_LIGHTMAPPAGEINFOS,
        LUMP_LEAF_AMBIENT_INDEX,
        LUMP_LIGHTING_HDR,
        LUMP_WORLDLIGHTS_HDR,
        LUMP_LEAF_AMBIENT_LIGHTING_HDR,
        LUMP_LEAF_AMBIENT_LIGHTING,
        LUMP_XZIPPAKFILE,
        LUMP_FACES_HDR,
        LUMP_MAP_FLAGS,
        LUMP_OVERLAY_FADES,
        LUMP_OVERLAY_SYSTEM_LEVELS,
        LUMP_PHYSLEVEL,
        LUMP_DISP_MULTIBLEND
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
        unsigned short  planenum;               // the plane number
        uint8_t           side;                   // faces opposite to the node's plane direction
        uint8_t            onNode;                 // 1 of on node, 0 if in leaf
        int             firstedge;              // index into surfedges
        short           numedges;               // number of surfedges
        short           texinfo;                // texture info
        short           dispinfo;               // displacement info
        short           surfaceFogVolumeID;     // ?
        uint8_t            styles[4];              // switchable lighting info
        int             lightofs;               // offset into lightmap lump
        float           area;                   // face area in units^2
        int             LightmapTextureMinsInLuxels[2]; // texture lighting info
        int             LightmapTextureSizeInLuxels[2]; // texture lighting info
        int             origFace;               // original face this was split from
        unsigned short  numPrims;               // primitives
        unsigned short  firstPrimID;
        unsigned int    smoothingGroups;        // lightmap smoothing group
    };

}

#endif //SRC_RENDERER_BSPSTRUCTS_H
