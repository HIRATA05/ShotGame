/*
* 
* Enemy
* 
* 敵
* 
*/

#include "math.h"
#include "DxLib.h"
#include "ShotG.h"

#include "InputDevice.h"

#include "game.h"
#include "player.h"
#include "enemybullet.h"


/*
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
*/

/*
*	敵の弾を管理するプログラムが使用する変数などの宣言
*/
struct ENEMYBULLET_MGR_P
{
	int	tmr;
	int	number_of_objects;
};

static struct  ENEMYBULLET_MGR_P *work=NULL;

/*
*	敵の弾本体のプログラムが使用する変数などの宣言
*/
/* このプログラムで使用する構造体をOBJWORK_Pと定義してしまい、見た目の汎用化を図る　*/
#define	OBJWORK_P	struct ENEMYBULLETOBJECT_P
static OBJWORK_P *objwork=NULL;

/* 片方向リンクを利用したプログラム */
static OBJWORK_P *CreateNewWork();
static OBJWORK_P *DeleteWork( OBJWORK_P *p );
/* ************************************************************************* */
/*static */void EnemyBullet_SetState( OBJWORK_P *n, int s ){ n->state = s; }
/*static */int EnemyBullet_GetState( OBJWORK_P *n ){ return n->state; }

/* ************************************************************************* */
static OBJWORK_P *CreateNewWork()
{
	if( !objwork )
	{// objworkがNULLの時(なにも登録されていないとき)
		OBJWORK_P *p = (OBJWORK_P *)malloc( sizeof( OBJWORK_P ) );
		if( p )
		{
			memset( p, 0, sizeof( OBJWORK_P ) );
			p->next = NULL;	// リンクの最後を表すフラグ
			objwork = p;	// 先頭を登録
			return p;
		} else {	/* 失敗した場合・・・致命的なエラーではあるが、今回は対応しない　*/
		}
		return NULL;
	}else
	{// すでに登録されているとき
		OBJWORK_P *n = objwork;
		// 最後のポインタを探す
		while( n->next )
		{
			n = n->next;
		}
		OBJWORK_P *p = (OBJWORK_P *)malloc( sizeof( OBJWORK_P ) );
		if( p )
		{
			memset( p, 0, sizeof( OBJWORK_P ) );
			p->next = NULL;	// リンクの最後を表すフラグ
			n->next = p;
			return p;
		} else {	/* 失敗した場合・・・致命的なエラーではあるが、今回は対応しない　*/
		}
		return NULL;
	}
}
/* ************************************************************************* */
static OBJWORK_P *DeleteWork( OBJWORK_P *p )
{
	OBJWORK_P *n = objwork;
	// pを含むポインタを探す
	if( !n )
	{// 登録されていない->致命的なエラー
		return NULL;
	}

	if( n == p )
	{//リストの先頭の場合
		n = p->next;
		free( p );
		p = NULL;
		objwork = n;
	}else
	{
		while( n->next )
		{
			if( n->next == p )
			{//発見
				n->next = p->next;	// リンクの張替え
				free( p );
				p = NULL;
				break; // whileの終了
			}
			n = n->next;
		}
	}

	if( p )
	{// 発見できていないとき->致命的なエラー
		return NULL;
	}
	if( n ){ return n->next; };
	return NULL;	// 全部を解放したとき(リストが空になった)
}

/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
int	EnemyBullet_Init()	/* GameMode INITで呼ぶ	*/
{
	if( !work ){ work = (struct ENEMYBULLET_MGR_P *)malloc( sizeof( struct ENEMYBULLET_MGR_P ) );	}
	if( work ){ memset( work, 0, sizeof( struct ENEMYBULLET_MGR_P ) );	}
	objwork = NULL;
	return 0;
}


/* *************************************************************************
*
*
*
*
*
* **************************************************************************/
int	EnemyBullet_End()	/* GameMode ENDで呼ぶ	*/
{
//不具合があるかも・・・
	if(objwork)
	{//リストがあるのですべてを空にする
		// 片方向の場合、全部をクリアするのが面倒
		OBJWORK_P *n = objwork;
		OBJWORK_P *p;
		while(n)
		{
			p = n->next;
			DeleteWork(n);
			n = p;
			/* 次のようなプログラムにするのはNG
			* DeleteWork( n );
			* n = n->next;
			* なぜならば、DeleteWork()関数によって、nの内容は無効になっているので、
			* nの内容を参照することを行ってはならない
			*/
		}
	}

	if(work){
		free(work); work=NULL;
	}
	return 0;
}
/* *************************************************************************
*
*
*
*
*
* **************************************************************************/
/* 動作種類 */
enum {
	ENEMYBULLETTYPE_0 = 0,
	ENEMYBULLETTYPE_RIGHT2LEFT,
	ENEMYBULLETTYPE_LEFT2RIGHT,
	ENEMYBULLETTYPE_UP2BOTTOM,
	ENEMYBULLETTYPE_BOTTOM2UP,
	ENEMYBULLETTYPE_END,
};

/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
int	EnemyBullet_Update()	/* GameMode DSPで呼ぶ	*/
{
	OBJWORK_P* n = objwork;
	OBJWORK_P* p;

	/* 表示処理 */
	while( n )
	{
		/* 移動前の位置を記録する */
		n->xold = n->x;
		n->yold = n->y;
		int	theEnd = false;
		switch( n->state )
		{
			case ENEMYBULLET_INIT:
				break;
			case ENEMYBULLET_SHOT:			/*飛翔中*/
				theEnd = false;
				switch( n->type )
				{
					case ENEMYBULLET_TYPE_0:
					{
						n->xf += n->spdx;
						n->yf += n->spdy;

						n->x = (int)n->xf;	/* ちゃんと型キャストを明示する [実数から整数への変換] */
						n->y = (int)n->yf;	/* ちゃんと型キャストを明示する [実数から整数への変換] */
						/* 画面外判定 */
						if((n->x < -ENEMYBULLET_SIZE)
							 || (n->x > SCRN_WIDTH + ENEMYBULLET_SIZE)
							 || (n->y < -ENEMYBULLET_SIZE)
							 || (n->y > SCRN_HEIGHT + ENEMYBULLET_SIZE)) {
							theEnd = true;
						}
						if(theEnd)
						{
							EnemyBullet_SetState(n,ENEMYBULLET_DONE);	/*画面外に行ってしまった*/
						}else
						{
							DrawCircle(n->x,n->y,ENEMYBULLET_SIZE,ENEMYBULLET_COLOR,TRUE);
						}
						break;
					}
					default:
						break;
				}
				break;
			case ENEMYBULLET_HIT:			/*当たり*/
				EnemyBullet_SetState(n, ENEMYBULLET_EXPLOSION);
				/* fall through で EXPLOSIONに移行する */
				/*break;*/
			case ENEMYBULLET_EXPLOSION:		/*破壊中*/
				EnemyBullet_SetState(n, ENEMYBULLET_DONE);
				break;
			case ENEMYBULLET_DONE:			/*終了中*/
				break;
		}
		n = n->next;
	}


	/* 削除処理 */
	n = objwork;
	while( n )
	{
		switch( n->state )
		{
			default:
			case ENEMYBULLET_INIT:
			case ENEMYBULLET_SHOT:			/*飛翔中*/
			case ENEMYBULLET_EXPLOSION:	/*破壊中*/
				n = n->next;
				break;
			case ENEMYBULLET_DONE:			/*終了中*/
				/* 終了処理 */
				p = n->next;
				DeleteWork( n );
				n = p;
				/* 次のようなプログラムにするのはNG
				* DeleteWork( n );
				* n = n->next;
				* なぜならば、DeleteWork()関数によって、nの内容は無効になっているので、
				* nの内容を参照することを行ってはならない
				*/
				break;
		}
	}

#if DEBUG
	{	// 有効な変数領域数を●であらわす
		OBJWORK_P *n = objwork;
		int x = 8;
		while( n )
		{
			DrawCircle( x, 500-14-14, 6, ENEMYBULLET_COLOR, TRUE );
			x = x +14;
			n = n->next;
		}
	}
#endif // DEBUG

	return 0;
}

/* *************************************************************************
* 敵を打つ関数
* 
* 
* 
* 
* **************************************************************************/
int EnemyBullet_Shoot( int x, int y,int type )	/* Enemy Position */
{
	OBJWORK_P *n;


	n = CreateNewWork();
	if( n )
	{
		EnemyBullet_SetState( n, ENEMYBULLET_SHOT );	/*撃つ*/
		n->x = x;
		n->y = y;
		n->xold = n->x;
		n->yold = n->y;
		n->spd = ENEMYBULLET_SPD;
		n->type = type;
		n->size = ENEMYBULLET_SIZE;
		GetPlayerPosition( &n->tagx, &n->tagy );	/* Playerの場所を取得する */
		/* 移動速度の計算 */
		double diffx, diffy;
		double dist,frm;
		diffx = (double)n->tagx - ( double )n->x;
		diffy = ( double )n->tagy - ( double )n->y;
		dist = (float)sqrt( diffx * diffx + diffy * diffy );	// 2点間距離
		frm = dist / n->spd;	// frm ... 目的地に到達するまでのフレーム数
		n->spdx = (float)(diffx / frm);	// 各方向移動速度
		n->spdy = (float)(diffy / frm);
		n->xf = (float)n->x;	/* ちゃんと型キャストを明示する [整数から実数への変換] */
		n->yf = (float)n->y;	/* ちゃんと型キャストを明示する [整数から実数への変換] */

		//DrawString( x-4, y-32, "X", GetColor( 240,40,40 ) );
	}else
	{/*弾を打てなかった！*/
		//DrawString( x-4-12, y-32, "スカ！", GetColor( 240,40,40 ) );
	}
	return 0;
}

/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
OBJWORK_P *GetObject_EnemyBullet( int n )	/* プレイヤーの弾の変数領域の先頭のポインタを取得する	*/
{
	return objwork;
}


