/*
* キーボードやマウスの入力に対する関数を用意する
*
*
*
*
*
*/
#pragma once


bool InputDevice_Init( );	//入力デバイス関数の初期化
bool InputDevice_End( );	//入力デバイス関数の終了化
bool InputDevice_Update( );	//入力デバイス関係のデータの更新


// マウス関係
int	 GetMouseInputAll( );		// MOUSE_INPUT_の状態を返す
int	 GetMouseInputOn();			// MOUSE_INPUT_の押された瞬間の状態を返す
int	 GetMouseInputRelease( );	// MOUSE_INPUT_の離された瞬間の状態を返す
int  GetMousePosition( int *x, int *y );// Mouseの位置を取得する

bool isMouseInputDown( int mouse_input );	// MOUSE_INPUT_ が押されていたら、trueを返す
bool isMouseInputOn( int mouse_input );		// MOUSE_INPUT_ が押された瞬間、trueを返す
bool isMouseInputRelease( int mouse_input );// MOUSE_INPUT_ が離された瞬間、trueを返す

// キーボード関係
bool isKeyDown( int keycode );		// keycodeで示されるキーボードが押されていたら、trueを返す
bool isKeyOn( int keycode );		// keycodeで示されるキーボードが押された瞬間、trueを返す
bool isKeyRelease( int keycode );	// keycodeで示されるキーボードが離された瞬間、trueを返す

