
//{{BLOCK(horizon)

//======================================================================
//
//	horizon, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 15 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 16x16 
//	Metatiled by 2x2
//	Total size: 512 + 480 + 56 + 512 = 1560
//
//	Time-stamp: 2022-05-06, 01:09:24
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_HORIZON_H
#define GRIT_HORIZON_H

#define horizonTilesLen 480
extern const unsigned short horizonTiles[240];

#define horizonMetaTilesLen 56
extern const unsigned short horizonMetaTiles[28];

#define horizonMetaMapLen 512
extern const unsigned short horizonMetaMap[256];

#define horizonPalLen 512
extern const unsigned short horizonPal[256];

#endif // GRIT_HORIZON_H

//}}BLOCK(horizon)
