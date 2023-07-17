#pragma once
/*
*
* Player Bullet
*
* �v���C���[�̒e
*
*/


/*
*	�e�{�̂̃v���O�������g�p����ϐ��Ȃǂ̐錾
*/
enum {
	PLAYERBULLET_INIT	=	0,
	PLAYERBULLET_SHOT,			/*���Ē�*/
//	PLAYERBULLET_SLASH,			/*�a��*/ //�ǉ�
	PLAYERBULLET_HIT,			/*������*/
	PLAYERBULLET_EXPLOSION,		/*�j��*/
	PLAYERBULLET_DONE,			/*�I����*/
};

enum {
	PLBULLET_TYPE_BULLET	= 0, /*�e�̎��*/
	PLBULLET_TYPE_SLASH,
};
struct PLAYERBULLETOBJECT_P
{
	int	tmr;
	int	state;		/* ��� */
	int	x, y;		/* �ʒu�̐����l	*/
	int xold, yold;	/* �ʒu�̑O��l */
	int size;		/* ���̂̑傫�� */
	int type;		/* ���̂̃^�C�v */
	int spd;		/* ���đ��x(�X�J��)	*/

	int tagx,tagy;	/* �ړI�n */

	int weapontype;	/* ����̎�� �e �� */
	int	ax1, ay1, ax2, ay2;
	int slashtmr;
#if !_ARRAY_MODE_
	struct PLAYERBULLETOBJECT_P *next;
#endif
};

int	PlayerBullet_Init();	/* GameMode INIT�ŌĂ�	*/
int	PlayerBullet_Update();	/* GameMode DSP�ŌĂ�	*/
int	PlayerBullet_End();	/* GameMode FINISH�ŌĂ�	*/
int PlayerBullet_Shoot( int x, int y );	/* Player Position */
int PlayerBullet_Slash(int x, int y);
struct PLAYERBULLETOBJECT_P *GetObject_PlayerBullet( int n );	/* �v���C���[�̒e�̕ϐ��̈�̐擪�̃|�C���^���擾����	*/
void PlayerBullet_SetState( struct PLAYERBULLETOBJECT_P *n, int state );
int PlayerBullet_GetState( struct PLAYERBULLETOBJECT_P *n );
