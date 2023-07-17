#pragma once
/*
* Game mode
*/

/* �Q�[���Ɋւ���e��ݒ� */
#define DEFAULT_PLAYER_LEFT	3								// �c�@���̏����l
#define PLYNUM_DISP_POSX	(SCRN_WIDTH - 2 * 14)			// �c�@���̕\���ʒu X
#define PLYNUM_DISP_POSY	0								// �c�@����*�\���ʒu Y
#define PLYNUM_DISP_COLOR	GetColor( 240,240,240 )			// �c�@���̕\���F
#define SCORE_DISP_POSX		0								// �X�R�A�̕\���ʒu X
#define SCORE_DISP_POSY		0								// �X�R�A�̕\���ʒu Y
#define SCORE_DISP_COLOR	GetColor( 240,240,240 )			// �X�R�A�̕\���F
#define GAMEOVER_DISP_POSX	(SCRN_WIDTH/2 - (14 * 9 / 2) )	// GAME OVER�̕\���ʒu X
#define GAMEOVER_DISP_POSY	((SCRN_HEIGHT - 14)/2)			// GAME OVER�̕\���ʒu Y
#define GAMEOVER_DISP_COLOR	GetColor( 240,240,240 )			// GAME OVER�̕\���F

/* player�ɑ΂���ݒ� */
#define	PLAYER_R			8		/* PLayer�̑傫�� */ 
#define	PLAYER_COLOR1		GetColor( 240,0,0 )
#define	PLAYER_COLOR2		GetColor( 220,220,220 )
#define	PL_XSPD				8		/* player��x�����̈ړ����x */
#define	PL_YSPD				8		/* player��y�����̈ړ����x (���g�p) */
#define PL_EXPLOSION_TIME	(60*2)	/* �����\������ */
#define PL_MUTEKI_TIME		(60*1)	/* ���G�񕜊��� */

/* player�̒e�ɑ΂���ݒ� */
#define	PLAYERBULLET_COLOR	GetColor( 240, 240, 40 )
#define	PLAYERBULLET_SIZE	(8)	/* player�̒e�̑傫�� */
#define	PLAYERBULLET_SPD	(8*2)	/*	�e�̈ړ����x	*/


/* �G�ɑ΂���ݒ� */
#define	ENEMY_COLOR			GetColor( 40, 240, 40 )
#define	ENEMY_SIZE			16	/* �G�̑傫�� */

/* �G�̒e�ɑ΂���ݒ� */
#define	ENEMYBULLET_COLOR	GetColor( 240, 140, 40 )
#define	ENEMYBULLET_SIZE	6	/* �G�̒e�̑傫�� */

int modeGame_Init( void );
int modeGame_End( void );
int modeGame_Dsp( void );

int GetGameTimer( void );				/* �Q�[�����J�n���Ă���̃t���[�����̎擾 */

int modeGame_AddScore( int score );		/* �X�R�A�̉��Z */
int modeGame_AddScore2( int score);		/* �X�R�A�̉��Z */
int modeGame_AddPlayer( int plnum );	/* �c�@�̉��Z */
int modeGame_SubPlayer( int plnum );	/* �c�@�̌��Z */
