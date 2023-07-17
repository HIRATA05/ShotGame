// ��ʑJ��(���[�h,�V�[��)����

#include "assert.h"
#include "DxLib.h"

#include "ShotG.h"
#include "InputDevice.h"
#include "mode.h"

#include "title.h"
#include "game.h"

// mode�̒�`
enum {
	MODE_INIT = 0,		// Mode Control�̏������p
	MODE_TITLE_INIT,	// �Q�[���^�C�g���\���@�������p
	MODE_TITLE_DSP,		// �Q�[���^�C�g���\���@�\���p
	MODE_TITLE_END,		// �Q�[���^�C�g���\���@�I�����p
	MODE_GAME_INIT,		// �Q�[���{�� �������p
	MODE_GAME_DSP,		// �Q�[���{�� �\���p
	MODE_GAME_END,		// �Q�[���{�� �I�����p
	MODE_END
};

#if DEBUG
static const char *mstr[] = {
	"MODE_INIT",		// Mode Control�̏������p
	"MODE_TITLE_INIT",	// �Q�[���^�C�g���\���@�������p
	"MODE_TITLE_DSP",		// �Q�[���^�C�g���\���@�\���p
	"MODE_TITLE_END",		// �Q�[���^�C�g���\���@�I�����p
	"MODE_GAME_INIT",		// �Q�[���{�� �������p
	"MODE_GAME_DSP",		// �Q�[���{�� �\���p
	"MODE_GAME_END",		// �Q�[���{�� �I�����p
	"MODE_END"
};

#endif // DEBUG


// mode�Ǘ��p�ϐ��̈�̊m��
// �\����
struct MODE_P
{
	int	mode;				// ���[�h�ԍ�(enum)
	int	mode_tm;			// ���[�h�o�ߎ���
	int mode_game_request;	// GameMode�ɐi�ރt���O
	int ed_flg; //�Q�[�����I���������̃t���O
};

static struct MODE_P *mode_p = NULL;

#define	MODE_TITLE_EXPIRE_SECOND	(600*FRAME_PER_SECOND)	// MODE_TITLE_DSP�����Ԑ؂�ɂȂ�܂ł̕b��

/* ���[�h��ݒ肷�� */
static void SetMode( int x ) {	mode_p->mode = x;	mode_p->mode_tm = 0;	}
/* ���݂̃��[�h��Ԃ� */
static int GetMode( ) { return mode_p->mode; }

/* �Q�[�����N�G�X�g��1�ɂ��� */
static void SetGameRequest( ) {	mode_p->mode_game_request = 1;}
/* �Q�[�����N�G�X�g��0�ɂ��� */
static void ClearGameRequest() { mode_p->mode_game_request = 0; }
/* �Q�[�����N�G�X�g�̏�Ԃ�Ԃ� */
static int GetGameRequest( ) { return mode_p->mode_game_request; }


// mode�̏�����
int Mode_Init( )
{
	// �����Amode_p��NULL�łȂ�������(����������Ă��Ȃ�������)�G���[
	// �����ł� assert�̎g�����̌��{�Ƃ��Ďg�p����
	assert( mode_p == NULL );

	mode_p = (struct MODE_P*)malloc( sizeof( struct MODE_P ) );
	if( mode_p )
	{// �\���̂̏�����
		mode_p->mode = MODE_TITLE_INIT;
		mode_p->mode_tm = 0;
		mode_p->mode_game_request = 0;
		return TRUE;
	}
	/*
	* FALSE��Ԃ��P�[�X�������Ă͂Ȃ�Ȃ�
	* �����A�����܂ŗ��Ă��܂�����A����͒v���I�ȃG���[�Ƃ�����
	*/
	return FALSE;
}

// mode�̏I��
int Mode_End()
{
	// �����Amode_p���ݒ肳��Ă��Ȃ�������G���[
	// �����ł� assert�̎g�����̌��{�Ƃ��Ďg�p����

	assert(mode_p != NULL);

	if ( mode_p ) {
		free( mode_p );
		mode_p = NULL;
		return TRUE;
	}
	/*
	* FALSE��Ԃ��P�[�X�������Ă͂Ȃ�Ȃ�
	* �����A�����܂ŗ��Ă��܂�����A����͒v���I�ȃG���[�Ƃ�����
	*/
	return FALSE;
}



// ����
// 
int Mode_Ctrl()
{
	/*
	* Start Button(�X�y�[�X�L�[)����
	*/
	if( (mode_p->mode == MODE_TITLE_DSP ) )
	{// MODE_TITLE_DSP �̂Ƃ��̂ݓ��̓`�F�b�N
		if( isKeyDown(KEY_INPUT_SPACE ) )
		{// �X�y�[�X�L�[�������ꂽ��GameMode�ɍs�����N�G�X�g�t���O�𗧂Ă�
			SetGameRequest();
		}
	}

	/*
	*	���Ԑ؂ꃂ�[�h�i�s
	*/
	mode_p->mode_tm++;
	if(mode_p->mode_tm > MODE_TITLE_EXPIRE_SECOND )
	{
		mode_p->mode++;	// Mode���ЂƂi�߂�
		mode_p->mode_tm = 0;
	}

	switch(mode_p->mode )
	{
		case MODE_TITLE_INIT:	// �^�C�g����� ����
			{
				modeTitle_Init();
				SetMode( MODE_TITLE_DSP );
			}
			break;
		case MODE_TITLE_DSP:	// �^�C�g����� �\��
			{
				int n = modeTitle_Dsp();
				if( n )
				{	/* �^�C���A�E�g�����̂Ŏ��̃��[�h�ɑJ�ڂ��� */
					SetMode(MODE_TITLE_END);
				}
				else if ( GetGameRequest() )
				{/* GameModeRequest�t���O�������Ă���̂ŁA���݂̃��[�h���I������ */
						SetMode(MODE_TITLE_END);
				}
			}
			break;
		case MODE_TITLE_END:	// �^�C�g����� �I��
			{
				modeTitle_End();
				if( GetGameRequest( ) )
				{/* GameModeRequest�t���O�������Ă���̂ŁA���݂̃��[�h���I������ */
					ClearGameRequest(); /* GameModeRequest�t���O�̃N���A */
					SetMode( MODE_GAME_INIT );	/* GameMode�֑J�ڂ��� */	
				}else{
					SetMode( MODE_TITLE_INIT ); /* TITLE�����[�v */
				}
			}
			break;
		case MODE_GAME_INIT:	// �Q�[����� ����
			{
			mode_p->ed_flg = false;
				modeGame_Init();
				SetMode( MODE_GAME_DSP ); //
				//PlaySound	( "���y�t�@�C���̏ꏊ" , �Đ��^�C�v ) ;
				//DX_PLAYTYPE_BACK
				PlaySound("Battle4.ogg", DX_PLAYTYPE_BACK);
			}
			break;
		case MODE_GAME_DSP:	// �Q�[����� �\��
			{
				if( modeGame_Dsp() )
				{
					SetMode( MODE_GAME_END ); // Game End��
				}else{
					// ���Ԑ؂�ɂȂ�Ȃ��悤�Ɏ��Ԃ������߂�
					mode_p->mode_tm--;
				}
			}
			break;
		case MODE_GAME_END:	// �Q�[����� �I��
			{
				PlaySound("", DX_PLAYTYPE_BACK);
				mode_p->ed_flg = true;
				modeGame_End();
				SetMode( MODE_TITLE_INIT ); //	TITLE�փ��[�v
			}
			break;

		default:
			{// ����̂Ȃ��v���O�����̃��[�h�ɗ�����
				SetMode( MODE_INIT ); // TITLE�փ��[�v
			}
			break;
	}

#if DEBUG
	{
		char n[ 128 ];

		DrawString( 0, SCRN_HEIGHT - 14 * 2, mstr[ GetMode() ], GetColor( 240,240,240 ) );
		sprintf_s( n, "ELAPS TIME:%d", mode_p->mode_tm );
		DrawString(0, SCRN_HEIGHT - 14 * 1, n, GetColor(240, 240, 240));
	}
#endif


	// ���[�h�ԍ��̃I�[�o�[�t���[�ɑΉ�����
	if(mode_p->mode >= MODE_END )
	{
		mode_p->mode = MODE_INIT + 1;
		mode_p->mode_tm = 0;
	}

	return FALSE;
}



