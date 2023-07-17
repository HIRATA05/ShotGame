#pragma once
/*
* Game mode
*/

/* ゲームに関する各種設定 */
#define DEFAULT_PLAYER_LEFT	3								// 残機数の初期値
#define PLYNUM_DISP_POSX	(SCRN_WIDTH - 2 * 14)			// 残機数の表示位置 X
#define PLYNUM_DISP_POSY	0								// 残機数の*表示位置 Y
#define PLYNUM_DISP_COLOR	GetColor( 240,240,240 )			// 残機数の表示色
#define SCORE_DISP_POSX		0								// スコアの表示位置 X
#define SCORE_DISP_POSY		0								// スコアの表示位置 Y
#define SCORE_DISP_COLOR	GetColor( 240,240,240 )			// スコアの表示色
#define GAMEOVER_DISP_POSX	(SCRN_WIDTH/2 - (14 * 9 / 2) )	// GAME OVERの表示位置 X
#define GAMEOVER_DISP_POSY	((SCRN_HEIGHT - 14)/2)			// GAME OVERの表示位置 Y
#define GAMEOVER_DISP_COLOR	GetColor( 240,240,240 )			// GAME OVERの表示色

/* playerに対する設定 */
#define	PLAYER_R			8		/* PLayerの大きさ */ 
#define	PLAYER_COLOR1		GetColor( 240,0,0 )
#define	PLAYER_COLOR2		GetColor( 220,220,220 )
#define	PL_XSPD				8		/* playerのx方向の移動速度 */
#define	PL_YSPD				8		/* playerのy方向の移動速度 (未使用) */
#define PL_EXPLOSION_TIME	(60*2)	/* 爆発表示期間 */
#define PL_MUTEKI_TIME		(60*1)	/* 無敵回復期間 */

/* playerの弾に対する設定 */
#define	PLAYERBULLET_COLOR	GetColor( 240, 240, 40 )
#define	PLAYERBULLET_SIZE	(8)	/* playerの弾の大きさ */
#define	PLAYERBULLET_SPD	(8*2)	/*	弾の移動速度	*/


/* 敵に対する設定 */
#define	ENEMY_COLOR			GetColor( 40, 240, 40 )
#define	ENEMY_SIZE			16	/* 敵の大きさ */

/* 敵の弾に対する設定 */
#define	ENEMYBULLET_COLOR	GetColor( 240, 140, 40 )
#define	ENEMYBULLET_SIZE	6	/* 敵の弾の大きさ */

int modeGame_Init( void );
int modeGame_End( void );
int modeGame_Dsp( void );

int GetGameTimer( void );				/* ゲームが開始してからのフレーム数の取得 */

int modeGame_AddScore( int score );		/* スコアの加算 */
int modeGame_AddScore2( int score);		/* スコアの加算 */
int modeGame_AddPlayer( int plnum );	/* 残機の加算 */
int modeGame_SubPlayer( int plnum );	/* 残機の減算 */
