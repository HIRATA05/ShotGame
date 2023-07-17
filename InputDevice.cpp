/*
* キーボードやマウスの入力に対する関数を用意する
* 
* 
* 
* 
* 
*/
#include "stdio.h"
#include "DxLib.h"
#include "InputDevice.h"

// Mouse
static int mouse_input		= 0;
static int mouse_input_old	= 0;
static int mouse_input_on	= 0;
static int mouse_input_release = 0;
static int mouse_x=0, mouse_y=0;
// Keyboard
#define MAX_KEYBUFFER_SIZE	256	// GetHitKeyStateAll()が返す情報を確保するのに必要な要素数
static DX_CHAR *keyBuf			= NULL;	// キーボードの現在の状態を取得して保存する領域
static DX_CHAR *keyBufOld		= NULL;	// ひとつ前のフレームにおけるキーボードの状態を保存する領域
static DX_CHAR *keyOnBuf		= NULL;	// キーがOnになった瞬間だけ1になる値が格納される領域
static DX_CHAR *keyReleaseBuf	= NULL;	// キーがOffになった瞬間だけ1になる値が格納される領域

//入力デバイスの初期化
bool InputDevice_Init( )
{
	//　値を格納する領域を確保する
	if( keyBuf ){}else{ keyBuf = (DX_CHAR*)malloc( sizeof(DX_CHAR) * MAX_KEYBUFFER_SIZE );}
	if( !keyBufOld ){ keyBufOld =(DX_CHAR*)malloc( sizeof(DX_CHAR) * MAX_KEYBUFFER_SIZE );}
	if( !keyOnBuf ) { keyOnBuf = (DX_CHAR*)malloc( sizeof(DX_CHAR) * MAX_KEYBUFFER_SIZE );}
	if( !keyReleaseBuf ) { keyReleaseBuf = (DX_CHAR*)malloc( sizeof(DX_CHAR) * MAX_KEYBUFFER_SIZE );}
	
	// Memoryの確保ができなければ、falseを返す
	// すでに確保しているMemoryがある場合は、開放すべきだが、今回は行わない
	// 原則、ここでエラーが返る場合は致命的(Critical)なので、アプリケーションを終了すべき
	if( !keyBuf )	{	return false; }
	if( !keyBufOld ){	return false; }
	if( !keyOnBuf ) {	return false; }
	if( !keyReleaseBuf ) {	return false; }

	// keyBufを0 fillする
	for(int i = 0;i<MAX_KEYBUFFER_SIZE; i++){
		keyBuf[i] = 0;
	}
	// 上記のfor文の内容をmemsetという関数で行うことができる
	memset( keyBuf, 0, sizeof(DX_CHAR)*MAX_KEYBUFFER_SIZE );
	return true;
}

//入力デバイス関数の終了化
bool InputDevice_End( )
{
	// 確保した領域を開放する
	if( keyBuf )	{	free( keyBuf );	keyBuf = NULL;	}
	if( keyBufOld ) {	free( keyBufOld );	keyBufOld = NULL;	}
	if( keyOnBuf )	{	free( keyOnBuf );	keyOnBuf = NULL;	}
	if( keyReleaseBuf ){free( keyReleaseBuf );	keyReleaseBuf = NULL;	}
	return true;
}

//入力デバイス関係のデータの更新
bool InputDevice_Update( )	
{
	// キーボードの処理
	// 直前の状態を別の領域に保存する
	for ( int i = 0; i < MAX_KEYBUFFER_SIZE; i++ ) { keyBufOld[ i ] = keyBuf[ i ]; }   // 最後のキーボード状態をコピーする
	// 最新の状態を取得する
	GetHitKeyStateAll( keyBuf );// 現在のキーボードの状態を取得する
	// キーボードの各状態を更新する
	for ( int i = 0; i < MAX_KEYBUFFER_SIZE; i++ ){
		int st = keyBufOld[ i ] ^ keyBuf[ i ];
		keyOnBuf[ i ] = st & keyBuf[ i ];			// On Trigger
		keyReleaseBuf[ i ] = st & keyBufOld[ i ];   // Off Trigger
	}

	// マウスの処理
	// 直前の状態を保存
	mouse_input_old = mouse_input;
	// 最新の状態を取得する
	mouse_input = GetMouseInput();

	// マウスの各状態を更新する
	int st = mouse_input_old ^ mouse_input;
	mouse_input_on = st & mouse_input;				// On Trigger
	mouse_input_release = st & mouse_input_old;		// Off Trigger

	GetMousePoint( &mouse_x, &mouse_y );
	return true;
}

// マウス関係
// MOUSE_INPUT_の状態を返す
int	 GetMouseInputAll( )
{
	return mouse_input;
}

// MOUSE_INPUT_の押された瞬間の状態を返す
int	 GetMouseInputOn( )
{
	return mouse_input_on;
}

// MOUSE_INPUT_の離された瞬間の状態を返す
int	 GetMouseInputRelease( )
{
	return mouse_input_release;
}

// Mouseの位置を取得する
int  GetMousePosition( int* x, int* y )
{
	*x = mouse_x;
	*y = mouse_y;
	return true;
}

// MOUSE_INPUT_ が押されていたら、trueを返す
bool isMouseInputDown( int mouse_in_ )
{
	return mouse_input & mouse_in_;
}

// MOUSE_INPUT_ が押された瞬間、trueを返す
bool isMouseInputOn( int mouse_in_ )
{
	return mouse_input_on & mouse_in_;
}

// MOUSE_INPUT_ が離された瞬間、trueを返す
bool isMouseInputRelease( int mouse_in_ )
{
	return mouse_input_release & mouse_in_;
}


// キーボード関係
// keycodeで示されるキーボードが押されていたら、trueを返す
bool isKeyDown( int keycode )
{
	// エラー処理
	if( keycode >= MAX_KEYBUFFER_SIZE ){ return false; }
	if( keycode < 0 ) { return false; }
	// 一行で書くと
	if( (keycode >= MAX_KEYBUFFER_SIZE) || (keycode < 0) ){ return false; }
	// 結果を返す
	return keyBuf[ keycode ];
}

// keycodeで示されるキーボードが押された瞬間、trueを返す
bool isKeyOn( int keycode )
{
	// エラー処理
	if ( keycode >= MAX_KEYBUFFER_SIZE ) { return false; }
	if ( keycode < 0 ) { return false; }
	// 結果を返す
	return keyOnBuf[ keycode ];
}

// keycodeで示されるキーボードが離された瞬間、trueを返す
bool isKeyRelease( int keycode )
{
	// エラー処理
	if ( keycode >= MAX_KEYBUFFER_SIZE ) { return false; }
	if ( keycode < 0 ) { return false; }
	// 結果を返す
	return keyReleaseBuf[ keycode ];
}
