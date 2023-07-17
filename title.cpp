/*
* Title Mode
* 
* modectrl����Ă΂��֐��́A�Ԃ�l�Ƃ���
*  0 ... mode ctrl�ɕύX��`���Ȃ�
*  1 ... mode ctrl�ɕύX��`����
* ��Ԃ��B
* 
*/
#include "math.h"
#include "DxLib.h"
#include "ShotG.h"
#include "game.h"

#include "InputDevice.h"

#include "title.h"


#define	TITLEMDOE_TIMEOUT	(5*FRAME_PER_SECOND)	// second

/*
* Title Mode�p�̕ϐ�
*/
struct TITLE_P
{
	int	tmr;

	int	r,g,b;
};

static struct TITLE_P *work = NULL;
/*
* Title Mode �̏�����
*/
int modeTitle_Init(void) 
{
	if( !work ){	work = (struct TITLE_P*)malloc( sizeof( struct TITLE_P) ); }
	if( !work ){	return 0;	}
	memset( work, 0, sizeof(struct TITLE_P) );
	return 1;
}

/*
* Title Mode �̏I����
*/
int modeTitle_End( void ) {
	if ( work ) 
	{
		free( work ); work = NULL;
	}
	return 1;
}

/*
* modeTitle_Dsp()
* �Ԃ�l
*	0 ... �Ȃɂ��Ȃ�
*	1 ... ���Ԑ؂�
*/
int modeTitle_Dsp(void) 
{
	/*work->r = ((work->tmr * 2) % 16) * 16;
	work->g = ( ( work->tmr * 2 / 16 ) % 16 ) * 16;
	work->b = ( ( work->tmr * 2 / 256 ) % 16 ) * 16;
	int c = GetColor( work->r, work->g, work->b );*/
	work->tmr++;
	
	/* ��ʂ̒����ʒu	*/
	/*int x = SCRN_WIDTH / 4;
	int y = SCRN_HEIGHT / 2;*/

	/*	�~�̑傫��	*/
	/*int rx = SCRN_WIDTH < SCRN_HEIGHT ? SCRN_WIDTH / 4 : SCRN_HEIGHT / 4;
	int ry = SCRN_WIDTH < SCRN_HEIGHT ? SCRN_WIDTH / 4 : SCRN_HEIGHT / 4;
	int rx2 = SCRN_WIDTH < SCRN_HEIGHT ? SCRN_WIDTH / 4 : SCRN_HEIGHT / 4;

	double rad = ((double)work->tmr * 4.0 / 360.0) * (3.14);

	rx = (int)fabs(rx * sin(rad));
	rx2 = (int)fabs( (double)rx2 * (((work->tmr + 45) % 90) - 45) / 45.0);*/

	int x, y;
	bool n;
	//x = SCRN_HEIGHT / 1.5, y = 530;
	n = TRUE;
	//while (n) {
	DrawString(10, 10, "[�������]\nZ�L�[�Œe�𔭎�\nX�L�[�Ō���U��", GetColor(10, 240, 10));

	DrawString(10, 70, "�㉺���E�̖��L�[�ňړ�", GetColor(10, 240, 10));
	//y = y - 8;
	/*	if (y < 0) {
			break;
		}
	}*/

	DrawString(SCRN_WIDTH / 2.5, SCRN_HEIGHT / 2, "<ShotG/Gun&Slash>", GetColor(10, 240, 10));
	DrawString(SCRN_WIDTH / 2.6, SCRN_HEIGHT / 1.8, "�X�y�[�X�L�[�ŃX�^�[�g", GetColor(10, 240, 10));

/*#define	LINEW	(512)
	double ux = fabs(sin(rad));
	double dx = fabs(((( work->tmr + 45) % 90) - 45) / 45.0);

	c = GetColor(240, 240, 240);
	DrawOval(x, y, rx, ry, c, true);
	DrawOval(x + SCRN_WIDTH / 2, y, rx2, ry, c, true);

	DrawLine(100, SCRN_HEIGHT * 7 / 8, 100 + LINEW, SCRN_HEIGHT * 7 / 8, GetColor(240, 240, 240));
	DrawTriangle( (int)(100 + ux * LINEW), (SCRN_HEIGHT * 7 / 8) - 1, (int)(100 + ux * LINEW - 2), (SCRN_HEIGHT * 7 / 8) - 40, (int)(100 + ux * LINEW + 2), (SCRN_HEIGHT * 7 / 8) - 40, GetColor(240, 0, 0), true);
	DrawTriangle( (int)(100 + dx * LINEW), (SCRN_HEIGHT * 7 / 8) + 1, (int)(100 + dx * LINEW - 2), (SCRN_HEIGHT * 7 / 8) + 40, (int)(100 + dx * LINEW + 2), (SCRN_HEIGHT * 7 / 8) + 40, GetColor(240, 0, 0), true);
	*/

	
	/* �^�C���A�E�g�����Ƃ���1��Ԃ�*/
	if( work->tmr > TITLEMDOE_TIMEOUT ){	return 1;	}
	return 0;
}



