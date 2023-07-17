/*
* Player
* 
* 
* 
* 
*/

/*
* Playerに関する処理
* 
* 
* 
* 
* 
*/
#include "math.h"
#include "DxLib.h"
#include "ShotG.h"

#include "InputDevice.h"

#include "game.h"
#include "player.h"
#include "playerbullet.h"

#include "sfx.h"

struct PLYER_P
{
	int	tmr;
	int	state;		/* 状態 */
	int	x, y;		/* 位置の整数値	*/
	int xold, yold;	/* 位置の前回値 */
	int size;		/* 物体の大きさ */
	int spd;		/* 飛翔速度(スカラ)	*/

	int slashflag;
};


static struct PLYER_P *work;

static void player_graphics( int x, int y );	/* playerを描画する関数 */
static void player_graphics2( int x, int y );	/* playerを描画する関数 */
/* *************************************************************************
* 
* **************************************************************************/
/* playerの状態を設定する */
void SetPlayerState( int state ) { work->state = state; }
/* playerの状態を取得する */
int GetPlayerState(  ) { return work->state; }
/* playerの位置を取得する */
void GetPlayerPosition( int* x, int* y ) {	*x = work->x;	*y = work->y;	return; }
void GetPlayerOldPosition(int* x, int* y) { *x = work->xold;	*y = work->yold;	return; }
/* playerの大きさを取得する */
void GetPlayerSize( int* size ) { *size = work->size;	return;	}

/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
int Player_Init( void ) 
{
	if( !work ){	work = (struct PLYER_P*)malloc( sizeof( struct PLYER_P) ); }
	if( !work ){	return 0;}
	memset( work, 0, sizeof(struct PLYER_P) );

	work->state = PLSTATE_INIT;
	//work->x = SCRN_WIDTH/2;
	//work->y = SCRN_HEIGHT - SCRN_HEIGHT/16;
	work->x = SCRN_WIDTH / 3;
	work->y = SCRN_HEIGHT / 2;
	work->size = PLAYER_R;

	return 1;
	
}
/* *************************************************************************
*
*
*
*
*
* **************************************************************************/
int Player_End(void)
{
	if(work) {
		free(work); work = NULL;
	}
	return 1;
}

/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
int Player_Dsp( void ) 
{
	/* 移動前の位置を記録する */
	work->xold = work->x;
	work->yold = work->y;

	switch (work->state) 
	{
		case PLSTATE_INIT:		// 初期状態
			{
				SetPlayerState( PLSTATE_NORMAL );
			}
			break;
		case PLSTATE_NORMAL:			// 通常状態
			{
				/* 発射 */
				if (isKeyOn( KEY_INPUT_Z )) { PlayerBullet_Shoot( work->x,work->y ); }

				/* 抜刀 */
				if (isKeyOn(KEY_INPUT_X)) {
					if (work->slashflag == 0) { //もしも剣が出ていなければ実行
						work->slashflag = 1; //剣が発生したフラグ
						PlayerBullet_Slash(work->x, work->y);
						//DrawString(work->x + 20, work->y - 10, "=={<*>>>", GetColor(240, 40, 40));
					}
				}

				/* 移動 */
				if (isKeyDown( KEY_INPUT_LEFT )) { work->x = work->x > 0 ? work->x - PL_XSPD : 0; }
				if (isKeyDown( KEY_INPUT_RIGHT )) { work->x = work->x < SCRN_WIDTH ? work->x + PL_XSPD : SCRN_WIDTH; }

				if (isKeyDown(KEY_INPUT_UP)) { work->y = work->y > 0 ? work->y - PL_YSPD : 0; }
				if (isKeyDown(KEY_INPUT_DOWN)) { work->y = work->y < SCRN_HEIGHT ? work->y + PL_YSPD : SCRN_HEIGHT; }

				player_graphics( work->x, work->y );
			}
			break;
		case PLSTATE_HIT:			// 当たった瞬間
			{
				work->tmr = 0;
				sfx_PlHit();	/* 効果音 */
				SetPlayerState( PLSTATE_EXPLOSION );
			}
			break;
		case PLSTATE_EXPLOSION:		// 爆発中
			{
				work->tmr++;
				if ( work->tmr & 4 ) 
				{
					player_graphics( work->x, work->y );
				} else {
					player_graphics2( work->x, work->y );
				}
				if ( work->tmr > PL_EXPLOSION_TIME ) 
				{
					int n;
					n = modeGame_SubPlayer( 1 );
					if( n > 0 )
					{
						SetPlayerState( PLSTATE_MUTEKI );
					}else{
						sfx_GaneOver();
						SetPlayerState( PLSTATE_GAMEOVER );
					}
				}
			}
			break;
		case PLSTATE_MUTEKI:			// 回復状態
			{
				/* 発射 */
				if ( isKeyOn( KEY_INPUT_Z ) ) { PlayerBullet_Shoot( work->x, work->y ); }

				/* 抜刀 */
				if (isKeyOn(KEY_INPUT_X)) {
					if (work->slashflag == 0) { //もしも剣が出ていなければ実行
						work->slashflag = 1; //剣が発生したフラグ 剣を表示している間はTrue
						PlayerBullet_Slash(work->x, work->y);
						//DrawString(work->x + 20, work->y - 10, "=={<*>>>", GetColor(240, 40, 40));
					}
				}

				/* 移動 */
				if ( isKeyDown( KEY_INPUT_LEFT ) ) { work->x = work->x > 0 ? work->x - PL_XSPD : 0; }
				if ( isKeyDown( KEY_INPUT_RIGHT ) ) { work->x = work->x < SCRN_WIDTH ? work->x + PL_XSPD : SCRN_WIDTH; }

				if (isKeyDown(KEY_INPUT_UP)) { work->y = work->y > 0 ? work->y - PL_YSPD : 0; }
				if (isKeyDown(KEY_INPUT_DOWN)) { work->y = work->y < SCRN_HEIGHT ? work->y + PL_YSPD : SCRN_HEIGHT; }

				work->tmr++;
				if ( work->tmr & 1 ) 
				{
					player_graphics( work->x, work->y );
				}
				if ( work->tmr > PL_EXPLOSION_TIME + PL_MUTEKI_TIME ) 
				{
					SetPlayerState( PLSTATE_NORMAL );
				}
			}
			break;
		case PLSTATE_GAMEOVER:		// ゲームオーバー時
			{
				PlaySound("", DX_PLAYTYPE_BACK);
				work->tmr++;
				if ( work->tmr & 4 ) 
				{
					player_graphics( work->x, work->y );
				} else {
					player_graphics2( work->x, work->y );
				}

			}
			break;
		default:	
			{
			}
			break;
	}

	return 0;
}


/* *************************************************************************
*	Playerの表示関数
*	
* 
* 
* 
* **************************************************************************/
static void player_graphics( int x, int y ) 
{
	int xl, xr;
	int yt, yb;


	//xl = x - PLAYER_R * 2;
	//xr = x + PLAYER_R * 2;

	//yt = y - PLAYER_R * 2;
	//yb = y + PLAYER_R * 1;
	xl = x - PLAYER_R * 1;
	xr = x + PLAYER_R * 2;

	yt = y - PLAYER_R * 2;
	yb = y + PLAYER_R * 2;

	//DrawTriangle( xl, yb, xr, yb, x, yt, PLAYER_COLOR2, true );
	DrawTriangle(xl, yb, xl, yt, xr, y, PLAYER_COLOR2, true);
	DrawCircle( x, y, PLAYER_R, PLAYER_COLOR1, true );
}
static void player_graphics2( int x, int y )
{
	int xl, xr;
	int yt, yb;


	xl = x - PLAYER_R * 2;
	xr = x + PLAYER_R * 2;

	yt = y - PLAYER_R * 2;
	yb = y + PLAYER_R * 1;

	DrawTriangle( xl, yb, xr, yb, x, yt, PLAYER_COLOR1, true );
	DrawCircle( x, y, PLAYER_R, PLAYER_COLOR2, true );

}


void ClearSlashFlag(void)
{
	work->slashflag = 0;
}
