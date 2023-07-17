/*
* 
* Enemy
* 
* �G
* 
*/

#include "DxLib.h"
#include "ShotG.h"

#include "InputDevice.h"

#include "game.h"
#include "player.h"
#include "enemy.h"
#include "enemybullet.h"

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
*	�G���Ǘ�����v���O�������g�p����ϐ��Ȃǂ̐錾
*/
struct ENEMY_MGR_P
{
	int	tmr;
	int	number_of_objects;
};

static struct  ENEMY_MGR_P *work=NULL;

/*
*	�G�{�̂̃v���O�������g�p����ϐ��Ȃǂ̐錾
*/
/* ���̃v���O�����Ŏg�p����\���̂�OBJWORK_P�ƒ�`���Ă��܂��A�����ڂ̔ėp����}��@*/
#define	OBJWORK_P	struct ENEMYOBJECT_P
static OBJWORK_P *objwork=NULL;

/* �Е��������N�𗘗p�����v���O���� */
static OBJWORK_P *CreateNewWork();
static OBJWORK_P *DeleteWork( OBJWORK_P *p );
/* ************************************************************************* */
/*static */void Enemy_SetState( OBJWORK_P *n, int s ){ n->state = s; }	/* �G��state��ݒ肷�� */
/*static */int Enemy_GetState( OBJWORK_P *n ){ return n->state; }		/* �G��state���擾����@*/

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
int	Enemy_Init()	/* GameMode INIT�ŌĂ�	*/
{
	if( !work ){ work = (struct ENEMY_MGR_P *)malloc( sizeof( struct ENEMY_MGR_P ) );	}
	if( work ){ memset( work, 0, sizeof( struct ENEMY_MGR_P ) );	}
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
int	Enemy_End()	/* GameMode END�ŌĂ�	*/
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
struct ENEMYPOS {
	int		x, y;		/* �����ʒu */
	int		type;		/* ����^�C�v */
	int		btm;		/* �e�����܂ł̎��� */
	int		startTm;	/* �������� */
};

struct ENEMYPOSDT {
	int		startTm;	/* �o������ */
	struct ENEMYPOS data;
};

static struct ENEMYPOSDT enemyposdt[ ] = 
{
		{	_1SECOND * 1,	{	980,	SCRN_HEIGHT / 2,	ENEMYTYPE_MYMOVE_Q,		_1SECOND, 0 }},
		{	_1SECOND * 1,	{	980,	SCRN_HEIGHT / 2,	ENEMYTYPE_MYMOVE_Z,		_1SECOND, 0 }},

		{	_1SECOND * 1.5,	{	980,	SCRN_HEIGHT / 1.5,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 1.5,	{	980,	SCRN_HEIGHT / 3,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},

		{	_1SECOND * 2,	{	980,	SCRN_HEIGHT / 1.5,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 2,	{	980,	SCRN_HEIGHT / 3,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 2.2,	{	980,	SCRN_HEIGHT / 1.5,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 2.2,	{	980,	SCRN_HEIGHT / 3,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 2.4,	{	980,	SCRN_HEIGHT / 1.5,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 2.4,	{	980,	SCRN_HEIGHT / 3,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 2.6,	{	980,	SCRN_HEIGHT / 1.5,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 2.6,	{	980,	SCRN_HEIGHT / 3,	ENEMYTYPE_RIGHT2LEFT,	_1SECOND, 0 }},
		{	_1SECOND * 2.6,	{	980,	SCRN_HEIGHT / 2,	ENEMYTYPE_MYMOVE_Q,		_1SECOND, 0 }},
		{	_1SECOND * 2.6,	{	980,	SCRN_HEIGHT / 2,	ENEMYTYPE_MYMOVE_Z,		_1SECOND, 0 }},

		{	_1SECOND * 3,	{	980,	SCRN_HEIGHT / 2,	ENEMYTYPE_MYMOVE_Q,		_1SECOND, 0 }},
		{	_1SECOND * 3,	{	980,	SCRN_HEIGHT / 2,	ENEMYTYPE_MYMOVE_Z,		_1SECOND, 0 }},
		{	_1SECOND * 3,	{	980,	SCRN_HEIGHT / 2,	ENEMYTYPE_MYMOVE_Z,		_1SECOND, 0 }},
		{	_1SECOND * 3,	{	980,	SCRN_HEIGHT / 2,	ENEMYTYPE_MYMOVE_Q,		_1SECOND, 0 }},
		/*
		{	_1SECOND * 1,	{	SCRN_WIDTH/2,	-60,	ENEMYTYPE_TOP2BOTTOM,	_1SECOND, 0	}},
		{	_1SECOND * 2,	{	SCRN_WIDTH/2,	-60,	ENEMYTYPE_TOP2BOTTOM,	_1SECOND, 0	}},
		{	_1SECOND * 3,	{	SCRN_WIDTH/2,	-60,	ENEMYTYPE_TOP2BOTTOM,	_1SECOND, 0	}},
		{	_1SECOND * 4,	{	SCRN_WIDTH/2,	-60,	ENEMYTYPE_TOP2BOTTOM,	_1SECOND, 0	}},
		{	_1SECOND * 5,	{	SCRN_WIDTH/2,	-60,	ENEMYTYPE_TOP2BOTTOM,	_1SECOND, 0	}},

		{	_1SECOND * 6,	{	-20,	SCRN_HEIGHT/2,	ENEMYTYPE_LEFT2RIGHT,	_1SECOND, 0	}},
		{	_1SECOND * 8,	{	-20,	SCRN_HEIGHT/2,	ENEMYTYPE_LEFT2RIGHT,	_1SECOND, 0	}},
		{	_1SECOND *10,	{	-20,	SCRN_HEIGHT/2,	ENEMYTYPE_LEFT2RIGHT,	_1SECOND, 0	}},
		{	_1SECOND *12,	{	-20,	SCRN_HEIGHT/2,	ENEMYTYPE_LEFT2RIGHT,	_1SECOND, 0	}},
		{	_1SECOND *14,	{	-20,	SCRN_HEIGHT/2,	ENEMYTYPE_LEFT2RIGHT,	_1SECOND, 0	}},
		*/
};
#define	MAX_ENEMY_TIME_TABLE	(_1SECOND *16)	/* �e�[�u���̍ŏI����+2�b	*/
#define	ENEMYPOSDT_NUM_MEMBER	(sizeof( enemyposdt ) / sizeof( struct ENEMYPOSDT ))
/* *************************************************************************
* 
* 
* 
* 
* 
* **************************************************************************/
int	Enemy_Update()	/* GameMode DSP�ŌĂ�	*/
{


	/* �G�A���� */
	int t = GetGameTimer( );
	t = t % MAX_ENEMY_TIME_TABLE;
	for ( int i = 0;i < ENEMYPOSDT_NUM_MEMBER; i++ )
	{
		if ( enemyposdt[ i ].startTm == t ) 
		{
			Enemy_Shoot( enemyposdt[ i ].data.x, enemyposdt[ i ].data.y, enemyposdt[ i ].data.type, enemyposdt[ i ].data.btm );
		}
	}

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
			case ENEMY_INIT:
				break;
			case ENEMY_SHOT:			/*���Ē�*/
				switch ( n->type ) 
				{
				case ENEMYTYPE_RIGHT2LEFT:
					n->x = n->x - n->spd;
					if ( n->x < -n->spd ) { theEnd = true; }
					break;
				case ENEMYTYPE_LEFT2RIGHT:
					n->x = n->x + n->spd;
					if ( n->x > ( SCRN_WIDTH + n->spd ) ) { theEnd = true; }
					break;
				case ENEMYTYPE_TOP2BOTTOM:
					n->y = n->y + n->spd;
					if ( n->y > ( SCRN_HEIGHT+n->spd) ) { theEnd = true; }
					break;
				case ENEMYTYPE_BOTTOM2TOP:
					n->y = n->y - n->spd;
					if ( n->y < -n->spd ) {	theEnd = true;	}
					break;
				case ENEMYTYPE_MYMOVE_M:
					n->y = n->y + n->spd;
					n->x = n->x + (n->spd / 2);

					if (n->y > (SCRN_HEIGHT + n->spd)) { theEnd = true; }
					break;

				case ENEMYTYPE_MYMOVE_B:
					n->x = n->x + n->spd;
					n->x = n->x - (n->spd / 2);
					if (n->y > (SCRN_HEIGHT + n->spd)) { theEnd = true; }
					break;

				case ENEMYTYPE_MYMOVE_Q:
					n->x = n->x - n->spd;
					n->y = n->y - (n->spd / 3);
					if (n->x < -n->spd) { theEnd = true; }
					break;

				case ENEMYTYPE_MYMOVE_Z:
					n->x = n->x - n->spd;
					n->y = n->y + (n->spd / 3);
					if (n->x < -n->spd) { theEnd = true; }
					break;
				}
				if( theEnd )
				{
					Enemy_SetState( n, ENEMY_DONE );	/*��ʊO�ɍs���Ă��܂���*/
				}else{
					DrawCircle( n->x, n->y, n->size, ENEMY_COLOR, TRUE );
					n->tmr++;
					if ( n->tmr == n->btm ) { /* �e��ł^�C�~���O�ɂȂ����Ȃ� */
						EnemyBullet_Shoot( n->x, n->y,ENEMYBULLET_TYPE_0 );
					}
				}
				break;
			case ENEMY_HIT:			/*������*/
				{
					/* �����ŃX�R�A�A�b�v������A���ʉ���炵���肷��̂�
					* �̂��Ɂu�ϋv�G�v�Ȃǂւ̑Ή����\�ɂ��₷�����邽�߂ł�	*/
					/* �X�R�A�A�b�v */
					modeGame_AddScore(1);
					/* ���ʉ� */
					sfx_EnmHit();

					Enemy_SetState(n, ENEMY_EXPLOSION);
					/* fall through �� EXPLOSION�Ɉڍs���Ă��悢 */
					break;
				}	
			//
			case ENEMY_HIT2:			/*������2*/
				{
					/* �����ŃX�R�A�A�b�v������A���ʉ���炵���肷��̂�
					* �̂��Ɂu�ϋv�G�v�Ȃǂւ̑Ή����\�ɂ��₷�����邽�߂ł�	*/
					/* �X�R�A�A�b�v */
					modeGame_AddScore2(2);
					/* ���ʉ� */
					sfx_EnmHit();

					Enemy_SetState(n, ENEMY_EXPLOSION);
					/* fall through �� EXPLOSION�Ɉڍs���Ă��悢 */
					break;
				}
			//
			case ENEMY_EXPLOSION:	/*�j��*/
				{
					n->etmr++;
					if( n->etmr > _1SECOND )
					{
						Enemy_SetState( n, ENEMY_DONE );	/*��ʊO�ɍs���Ă��܂��� �� �I�� */
					}
					if( n->etmr & 4 )
					{
						DrawCircle( n->x, n->y, n->size, ENEMY_COLOR, TRUE );
					}else{
						DrawCircle( n->x, n->y, n->size, ~ENEMY_COLOR, TRUE );
					} 
				}
				break;
			case ENEMY_DONE:			/*�I����*/
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
			case ENEMY_INIT:
			case ENEMY_SHOT:		/*���Ē�*/
			case ENEMY_HIT:			/*������*/
			case ENEMY_EXPLOSION:	/*�j��*/
				n = n->next;
				break;
			case ENEMY_DONE:		/*�I����*/
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
			DrawCircle( x, 500-14, 6, ENEMY_COLOR, TRUE );
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
int Enemy_Shoot( int x, int y,int type, int btm )	/* Enemy Position */
{
	OBJWORK_P *n;


	n = CreateNewWork();
	if( n )
	{
		Enemy_SetState( n, ENEMY_SHOT );	/*����*/
		n->x = x;
		n->y = y;
		n->xold = n->x;
		n->yold = n->y;
		n->spd = ENEMY_SPD;
		n->type = type;
		n->btm = btm;
		n->size = ENEMY_SIZE;
		DrawString( x-4, y-32, "X", GetColor( 240,40,40 ) );
	}else
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
OBJWORK_P *GetObject_Enemy( int n )	/* �v���C���[�̒e�̕ϐ��̈�̐擪�̃|�C���^���擾����	*/
{
	return objwork;
}


