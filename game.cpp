/*
* Title Mode
* 
* modectrlから呼ばれる関数は、返り値として
*  0 ... mode ctrlに変更を伝えない
*  1 ... mode ctrlに変更を伝える
* を返す。
* 
*/
#include "math.h"
#include "DxLib.h"
#include "ShotG.h"

#include "InputDevice.h"

#include "game.h"
#include "player.h"
#include "playerbullet.h"
#include "enemy.h"
#include "enemybullet.h"
#include "collilsion.h"
#include "sfx.h"

#define	GAMEMDOE_TIMEOUT	(30*FRAME_PER_SECOND)	// second	ゲームの制限時間の設定
#define	GAMEOVER_TIMEOUT	(5*FRAME_PER_SECOND)	// second
/*
* Game Mode用の変数
*/
struct GAME_P
{
	int	tmr;

	int	r,g,b;
	int playerLeft;	//残機数
	int score_bullet;		//得点
	int score_slash;		//得点
	int	subtmr;

};

static struct GAME_P *work = NULL;

/*
* 
* ゲーム内カウンタを取得する
* 
*/
int GetGameTimer( void ) 
{
	if ( work ) {
		return work->tmr;
	}
	return 0;
}

int modeGame_Init( void )
{
	if( !work ){	work = (struct GAME_P*)malloc( sizeof( struct GAME_P) ); }
	if( !work ){	return 0;}
	memset( work, 0, sizeof(struct GAME_P) );
	work->playerLeft = DEFAULT_PLAYER_LEFT;

	SFX_Init();
	/*プレイヤー初期化*/
	Player_Init();
	PlayerBullet_Init();

	Enemy_Init( );
	EnemyBullet_Init( );

	return 1;
}

int modeGame_End(void)
{
	/* 敵の弾の終了 */
	EnemyBullet_End();
	/* 敵の終了 */
	Enemy_End();
	/* プレイヤーの弾の終了 */
	PlayerBullet_End();
	/* プレイヤーの終了 */
	Player_End();

	SFX_End();

	/* game modeの変数領域の開放	*/
	if(work) {
		free(work); work = NULL;
	}
	return 1;
}


int modeGame_Dsp(void)
{
	work->tmr++;

	/*プレイヤーの表示*/
	Player_Dsp();
	/*プレイヤーの弾*/
	PlayerBullet_Update();

	Enemy_Update( );
	EnemyBullet_Update( );

	Collision_Update( );

	/*弾のスコア */
	{
		//弾のスコア
		char n[ 128 ];
		sprintf_s( n, "BULLET_SCORE:%06d00", work->score_bullet );
		DrawString(SCORE_DISP_POSX, SCORE_DISP_POSY, n, SCORE_DISP_COLOR);

		//剣のスコア
		sprintf_s(n, "SWORD_SCORE:%06d00", work->score_slash);
		DrawString(SCORE_DISP_POSX + 200, SCORE_DISP_POSY, n, SCORE_DISP_COLOR);

		//

		sprintf_s( n, ":%d", work->playerLeft );
		DrawString(PLYNUM_DISP_POSX, PLYNUM_DISP_POSY, n, PLYNUM_DISP_COLOR);
	}

	/* 残機チェック */
	if( work->playerLeft == 0 )
	{
		/* プレイヤーの状態を確認する */
		if ( GetPlayerState( ) == PLSTATE_GAMEOVER ) 
		{	/* ゲームオーバー の表示 */
			char n[ 128 ];
			sprintf_s( n, "GAME OVER" );
			DrawString( GAMEOVER_DISP_POSX, GAMEOVER_DISP_POSY, n, GAMEOVER_DISP_COLOR);
			work->subtmr++;
			if( work->subtmr > GAMEOVER_TIMEOUT )
			{	/* モードの終了を伝える */
				return 1; 
			}
		}
	}

	if( work->tmr > GAMEMDOE_TIMEOUT ){	return 1;	}
	return 0;
}




/* スコアの加算 */
int modeGame_AddScore( int score )
{
	work->score_bullet += score;
	return work->score_bullet;
}

int modeGame_AddScore2(int score)
{
	work->score_slash += score;
	return work->score_slash;

}

/* 残機の加算 */
int modeGame_AddPlayer( int plnum )
{
	work->playerLeft += plnum;
	return work->playerLeft;
}

/* 残機の減算 */
int modeGame_SubPlayer( int plnum )
{
	work->playerLeft -= plnum;
	return work->playerLeft;
}
