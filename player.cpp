/*
* Player
* 
* 
* 
* 
*/

/*
* Player�Ɋւ��鏈��
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
	int	state;		/* ��� */
	int	x, y;		/* �ʒu�̐����l	*/
	int xold, yold;	/* �ʒu�̑O��l */
	int size;		/* ���̂̑傫�� */
	int spd;		/* ���đ��x(�X�J��)	*/

	int slashflag;
};


static struct PLYER_P *work;

static void player_graphics( int x, int y );	/* player��`�悷��֐� */
static void player_graphics2( int x, int y );	/* player��`�悷��֐� */
/* *************************************************************************
* 
* **************************************************************************/
/* player�̏�Ԃ�ݒ肷�� */
void SetPlayerState( int state ) { work->state = state; }
/* player�̏�Ԃ��擾���� */
int GetPlayerState(  ) { return work->state; }
/* player�̈ʒu���擾���� */
void GetPlayerPosition( int* x, int* y ) {	*x = work->x;	*y = work->y;	return; }
void GetPlayerOldPosition(int* x, int* y) { *x = work->xold;	*y = work->yold;	return; }
/* player�̑傫�����擾���� */
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
	/* �ړ��O�̈ʒu���L�^���� */
	work->xold = work->x;
	work->yold = work->y;

	switch (work->state) 
	{
		case PLSTATE_INIT:		// �������
			{
				SetPlayerState( PLSTATE_NORMAL );
			}
			break;
		case PLSTATE_NORMAL:			// �ʏ���
			{
				/* ���� */
				if (isKeyOn( KEY_INPUT_Z )) { PlayerBullet_Shoot( work->x,work->y ); }

				/* ���� */
				if (isKeyOn(KEY_INPUT_X)) {
					if (work->slashflag == 0) { //�����������o�Ă��Ȃ���Ύ��s
						work->slashflag = 1; //�������������t���O
						PlayerBullet_Slash(work->x, work->y);
						//DrawString(work->x + 20, work->y - 10, "=={<*>>>", GetColor(240, 40, 40));
					}
				}

				/* �ړ� */
				if (isKeyDown( KEY_INPUT_LEFT )) { work->x = work->x > 0 ? work->x - PL_XSPD : 0; }
				if (isKeyDown( KEY_INPUT_RIGHT )) { work->x = work->x < SCRN_WIDTH ? work->x + PL_XSPD : SCRN_WIDTH; }

				if (isKeyDown(KEY_INPUT_UP)) { work->y = work->y > 0 ? work->y - PL_YSPD : 0; }
				if (isKeyDown(KEY_INPUT_DOWN)) { work->y = work->y < SCRN_HEIGHT ? work->y + PL_YSPD : SCRN_HEIGHT; }

				player_graphics( work->x, work->y );
			}
			break;
		case PLSTATE_HIT:			// ���������u��
			{
				work->tmr = 0;
				sfx_PlHit();	/* ���ʉ� */
				SetPlayerState( PLSTATE_EXPLOSION );
			}
			break;
		case PLSTATE_EXPLOSION:		// ������
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
		case PLSTATE_MUTEKI:			// �񕜏��
			{
				/* ���� */
				if ( isKeyOn( KEY_INPUT_Z ) ) { PlayerBullet_Shoot( work->x, work->y ); }

				/* ���� */
				if (isKeyOn(KEY_INPUT_X)) {
					if (work->slashflag == 0) { //�����������o�Ă��Ȃ���Ύ��s
						work->slashflag = 1; //�������������t���O ����\�����Ă���Ԃ�True
						PlayerBullet_Slash(work->x, work->y);
						//DrawString(work->x + 20, work->y - 10, "=={<*>>>", GetColor(240, 40, 40));
					}
				}

				/* �ړ� */
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
		case PLSTATE_GAMEOVER:		// �Q�[���I�[�o�[��
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
*	Player�̕\���֐�
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
