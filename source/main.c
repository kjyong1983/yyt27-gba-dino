#include <stdio.h>
#include <string.h>
#include <tonc.h>

// bg
// 맵으로 넣을 때는 sbb 포맷으로

// sprite
#include "sprite.h"

// #include "whitemap.h"

#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))

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
const int offMod = 16;
int bodySprOffset = 16 * 3;

OBJ_ATTR trex[2] = 
{
	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, 0}, // head: 0, 16, 32
	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, offMod * 3} // body: 48, 64, 80
};

OBJ_ATTR obs_small[4] =
{
	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, offMod * 6}, // 96
	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, offMod * 7}, // 112
	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, offMod * 8}, // 128
	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, offMod * 9}  // 144
};

// empty: 160, 176

OBJ_ATTR obs_large[6] = 
{
	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, offMod * 12}, // 192
	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, offMod * 15},//

	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, offMod * 13},// 192
	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, offMod * 16},//

	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, offMod * 14},//
	{ATTR0_SQUARE, ATTR1_SIZE_32x32, 0, offMod * 17},//
};

bool isPlaying = false;
bool isCrashed = false;

float x = 30;
float y = 140;

int ground = 140;

int pal = 0;
int tid = 0;

const int trexXOffset = -16;
const int trexYOffset = -28;

const int headXOffset = 10;
const int headYOffset = -32;

const int obsLargeYOffset = 32;

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

void init_sprite()
{
	oam_init(obj_buffer, 128);

	memcpy(&tile_mem[4][0], spriteTiles, spriteTilesLen);
	memcpy(pal_obj_mem, spritePal, spritePalLen);

	trex[1].attr2 = bodySprOffset;

	obs_small[0].attr2 = offMod * 6;
	obs_small[1].attr2 = offMod * 7;
	obs_small[2].attr2 = offMod * 8;
	obs_small[3].attr2 = offMod * 9;

	// empty: 160, 176

	obs_large[0].attr2 = offMod * 12;
	obs_large[1].attr2 = offMod * 15;
	obs_large[2].attr2 = offMod * 13;
	obs_large[3].attr2 = offMod * 16;
	obs_large[4].attr2 = offMod * 14;
	obs_large[5].attr2 = offMod * 17;
}

int main()
{
	// Init interrupts and VBlank irq.
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);
	
	REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D | DCNT_BG0;

// 	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;

// 	// Init BG 0 for text on screen entries.
	tte_init_se_default(0, BG_CBB(0)|BG_SBB(31));

	// tte_write("#{P:72,64}");		// Goto (72, 64).
	// tte_write("Hello World!");		// Print "Hello world!"

	init_sprite();

	while(1)
	{
		VBlankIntrWait();
		key_poll();

		Update();

		tte_write("#{P:72,64}");		// Goto (72, 64).
		tte_write("Hello World!");		// Print "Hello world!"

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

void Update()
{
	int oamCount = 0;

	// debugTrex();
	// debugObs();

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

	if (y >= 140 && isJumping)
	{
		y = 140;
		isJumping = false;
	}
	
	obj_set_pos(&trex[0], x + headXOffset + trexXOffset, y + headYOffset + trexYOffset);
	obj_set_pos(&trex[1], x + trexXOffset, y + trexYOffset);

	// draw multiple
	oam_copy(&obj_buffer[0], trex, _countof(trex));
	oamCount += _countof(trex);

	obj_set_pos(&obs_small[0], 100, 40);
	obj_set_pos(&obs_small[1], 120, 40);
	obj_set_pos(&obs_small[2], 140, 40);
	obj_set_pos(&obs_small[3], 160, 40);

	oamCount += _countof(obs_small);
	oam_copy(&obj_buffer[2], obs_small, 4);

	obj_set_pos(&obs_large[0], 100, 80);
	obj_set_pos(&obs_large[1], 100, 80 + obsLargeYOffset);
	obj_set_pos(&obs_large[2], 140, 80);
	obj_set_pos(&obs_large[3], 140, 80 + obsLargeYOffset);
	obj_set_pos(&obs_large[4], 180, 80);
	obj_set_pos(&obs_large[5], 180, 80 + obsLargeYOffset);

	oam_copy(&obj_buffer[oamCount], obs_large, _countof(obs_large));
	oamCount += _countof(obs_large);

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
