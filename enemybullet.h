#pragma once
/*
*
* Enemy
*
* �G
*
*/

#define	ENEMYBULLET_SPD	6	/*	�e�̈ړ����x	*/

/*
*	�e�{�̂̃v���O�������g�p����ϐ��Ȃǂ̐錾
*/
/*
* �e�̏�ԑJ�ڂ̒�`
*/
enum {
	ENEMYBULLET_INIT	=	0,
	ENEMYBULLET_SHOT,			/*���Ē�*/
	ENEMYBULLET_HIT,			/*������*/
	ENEMYBULLET_EXPLOSION,		/*�j��*/
	ENEMYBULLET_DONE,			/*�I����*/
};

/*
*	�e�̎�ނ̒�` 
*/
enum {
	ENEMYBULLET_TYPE_0	=	0,
	ENEMYBULLET_TYPE_1,
	ENEMYBULLET_TYPE_2,
	ENEMYBULLET_TYPE_3,
	ENEMYBULLET_TYPE_4,
	ENEMYBULLET_TYPE_5,
};

struct ENEMYBULLETOBJECT_P
{
	int	tmr;
	int	state;		/* ��� */
	int	x, y;		/* �ʒu�̐����l	*/
	int xold, yold;	/* �ʒu�̑O��l */
	int size;		/* ���̂̑傫�� */
	int type;		/* ���̂̃^�C�v */
	int spd;		/* ���đ��x(�X�J��)	*/

	int tagx,tagy;	/* �ړI�n */
	float spdx, spdy;	/* �e�����̈ړ����x	*/
	float xf, yf;		/* �ʒu�̕��������_�l	*/

	struct ENEMYBULLETOBJECT_P *next;
};

int	EnemyBullet_Init();	/* GameMode INIT�ŌĂ�	*/
int	EnemyBullet_Update();	/* GameMode DSP�ŌĂ�	*/
int	EnemyBullet_End();	/* GameMode FINISH�ŌĂ�	*/
int EnemyBullet_Shoot( int x, int y, int type );	/* Enemy Position */
struct ENEMYBULLETOBJECT_P *GetObject_EnemyBullet( int n );	/* �G�̕ϐ��̈�̐擪�̃|�C���^���擾����	*/
void EnemyBullet_SetState( struct ENEMYBULLETOBJECT_P *n, int state );
int EnemyBullet_GetState( struct ENEMYBULLETOBJECT_P *n );