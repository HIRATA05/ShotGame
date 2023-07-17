#pragma once
/*
* Playerに関する処理
* 
* 
* 
* 
* 
*/

enum {
	PLSTATE_INIT = 0,		// 初期状態
	PLSTATE_NORMAL,			// 通常状態
	PLSTATE_HIT,			// 当たった瞬間
	PLSTATE_EXPLOSION,		// 爆発中
	PLSTATE_MUTEKI,			// 回復状態
	PLSTATE_GAMEOVER,		// ゲームオーバー時

};

int Player_Init( void );
int Player_Dsp( void );
int Player_End( void );


void SetPlayerState( int state );			/* playerの状態を設定する */
int GetPlayerState( );						/* playerの状態を取得する */
void GetPlayerPosition( int* x, int* y );	/* playerの位置を取得する */
void GetPlayerOldPosition(int* x, int* y);
void GetPlayerSize( int* size );			/* playerの大きさを取得する */
void ClearSlashFlag( void );
