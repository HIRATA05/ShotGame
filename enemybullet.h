#pragma once
/*
*
* Enemy
*
* 敵
*
*/

#define	ENEMYBULLET_SPD	6	/*	弾の移動速度	*/

/*
*	弾本体のプログラムが使用する変数などの宣言
*/
/*
* 弾の状態遷移の定義
*/
enum {
	ENEMYBULLET_INIT	=	0,
	ENEMYBULLET_SHOT,			/*飛翔中*/
	ENEMYBULLET_HIT,			/*当たり*/
	ENEMYBULLET_EXPLOSION,		/*破壊中*/
	ENEMYBULLET_DONE,			/*終了中*/
};

/*
*	弾の種類の定義 
*/
enum {
	ENEMYBULLET_TYPE_0	=	0,
	ENEMYBULLET_TYPE_1,
	ENEMYBULLET_TYPE_2,
	ENEMYBULLET_TYPE_3,
	ENEMYBULLET_TYPE_4,
	ENEMYBULLET_TYPE_5,
};

struct ENEMYBULLETOBJECT_P
{
	int	tmr;
	int	state;		/* 状態 */
	int	x, y;		/* 位置の整数値	*/
	int xold, yold;	/* 位置の前回値 */
	int size;		/* 物体の大きさ */
	int type;		/* 物体のタイプ */
	int spd;		/* 飛翔速度(スカラ)	*/

	int tagx,tagy;	/* 目的地 */
	float spdx, spdy;	/* 各方向の移動速度	*/
	float xf, yf;		/* 位置の浮動小数点値	*/

	struct ENEMYBULLETOBJECT_P *next;
};

int	EnemyBullet_Init();	/* GameMode INITで呼ぶ	*/
int	EnemyBullet_Update();	/* GameMode DSPで呼ぶ	*/
int	EnemyBullet_End();	/* GameMode FINISHで呼ぶ	*/
int EnemyBullet_Shoot( int x, int y, int type );	/* Enemy Position */
struct ENEMYBULLETOBJECT_P *GetObject_EnemyBullet( int n );	/* 敵の変数領域の先頭のポインタを取得する	*/
void EnemyBullet_SetState( struct ENEMYBULLETOBJECT_P *n, int state );
int EnemyBullet_GetState( struct ENEMYBULLETOBJECT_P *n );