#pragma once
/*
*
* Player Bullet
*
* プレイヤーの弾
*
*/


/*
*	弾本体のプログラムが使用する変数などの宣言
*/
enum {
	PLAYERBULLET_INIT	=	0,
	PLAYERBULLET_SHOT,			/*飛翔中*/
//	PLAYERBULLET_SLASH,			/*斬撃*/ //追加
	PLAYERBULLET_HIT,			/*当たり*/
	PLAYERBULLET_EXPLOSION,		/*破壊中*/
	PLAYERBULLET_DONE,			/*終了中*/
};

enum {
	PLBULLET_TYPE_BULLET	= 0, /*弾の種類*/
	PLBULLET_TYPE_SLASH,
};
struct PLAYERBULLETOBJECT_P
{
	int	tmr;
	int	state;		/* 状態 */
	int	x, y;		/* 位置の整数値	*/
	int xold, yold;	/* 位置の前回値 */
	int size;		/* 物体の大きさ */
	int type;		/* 物体のタイプ */
	int spd;		/* 飛翔速度(スカラ)	*/

	int tagx,tagy;	/* 目的地 */

	int weapontype;	/* 武器の種類 弾 剣 */
	int	ax1, ay1, ax2, ay2;
	int slashtmr;
#if !_ARRAY_MODE_
	struct PLAYERBULLETOBJECT_P *next;
#endif
};

int	PlayerBullet_Init();	/* GameMode INITで呼ぶ	*/
int	PlayerBullet_Update();	/* GameMode DSPで呼ぶ	*/
int	PlayerBullet_End();	/* GameMode FINISHで呼ぶ	*/
int PlayerBullet_Shoot( int x, int y );	/* Player Position */
int PlayerBullet_Slash(int x, int y);
struct PLAYERBULLETOBJECT_P *GetObject_PlayerBullet( int n );	/* プレイヤーの弾の変数領域の先頭のポインタを取得する	*/
void PlayerBullet_SetState( struct PLAYERBULLETOBJECT_P *n, int state );
int PlayerBullet_GetState( struct PLAYERBULLETOBJECT_P *n );
