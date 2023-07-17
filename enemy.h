#pragma once
/*
*
* Enemy
*
* 敵
*
*/

#define	ENEMY_SPD	4	/*	敵の移動速度	*/

/*
*	敵本体のプログラムが使用する変数などの宣言
*/
/*
* 敵の状態遷移の定義
*/
enum {
	ENEMY_INIT	=	0,
	ENEMY_SHOT,			/*飛翔中*/
	ENEMY_HIT,			/*当たり*/
	ENEMY_HIT2,			/*当たり*/
	ENEMY_EXPLOSION,	/*破壊中*/
	ENEMY_DONE,			/*終了中*/
};

/*
*	敵の種類の定義
*/
enum {
	ENEMYTYPE_0 = 0,
	ENEMYTYPE_RIGHT2LEFT,
	ENEMYTYPE_LEFT2RIGHT,
	ENEMYTYPE_TOP2BOTTOM,
	ENEMYTYPE_BOTTOM2TOP,
	ENEMYTYPE_MYMOVE_M,
	ENEMYTYPE_MYMOVE_B,
	ENEMYTYPE_MYMOVE_Q,
	ENEMYTYPE_MYMOVE_Z,
	ENEMYTYPE_END,
};


struct ENEMYOBJECT_P
{
	int	tmr;
	int	state;		/* 状態 */
	int	x, y;		/* 位置の整数値	*/
	int xold, yold;	/* 位置の前回値 */
	int size;		/* 物体の大きさ */
	int type;		/* 物体のタイプ */
	int spd;		/* 飛翔速度(スカラ)	*/

	int btm;		/* 弾を撃つ時間 */
	int tagx,tagy;	/* 目的地 */
	int etmr;

	struct ENEMYOBJECT_P *next;
};

int	Enemy_Init();	/* GameMode INITで呼ぶ	*/
int	Enemy_Update();	/* GameMode DSPで呼ぶ	*/
int	Enemy_End();	/* GameMode FINISHで呼ぶ	*/

int Enemy_Shoot( int x, int y, int type, int btm );	/* Enemyの発生 */

struct ENEMYOBJECT_P *GetObject_Enemy( int n );	/* 敵の変数領域の先頭のポインタを取得する	*/
void Enemy_SetState( struct ENEMYOBJECT_P* n, int state );	/* 敵のstateを設定する */
int Enemy_GetState( struct ENEMYOBJECT_P* n );			/* 敵のstateを取得する　*/
