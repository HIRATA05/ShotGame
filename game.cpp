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

#include "InputDevice.h"

#include "game.h"
#include "player.h"
#include "playerbullet.h"
#include "enemy.h"
#include "enemybullet.h"
#include "collilsion.h"
#include "sfx.h"

#define	GAMEMDOE_TIMEOUT	(30*FRAME_PER_SECOND)	// second	�Q�[���̐������Ԃ̐ݒ�
#define	GAMEOVER_TIMEOUT	(5*FRAME_PER_SECOND)	// second
/*
* Game Mode�p�̕ϐ�
*/
struct GAME_P
{
	int	tmr;

	int	r,g,b;
	int playerLeft;	//�c�@��
	int score_bullet;		//���_
	int score_slash;		//���_
	int	subtmr;

};

static struct GAME_P *work = NULL;

/*
* 
* �Q�[�����J�E���^���擾����
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
	/*�v���C���[������*/
	Player_Init();
	PlayerBullet_Init();

	Enemy_Init( );
	EnemyBullet_Init( );

	return 1;
}

int modeGame_End(void)
{
	/* �G�̒e�̏I�� */
	EnemyBullet_End();
	/* �G�̏I�� */
	Enemy_End();
	/* �v���C���[�̒e�̏I�� */
	PlayerBullet_End();
	/* �v���C���[�̏I�� */
	Player_End();

	SFX_End();

	/* game mode�̕ϐ��̈�̊J��	*/
	if(work) {
		free(work); work = NULL;
	}
	return 1;
}


int modeGame_Dsp(void)
{
	work->tmr++;

	/*�v���C���[�̕\��*/
	Player_Dsp();
	/*�v���C���[�̒e*/
	PlayerBullet_Update();

	Enemy_Update( );
	EnemyBullet_Update( );

	Collision_Update( );

	/*�e�̃X�R�A */
	{
		//�e�̃X�R�A
		char n[ 128 ];
		sprintf_s( n, "BULLET_SCORE:%06d00", work->score_bullet );
		DrawString(SCORE_DISP_POSX, SCORE_DISP_POSY, n, SCORE_DISP_COLOR);

		//���̃X�R�A
		sprintf_s(n, "SWORD_SCORE:%06d00", work->score_slash);
		DrawString(SCORE_DISP_POSX + 200, SCORE_DISP_POSY, n, SCORE_DISP_COLOR);

		//

		sprintf_s( n, ":%d", work->playerLeft );
		DrawString(PLYNUM_DISP_POSX, PLYNUM_DISP_POSY, n, PLYNUM_DISP_COLOR);
	}

	/* �c�@�`�F�b�N */
	if( work->playerLeft == 0 )
	{
		/* �v���C���[�̏�Ԃ��m�F���� */
		if ( GetPlayerState( ) == PLSTATE_GAMEOVER ) 
		{	/* �Q�[���I�[�o�[ �̕\�� */
			char n[ 128 ];
			sprintf_s( n, "GAME OVER" );
			DrawString( GAMEOVER_DISP_POSX, GAMEOVER_DISP_POSY, n, GAMEOVER_DISP_COLOR);
			work->subtmr++;
			if( work->subtmr > GAMEOVER_TIMEOUT )
			{	/* ���[�h�̏I����`���� */
				return 1; 
			}
		}
	}

	if( work->tmr > GAMEMDOE_TIMEOUT ){	return 1;	}
	return 0;
}




/* �X�R�A�̉��Z */
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

/* �c�@�̉��Z */
int modeGame_AddPlayer( int plnum )
{
	work->playerLeft += plnum;
	return work->playerLeft;
}

/* �c�@�̌��Z */
int modeGame_SubPlayer( int plnum )
{
	work->playerLeft -= plnum;
	return work->playerLeft;
}
