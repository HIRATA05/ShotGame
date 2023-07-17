/*
* 
* Player Bullet
* 
* プレイヤーの弾
* 
*/

#include "DxLib.h"
#include "ShotG.h"

#include "InputDevice.h"

#include "game.h"
#include "player.h"
#include "playerbullet.h"
#include "sfx.h"

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
*	弾を管理するプログラムが使用する変数などの宣言
*/
struct PLAYERBULLET_MGR_P
{
	int	tmr;
	int	number_of_objects;
};

static struct  PLAYERBULLET_MGR_P *work=NULL;

/*
*	弾本体のプログラムが使用する変数などの宣言
*/



/* 配列を利用したプログラム */
#if _ARRAY_MODE_
#define	MAX_PLAYER_BULLET	3	/*最大弾数*/
static struct PLAYERBULLETOBJECT_P	objwork[ MAX_PLAYER_BULLET ];
static void PlayerBullet_SetState( int n, int s ){ objwork[n].state = s; }
/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
int	PlayerBullet_Init()	/* GameMode INITで呼ぶ	*/
{
	if( !work ){ work = (struct PLAYERBULLET_MGR_P *)malloc( sizeof( struct PLAYERBULLET_MGR_P ) );	}
	if( work ){ memset( work, 0, sizeof( struct PLAYERBULLET_MGR_P ) );	}
	return 0;
}
/* *************************************************************************
*
*
*
*
*
* **************************************************************************/
int	PlayerBullet_End( )	/* GameMode ENDで呼ぶ	*/
{
	if( work ) { free( work ); work = NULL; }
	return 0;
}


/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
int	PlayerBullet_Update()	/* GameMode DSPで呼ぶ	*/
{
	int i;
	for( i=0; i< MAX_PLAYER_BULLET; i++ )
	{
		switch( objwork[i].state )
		{
			case PLAYERBULLET_INIT:
				break;
			case PLAYERBULLET_SHOT:			/*飛翔中*/
				objwork[i].y = objwork[i].y - objwork[i].spd;
				if( objwork[i].y < (-objwork[i].spd) )
				{
					PlayerBullet_SetState( i, PLAYERBULLET_DONE );	/*画面外に行ってしまった*/
				}else{
					DrawCircle( objwork[i].x, objwork[i].y, PLAYERBULLET_SIZE, PLAYERBULLET_COLOR, TRUE );
				}
				break;
			case PLAYERBULLET_EXPLOSION:	/*破壊中*/
				break;
			case PLAYERBULLET_DONE:			/*終了中*/
				PlayerBullet_SetState( i, PLAYERBULLET_INIT );	/* 終了処理 */
				break;
		}
	}
	return 0;
}

/* *************************************************************************
* 弾を打つ関数
* 
* 
* 
* 
* **************************************************************************/
int PlayerBullet_Shoot( int x, int y )	/* Player Position */
{
	int i;
	int f=FALSE;/*弾を撃つことができたかどうかのフラグ*/
	for( i=0; i< MAX_PLAYER_BULLET; i++ )
	{
		switch( objwork[i].state )
		{
			case PLAYERBULLET_INIT:
				/* 弾が打てる状態 */
				PlayerBullet_SetState( i, PLAYERBULLET_SHOT );	/*撃つ*/
				objwork[i].x = x;
				objwork[i].y = y;
				objwork[i].spd = PLAYERBULLET_SPD;
				DrawString( x-4, y-32, "X", GetColor( 240,40,40 ) );
				f=TRUE;
				break;
			case PLAYERBULLET_SHOT:			/*飛翔中*/
			case PLAYERBULLET_EXPLOSION:	/*破壊中*/
			case PLAYERBULLET_DONE:			/*終了中*/
				break;
		}
		if( f ){ break; }/*弾を打つことに成功したのでfor文を終了させる*/
	}
	if( !f )
	{/*弾を打てなかった！*/
		DrawString( x-4-12, y-32, "スカ！", GetColor( 240,40,40 ) );
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
struct PLAYERBULLETOBJECT_P *GetObject_PlayerBullet( int n )	/* プレイヤーの弾の変数領域の先頭のポインタを取得する	*/
{
	return objwork;
}


/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */
#else  // !_ARRAY_MODE_
/* このプログラムで使用する構造体をOBJWORK_Pと定義してしまい、見た目の汎用化を図る　*/
#define	OBJWORK_P	struct PLAYERBULLETOBJECT_P
static OBJWORK_P *objwork=NULL;

/* 片方向リンクを利用したプログラム */
static OBJWORK_P *CreateNewWork();
static OBJWORK_P *DeleteWork( OBJWORK_P *p );
/* ************************************************************************* */
/* 状態遷移に関する関数 */
/*static */void PlayerBullet_SetState( OBJWORK_P *n, int s ){ n->state = s; }
/*static */int PlayerBullet_GetState( OBJWORK_P *n ){ return n->state; }
/* ************************************************************************* */
/* リンクリストに新規データを追加して、その領域のポインタを返す　*/
static OBJWORK_P *CreateNewWork()
{
	if( !objwork )
	{	/* objworkがNULLの時は、リストが存在しないとき(なにも登録されていない)
		*	変数領域を借用する(malloc)
		*	リストの先頭になるので、objworkにポインタを登録する
		*/
		/* メモリを借用する */
		OBJWORK_P *p = (OBJWORK_P *)malloc( sizeof( OBJWORK_P ) );
		if ( p ) {	/* メモリの借用に成功 */
			memset( p, 0, sizeof( OBJWORK_P ) );	/* 領域を0クリア */
			p->next = NULL;	/* nextをNULLにする → リンクの最後(終端)であることを表す	*/
			objwork = p;	/* 先頭に登録 */
			return p;		/* 借用したポインタを返す */
		} else 
		{	/* 失敗した場合・・・致命的なエラーではあるが、今回は対応しない　*/
		}
		/* NULLを返すようなことがある場合、それは致命的なエラーといえるので注意！！ */
		return NULL;
	}else
	{/*
	 *	すでに登録されているとき(リストが存在するとき)
	 *	変数領域を借用する(malloc)
	 *	リストの最後に追加する
	 */ 
		OBJWORK_P *n = objwork;
		/* 最後のポインタを探す */ 
		while( n->next )
		{
			n = n->next;
		}
		/* メモリを借用する */
		OBJWORK_P *p = (OBJWORK_P *)malloc( sizeof( OBJWORK_P ) );
		if( p )
		{	/* メモリの借用に成功 */
			memset( p, 0, sizeof( OBJWORK_P ) );	/* 領域を0クリア */
			p->next = NULL;	/* nextをNULLにする → リンクの最後(終端)であることを表す	*/
			n->next = p;	/* リストの最後に追加 */

			p->slashtmr = 0; //剣の表示時間の初期化
			return p;		/* 借用したポインタを返す */
		} else {	/* 失敗した場合・・・致命的なエラーではあるが、今回は対応しない　*/
		}
		/* NULLを返すようなことがある場合、それは致命的なエラーといえるので注意！！ */
		return NULL;
	}
}
/* ************************************************************************* */
/* リンクリストから除外し、変数領域を返納する */
static OBJWORK_P *DeleteWork( OBJWORK_P *p )
{
	OBJWORK_P *n = objwork;
	if( !n ) {	/* リストが存在しない →なにも登録されていない->致命的なエラー	*/
		return NULL;
	}

	/* リスト内のpと値の同じポインタを探す	*/
	if( n == p )
	{/*	リストの先頭の場合 */
		n = p->next;	/* p->nextをnに保存する */
		free( p );		/* メモリ(p)を返却する */
		p = NULL;		/* */
		objwork = n;	/* n(p->next)を先頭に登録する */
	}else
	{	/* リスト内に同じものが存在する場合 */
		while( n->next )
		{
			if( n->next == p )
			{	/* pを発見 */
				n->next = p->next;	/* リンクの張替え */
				free( p );			/* メモリ(p)を返却する */
				p = NULL;			/* */
				break;	/* whileの終了 */
			}
			n = n->next;
		}
	}

	if( p )
	{/* 発見できなかったとき、致命的な問題が起きていると考えたほうが良い */
		return NULL;	/* NULLを返すが、正常終了ではない */
	}
	if( n ){ return n->next; };
	return NULL;	/* 全部を解放したとき(リストが空になった) */
}

/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
int	PlayerBullet_Init()	/* GameMode INITで呼ぶ	*/
{
	if( !work ){ work = (struct PLAYERBULLET_MGR_P *)malloc( sizeof( struct PLAYERBULLET_MGR_P ) );	}
	if( work ){ memset( work, 0, sizeof( struct PLAYERBULLET_MGR_P ) );	}
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
int	PlayerBullet_End( )	/* GameMode ENDで呼ぶ	*/
{
//不具合があるかも・・・
	if( objwork ) 
	{//リストがあるのですべてを空にする
		// 片方向の場合、全部をクリアするのが面倒
		OBJWORK_P* n = objwork;
		OBJWORK_P* p;
		while( n ) {
			p = n->next;
			DeleteWork( n );
			n = p;
			/* 次のようなプログラムにするのはNG
			* DeleteWork( n );
			* n = n->next;
			* なぜならば、DeleteWork()関数によって、nの内容は無効になっているので、
			* nの内容を参照することを行ってはならない
			*/
		}
	}

	if( work ) { free( work ); work = NULL; }
	return 0;
}

/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
int	PlayerBullet_Update()	/* GameMode DSPで呼ぶ	*/
{
	OBJWORK_P *n = objwork;
	OBJWORK_P *p;

	/* 表示処理 */
	while( n )
	{
		/* 移動前の位置を記録する */
		n->xold = n->x;
		n->yold = n->y;
		switch( n->state )
		{
			case PLAYERBULLET_INIT:
				break;
			case PLAYERBULLET_SHOT:			/*飛翔中*/
				//n->y = n->y - n->spd;
				n->x = n->x + n->spd;
				//if( n->y < (-n->spd) )
				if( (n->x < (-n->spd)) || (n->x > SCRN_WIDTH+(n->spd)))
				{
					PlayerBullet_SetState( n, PLAYERBULLET_DONE );	/*画面外に行ってしまった*/
				}else{
					switch (n->weapontype)
					{
						case PLBULLET_TYPE_BULLET:
							DrawCircle(n->x, n->y, n->size, PLAYERBULLET_COLOR, TRUE);
							break;
						case PLBULLET_TYPE_SLASH:
						{
							n->slashtmr++; // 剣の表示時間を増加
							if (n->slashtmr > _0SECOND) { // 時間経過で剣が消滅
								PlayerBullet_SetState(n, PLAYERBULLET_DONE);
								ClearSlashFlag(); // 剣のフラグの消去
							}
							else {
								GetPlayerPosition(&n->x, &n->y);	/* Playerの位置を取得する */
								DrawString(n->x + 20, n->y - 10, "=={+>>>>", PLAYERBULLET_COLOR);
								int l = GetDrawStringWidth("=={+>>>>", strlen("=={+>>>>")); // 剣の画像の横幅を取得
								n->ax1 = n->x + 20; n->ay1 = n->y - 10;
								n->ax2 = n->x + 20 + l; n->ay2 = n->y - 10 + 14;
								//DrawBox(n->ax1, n->ay1, n->ax2, n->ay2, GetColor(240, 240, 0), 0); // 当たり判定の可視化
							}
						}
						break;
					}
				}
				break;
			case PLAYERBULLET_HIT:			/*当たり*/
				PlayerBullet_SetState(n, PLAYERBULLET_EXPLOSION);
				/* fall through で EXPLOSIONに移行する */
				/*break;*/
			case PLAYERBULLET_EXPLOSION:	/*破壊中*/
				ClearSlashFlag(); // 剣のフラグの消去
				PlayerBullet_SetState( n, PLAYERBULLET_DONE );
				break;
			case PLAYERBULLET_DONE:			/*終了中*/
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
			case PLAYERBULLET_INIT:
			case PLAYERBULLET_SHOT:			/*飛翔中*/
			//case PLAYERBULLET_SLASH:	//斬撃追加
			case PLAYERBULLET_HIT:			/*当たり*/
			case PLAYERBULLET_EXPLOSION:	/*破壊中*/
				n = n->next;
				break;
			case PLAYERBULLET_DONE:			/*終了中*/
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
			DrawCircle( x, 500, 6, PLAYERBULLET_COLOR, TRUE );
			x = x +14;
			n = n->next;
		}
	}
#endif // DEBUG

	return 0;
}


/* *************************************************************************
* 弾を打つ関数
* 
* 
* 
* 
* **************************************************************************/
int PlayerBullet_Shoot( int x, int y )	/* Player Position */
{
	OBJWORK_P *n;


	n = CreateNewWork();
	if( n )
	{
		sfx_PlShoot();
		PlayerBullet_SetState( n, PLAYERBULLET_SHOT );	/*撃つ*/
		n->x = x;
		n->y = y;
		n->xold = n->x;
		n->yold = n->y;
		n->spd = PLAYERBULLET_SPD;
		n->size = PLAYERBULLET_SIZE;
		n->weapontype = PLBULLET_TYPE_BULLET; // 武器の種類 弾
		DrawString( x+20, y-10, "X", GetColor( 240,40,40 ) );
	}else
	{/*弾を打てなかった！*/
		DrawString( x-4-12, y-32, "スカ！", GetColor( 240,40,40 ) );
	}
	return 0;
}

/* 追加*********************************************************************
*
*
*
*
*
* **************************************************************************/
int PlayerBullet_Slash(int x, int y)	/* Player Position */
{
	OBJWORK_P* n;


	n = CreateNewWork();
	if (n)
	{
		sfx_PlShoot();
		PlayerBullet_SetState(n, PLAYERBULLET_SHOT/*PLAYERBULLET_SLASH*/);	/*斬る*/
		n->x = x;
		n->y = y;
		n->xold = n->x;
		n->yold = n->y;
		n->size = PLAYERBULLET_SIZE;
		n->weapontype = PLBULLET_TYPE_SLASH; // 武器の種類 弾
		//DrawString( x-4, y-32, "X", GetColor( 240,40,40 ) );
		DrawString(x + 20, y-10, "+", GetColor(240, 40, 40));
	}
	else
	{/*弾を打てなかった！*/
		DrawString(x - 4 - 12, y - 32, "スカ！", GetColor(240, 40, 40));
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
OBJWORK_P *GetObject_PlayerBullet( int n )	/* プレイヤーの弾の変数領域の先頭のポインタを取得する	*/
{
	return objwork;
}



#endif // !_ARRAY_MODE_
