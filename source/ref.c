// #include <stdio.h>
// #include <string.h>
// #include <tonc.h>

// // bg
// // #include "brin.h"
// // #include "spacebg.h"
// // #include "yyt.h"
// // #include "title.h"
// // #include "gameover.h"

// // bg
// #include "gfx/horizon.h"


// // 맵으로 넣을 때는 sbb 포맷으로

// // sprite
// // #include "mygame.h"
// #include "gfx/cloud.h"
// #include "gfx/obstacle-large.h"
// #include "gfx/obstacle-small.h"
// #include "gfx/restart.h"
// #include "gfx/trex.h"

// // typedef enum GAME_SCENE
// // {
// // 	GAME_SCENE_NONE,
// // 	GAME_SCENE_LOGO,
// // 	GAME_SCENE_TITLE,
// // 	GAME_SCENE_GAME,
// // 	GAME_SCENE_GAMEOVER
// // } GAME_SCENE;

// OBJ_ATTR obj_buffer[128];
// OBJ_AFFINE *obj_aff_buffer = (OBJ_AFFINE*)obj_buffer;

// //sprite
// int oamCount = 0;

// typedef struct Vector2
// {
// 	int x;
// 	int y;
// } Vector2;

// typedef struct Entity
// {
// 	Vector2 pos;
// 	Vector2 size;
// 	CollisionBox col;
// 	int speed;

// 	OBJ_ATTR* sprite;
// 	u32 tileId;
// 	u32 palBank;
// } Entity;

// typedef struct CollisionBox
// {
// 	int x;
// 	int y;
// 	int w;
// 	int h;
// } CollisionBox;

// // player info
// Entity player;

// // bg info
// typedef struct BG
// {
// 	Vector2 pos;
// 	int speed;
// } BG;

// BG bg;

// // enemy info
// // TODO: enemy pooling
// // Entity enemy;

// // GAME_SCENE prevScene = GAME_SCENE_LOGO;
// // GAME_SCENE currentScene = GAME_SCENE_LOGO;

// // // scene management
// // void ChangeScene(GAME_SCENE newScene);
// // void EnterScene(GAME_SCENE scene);
// // void UpdateScene(GAME_SCENE scene);
// // void ExitScene(GAME_SCENE scene);

// // // scene
// // void enter_logo();
// // void update_logo();
// // void exit_logo();

// // void enter_title();
// // void update_title();
// // void exit_title();

// // void enter_game();
// // void update_game();
// // void exit_game();

// // void enter_gameOver();
// // void update_gameOver();
// // void exit_gameOver();

// // score
// int score = 0;

// // timer
// // int gameTimer = 0;
// float gameTimer = 0;

// // float GetTimer(int index)
// // {
// // 	switch (index)
// // 	{
// // 	case 0:
// // 		return REG_TM0D;
// // 		break;
// // 	case 1:
// // 		return REG_TM1D;
// // 		break;
// // 	case 2:
// // 		return REG_TM3D;
// // 		break;
// // 	case 3:
// // 		return REG_TM4D;
// // 		break;
	
// // 	default:
// // 		break;
// // 	}
// // }

// // void SetTimer(int index, float time)
// // {
// // 	switch (index)
// // 	{
// // 	case 0:
// // 		REG_TM0CNT = TM_ENABLE | TM_CASCADE;
// // 		return;
// // 	case 1:
// // 		REG_TM1CNT = TM_ENABLE | TM_CASCADE;
// // 		return;

// // 	case 2:
// // 		REG_TM3CNT = TM_ENABLE | TM_CASCADE;
// // 		return;
// // 	case 3:
// // 		REG_TM4CNT = TM_ENABLE | TM_CASCADE;
// // 		return;
	
// // 	default:
// // 		break;
// // 	}
// // }

// // void ToggleTimer(int index)
// // {
// // 	switch (index)
// // 	{
// // 	case 0:
// // 		REG_TM0CNT ^= TM_ENABLE;
// // 		return;

// // 	case 1:
// // 		REG_TM1CNT ^= TM_ENABLE;
// // 		return;

// // 	case 2:
// // 		REG_TM3CNT ^= TM_ENABLE;
// // 		return;
// // 	case 3:
// // 		REG_TM4CNT ^= TM_ENABLE;
// // 		return;
	
// // 	default:
// // 		break;
// // 	}
// // }

// // physics?
// // bool CheckCollision(Entity a, Entity b);
// bool CheckCollision(CollisionBox a, CollisionBox b);

// // bg
// void init_bg()
// {
// 	REG_DISPCNT |= DCNT_BG0;
// 	// REG_BG0CNT |= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_64x32;
// 	REG_BG0CNT |= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_64x64;

// 	// 빈 구조체 생성이 안됨
// 	// bg = {};
// 	// bg.pos.x = 192;
// 	// bg.pos.y = 64;

// 	// bg.speed = 1;

// 	// memcpy(pal_bg_mem, brinPal, brinPalLen);
// 	// memcpy(&tile_mem[0][0], brinTiles, brinTilesLen);
// 	// memcpy(&se_mem[30][0], brinMap, brinMapLen);

// 	memcpy(pal_bg_mem, spacebgPal, spacebgPalLen);
// 	memcpy(&tile_mem[0][0], spacebgTiles, spacebgTilesLen);
// 	memcpy(&se_mem[30][0], spacebgMap, spacebgMapLen);
// }

// void update_bg()
// {
// 	bg.pos.x += bg.speed * key_tri_horz();
// 	bg.pos.y += bg.speed * key_tri_vert();

// 	REG_BG0HOFS = bg.pos.x;
// 	REG_BG0VOFS = bg.pos.y;
// }

// void init_sprite()
// {
// 	REG_DISPCNT |= DCNT_OBJ | DCNT_OBJ_1D;

// 	memcpy(&tile_mem[4][0], mygameTiles, mygameTilesLen);
// 	memcpy(pal_obj_mem, mygamePal, mygamePalLen);
// 	oam_init(obj_buffer, 128);
// }

// // player sprite
// void init_player()
// {
// 	player.pos.x = SCREEN_WIDTH / 2;
// 	player.pos.y = SCREEN_HEIGHT / 2;

// 	player.size.x = 16;
// 	player.size.y = 16;

// 	player.speed = 2;
// 	player.tileId = 0;
// 	player.palBank = 0;

// 	// oam index
// 	player.sprite = &obj_buffer[0];

// 	obj_set_attr(player.sprite, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_PALBANK(player.palBank) | player.tileId);
// 	obj_set_pos(player.sprite, player.pos.x, player.pos.y);

// 	oamCount++;
// }

// // void clamp_player_position()
// // {
// // 	if (player.pos.x > SCREEN_WIDTH - player.size.x)
// // 		player.pos.x = SCREEN_WIDTH - player.size.x;

// // 	if (player.pos.x < 0)
// // 		player.pos.x = 0;

// // 	if (player.pos.y > SCREEN_HEIGHT - player.size.y)
// // 		player.pos.y = SCREEN_HEIGHT - player.size.y;

// // 	if (player.pos.y < 0)
// // 		player.pos.y = 0;
// // }

// void update_player()
// {
// 	// player.pos.x += player.speed * key_tri_horz();
// 	// player.pos.y += player.speed * key_tri_vert();
// 	// obj_set_pos(player.sprite, player.pos.x, player.pos.y);


// }

// // enemy
// void init_enemy()
// {
// 	enemy.pos.x = 10;
// 	enemy.pos.y = 10;

// 	enemy.size.x = 16;
// 	enemy.size.y = 16;

// 	enemy.tileId = 4;
// 	enemy.palBank = 0;

// 	enemy.speed = 1;

// 	// oam index
// 	enemy.sprite = &obj_buffer[1];

// 	obj_set_attr(enemy.sprite, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_PALBANK(enemy.palBank) | enemy.tileId);
// 	obj_set_pos(enemy.sprite, enemy.pos.x, enemy.pos.y);
// 	oamCount++;
// }

// void update_enemy()
// {
// 	int speed = enemy.speed;
// 	enemy.pos.x += enemy.pos.x > player.pos.x ? -speed : speed;
// 	enemy.pos.y += enemy.pos.y > player.pos.y ? -speed : speed;
// 	obj_set_pos(enemy.sprite, enemy.pos.x, enemy.pos.y);
// }

// void init_animation_dummy()
// {
	
// }

// void init_bullet_pool()
// {

// }

// void draw_sprite(){
// 	oam_copy(oam_mem, obj_buffer, oamCount);	// only need to update one
// }

// int main()
// {
// 	// Init interrupts and VBlank irq.
// 	irq_init(NULL);
// 	irq_add(II_VBLANK, NULL);
	
// 	REG_DISPCNT = DCNT_MODE0;

// 	// // Init 4bpp vwf text on bg 0.
// 	// tte_init_chr4c(0, 			// BG 0
// 	// 	BG_CBB(0)|BG_SBB(31),	// Charblock 0; screenblock 31
// 	// 	0xF000,					// Screen-entry offset
// 	// 	bytes2word(1,2,0,0),	// Color attributes.
// 	// 	CLR_YELLOW, 			// Yellow text
// 	// 	&verdana9Font,			// Verdana 9 font
// 	// 	NULL					// Use default chr4 renderer
// 	// 	);
	
// 	// // Initialize use of stdio.
// 	// tte_init_con();
	
// 	// // Printf something at 96,72
// 	// tte_printf("#{P:96,72}Hello World!");

// 	// ChangeScene(GAME_SCENE_LOGO);

// 	while(1)
// 	{
// 		VBlankIntrWait();
// 		key_poll();

// 		// UpdateScene(currentScene);
// 		Update();
// 	}

// 	return 0;

// }

// void ChangeScene(GAME_SCENE newScene)
// {
// 	prevScene = currentScene;
// 	currentScene = newScene;

// 	ExitScene(prevScene);
// 	EnterScene(currentScene);
// }

// void EnterScene(GAME_SCENE scene)
// {
// 	switch (scene)
// 	{
// 	case GAME_SCENE_NONE:
// 		break;

// 	case GAME_SCENE_LOGO:
// 		enter_logo();
// 		break;

// 	case GAME_SCENE_TITLE:
// 		enter_title();
// 		break;

// 	case GAME_SCENE_GAME:
// 		enter_game();
// 		break;

// 	case GAME_SCENE_GAMEOVER:
// 		enter_gameOver();
// 		break;
	
// 	default:
// 		break;
// 	}
// }

// void UpdateScene(GAME_SCENE scene)
// {
// 	switch (scene)
// 	{
// 	case GAME_SCENE_NONE:
// 		break;
	
// 	case GAME_SCENE_LOGO:
// 		update_logo();
// 		break;

// 	case GAME_SCENE_TITLE:
// 		update_title();
// 		break;

// 	case GAME_SCENE_GAME:
// 		update_game();
// 		break;

// 	case GAME_SCENE_GAMEOVER:
// 		update_gameOver();
// 		break;
	
// 	default:
// 		break;
// 	}
// }

// void ExitScene(GAME_SCENE scene)
// {
// 	switch (scene)
// 	{
// 	case GAME_SCENE_NONE:
// 		break;

// 	case GAME_SCENE_LOGO:
// 		exit_logo();
// 		break;

// 	case GAME_SCENE_TITLE:
// 		exit_title();
// 		break;

// 	case GAME_SCENE_GAME:
// 		exit_game();
// 		break;

// 	case GAME_SCENE_GAMEOVER:
// 		exit_gameOver();
// 		break;
	
// 	default:
// 		break;
// 	}
// }

// void enter_logo()
// {
// 	// 로고 씬 초기화
// 	REG_DISPCNT |= DCNT_BG0;
// 	REG_BG0CNT |= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_64x32;

// 	memcpy(pal_bg_mem, yytPal, yytPalLen);
// 	memcpy(&tile_mem[0][0], yytTiles, yytTilesLen);
// 	memcpy(&se_mem[30][0], yytMap, yytMapLen);
// }

// void update_logo()
// {
// 	// 2초 보여주고 자동 나가기

// 	if(key_hit(KEY_A))
// 		ChangeScene(GAME_SCENE_TITLE);
	
// }

// void exit_logo()
// {
// 	// 로고 씬 파괴
// }

// void enter_title()
// {
// 	// 타이틀 씬 초기화
	
// 	memcpy(pal_bg_mem, titlePal, titlePalLen);
// 	memcpy(&tile_mem[0][0], titleTiles, titleTilesLen);
// 	memcpy(&se_mem[30][0], titleMap, titleMapLen);
// }


// void update_title()
// {
// 	// 타이틀에서 게임 시작 입력 받기

// 	if(key_hit(KEY_A))
// 		ChangeScene(GAME_SCENE_GAME);

// }


// void exit_title()
// {
// 	// 게임 시작 누르면 게임 씬으로 이동
// }

// void enter_game()
// {
// 	init_bg();
	
// 	init_sprite();
	
// 	init_player();
// 	init_enemy();

// 	init_bullet_pool();

// 	init_animation_dummy();

// 	score = 0;
// }

// void update_game()
// {
// 	update_bg();
	
// 	update_player();
// 	clamp_player_position();
	
// 	update_enemy();

// 	// if(key_hit(KEY_A))
// 	// TODO: 적이 여러명일 때 처리
// 	if(CheckCollision(player, enemy))
// 		ChangeScene(GAME_SCENE_GAMEOVER);

// 	draw_sprite();

// 	score++;
// }

// int hidePos = 250;

// void exit_game()
// {
// 	// 배경 정리
// 	REG_BG0HOFS = 0;
// 	REG_BG0VOFS = 0;

// 	// 그려진것 어떻게 지우는지?
// 	// 임시: 위치 날리기

// 	player.pos.x = hidePos;
// 	player.pos.y = hidePos;

// 	enemy.pos.x = hidePos;
// 	enemy.pos.y = hidePos;

// 	update_player();
// 	update_enemy();
	
// 	draw_sprite();

// 	// 게임 씬 오브젝트 모두 정리
// 	player.sprite = NULL;
// 	enemy.sprite = NULL;

// 	oamCount = 0;
// }

// void enter_gameOver()
// {
// 	// 게임 오버 맵 파일 생성
// 	memcpy(pal_bg_mem, gameoverPal, gameoverPalLen);
// 	memcpy(&tile_mem[0][0], gameoverTiles, gameoverTilesLen);
// 	memcpy(&se_mem[30][0], gameoverMap, gameoverMapLen);
// }


// void update_gameOver()
// {
// 	// show game over screen
// 	// 1초간 입력 무시, 그 이후 a키 누르면 타이틀로 이동
// 	if(key_hit(KEY_A))
// 		ChangeScene(GAME_SCENE_TITLE);
// }


// void exit_gameOver()
// {
// 	// 게임 오버 씬 오브젝트 정리
// }

// bool CheckCollision(Entity a, Entity b)
// {
// 	if (a.pos.x < b.pos.x + b.size.x &&
// 		a.pos.x + a.size.x > b.pos.x &&
// 		a.pos.y < b.pos.y + b.size.y &&
// 		a.pos.y + a.size.y > b.pos.y)
// 	{
// 		return true;
// 	}

// 	return false;	
// }
