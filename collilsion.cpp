/*
* コリジョン
*
*
*
*
*/

#include "DxLib.h"
#include "ShotG.h"

#include "InputDevice.h"

#include "game.h"
#include "player.h"
#include "playerbullet.h"
#include "enemy.h"
#include "enemybullet.h"

#include "collilsion.h"


/* 外積を利用した当たり判定を使うときにtrue。
*	敵vsプレイヤーの弾で利用可能 
*	ただし、プレイヤーの弾が垂直に移動するため、同じ垂直に移動する敵とは
*	交差しないので注意	
*/
#define		_USE_OUTER_PRODUCT_	false	

/*
* 
* コリジョン
* 
* 各オブジェクトとの距離を測り、指定以下であれば、各ワークのstateを変更する
* 
* 
* 
*/

#if _USE_OUTER_PRODUCT_
struct linesedge {
	/* 線分1の開始点と終了点 */
	int	Ax, Ay;
	int Bx, By;

	/* 線分2の開始点と終了点 */
	int	Cx, Cy;
	int Dx, Dy;
};
int CrossCheck(struct linesedge* e);
#endif // _USE_OUTER_PRODUCT_



void Collision_Update( )
{
#if !_ARRAY_MODE_
	/* プレイヤーの状態を確認する */
	if (GetPlayerState() == PLSTATE_NORMAL)
	{
		/* Player 初期情報 */
		int	plx, ply, plsize;
		int	plxold, plyold;
		GetPlayerPosition(&plx, &ply);	/* Playerの位置を取得する */
		GetPlayerOldPosition(&plxold, &plyold);	/* Playerの位置を取得する */
		GetPlayerSize(&plsize);			/* Playerの大きさを取得する */
		/* プレイヤー vs 敵の弾　の当たり判定 */
		/* 敵の弾の変数領域の先頭のポインタを取得する	*/
		struct ENEMYBULLETOBJECT_P* objB = GetObject_EnemyBullet(0);
	#if _USE_OUTER_PRODUCT_
		/* 線分の交わりを調べる */
		struct linesedge e;
		e.Bx = plx;		e.By = ply;
		e.Ax = plxold;	e.Ay = plyold;
	#endif // _USE_OUTER_PRODUCT_

		while (objB != NULL)
		{
			if (EnemyBullet_GetState(objB) == ENEMYBULLET_SHOT)
			{	// 弾が通常状態のとき
#if _USE_OUTER_PRODUCT_
				e.Dx = objB->x;		e.Dy = objB->y;
				e.Cx = objB->xold;	e.Cy = objB->yold;
				if (CrossCheck(&e) ) 
				{	// stateを変更する
					EnemyBullet_SetState(objB, ENEMYBULLET_HIT);
					SetPlayerState(PLSTATE_HIT);
				}
#else
				int xlen = objB->x - plx;
				int ylen = objB->y - ply;
				int dist = objB->size + plsize;
				/* ２点間の距離が、直線距離（二つの大きさの和)より小さければ当たり　*/
				if (((xlen * xlen) + (ylen * ylen)) < (dist * dist))
				{	// stateを変更する
					EnemyBullet_SetState(objB, ENEMYBULLET_HIT);
					SetPlayerState(PLSTATE_HIT);
				}

#endif
			}
			/* 次のデータを見に行く */
			/* もし、->nextがNULLだったら、objBがNULLになるので whileが終了する */
			objB = objB->next;
		}
	}

	/* プレイヤー弾 vs 敵 の当たり判定 */
	/* 敵の変数領域の先頭のポインタを取得する	*/
	struct ENEMYOBJECT_P *objE = GetObject_Enemy( 0 );
	/* プレイヤー弾の変数領域の先頭のポインタを取得する	*/
	struct PLAYERBULLETOBJECT_P *objP = GetObject_PlayerBullet( 0 );
	/* whileの中で使用する変数 */
	struct PLAYERBULLETOBJECT_P *objPP = NULL;

	while( objE )
	{
	#if _USE_OUTER_PRODUCT_
		/* 線分の交わりを調べる */
		struct linesedge e;
	#endif // 0
		if( ( Enemy_GetState( objE ) == ENEMY_SHOT )
			/*|| ( objE->state == ENEMY_EXPLOSION )*/ )
		{	/* 敵が通常状態であれば、コリジョン検査を行う */
			objPP = objP; /* コピーを保存しておく */
		#if _USE_OUTER_PRODUCT_
			e.Ax = objE->xold;	e.Ay = objE->yold;
			e.Bx = objE->x;		e.By = objE->y;
		#endif // 0
			while( objPP )
			{
				if( ( PlayerBullet_GetState( objPP ) == PLAYERBULLET_SHOT )
					//|| (PlayerBullet_GetState(objPP) == PLAYERBULLET_SLASH )
					/*|| ( objPP->state == PLAYERBULLET_EXPLOSION )*/ )
				{	/* Playerの弾が通常状態であれば、コリジョン検査を行う */
				#if _USE_OUTER_PRODUCT_
					e.Cx = objPP->xold;	e.Cy = objPP->yold;
					e.Dx = objPP->x;	e.Dy = objPP->y;
					if( CrossCheck( &e ) )
					{	/* 状態の変更 */
						if(Enemy_GetState(objE) == ENEMY_SHOT)
						{	/* 状態の変更 */
							Enemy_SetState(objE,ENEMY_HIT);
						}
						if( PlayerBullet_GetState(objPP) == PLAYERBULLET_SHOT )
						{	/* 状態の変更 */
							PlayerBullet_SetState(objPP,PLAYERBULLET_HIT);
						}
					}
					#else

					
					
					int xlen = objE->x - objPP->x;
					int ylen = objE->y - objPP->y;
					int dist = objE->size + objPP->size;
					/* ２点間の距離が、直線距離（二つの大きさの和)より小さければ当たり　*/
					if ( ( ( xlen * xlen ) + ( ylen * ylen ) ) < ( dist * dist ) ) 
					{	/* 状態の変更 */
						if( Enemy_GetState( objE ) == ENEMY_SHOT )
						{	
							/* 状態の変更 */
							Enemy_SetState( objE, ENEMY_HIT ); 
						}
						if( PlayerBullet_GetState( objPP ) == PLAYERBULLET_SHOT )
						{	/* 状態の変更 */
							PlayerBullet_SetState( objPP, PLAYERBULLET_HIT ); 
						}
					}

					int xlen1 = objE->x - objPP->ax1;
					int ylen1 = objE->y - objPP->ay1;

					int xlen2 = objE->x - objPP->ax2;
					int ylen2 = objE->y - objPP->ay2;
					
					/* ２点間の距離が、直線距離（二つの大きさの和)より小さければ当たり　*/
					if (objPP->ax1 < objE->x && objE->x < objPP->ax2) 
					{
						if (objPP->ay1 < objE->y && objE->y < objPP->ay2)
						{	/* 状態の変更 */
							if (Enemy_GetState(objE) == ENEMY_SHOT)
							{
								/* 状態の変更 */
								Enemy_SetState(objE, ENEMY_HIT2);
							}
							//if (PlayerBullet_GetState(objPP) == PLAYERBULLET_SLASH)
							//{	/* 状態の変更 */
							//	PlayerBullet_SetState(objPP, PLAYERBULLET_HIT);
							//}
						}
					}
					//objPP->ax1

					#endif // 0
				}

				////
				if (( PlayerBullet_GetState(objPP) == PLAYERBULLET_SHOT/*PLAYERBULLET_SLASH*/)
					/*|| ( objPP->state == PLAYERBULLET_EXPLOSION )*/)
				{	/* Playerの弾が通常状態であれば、コリジョン検査を行う */
#if _USE_OUTER_PRODUCT_
					e.Cx = objPP->xold;	e.Cy = objPP->yold;
					e.Dx = objPP->x;	e.Dy = objPP->y;
					if (CrossCheck(&e))
					{	/* 状態の変更 */
						if (Enemy_GetState(objE) == ENEMY_SHOT)
						{	/* 状態の変更 */
							Enemy_SetState(objE, ENEMY_HIT);
						}
						if (PlayerBullet_GetState(objPP) == PLAYERBULLET_SHOT)
						{	/* 状態の変更 */
							PlayerBullet_SetState(objPP, PLAYERBULLET_HIT);
						}
					}
#else
					int xlen = objE->x + objPP->x;
					int ylen = objE->y + objPP->y;
					int dist = objE->size - objPP->size;
					/* ２点間の距離が、直線距離（二つの大きさの和)より小さければ当たり　*/
					if (((xlen * xlen) + (ylen * ylen)) < (dist * dist))
					{	/* 状態の変更 */
						if (Enemy_GetState(objE) == ENEMY_SHOT)
						{
							/* 状態の変更 */
							Enemy_SetState(objE, ENEMY_HIT);
						}
						//if (PlayerBullet_GetState(objPP) == PLAYERBULLET_SLASH)
						//{   /* 状態の変更 */
						//	PlayerBullet_SetState(objPP, PLAYERBULLET_HIT);
						//}
					}
#endif // 0
				}
				////
				/* 次のデータを見に行く */
				/* もし、->nextがNULLだったら、objPPがNULLになるので whileが終了する */
				objPP = objPP->next;
			}
		}
		/* 次のデータを見に行く */
		/* もし、->nextがNULLだったら、objEがNULLになるので whileが終了する */
		objE = objE->next;
	}

	//slash
	
	//
#endif
}

/*
*	外積を利用した当たり判定
*	二つの線分が交差するかどうかを調べるものだが、
*	プレイヤーは移動しないこともあるので使いにくい
* 
*	また、これを利用するためには移動する前と移動した後の情報が必要になる
* *https://yttm-work.jp/collision/collision_0011.html
*/
#if _USE_OUTER_PRODUCT_
int CrossCheck(struct linesedge* e)
{
	float ABx,ABy;
	float ACx,ACy;
	float ADx,ADy;

	float CDx,CDy;
	float CAx,CAy;
	float CBx,CBy;
	ABx = e->Bx - e->Ax;	ABy = e->By - e->Ay;	/*	AB	Main Line	*/
	ACx = e->Cx - e->Ax;	ACy = e->Cy - e->Ay;	/*	AC	*/
	ADx = e->Dx - e->Ax;	ADy = e->Dy - e->Ay;	/*	AD	*/

	CDx = e->Dx - e->Cx;	CDy = e->Dy - e->Cy;	/*	CD	Main Line */
	CAx = e->Ax - e->Cx;	CAy = e->Ay - e->Cy;	/*	CA	*/
	CBx = e->Bx - e->Cx;	CBy = e->By - e->Cy;	/*	CB	*/

	float f01,f02;
	float f11,f12;
	f01 = ABx * ACy - ACx * ABy;
	f02 = ABx * ADy - ADx * ABy;

	f11 = CDx * CAy - CAx * CDy;
	f12 = CDx * CBy - CBx * CDy;

	if (f01 * f02 > 0.0) {
		return 0;
	}
	if (f11 * f12 > 0.0) {
		return 0;
	}
	return 1;
}
#endif // _USE_OUTER_PRODUCT_

