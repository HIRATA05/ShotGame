/*
* 
* Player Bullet
* 
* �v���C���[�̒e
* 
*/

#include "DxLib.h"
#include "ShotG.h"

#include "InputDevice.h"

#include "game.h"
#include "player.h"
#include "playerbullet.h"
#include "sfx.h"

/*
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
* 
*/

/*
*	�e���Ǘ�����v���O�������g�p����ϐ��Ȃǂ̐錾
*/
struct PLAYERBULLET_MGR_P
{
	int	tmr;
	int	number_of_objects;
};

static struct  PLAYERBULLET_MGR_P *work=NULL;

/*
*	�e�{�̂̃v���O�������g�p����ϐ��Ȃǂ̐錾
*/



/* �z��𗘗p�����v���O���� */
#if _ARRAY_MODE_
#define	MAX_PLAYER_BULLET	3	/*�ő�e��*/
static struct PLAYERBULLETOBJECT_P	objwork[ MAX_PLAYER_BULLET ];
static void PlayerBullet_SetState( int n, int s ){ objwork[n].state = s; }
/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
int	PlayerBullet_Init()	/* GameMode INIT�ŌĂ�	*/
{
	if( !work ){ work = (struct PLAYERBULLET_MGR_P *)malloc( sizeof( struct PLAYERBULLET_MGR_P ) );	}
	if( work ){ memset( work, 0, sizeof( struct PLAYERBULLET_MGR_P ) );	}
	return 0;
}
/* *************************************************************************
*
*
*
*
*
* **************************************************************************/
int	PlayerBullet_End( )	/* GameMode END�ŌĂ�	*/
{
	if( work ) { free( work ); work = NULL; }
	return 0;
}


/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
int	PlayerBullet_Update()	/* GameMode DSP�ŌĂ�	*/
{
	int i;
	for( i=0; i< MAX_PLAYER_BULLET; i++ )
	{
		switch( objwork[i].state )
		{
			case PLAYERBULLET_INIT:
				break;
			case PLAYERBULLET_SHOT:			/*���Ē�*/
				objwork[i].y = objwork[i].y - objwork[i].spd;
				if( objwork[i].y < (-objwork[i].spd) )
				{
					PlayerBullet_SetState( i, PLAYERBULLET_DONE );	/*��ʊO�ɍs���Ă��܂���*/
				}else{
					DrawCircle( objwork[i].x, objwork[i].y, PLAYERBULLET_SIZE, PLAYERBULLET_COLOR, TRUE );
				}
				break;
			case PLAYERBULLET_EXPLOSION:	/*�j��*/
				break;
			case PLAYERBULLET_DONE:			/*�I����*/
				PlayerBullet_SetState( i, PLAYERBULLET_INIT );	/* �I������ */
				break;
		}
	}
	return 0;
}

/* *************************************************************************
* �e��ł֐�
* 
* 
* 
* 
* **************************************************************************/
int PlayerBullet_Shoot( int x, int y )	/* Player Position */
{
	int i;
	int f=FALSE;/*�e�������Ƃ��ł������ǂ����̃t���O*/
	for( i=0; i< MAX_PLAYER_BULLET; i++ )
	{
		switch( objwork[i].state )
		{
			case PLAYERBULLET_INIT:
				/* �e���łĂ��� */
				PlayerBullet_SetState( i, PLAYERBULLET_SHOT );	/*����*/
				objwork[i].x = x;
				objwork[i].y = y;
				objwork[i].spd = PLAYERBULLET_SPD;
				DrawString( x-4, y-32, "X", GetColor( 240,40,40 ) );
				f=TRUE;
				break;
			case PLAYERBULLET_SHOT:			/*���Ē�*/
			case PLAYERBULLET_EXPLOSION:	/*�j��*/
			case PLAYERBULLET_DONE:			/*�I����*/
				break;
		}
		if( f ){ break; }/*�e��ł��Ƃɐ��������̂�for�����I��������*/
	}
	if( !f )
	{/*�e��łĂȂ������I*/
		DrawString( x-4-12, y-32, "�X�J�I", GetColor( 240,40,40 ) );
	}
	return 0;
}

/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
struct PLAYERBULLETOBJECT_P *GetObject_PlayerBullet( int n )	/* �v���C���[�̒e�̕ϐ��̈�̐擪�̃|�C���^���擾����	*/
{
	return objwork;
}


/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */
#else  // !_ARRAY_MODE_
/* ���̃v���O�����Ŏg�p����\���̂�OBJWORK_P�ƒ�`���Ă��܂��A�����ڂ̔ėp����}��@*/
#define	OBJWORK_P	struct PLAYERBULLETOBJECT_P
static OBJWORK_P *objwork=NULL;

/* �Е��������N�𗘗p�����v���O���� */
static OBJWORK_P *CreateNewWork();
static OBJWORK_P *DeleteWork( OBJWORK_P *p );
/* ************************************************************************* */
/* ��ԑJ�ڂɊւ���֐� */
/*static */void PlayerBullet_SetState( OBJWORK_P *n, int s ){ n->state = s; }
/*static */int PlayerBullet_GetState( OBJWORK_P *n ){ return n->state; }
/* ************************************************************************* */
/* �����N���X�g�ɐV�K�f�[�^��ǉ����āA���̗̈�̃|�C���^��Ԃ��@*/
static OBJWORK_P *CreateNewWork()
{
	if( !objwork )
	{	/* objwork��NULL�̎��́A���X�g�����݂��Ȃ��Ƃ�(�Ȃɂ��o�^����Ă��Ȃ�)
		*	�ϐ��̈���ؗp����(malloc)
		*	���X�g�̐擪�ɂȂ�̂ŁAobjwork�Ƀ|�C���^��o�^����
		*/
		/* ���������ؗp���� */
		OBJWORK_P *p = (OBJWORK_P *)malloc( sizeof( OBJWORK_P ) );
		if ( p ) {	/* �������̎ؗp�ɐ��� */
			memset( p, 0, sizeof( OBJWORK_P ) );	/* �̈��0�N���A */
			p->next = NULL;	/* next��NULL�ɂ��� �� �����N�̍Ō�(�I�[)�ł��邱�Ƃ�\��	*/
			objwork = p;	/* �擪�ɓo�^ */
			return p;		/* �ؗp�����|�C���^��Ԃ� */
		} else 
		{	/* ���s�����ꍇ�E�E�E�v���I�ȃG���[�ł͂��邪�A����͑Ή����Ȃ��@*/
		}
		/* NULL��Ԃ��悤�Ȃ��Ƃ�����ꍇ�A����͒v���I�ȃG���[�Ƃ�����̂Œ��ӁI�I */
		return NULL;
	}else
	{/*
	 *	���łɓo�^����Ă���Ƃ�(���X�g�����݂���Ƃ�)
	 *	�ϐ��̈���ؗp����(malloc)
	 *	���X�g�̍Ō�ɒǉ�����
	 */ 
		OBJWORK_P *n = objwork;
		/* �Ō�̃|�C���^��T�� */ 
		while( n->next )
		{
			n = n->next;
		}
		/* ���������ؗp���� */
		OBJWORK_P *p = (OBJWORK_P *)malloc( sizeof( OBJWORK_P ) );
		if( p )
		{	/* �������̎ؗp�ɐ��� */
			memset( p, 0, sizeof( OBJWORK_P ) );	/* �̈��0�N���A */
			p->next = NULL;	/* next��NULL�ɂ��� �� �����N�̍Ō�(�I�[)�ł��邱�Ƃ�\��	*/
			n->next = p;	/* ���X�g�̍Ō�ɒǉ� */

			p->slashtmr = 0; //���̕\�����Ԃ̏�����
			return p;		/* �ؗp�����|�C���^��Ԃ� */
		} else {	/* ���s�����ꍇ�E�E�E�v���I�ȃG���[�ł͂��邪�A����͑Ή����Ȃ��@*/
		}
		/* NULL��Ԃ��悤�Ȃ��Ƃ�����ꍇ�A����͒v���I�ȃG���[�Ƃ�����̂Œ��ӁI�I */
		return NULL;
	}
}
/* ************************************************************************* */
/* �����N���X�g���珜�O���A�ϐ��̈��Ԕ[���� */
static OBJWORK_P *DeleteWork( OBJWORK_P *p )
{
	OBJWORK_P *n = objwork;
	if( !n ) {	/* ���X�g�����݂��Ȃ� ���Ȃɂ��o�^����Ă��Ȃ�->�v���I�ȃG���[	*/
		return NULL;
	}

	/* ���X�g����p�ƒl�̓����|�C���^��T��	*/
	if( n == p )
	{/*	���X�g�̐擪�̏ꍇ */
		n = p->next;	/* p->next��n�ɕۑ����� */
		free( p );		/* ������(p)��ԋp���� */
		p = NULL;		/* */
		objwork = n;	/* n(p->next)��擪�ɓo�^���� */
	}else
	{	/* ���X�g���ɓ������̂����݂���ꍇ */
		while( n->next )
		{
			if( n->next == p )
			{	/* p�𔭌� */
				n->next = p->next;	/* �����N�̒��ւ� */
				free( p );			/* ������(p)��ԋp���� */
				p = NULL;			/* */
				break;	/* while�̏I�� */
			}
			n = n->next;
		}
	}

	if( p )
	{/* �����ł��Ȃ������Ƃ��A�v���I�Ȗ�肪�N���Ă���ƍl�����ق����ǂ� */
		return NULL;	/* NULL��Ԃ����A����I���ł͂Ȃ� */
	}
	if( n ){ return n->next; };
	return NULL;	/* �S������������Ƃ�(���X�g����ɂȂ���) */
}

/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
int	PlayerBullet_Init()	/* GameMode INIT�ŌĂ�	*/
{
	if( !work ){ work = (struct PLAYERBULLET_MGR_P *)malloc( sizeof( struct PLAYERBULLET_MGR_P ) );	}
	if( work ){ memset( work, 0, sizeof( struct PLAYERBULLET_MGR_P ) );	}
	objwork = NULL;
	return 0;
}
/* *************************************************************************
*
*
*
*
*
* **************************************************************************/
int	PlayerBullet_End( )	/* GameMode END�ŌĂ�	*/
{
//�s������邩���E�E�E
	if( objwork ) 
	{//���X�g������̂ł��ׂĂ���ɂ���
		// �Е����̏ꍇ�A�S�����N���A����̂��ʓ|
		OBJWORK_P* n = objwork;
		OBJWORK_P* p;
		while( n ) {
			p = n->next;
			DeleteWork( n );
			n = p;
			/* ���̂悤�ȃv���O�����ɂ���̂�NG
			* DeleteWork( n );
			* n = n->next;
			* �Ȃ��Ȃ�΁ADeleteWork()�֐��ɂ���āAn�̓��e�͖����ɂȂ��Ă���̂ŁA
			* n�̓��e���Q�Ƃ��邱�Ƃ��s���Ă͂Ȃ�Ȃ�
			*/
		}
	}

	if( work ) { free( work ); work = NULL; }
	return 0;
}

/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
int	PlayerBullet_Update()	/* GameMode DSP�ŌĂ�	*/
{
	OBJWORK_P *n = objwork;
	OBJWORK_P *p;

	/* �\������ */
	while( n )
	{
		/* �ړ��O�̈ʒu���L�^���� */
		n->xold = n->x;
		n->yold = n->y;
		switch( n->state )
		{
			case PLAYERBULLET_INIT:
				break;
			case PLAYERBULLET_SHOT:			/*���Ē�*/
				//n->y = n->y - n->spd;
				n->x = n->x + n->spd;
				//if( n->y < (-n->spd) )
				if( (n->x < (-n->spd)) || (n->x > SCRN_WIDTH+(n->spd)))
				{
					PlayerBullet_SetState( n, PLAYERBULLET_DONE );	/*��ʊO�ɍs���Ă��܂���*/
				}else{
					switch (n->weapontype)
					{
						case PLBULLET_TYPE_BULLET:
							DrawCircle(n->x, n->y, n->size, PLAYERBULLET_COLOR, TRUE);
							break;
						case PLBULLET_TYPE_SLASH:
						{
							n->slashtmr++; // ���̕\�����Ԃ𑝉�
							if (n->slashtmr > _0SECOND) { // ���Ԍo�߂Ō�������
								PlayerBullet_SetState(n, PLAYERBULLET_DONE);
								ClearSlashFlag(); // ���̃t���O�̏���
							}
							else {
								GetPlayerPosition(&n->x, &n->y);	/* Player�̈ʒu���擾���� */
								DrawString(n->x + 20, n->y - 10, "=={+>>>>", PLAYERBULLET_COLOR);
								int l = GetDrawStringWidth("=={+>>>>", strlen("=={+>>>>")); // ���̉摜�̉������擾
								n->ax1 = n->x + 20; n->ay1 = n->y - 10;
								n->ax2 = n->x + 20 + l; n->ay2 = n->y - 10 + 14;
								//DrawBox(n->ax1, n->ay1, n->ax2, n->ay2, GetColor(240, 240, 0), 0); // �����蔻��̉���
							}
						}
						break;
					}
				}
				break;
			case PLAYERBULLET_HIT:			/*������*/
				PlayerBullet_SetState(n, PLAYERBULLET_EXPLOSION);
				/* fall through �� EXPLOSION�Ɉڍs���� */
				/*break;*/
			case PLAYERBULLET_EXPLOSION:	/*�j��*/
				ClearSlashFlag(); // ���̃t���O�̏���
				PlayerBullet_SetState( n, PLAYERBULLET_DONE );
				break;
			case PLAYERBULLET_DONE:			/*�I����*/
				break;
		}
		n = n->next;
	}


	/* �폜���� */
	n = objwork;
	while( n )
	{
		switch( n->state )
		{
			default:
			case PLAYERBULLET_INIT:
			case PLAYERBULLET_SHOT:			/*���Ē�*/
			//case PLAYERBULLET_SLASH:	//�a���ǉ�
			case PLAYERBULLET_HIT:			/*������*/
			case PLAYERBULLET_EXPLOSION:	/*�j��*/
				n = n->next;
				break;
			case PLAYERBULLET_DONE:			/*�I����*/
				/* �I������ */
				p = n->next;
				DeleteWork( n );
				n = p;
				/* ���̂悤�ȃv���O�����ɂ���̂�NG
				* DeleteWork( n );
				* n = n->next;
				* �Ȃ��Ȃ�΁ADeleteWork()�֐��ɂ���āAn�̓��e�͖����ɂȂ��Ă���̂ŁA
				* n�̓��e���Q�Ƃ��邱�Ƃ��s���Ă͂Ȃ�Ȃ�
				*/
				break;
		}
	}

#if DEBUG
	{	// �L���ȕϐ��̈搔�����ł���킷
		OBJWORK_P *n = objwork;
		int x = 8;
		while( n )
		{
			DrawCircle( x, 500, 6, PLAYERBULLET_COLOR, TRUE );
			x = x +14;
			n = n->next;
		}
	}
#endif // DEBUG

	return 0;
}


/* *************************************************************************
* �e��ł֐�
* 
* 
* 
* 
* **************************************************************************/
int PlayerBullet_Shoot( int x, int y )	/* Player Position */
{
	OBJWORK_P *n;


	n = CreateNewWork();
	if( n )
	{
		sfx_PlShoot();
		PlayerBullet_SetState( n, PLAYERBULLET_SHOT );	/*����*/
		n->x = x;
		n->y = y;
		n->xold = n->x;
		n->yold = n->y;
		n->spd = PLAYERBULLET_SPD;
		n->size = PLAYERBULLET_SIZE;
		n->weapontype = PLBULLET_TYPE_BULLET; // ����̎�� �e
		DrawString( x+20, y-10, "X", GetColor( 240,40,40 ) );
	}else
	{/*�e��łĂȂ������I*/
		DrawString( x-4-12, y-32, "�X�J�I", GetColor( 240,40,40 ) );
	}
	return 0;
}

/* �ǉ�*********************************************************************
*
*
*
*
*
* **************************************************************************/
int PlayerBullet_Slash(int x, int y)	/* Player Position */
{
	OBJWORK_P* n;


	n = CreateNewWork();
	if (n)
	{
		sfx_PlShoot();
		PlayerBullet_SetState(n, PLAYERBULLET_SHOT/*PLAYERBULLET_SLASH*/);	/*�a��*/
		n->x = x;
		n->y = y;
		n->xold = n->x;
		n->yold = n->y;
		n->size = PLAYERBULLET_SIZE;
		n->weapontype = PLBULLET_TYPE_SLASH; // ����̎�� �e
		//DrawString( x-4, y-32, "X", GetColor( 240,40,40 ) );
		DrawString(x + 20, y-10, "+", GetColor(240, 40, 40));
	}
	else
	{/*�e��łĂȂ������I*/
		DrawString(x - 4 - 12, y - 32, "�X�J�I", GetColor(240, 40, 40));
	}
	return 0;
}
/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
OBJWORK_P *GetObject_PlayerBullet( int n )	/* �v���C���[�̒e�̕ϐ��̈�̐擪�̃|�C���^���擾����	*/
{
	return objwork;
}



#endif // !_ARRAY_MODE_
