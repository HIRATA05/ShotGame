/*
* 
* Enemy
* 
* 敵
* 
*/

#include "DxLib.h"
#include "ShotG.h"

#include "InputDevice.h"

#include "game.h"
#include "player.h"
#include "enemy.h"
#include "enemybullet.h"

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
*	敵を管理するプログラムが使用する変数などの宣言
*/
struct ENEMY_MGR_P
{
	int	tmr;
	int	number_of_objects;
};

static struct  ENEMY_MGR_P *work=NULL;

/*
*	敵本体のプログラムが使用する変数などの宣言
*/
/* このプログラムで使用する構造体をOBJWORK_Pと定義してしまい、見た目の汎用化を図る　*/
#define	OBJWORK_P	struct ENEMYOBJECT_P
static OBJWORK_P *objwork=NULL;

/* 片方向リンクを利用したプログラム */
static OBJWORK_P *CreateNewWork();
static OBJWORK_P *DeleteWork( OBJWORK_P *p );
/* ************************************************************************* */
/*static */void Enemy_SetState( OBJWORK_P *n, int s ){ n->state = s; }	/* 敵のstateを設定する */
/*static */int Enemy_GetState( OBJWORK_P *n ){ return n->state; }		/* 敵のstateを取得する　*/

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
int	Enemy_Init()	/* GameMode INITで呼ぶ	*/
{
	if( !work ){ work = (struct ENEMY_MGR_P *)malloc( sizeof( struct ENEMY_MGR_P ) );	}
	if( work ){ memset( work, 0, sizeof( struct ENEMY_MGR_P ) );	}
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
int	Enemy_End()	/* GameMode ENDで呼ぶ	*/
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
struct ENEMYPOS {
	int		x, y;		/* 発生位置 */
	int		type;		/* 動作タイプ */
	int		btm;		/* 弾を撃つまでの時間 */
	int		startTm;	/* 発生時間 */
};

struct ENEMYPOSDT {
	int		startTm;	/* 出現時間 */
	struct ENEMYPOS data;
};

static struct ENEMYPOSDT enemyposdt[ ] = 
{
		{	_1SECOND * 1,	{	980,	SCRN_HEIGHT / 2,	ENEMYTYPE_MYMOVE_Q,		_1SECOND, 0 }},
		{	_1SECOND * 1,	{	980,	SCRN_HEIGHT / 2,	ENEMYTYPE_MYMOVE_Z,		_1SECOND, 0 }},

		{	_1SECOND * 1.5,	{	980,	SCRN_HEIGHT / 1.5,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 1.5,	{	980,	SCRN_HEIGHT / 3,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},

		{	_1SECOND * 2,	{	980,	SCRN_HEIGHT / 1.5,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 2,	{	980,	SCRN_HEIGHT / 3,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 2.2,	{	980,	SCRN_HEIGHT / 1.5,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 2.2,	{	980,	SCRN_HEIGHT / 3,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 2.4,	{	980,	SCRN_HEIGHT / 1.5,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 2.4,	{	980,	SCRN_HEIGHT / 3,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 2.6,	{	980,	SCRN_HEIGHT / 1.5,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 2.6,	{	980,	SCRN_HEIGHT / 3,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 2.6,	{	980,	SCRN_HEIGHT / 2,	ENEMYTYPE_MYMOVE_Q,		_1SECOND, 0 }},
		{	_1SECOND * 2.6,	{	980,	SCRN_HEIGHT / 2,	ENEMYTYPE_MYMOVE_Z,		_1SECOND, 0 }},

		{	_1SECOND * 3,	{	980,	SCRN_HEIGHT / 2,	ENEMYTYPE_MYMOVE_Q,		_1SECOND, 0 }},
		{	_1SECOND * 3,	{	980,	SCRN_HEIGHT / 2,	ENEMYTYPE_MYMOVE_Z,		_1SECOND, 0 }},
		{	_1SECOND * 3,	{	980,	SCRN_HEIGHT / 2,	ENEMYTYPE_MYMOVE_Z,		_1SECOND, 0 }},
		{	_1SECOND * 3,	{	980,	SCRN_HEIGHT / 2,	ENEMYTYPE_MYMOVE_Q,		_1SECOND, 0 }},
		/*
		{	_1SECOND * 1,	{	SCRN_WIDTH/2,	-60,	ENEMYTYPE_TOP2BOTTOM,	_1SECOND, 0	}},
		{	_1SECOND * 2,	{	SCRN_WIDTH/2,	-60,	ENEMYTYPE_TOP2BOTTOM,	_1SECOND, 0	}},
		{	_1SECOND * 3,	{	SCRN_WIDTH/2,	-60,	ENEMYTYPE_TOP2BOTTOM,	_1SECOND, 0	}},
		{	_1SECOND * 4,	{	SCRN_WIDTH/2,	-60,	ENEMYTYPE_TOP2BOTTOM,	_1SECOND, 0	}},
		{	_1SECOND * 5,	{	SCRN_WIDTH/2,	-60,	ENEMYTYPE_TOP2BOTTOM,	_1SECOND, 0	}},

		{	_1SECOND * 6,	{	-20,	SCRN_HEIGHT/2,	ENEMYTYPE_LEFT2RIGHT,	_1SECOND, 0	}},
		{	_1SECOND * 8,	{	-20,	SCRN_HEIGHT/2,	ENEMYTYPE_LEFT2RIGHT,	_1SECOND, 0	}},
		{	_1SECOND *10,	{	-20,	SCRN_HEIGHT/2,	ENEMYTYPE_LEFT2RIGHT,	_1SECOND, 0	}},
		{	_1SECOND *12,	{	-20,	SCRN_HEIGHT/2,	ENEMYTYPE_LEFT2RIGHT,	_1SECOND, 0	}},
		{	_1SECOND *14,	{	-20,	SCRN_HEIGHT/2,	ENEMYTYPE_LEFT2RIGHT,	_1SECOND, 0	}},
		*/
};
#define	MAX_ENEMY_TIME_TABLE	(_1SECOND *16)	/* テーブルの最終時刻+2秒	*/
#define	ENEMYPOSDT_NUM_MEMBER	(sizeof( enemyposdt ) / sizeof( struct ENEMYPOSDT ))
/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
int	Enemy_Update()	/* GameMode DSPで呼ぶ	*/
{


	/* 敵、発生 */
	int t = GetGameTimer( );
	t = t % MAX_ENEMY_TIME_TABLE;
	for ( int i = 0;i < ENEMYPOSDT_NUM_MEMBER; i++ )
	{
		if ( enemyposdt[ i ].startTm == t ) 
		{
			Enemy_Shoot( enemyposdt[ i ].data.x, enemyposdt[ i ].data.y, enemyposdt[ i ].data.type, enemyposdt[ i ].data.btm );
		}
	}

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
			case ENEMY_INIT:
				break;
			case ENEMY_SHOT:			/*飛翔中*/
				switch ( n->type ) 
				{
				case ENEMYTYPE_RIGHT2LEFT:
					n->x = n->x - n->spd;
					if ( n->x < -n->spd ) { theEnd = true; }
					break;
				case ENEMYTYPE_LEFT2RIGHT:
					n->x = n->x + n->spd;
					if ( n->x > ( SCRN_WIDTH + n->spd ) ) { theEnd = true; }
					break;
				case ENEMYTYPE_TOP2BOTTOM:
					n->y = n->y + n->spd;
					if ( n->y > ( SCRN_HEIGHT+n->spd) ) { theEnd = true; }
					break;
				case ENEMYTYPE_BOTTOM2TOP:
					n->y = n->y - n->spd;
					if ( n->y < -n->spd ) {	theEnd = true;	}
					break;
				case ENEMYTYPE_MYMOVE_M:
					n->y = n->y + n->spd;
					n->x = n->x + (n->spd / 2);

					if (n->y > (SCRN_HEIGHT + n->spd)) { theEnd = true; }
					break;

				case ENEMYTYPE_MYMOVE_B:
					n->x = n->x + n->spd;
					n->x = n->x - (n->spd / 2);
					if (n->y > (SCRN_HEIGHT + n->spd)) { theEnd = true; }
					break;

				case ENEMYTYPE_MYMOVE_Q:
					n->x = n->x - n->spd;
					n->y = n->y - (n->spd / 3);
					if (n->x < -n->spd) { theEnd = true; }
					break;

				case ENEMYTYPE_MYMOVE_Z:
					n->x = n->x - n->spd;
					n->y = n->y + (n->spd / 3);
					if (n->x < -n->spd) { theEnd = true; }
					break;
				}
				if( theEnd )
				{
					Enemy_SetState( n, ENEMY_DONE );	/*画面外に行ってしまった*/
				}else{
					DrawCircle( n->x, n->y, n->size, ENEMY_COLOR, TRUE );
					n->tmr++;
					if ( n->tmr == n->btm ) { /* 弾を打つタイミングになったなら */
						EnemyBullet_Shoot( n->x, n->y,ENEMYBULLET_TYPE_0 );
					}
				}
				break;
			case ENEMY_HIT:			/*当たり*/
				{
					/* ここでスコアアップしたり、効果音を鳴らしたりするのは
					* のちに「耐久敵」などへの対応を可能にしやすくするためです	*/
					/* スコアアップ */
					modeGame_AddScore(1);
					/* 効果音 */
					sfx_EnmHit();

					Enemy_SetState(n, ENEMY_EXPLOSION);
					/* fall through で EXPLOSIONに移行してもよい */
					break;
				}	
			//
			case ENEMY_HIT2:			/*当たり2*/
				{
					/* ここでスコアアップしたり、効果音を鳴らしたりするのは
					* のちに「耐久敵」などへの対応を可能にしやすくするためです	*/
					/* スコアアップ */
					modeGame_AddScore2(2);
					/* 効果音 */
					sfx_EnmHit();

					Enemy_SetState(n, ENEMY_EXPLOSION);
					/* fall through で EXPLOSIONに移行してもよい */
					break;
				}
			//
			case ENEMY_EXPLOSION:	/*破壊中*/
				{
					n->etmr++;
					if( n->etmr > _1SECOND )
					{
						Enemy_SetState( n, ENEMY_DONE );	/*画面外に行ってしまった → 終了 */
					}
					if( n->etmr & 4 )
					{
						DrawCircle( n->x, n->y, n->size, ENEMY_COLOR, TRUE );
					}else{
						DrawCircle( n->x, n->y, n->size, ~ENEMY_COLOR, TRUE );
					} 
				}
				break;
			case ENEMY_DONE:			/*終了中*/
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
			case ENEMY_INIT:
			case ENEMY_SHOT:		/*飛翔中*/
			case ENEMY_HIT:			/*当たり*/
			case ENEMY_EXPLOSION:	/*破壊中*/
				n = n->next;
				break;
			case ENEMY_DONE:		/*終了中*/
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
			DrawCircle( x, 500-14, 6, ENEMY_COLOR, TRUE );
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
int Enemy_Shoot( int x, int y,int type, int btm )	/* Enemy Position */
{
	OBJWORK_P *n;


	n = CreateNewWork();
	if( n )
	{
		Enemy_SetState( n, ENEMY_SHOT );	/*撃つ*/
		n->x = x;
		n->y = y;
		n->xold = n->x;
		n->yold = n->y;
		n->spd = ENEMY_SPD;
		n->type = type;
		n->btm = btm;
		n->size = ENEMY_SIZE;
		DrawString( x-4, y-32, "X", GetColor( 240,40,40 ) );
	}else
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
OBJWORK_P *GetObject_Enemy( int n )	/* プレイヤーの弾の変数領域の先頭のポインタを取得する	*/
{
	return objwork;
}


