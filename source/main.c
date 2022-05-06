#include <stdio.h>
// #include <string.h>
#include <tonc.h>

// // bg
// // 맵으로 넣을 때는 sbb 포맷으로
#include "horizon.h"

// // sprite
#include "cloud.h"
#include "obstacle_large_c.h"
#include "obstacle_small_c.h"
#include "restart.h"
#include "trex_c4.h"

#include "whitemap.h"

typedef struct CollisionBox
{
	int x;
	int y;
	int w;
	int h;
} CollisionBox;

// typedef struct Entity
// {
//     int px;
//     int py;

//     int sx;
//     int sy;

// 	CollisionBox col;
// 	int speed;

// 	OBJ_ATTR* sprite;
// 	u32 tileId;
// 	u32 palBank;
// } Entity;

OBJ_ATTR obj_buffer[128];


// trex
int headInc = 16;
int bodyOffset = 48;
int bodyInc = 16;

const int tile_trex_offset = trex_c4TilesLen / (8 * 4);
const int tile_obstacle_small_offset = obstacle_small_cTilesLen / (8 * 4);
const int tile_obstacle_large_offset = obstacle_large_cTilesLen / (8 * 4);

OBJ_ATTR trex[2] = 
{
	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, 0}, // head
	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, 48} // body
};

OBJ_ATTR obs_small[4] =
{
	{ATTR0_TALL, ATTR1_SIZE_16x32, 0, tile_trex_offset + 0},
	{ATTR0_TALL, ATTR1_SIZE_16x32, 0, tile_trex_offset + 2},
	{ATTR0_TALL, ATTR1_SIZE_16x32, 0, tile_trex_offset + 4},
	{ATTR0_TALL, ATTR1_SIZE_16x32, 0, tile_trex_offset + 6}
};

OBJ_ATTR obs_large[6] = 
{
	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, 0},
	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, 48},

	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, 16},
	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, 64},

	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, 32},
	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, 96},
};

bool isPlaying = false;
bool isCrashed = false;

float x = 30;
float y = 140;

int ground = 140;

int pal = 0;
int tid = 0;

int xOffset = -16;
int yOffset = -28;

int headXOffset = 10;
int headYOffset = -32;

// physics
bool CheckCollision(CollisionBox a, CollisionBox b);

// game logics

// initialization
void Init();

// game start
// void Start();

// game update
void Update();

// void GameOver();

// horizon line
// bg
void init_bg()
{
	// REG_BG0CNT |= BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;

	// 빈 구조체 생성이 안됨
	// memcpy(pal_bg_mem, horizonPal, horizonPalLen);
	// memcpy(&tile_mem[0][0], horizonTiles, horizonTilesLen);
	// memcpy(&se_mem[30][0], horizonMetaMap, horizonMetaMapLen);

	memcpy(pal_bg_mem, horizonPal, horizonPalLen);
	memcpy(&tile_mem[0][0], horizonTiles, horizonTilesLen);
	memcpy(&se_mem[30][0], horizonMetaMap, horizonMetaMapLen);
}

void init_trex()
{
	memcpy(&tile_mem[4][0], trex_c4Tiles, trex_c4TilesLen);
	memcpy(pal_obj_mem, trex_c4Pal, trex_c4PalLen);

	trex[1].attr2 = bodyOffset;
}

void init_cloud()
{
	memcpy(&tile_mem[5][0], cloudTiles, cloudTilesLen);
	memcpy(pal_obj_mem, cloudPal, cloudPalLen);
}

void init_obstacles()
{
	// memcpy(&tile_mem[5][0], obstacle_small_cPal, obstacle_small_cTilesLen);
	// memcpy(pal_obj_mem, obstacle_small_cPal, obstacle_small_cPalLen); // 팔레트 복사가 필요한지?

	// memcpy(&tile_mem[4][0], obstacle_large_cPal, obstacle_large_cTilesLen);
	// memcpy(pal_obj_mem, obstacle_large_cPal, obstacle_large_cPalLen);

	// (타일 길이 / 32)를 더하면 이전에 로드한 타일 뒤에 바로 붙일 수 있다.
	memcpy(&tile_mem[4][tile_trex_offset], obstacle_small_cPal, obstacle_small_cTilesLen);
	memcpy(&tile_mem[4][tile_trex_offset + tile_obstacle_small_offset], obstacle_large_cPal, obstacle_large_cTilesLen);
}

void init_sprite()
{
	oam_init(obj_buffer, 128);

	init_cloud();
	init_obstacles();
	init_trex();
}

// player sprite
void init_player()
{

}

int main()
{
	// Init interrupts and VBlank irq.
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);
	
	REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D;

	init_bg();
	init_sprite();
	// init_player();

	while(1)
	{
		VBlankIntrWait();
		key_poll();

		Update();
	}

	return 0;

}

void debugTrex()
{
	// move left/right
	x += 2*key_tri_horz();

	// move up/down
	y += 2*key_tri_vert();

	// 0 16 32 
	if(key_hit(KEY_R))
		trex[0].attr2 += 16;

	if(key_hit(KEY_L))
		trex[0].attr2 -= 16;

	// 48 64 80
	if(key_hit(KEY_A))
		trex[1].attr2 += 16;

	if(key_hit(KEY_B))
		trex[1].attr2 -= 16;
}

void debugObs()
{
	if(key_held(KEY_A))
		obs_small[0].attr2 += 2;

	if(key_held(KEY_B))
		obs_small[0].attr2 -= 2;
}

bool isJumping = false;

float dy = 0;
float jumpPower = -6.0f;
float gravity = 0.5f;

void StartJump()
{
	isJumping = true;
	dy = jumpPower;
	y += dy;
}

void UpdateJump()
{
	y += dy;
	dy += gravity;
}

// int oamCount = 0;

void Update()
{
	int oamCount = 0;

	// debugTrex();
	debugObs();

	// increment/decrement starting tile with R/L
	// tid += bit_tribool(key_hit(-1), KI_R, KI_L);

	// flip
	// if(key_hit(KEY_A))	// horizontally
	// 	trex->attr1 ^= ATTR1_HFLIP;
	// if(key_hit(KEY_B))	// vertically
	// 	trex->attr1 ^= ATTR1_VFLIP;
	
	// player jump

	if(key_hit(KEY_A) && !isJumping)
		StartJump();

	if (isJumping)
		UpdateJump();

	// if(y < 140 && isJumping)
	// {
	// 	// y += 1;
	// }
	// else
	if (y >= 140 && isJumping)
	{
		y = 140;
		isJumping = false;
	}
	
	obj_set_pos(&trex[0], x + headXOffset + xOffset, y + headYOffset + yOffset);
	obj_set_pos(&trex[1], x + xOffset, y + yOffset);

	// draw only trex
	// oam_copy(oam_mem, trex, 3);	// only need to update one

	// draw multiple
	oamCount += 2;

	oam_copy(&obj_buffer[0], trex, 2);

	obj_set_pos(&obs_small[0], 100, 40);
	obj_set_pos(&obs_small[1], 120, 40);
	obj_set_pos(&obs_small[2], 140, 40);
	obj_set_pos(&obs_small[3], 160, 40);

	oamCount += 4;

	oam_copy(&obj_buffer[2], obs_small, 4);

	// draw all
	oam_copy(oam_mem, obj_buffer, oamCount);
}

bool CheckCollision(CollisionBox a, CollisionBox b)
{
	if (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y)
	{
		return true;
	}

	return false;	
}


// #include <stdio.h>
// #include <tonc.h>

// int main()
// {
// 	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;

// 	// Init BG 0 for text on screen entries.
// 	tte_init_se_default(0, BG_CBB(0)|BG_SBB(31));

// 	tte_write("#{P:72,64}");		// Goto (72, 64).
// 	tte_write("Hello World!");		// Print "Hello world!"

// 	while(1);

// 	return 0;
// }
