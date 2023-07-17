// 画面遷移(モード,シーン)制御

#include "assert.h"
#include "DxLib.h"

#include "ShotG.h"
#include "InputDevice.h"
#include "mode.h"

#include "title.h"
#include "game.h"

// modeの定義
enum {
	MODE_INIT = 0,		// Mode Controlの初期化用
	MODE_TITLE_INIT,	// ゲームタイトル表示　初期化用
	MODE_TITLE_DSP,		// ゲームタイトル表示　表示用
	MODE_TITLE_END,		// ゲームタイトル表示　終了化用
	MODE_GAME_INIT,		// ゲーム本体 初期化用
	MODE_GAME_DSP,		// ゲーム本体 表示用
	MODE_GAME_END,		// ゲーム本体 終了化用
	MODE_END
};

#if DEBUG
static const char *mstr[] = {
	"MODE_INIT",		// Mode Controlの初期化用
	"MODE_TITLE_INIT",	// ゲームタイトル表示　初期化用
	"MODE_TITLE_DSP",		// ゲームタイトル表示　表示用
	"MODE_TITLE_END",		// ゲームタイトル表示　終了化用
	"MODE_GAME_INIT",		// ゲーム本体 初期化用
	"MODE_GAME_DSP",		// ゲーム本体 表示用
	"MODE_GAME_END",		// ゲーム本体 終了化用
	"MODE_END"
};

#endif // DEBUG


// mode管理用変数領域の確保
// 構造体
struct MODE_P
{
	int	mode;				// モード番号(enum)
	int	mode_tm;			// モード経過時間
	int mode_game_request;	// GameModeに進むフラグ
	int ed_flg; //ゲームが終了したかのフラグ
};

static struct MODE_P *mode_p = NULL;

#define	MODE_TITLE_EXPIRE_SECOND	(600*FRAME_PER_SECOND)	// MODE_TITLE_DSPが時間切れになるまでの秒数

/* モードを設定する */
static void SetMode( int x ) {	mode_p->mode = x;	mode_p->mode_tm = 0;	}
/* 現在のモードを返す */
static int GetMode( ) { return mode_p->mode; }

/* ゲームリクエストを1にする */
static void SetGameRequest( ) {	mode_p->mode_game_request = 1;}
/* ゲームリクエストを0にする */
static void ClearGameRequest() { mode_p->mode_game_request = 0; }
/* ゲームリクエストの状態を返す */
static int GetGameRequest( ) { return mode_p->mode_game_request; }


// modeの初期化
int Mode_Init( )
{
	// もし、mode_pがNULLでなかったら(初期化されていなかったら)エラー
	// ここでは assertの使い方の見本として使用した
	assert( mode_p == NULL );

	mode_p = (struct MODE_P*)malloc( sizeof( struct MODE_P ) );
	if( mode_p )
	{// 構造体の初期化
		mode_p->mode = MODE_TITLE_INIT;
		mode_p->mode_tm = 0;
		mode_p->mode_game_request = 0;
		return TRUE;
	}
	/*
	* FALSEを返すケースがあってはならない
	* もし、ここまで来てしまったら、それは致命的なエラーといえる
	*/
	return FALSE;
}

// modeの終了
int Mode_End()
{
	// もし、mode_pが設定されていなかったらエラー
	// ここでは assertの使い方の見本として使用した

	assert(mode_p != NULL);

	if ( mode_p ) {
		free( mode_p );
		mode_p = NULL;
		return TRUE;
	}
	/*
	* FALSEを返すケースがあってはならない
	* もし、ここまで来てしまったら、それは致命的なエラーといえる
	*/
	return FALSE;
}



// 制御
// 
int Mode_Ctrl()
{
	/*
	* Start Button(スペースキー)判定
	*/
	if( (mode_p->mode == MODE_TITLE_DSP ) )
	{// MODE_TITLE_DSP のときのみ入力チェック
		if( isKeyDown(KEY_INPUT_SPACE ) )
		{// スペースキーが押されたらGameModeに行くリクエストフラグを立てる
			SetGameRequest();
		}
	}

	/*
	*	時間切れモード進行
	*/
	mode_p->mode_tm++;
	if(mode_p->mode_tm > MODE_TITLE_EXPIRE_SECOND )
	{
		mode_p->mode++;	// Modeをひとつ進める
		mode_p->mode_tm = 0;
	}

	switch(mode_p->mode )
	{
		case MODE_TITLE_INIT:	// タイトル画面 準備
			{
				modeTitle_Init();
				SetMode( MODE_TITLE_DSP );
			}
			break;
		case MODE_TITLE_DSP:	// タイトル画面 表示
			{
				int n = modeTitle_Dsp();
				if( n )
				{	/* タイムアウトしたので次のモードに遷移する */
					SetMode(MODE_TITLE_END);
				}
				else if ( GetGameRequest() )
				{/* GameModeRequestフラグが立っているので、現在のモードを終了する */
						SetMode(MODE_TITLE_END);
				}
			}
			break;
		case MODE_TITLE_END:	// タイトル画面 終了
			{
				modeTitle_End();
				if( GetGameRequest( ) )
				{/* GameModeRequestフラグが立っているので、現在のモードを終了する */
					ClearGameRequest(); /* GameModeRequestフラグのクリア */
					SetMode( MODE_GAME_INIT );	/* GameModeへ遷移する */	
				}else{
					SetMode( MODE_TITLE_INIT ); /* TITLEをループ */
				}
			}
			break;
		case MODE_GAME_INIT:	// ゲーム画面 準備
			{
			mode_p->ed_flg = false;
				modeGame_Init();
				SetMode( MODE_GAME_DSP ); //
				//PlaySound	( "音楽ファイルの場所" , 再生タイプ ) ;
				//DX_PLAYTYPE_BACK
				PlaySound("Battle4.ogg", DX_PLAYTYPE_BACK);
			}
			break;
		case MODE_GAME_DSP:	// ゲーム画面 表示
			{
				if( modeGame_Dsp() )
				{
					SetMode( MODE_GAME_END ); // Game Endへ
				}else{
					// 時間切れにならないように時間を巻き戻す
					mode_p->mode_tm--;
				}
			}
			break;
		case MODE_GAME_END:	// ゲーム画面 終了
			{
				PlaySound("", DX_PLAYTYPE_BACK);
				mode_p->ed_flg = true;
				modeGame_End();
				SetMode( MODE_TITLE_INIT ); //	TITLEへループ
			}
			break;

		default:
			{// 制御のないプログラムのモードに来た時
				SetMode( MODE_INIT ); // TITLEへループ
			}
			break;
	}

#if DEBUG
	{
		char n[ 128 ];

		DrawString( 0, SCRN_HEIGHT - 14 * 2, mstr[ GetMode() ], GetColor( 240,240,240 ) );
		sprintf_s( n, "ELAPS TIME:%d", mode_p->mode_tm );
		DrawString(0, SCRN_HEIGHT - 14 * 1, n, GetColor(240, 240, 240));
	}
#endif


	// モード番号のオーバーフローに対応する
	if(mode_p->mode >= MODE_END )
	{
		mode_p->mode = MODE_INIT + 1;
		mode_p->mode_tm = 0;
	}

	return FALSE;
}



