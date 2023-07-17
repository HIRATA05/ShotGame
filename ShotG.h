#pragma once

#include "resource.h"

#define	DEBUG	_DEBUG	/* Release/Debug�̐ݒ�Ő؂�ւ�� */

/* true .... player�̒e�̐����z��ōs���ꍇ/�R���W�����͖����ɂȂ�܂� */
/* false ... player�̒e�̐���������N���X�g�ōs���ꍇ/�R���W�����͗L���ɂȂ�܂� */
#define	_ARRAY_MODE_	false	/*	�e�̊Ǘ����@�̑I��	/	FALSE...Link / TRUE...Array	*/



#if DEBUG
/* DEBUG����Window Mode��Window Size */
#define	SCRN_WIDTH	(1920/2)		// window width 960 480
#define	SCRN_HEIGHT	(1080/2)		// window height 540 270
#define	SCRN_WINDOW_MODE	true	// window screen mode
#else
/* Release����Full Screen��Size */
#define	SCRN_WIDTH	(1920)
#define	SCRN_HEIGHT	(1080)
#define	SCRN_WINDOW_MODE	false	// full screen mode
#endif // DEBUG
#define SCRN_DEPTH	(32)



#define FRAME_PER_SECOND	60	// �P�b�Ԃ̕\����
#define	_0SECOND	(FRAME_PER_SECOND / 2) // _1SECOND�̔������`
#define	_1SECOND	(FRAME_PER_SECOND)
#define	_2SECOND	(_1SECOND+_1SECOND)
#define	_3SECOND	(_1SECOND+_2SECOND)
#define	_4SECOND	(_2SECOND+_2SECOND)
#define	_5SECOND	(_2SECOND+_3SECOND)


