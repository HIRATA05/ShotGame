/*
* �R���W����
*
*
*
*
*/

#include "DxLib.h"
#include "ShotG.h"

#include "InputDevice.h"

#include "game.h"
#include "player.h"
#include "playerbullet.h"
#include "enemy.h"
#include "enemybullet.h"

#include "collilsion.h"


/* �O�ς𗘗p���������蔻����g���Ƃ���true�B
*	�Gvs�v���C���[�̒e�ŗ��p�\ 
*	�������A�v���C���[�̒e�������Ɉړ����邽�߁A���������Ɉړ�����G�Ƃ�
*	�������Ȃ��̂Œ���	
*/
#define		_USE_OUTER_PRODUCT_	false	

/*
* 
* �R���W����
* 
* �e�I�u�W�F�N�g�Ƃ̋����𑪂�A�w��ȉ��ł���΁A�e���[�N��state��ύX����
* 
* 
* 
*/

#if _USE_OUTER_PRODUCT_
struct linesedge {
	/* ����1�̊J�n�_�ƏI���_ */
	int	Ax, Ay;
	int Bx, By;

	/* ����2�̊J�n�_�ƏI���_ */
	int	Cx, Cy;
	int Dx, Dy;
};
int CrossCheck(struct linesedge* e);
#endif // _USE_OUTER_PRODUCT_



void Collision_Update( )
{
#if !_ARRAY_MODE_
	/* �v���C���[�̏�Ԃ��m�F���� */
	if (GetPlayerState() == PLSTATE_NORMAL)
	{
		/* Player ������� */
		int	plx, ply, plsize;
		int	plxold, plyold;
		GetPlayerPosition(&plx, &ply);	/* Player�̈ʒu���擾���� */
		GetPlayerOldPosition(&plxold, &plyold);	/* Player�̈ʒu���擾���� */
		GetPlayerSize(&plsize);			/* Player�̑傫�����擾���� */
		/* �v���C���[ vs �G�̒e�@�̓����蔻�� */
		/* �G�̒e�̕ϐ��̈�̐擪�̃|�C���^���擾����	*/
		struct ENEMYBULLETOBJECT_P* objB = GetObject_EnemyBullet(0);
	#if _USE_OUTER_PRODUCT_
		/* �����̌����𒲂ׂ� */
		struct linesedge e;
		e.Bx = plx;		e.By = ply;
		e.Ax = plxold;	e.Ay = plyold;
	#endif // _USE_OUTER_PRODUCT_

		while (objB != NULL)
		{
			if (EnemyBullet_GetState(objB) == ENEMYBULLET_SHOT)
			{	// �e���ʏ��Ԃ̂Ƃ�
#if _USE_OUTER_PRODUCT_
				e.Dx = objB->x;		e.Dy = objB->y;
				e.Cx = objB->xold;	e.Cy = objB->yold;
				if (CrossCheck(&e) ) 
				{	// state��ύX����
					EnemyBullet_SetState(objB, ENEMYBULLET_HIT);
					SetPlayerState(PLSTATE_HIT);
				}
#else
				int xlen = objB->x - plx;
				int ylen = objB->y - ply;
				int dist = objB->size + plsize;
				/* �Q�_�Ԃ̋������A���������i��̑傫���̘a)��菬������Γ�����@*/
				if (((xlen * xlen) + (ylen * ylen)) < (dist * dist))
				{	// state��ύX����
					EnemyBullet_SetState(objB, ENEMYBULLET_HIT);
					SetPlayerState(PLSTATE_HIT);
				}

#endif
			}
			/* ���̃f�[�^�����ɍs�� */
			/* �����A->next��NULL��������AobjB��NULL�ɂȂ�̂� while���I������ */
			objB = objB->next;
		}
	}

	/* �v���C���[�e vs �G �̓����蔻�� */
	/* �G�̕ϐ��̈�̐擪�̃|�C���^���擾����	*/
	struct ENEMYOBJECT_P *objE = GetObject_Enemy( 0 );
	/* �v���C���[�e�̕ϐ��̈�̐擪�̃|�C���^���擾����	*/
	struct PLAYERBULLETOBJECT_P *objP = GetObject_PlayerBullet( 0 );
	/* while�̒��Ŏg�p����ϐ� */
	struct PLAYERBULLETOBJECT_P *objPP = NULL;

	while( objE )
	{
	#if _USE_OUTER_PRODUCT_
		/* �����̌����𒲂ׂ� */
		struct linesedge e;
	#endif // 0
		if( ( Enemy_GetState( objE ) == ENEMY_SHOT )
			/*|| ( objE->state == ENEMY_EXPLOSION )*/ )
		{	/* �G���ʏ��Ԃł���΁A�R���W�����������s�� */
			objPP = objP; /* �R�s�[��ۑ����Ă��� */
		#if _USE_OUTER_PRODUCT_
			e.Ax = objE->xold;	e.Ay = objE->yold;
			e.Bx = objE->x;		e.By = objE->y;
		#endif // 0
			while( objPP )
			{
				if( ( PlayerBullet_GetState( objPP ) == PLAYERBULLET_SHOT )
					//|| (PlayerBullet_GetState(objPP) == PLAYERBULLET_SLASH )
					/*|| ( objPP->state == PLAYERBULLET_EXPLOSION )*/ )
				{	/* Player�̒e���ʏ��Ԃł���΁A�R���W�����������s�� */
				#if _USE_OUTER_PRODUCT_
					e.Cx = objPP->xold;	e.Cy = objPP->yold;
					e.Dx = objPP->x;	e.Dy = objPP->y;
					if( CrossCheck( &e ) )
					{	/* ��Ԃ̕ύX */
						if(Enemy_GetState(objE) == ENEMY_SHOT)
						{	/* ��Ԃ̕ύX */
							Enemy_SetState(objE,ENEMY_HIT);
						}
						if( PlayerBullet_GetState(objPP) == PLAYERBULLET_SHOT )
						{	/* ��Ԃ̕ύX */
							PlayerBullet_SetState(objPP,PLAYERBULLET_HIT);
						}
					}
					#else

					
					
					int xlen = objE->x - objPP->x;
					int ylen = objE->y - objPP->y;
					int dist = objE->size + objPP->size;
					/* �Q�_�Ԃ̋������A���������i��̑傫���̘a)��菬������Γ�����@*/
					if ( ( ( xlen * xlen ) + ( ylen * ylen ) ) < ( dist * dist ) ) 
					{	/* ��Ԃ̕ύX */
						if( Enemy_GetState( objE ) == ENEMY_SHOT )
						{	
							/* ��Ԃ̕ύX */
							Enemy_SetState( objE, ENEMY_HIT ); 
						}
						if( PlayerBullet_GetState( objPP ) == PLAYERBULLET_SHOT )
						{	/* ��Ԃ̕ύX */
							PlayerBullet_SetState( objPP, PLAYERBULLET_HIT ); 
						}
					}

					int xlen1 = objE->x - objPP->ax1;
					int ylen1 = objE->y - objPP->ay1;

					int xlen2 = objE->x - objPP->ax2;
					int ylen2 = objE->y - objPP->ay2;
					
					/* �Q�_�Ԃ̋������A���������i��̑傫���̘a)��菬������Γ�����@*/
					if (objPP->ax1 < objE->x && objE->x < objPP->ax2) 
					{
						if (objPP->ay1 < objE->y && objE->y < objPP->ay2)
						{	/* ��Ԃ̕ύX */
							if (Enemy_GetState(objE) == ENEMY_SHOT)
							{
								/* ��Ԃ̕ύX */
								Enemy_SetState(objE, ENEMY_HIT2);
							}
							//if (PlayerBullet_GetState(objPP) == PLAYERBULLET_SLASH)
							//{	/* ��Ԃ̕ύX */
							//	PlayerBullet_SetState(objPP, PLAYERBULLET_HIT);
							//}
						}
					}
					//objPP->ax1

					#endif // 0
				}

				////
				if (( PlayerBullet_GetState(objPP) == PLAYERBULLET_SHOT/*PLAYERBULLET_SLASH*/)
					/*|| ( objPP->state == PLAYERBULLET_EXPLOSION )*/)
				{	/* Player�̒e���ʏ��Ԃł���΁A�R���W�����������s�� */
#if _USE_OUTER_PRODUCT_
					e.Cx = objPP->xold;	e.Cy = objPP->yold;
					e.Dx = objPP->x;	e.Dy = objPP->y;
					if (CrossCheck(&e))
					{	/* ��Ԃ̕ύX */
						if (Enemy_GetState(objE) == ENEMY_SHOT)
						{	/* ��Ԃ̕ύX */
							Enemy_SetState(objE, ENEMY_HIT);
						}
						if (PlayerBullet_GetState(objPP) == PLAYERBULLET_SHOT)
						{	/* ��Ԃ̕ύX */
							PlayerBullet_SetState(objPP, PLAYERBULLET_HIT);
						}
					}
#else
					int xlen = objE->x + objPP->x;
					int ylen = objE->y + objPP->y;
					int dist = objE->size - objPP->size;
					/* �Q�_�Ԃ̋������A���������i��̑傫���̘a)��菬������Γ�����@*/
					if (((xlen * xlen) + (ylen * ylen)) < (dist * dist))
					{	/* ��Ԃ̕ύX */
						if (Enemy_GetState(objE) == ENEMY_SHOT)
						{
							/* ��Ԃ̕ύX */
							Enemy_SetState(objE, ENEMY_HIT);
						}
						//if (PlayerBullet_GetState(objPP) == PLAYERBULLET_SLASH)
						//{   /* ��Ԃ̕ύX */
						//	PlayerBullet_SetState(objPP, PLAYERBULLET_HIT);
						//}
					}
#endif // 0
				}
				////
				/* ���̃f�[�^�����ɍs�� */
				/* �����A->next��NULL��������AobjPP��NULL�ɂȂ�̂� while���I������ */
				objPP = objPP->next;
			}
		}
		/* ���̃f�[�^�����ɍs�� */
		/* �����A->next��NULL��������AobjE��NULL�ɂȂ�̂� while���I������ */
		objE = objE->next;
	}

	//slash
	
	//
#endif
}

/*
*	�O�ς𗘗p���������蔻��
*	��̐������������邩�ǂ����𒲂ׂ���̂����A
*	�v���C���[�͈ړ����Ȃ����Ƃ�����̂Ŏg���ɂ���
* 
*	�܂��A����𗘗p���邽�߂ɂ͈ړ�����O�ƈړ�������̏�񂪕K�v�ɂȂ�
* *https://yttm-work.jp/collision/collision_0011.html
*/
#if _USE_OUTER_PRODUCT_
int CrossCheck(struct linesedge* e)
{
	float ABx,ABy;
	float ACx,ACy;
	float ADx,ADy;

	float CDx,CDy;
	float CAx,CAy;
	float CBx,CBy;
	ABx = e->Bx - e->Ax;	ABy = e->By - e->Ay;	/*	AB	Main Line	*/
	ACx = e->Cx - e->Ax;	ACy = e->Cy - e->Ay;	/*	AC	*/
	ADx = e->Dx - e->Ax;	ADy = e->Dy - e->Ay;	/*	AD	*/

	CDx = e->Dx - e->Cx;	CDy = e->Dy - e->Cy;	/*	CD	Main Line */
	CAx = e->Ax - e->Cx;	CAy = e->Ay - e->Cy;	/*	CA	*/
	CBx = e->Bx - e->Cx;	CBy = e->By - e->Cy;	/*	CB	*/

	float f01,f02;
	float f11,f12;
	f01 = ABx * ACy - ACx * ABy;
	f02 = ABx * ADy - ADx * ABy;

	f11 = CDx * CAy - CAx * CDy;
	f12 = CDx * CBy - CBx * CDy;

	if (f01 * f02 > 0.0) {
		return 0;
	}
	if (f11 * f12 > 0.0) {
		return 0;
	}
	return 1;
}
#endif // _USE_OUTER_PRODUCT_

