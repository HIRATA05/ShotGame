#pragma once
/*
*
* Enemy
*
* �G
*
*/

#define	ENEMY_SPD	4	/*	�G�̈ړ����x	*/

/*
*	�G�{�̂̃v���O�������g�p����ϐ��Ȃǂ̐錾
*/
/*
* �G�̏�ԑJ�ڂ̒�`
*/
enum {
	ENEMY_INIT	=	0,
	ENEMY_SHOT,			/*���Ē�*/
	ENEMY_HIT,			/*������*/
	ENEMY_HIT2,			/*������*/
	ENEMY_EXPLOSION,	/*�j��*/
	ENEMY_DONE,			/*�I����*/
};

/*
*	�G�̎�ނ̒�`
*/
enum {
	ENEMYTYPE_0 = 0,
	ENEMYTYPE_RIGHT2LEFT,
	ENEMYTYPE_LEFT2RIGHT,
	ENEMYTYPE_TOP2BOTTOM,
	ENEMYTYPE_BOTTOM2TOP,
	ENEMYTYPE_MYMOVE_M,
	ENEMYTYPE_MYMOVE_B,
	ENEMYTYPE_MYMOVE_Q,
	ENEMYTYPE_MYMOVE_Z,
	ENEMYTYPE_END,
};


struct ENEMYOBJECT_P
{
	int	tmr;
	int	state;		/* ��� */
	int	x, y;		/* �ʒu�̐����l	*/
	int xold, yold;	/* �ʒu�̑O��l */
	int size;		/* ���̂̑傫�� */
	int type;		/* ���̂̃^�C�v */
	int spd;		/* ���đ��x(�X�J��)	*/

	int btm;		/* �e�������� */
	int tagx,tagy;	/* �ړI�n */
	int etmr;

	struct ENEMYOBJECT_P *next;
};

int	Enemy_Init();	/* GameMode INIT�ŌĂ�	*/
int	Enemy_Update();	/* GameMode DSP�ŌĂ�	*/
int	Enemy_End();	/* GameMode FINISH�ŌĂ�	*/

int Enemy_Shoot( int x, int y, int type, int btm );	/* Enemy�̔��� */

struct ENEMYOBJECT_P *GetObject_Enemy( int n );	/* �G�̕ϐ��̈�̐擪�̃|�C���^���擾����	*/
void Enemy_SetState( struct ENEMYOBJECT_P* n, int state );	/* �G��state��ݒ肷�� */
int Enemy_GetState( struct ENEMYOBJECT_P* n );			/* �G��state���擾����@*/
