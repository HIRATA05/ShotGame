#pragma once

#include "resource.h"

#define	DEBUG	_DEBUG	/* Release/Debugの設定で切り替わる */

/* true .... playerの弾の制御を配列で行う場合/コリジョンは無効になります */
/* false ... playerの弾の制御をリンクリストで行う場合/コリジョンは有効になります */
#define	_ARRAY_MODE_	false	/*	弾の管理方法の選択	/	FALSE...Link / TRUE...Array	*/



#if DEBUG
/* DEBUG時のWindow ModeのWindow Size */
#define	SCRN_WIDTH	(1920/2)		// window width 960 480
#define	SCRN_HEIGHT	(1080/2)		// window height 540 270
#define	SCRN_WINDOW_MODE	true	// window screen mode
#else
/* Release時のFull ScreenのSize */
#define	SCRN_WIDTH	(1920)
#define	SCRN_HEIGHT	(1080)
#define	SCRN_WINDOW_MODE	false	// full screen mode
#endif // DEBUG
#define SCRN_DEPTH	(32)



#define FRAME_PER_SECOND	60	// １秒間の表示回数
#define	_0SECOND	(FRAME_PER_SECOND / 2) // _1SECONDの半分を定義
#define	_1SECOND	(FRAME_PER_SECOND)
#define	_2SECOND	(_1SECOND+_1SECOND)
#define	_3SECOND	(_1SECOND+_2SECOND)
#define	_4SECOND	(_2SECOND+_2SECOND)
#define	_5SECOND	(_2SECOND+_3SECOND)


