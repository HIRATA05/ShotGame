/*
* 
* Enemy
* 
* �G
* 
*/

#include "math.h"
#include "DxLib.h"
#include "ShotG.h"

#include "InputDevice.h"

#include "game.h"
#include "player.h"
#include "enemybullet.h"


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
*	�G�̒e���Ǘ�����v���O�������g�p����ϐ��Ȃǂ̐錾
*/
struct ENEMYBULLET_MGR_P
{
	int	tmr;
	int	number_of_objects;
};

static struct  ENEMYBULLET_MGR_P *work=NULL;

/*
*	�G�̒e�{�̂̃v���O�������g�p����ϐ��Ȃǂ̐錾
*/
/* ���̃v���O�����Ŏg�p����\���̂�OBJWORK_P�ƒ�`���Ă��܂��A�����ڂ̔ėp����}��@*/
#define	OBJWORK_P	struct ENEMYBULLETOBJECT_P
static OBJWORK_P *objwork=NULL;

/* �Е��������N�𗘗p�����v���O���� */
static OBJWORK_P *CreateNewWork();
static OBJWORK_P *DeleteWork( OBJWORK_P *p );
/* ************************************************************************* */
/*static */void EnemyBullet_SetState( OBJWORK_P *n, int s ){ n->state = s; }
/*static */int EnemyBullet_GetState( OBJWORK_P *n ){ return n->state; }

/* ************************************************************************* */
static OBJWORK_P *CreateNewWork()
{
	if( !objwork )
	{// objwork��NULL�̎�(�Ȃɂ��o�^����Ă��Ȃ��Ƃ�)
		OBJWORK_P *p = (OBJWORK_P *)malloc( sizeof( OBJWORK_P ) );
		if( p )
		{
			memset( p, 0, sizeof( OBJWORK_P ) );
			p->next = NULL;	// �����N�̍Ō��\���t���O
			objwork = p;	// �擪��o�^
			return p;
		} else {	/* ���s�����ꍇ�E�E�E�v���I�ȃG���[�ł͂��邪�A����͑Ή����Ȃ��@*/
		}
		return NULL;
	}else
	{// ���łɓo�^����Ă���Ƃ�
		OBJWORK_P *n = objwork;
		// �Ō�̃|�C���^��T��
		while( n->next )
		{
			n = n->next;
		}
		OBJWORK_P *p = (OBJWORK_P *)malloc( sizeof( OBJWORK_P ) );
		if( p )
		{
			memset( p, 0, sizeof( OBJWORK_P ) );
			p->next = NULL;	// �����N�̍Ō��\���t���O
			n->next = p;
			return p;
		} else {	/* ���s�����ꍇ�E�E�E�v���I�ȃG���[�ł͂��邪�A����͑Ή����Ȃ��@*/
		}
		return NULL;
	}
}
/* ************************************************************************* */
static OBJWORK_P *DeleteWork( OBJWORK_P *p )
{
	OBJWORK_P *n = objwork;
	// p���܂ރ|�C���^��T��
	if( !n )
	{// �o�^����Ă��Ȃ�->�v���I�ȃG���[
		return NULL;
	}

	if( n == p )
	{//���X�g�̐擪�̏ꍇ
		n = p->next;
		free( p );
		p = NULL;
		objwork = n;
	}else
	{
		while( n->next )
		{
			if( n->next == p )
			{//����
				n->next = p->next;	// �����N�̒��ւ�
				free( p );
				p = NULL;
				break; // while�̏I��
			}
			n = n->next;
		}
	}

	if( p )
	{// �����ł��Ă��Ȃ��Ƃ�->�v���I�ȃG���[
		return NULL;
	}
	if( n ){ return n->next; };
	return NULL;	// �S������������Ƃ�(���X�g����ɂȂ���)
}

/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
int	EnemyBullet_Init()	/* GameMode INIT�ŌĂ�	*/
{
	if( !work ){ work = (struct ENEMYBULLET_MGR_P *)malloc( sizeof( struct ENEMYBULLET_MGR_P ) );	}
	if( work ){ memset( work, 0, sizeof( struct ENEMYBULLET_MGR_P ) );	}
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
int	EnemyBullet_End()	/* GameMode END�ŌĂ�	*/
{
//�s������邩���E�E�E
	if(objwork)
	{//���X�g������̂ł��ׂĂ���ɂ���
		// �Е����̏ꍇ�A�S�����N���A����̂��ʓ|
		OBJWORK_P *n = objwork;
		OBJWORK_P *p;
		while(n)
		{
			p = n->next;
			DeleteWork(n);
			n = p;
			/* ���̂悤�ȃv���O�����ɂ���̂�NG
			* DeleteWork( n );
			* n = n->next;
			* �Ȃ��Ȃ�΁ADeleteWork()�֐��ɂ���āAn�̓��e�͖����ɂȂ��Ă���̂ŁA
			* n�̓��e���Q�Ƃ��邱�Ƃ��s���Ă͂Ȃ�Ȃ�
			*/
		}
	}

	if(work){
		free(work); work=NULL;
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
/* ������ */
enum {
	ENEMYBULLETTYPE_0 = 0,
	ENEMYBULLETTYPE_RIGHT2LEFT,
	ENEMYBULLETTYPE_LEFT2RIGHT,
	ENEMYBULLETTYPE_UP2BOTTOM,
	ENEMYBULLETTYPE_BOTTOM2UP,
	ENEMYBULLETTYPE_END,
};

/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
int	EnemyBullet_Update()	/* GameMode DSP�ŌĂ�	*/
{
	OBJWORK_P* n = objwork;
	OBJWORK_P* p;

	/* �\������ */
	while( n )
	{
		/* �ړ��O�̈ʒu���L�^���� */
		n->xold = n->x;
		n->yold = n->y;
		int	theEnd = false;
		switch( n->state )
		{
			case ENEMYBULLET_INIT:
				break;
			case ENEMYBULLET_SHOT:			/*���Ē�*/
				theEnd = false;
				switch( n->type )
				{
					case ENEMYBULLET_TYPE_0:
					{
						n->xf += n->spdx;
						n->yf += n->spdy;

						n->x = (int)n->xf;	/* �����ƌ^�L���X�g�𖾎����� [�������琮���ւ̕ϊ�] */
						n->y = (int)n->yf;	/* �����ƌ^�L���X�g�𖾎����� [�������琮���ւ̕ϊ�] */
						/* ��ʊO���� */
						if((n->x < -ENEMYBULLET_SIZE)
							 || (n->x > SCRN_WIDTH + ENEMYBULLET_SIZE)
							 || (n->y < -ENEMYBULLET_SIZE)
							 || (n->y > SCRN_HEIGHT + ENEMYBULLET_SIZE)) {
							theEnd = true;
						}
						if(theEnd)
						{
							EnemyBullet_SetState(n,ENEMYBULLET_DONE);	/*��ʊO�ɍs���Ă��܂���*/
						}else
						{
							DrawCircle(n->x,n->y,ENEMYBULLET_SIZE,ENEMYBULLET_COLOR,TRUE);
						}
						break;
					}
					default:
						break;
				}
				break;
			case ENEMYBULLET_HIT:			/*������*/
				EnemyBullet_SetState(n, ENEMYBULLET_EXPLOSION);
				/* fall through �� EXPLOSION�Ɉڍs���� */
				/*break;*/
			case ENEMYBULLET_EXPLOSION:		/*�j��*/
				EnemyBullet_SetState(n, ENEMYBULLET_DONE);
				break;
			case ENEMYBULLET_DONE:			/*�I����*/
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
			case ENEMYBULLET_INIT:
			case ENEMYBULLET_SHOT:			/*���Ē�*/
			case ENEMYBULLET_EXPLOSION:	/*�j��*/
				n = n->next;
				break;
			case ENEMYBULLET_DONE:			/*�I����*/
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
			DrawCircle( x, 500-14-14, 6, ENEMYBULLET_COLOR, TRUE );
			x = x +14;
			n = n->next;
		}
	}
#endif // DEBUG

	return 0;
}

/* *************************************************************************
* �G��ł֐�
* 
* 
* 
* 
* **************************************************************************/
int EnemyBullet_Shoot( int x, int y,int type )	/* Enemy Position */
{
	OBJWORK_P *n;


	n = CreateNewWork();
	if( n )
	{
		EnemyBullet_SetState( n, ENEMYBULLET_SHOT );	/*����*/
		n->x = x;
		n->y = y;
		n->xold = n->x;
		n->yold = n->y;
		n->spd = ENEMYBULLET_SPD;
		n->type = type;
		n->size = ENEMYBULLET_SIZE;
		GetPlayerPosition( &n->tagx, &n->tagy );	/* Player�̏ꏊ���擾���� */
		/* �ړ����x�̌v�Z */
		double diffx, diffy;
		double dist,frm;
		diffx = (double)n->tagx - ( double )n->x;
		diffy = ( double )n->tagy - ( double )n->y;
		dist = (float)sqrt( diffx * diffx + diffy * diffy );	// 2�_�ԋ���
		frm = dist / n->spd;	// frm ... �ړI�n�ɓ��B����܂ł̃t���[����
		n->spdx = (float)(diffx / frm);	// �e�����ړ����x
		n->spdy = (float)(diffy / frm);
		n->xf = (float)n->x;	/* �����ƌ^�L���X�g�𖾎����� [������������ւ̕ϊ�] */
		n->yf = (float)n->y;	/* �����ƌ^�L���X�g�𖾎����� [������������ւ̕ϊ�] */

		//DrawString( x-4, y-32, "X", GetColor( 240,40,40 ) );
	}else
	{/*�e��łĂȂ������I*/
		//DrawString( x-4-12, y-32, "�X�J�I", GetColor( 240,40,40 ) );
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
OBJWORK_P *GetObject_EnemyBullet( int n )	/* �v���C���[�̒e�̕ϐ��̈�̐擪�̃|�C���^���擾����	*/
{
	return objwork;
}


