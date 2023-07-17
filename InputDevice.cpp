/*
* �L�[�{�[�h��}�E�X�̓��͂ɑ΂���֐���p�ӂ���
* 
* 
* 
* 
* 
*/
#include "stdio.h"
#include "DxLib.h"
#include "InputDevice.h"

// Mouse
static int mouse_input		= 0;
static int mouse_input_old	= 0;
static int mouse_input_on	= 0;
static int mouse_input_release = 0;
static int mouse_x=0, mouse_y=0;
// Keyboard
#define MAX_KEYBUFFER_SIZE	256	// GetHitKeyStateAll()���Ԃ������m�ۂ���̂ɕK�v�ȗv�f��
static DX_CHAR *keyBuf			= NULL;	// �L�[�{�[�h�̌��݂̏�Ԃ��擾���ĕۑ�����̈�
static DX_CHAR *keyBufOld		= NULL;	// �ЂƂO�̃t���[���ɂ�����L�[�{�[�h�̏�Ԃ�ۑ�����̈�
static DX_CHAR *keyOnBuf		= NULL;	// �L�[��On�ɂȂ����u�Ԃ���1�ɂȂ�l���i�[�����̈�
static DX_CHAR *keyReleaseBuf	= NULL;	// �L�[��Off�ɂȂ����u�Ԃ���1�ɂȂ�l���i�[�����̈�

//���̓f�o�C�X�̏�����
bool InputDevice_Init( )
{
	//�@�l���i�[����̈���m�ۂ���
	if( keyBuf ){}else{ keyBuf = (DX_CHAR*)malloc( sizeof(DX_CHAR) * MAX_KEYBUFFER_SIZE );}
	if( !keyBufOld ){ keyBufOld =(DX_CHAR*)malloc( sizeof(DX_CHAR) * MAX_KEYBUFFER_SIZE );}
	if( !keyOnBuf ) { keyOnBuf = (DX_CHAR*)malloc( sizeof(DX_CHAR) * MAX_KEYBUFFER_SIZE );}
	if( !keyReleaseBuf ) { keyReleaseBuf = (DX_CHAR*)malloc( sizeof(DX_CHAR) * MAX_KEYBUFFER_SIZE );}
	
	// Memory�̊m�ۂ��ł��Ȃ���΁Afalse��Ԃ�
	// ���łɊm�ۂ��Ă���Memory������ꍇ�́A�J�����ׂ������A����͍s��Ȃ�
	// �����A�����ŃG���[���Ԃ�ꍇ�͒v���I(Critical)�Ȃ̂ŁA�A�v���P�[�V�������I�����ׂ�
	if( !keyBuf )	{	return false; }
	if( !keyBufOld ){	return false; }
	if( !keyOnBuf ) {	return false; }
	if( !keyReleaseBuf ) {	return false; }

	// keyBuf��0 fill����
	for(int i = 0;i<MAX_KEYBUFFER_SIZE; i++){
		keyBuf[i] = 0;
	}
	// ��L��for���̓��e��memset�Ƃ����֐��ōs�����Ƃ��ł���
	memset( keyBuf, 0, sizeof(DX_CHAR)*MAX_KEYBUFFER_SIZE );
	return true;
}

//���̓f�o�C�X�֐��̏I����
bool InputDevice_End( )
{
	// �m�ۂ����̈���J������
	if( keyBuf )	{	free( keyBuf );	keyBuf = NULL;	}
	if( keyBufOld ) {	free( keyBufOld );	keyBufOld = NULL;	}
	if( keyOnBuf )	{	free( keyOnBuf );	keyOnBuf = NULL;	}
	if( keyReleaseBuf ){free( keyReleaseBuf );	keyReleaseBuf = NULL;	}
	return true;
}

//���̓f�o�C�X�֌W�̃f�[�^�̍X�V
bool InputDevice_Update( )	
{
	// �L�[�{�[�h�̏���
	// ���O�̏�Ԃ�ʂ̗̈�ɕۑ�����
	for ( int i = 0; i < MAX_KEYBUFFER_SIZE; i++ ) { keyBufOld[ i ] = keyBuf[ i ]; }   // �Ō�̃L�[�{�[�h��Ԃ��R�s�[����
	// �ŐV�̏�Ԃ��擾����
	GetHitKeyStateAll( keyBuf );// ���݂̃L�[�{�[�h�̏�Ԃ��擾����
	// �L�[�{�[�h�̊e��Ԃ��X�V����
	for ( int i = 0; i < MAX_KEYBUFFER_SIZE; i++ ){
		int st = keyBufOld[ i ] ^ keyBuf[ i ];
		keyOnBuf[ i ] = st & keyBuf[ i ];			// On Trigger
		keyReleaseBuf[ i ] = st & keyBufOld[ i ];   // Off Trigger
	}

	// �}�E�X�̏���
	// ���O�̏�Ԃ�ۑ�
	mouse_input_old = mouse_input;
	// �ŐV�̏�Ԃ��擾����
	mouse_input = GetMouseInput();

	// �}�E�X�̊e��Ԃ��X�V����
	int st = mouse_input_old ^ mouse_input;
	mouse_input_on = st & mouse_input;				// On Trigger
	mouse_input_release = st & mouse_input_old;		// Off Trigger

	GetMousePoint( &mouse_x, &mouse_y );
	return true;
}

// �}�E�X�֌W
// MOUSE_INPUT_�̏�Ԃ�Ԃ�
int	 GetMouseInputAll( )
{
	return mouse_input;
}

// MOUSE_INPUT_�̉����ꂽ�u�Ԃ̏�Ԃ�Ԃ�
int	 GetMouseInputOn( )
{
	return mouse_input_on;
}

// MOUSE_INPUT_�̗����ꂽ�u�Ԃ̏�Ԃ�Ԃ�
int	 GetMouseInputRelease( )
{
	return mouse_input_release;
}

// Mouse�̈ʒu���擾����
int  GetMousePosition( int* x, int* y )
{
	*x = mouse_x;
	*y = mouse_y;
	return true;
}

// MOUSE_INPUT_ ��������Ă�����Atrue��Ԃ�
bool isMouseInputDown( int mouse_in_ )
{
	return mouse_input & mouse_in_;
}

// MOUSE_INPUT_ �������ꂽ�u�ԁAtrue��Ԃ�
bool isMouseInputOn( int mouse_in_ )
{
	return mouse_input_on & mouse_in_;
}

// MOUSE_INPUT_ �������ꂽ�u�ԁAtrue��Ԃ�
bool isMouseInputRelease( int mouse_in_ )
{
	return mouse_input_release & mouse_in_;
}


// �L�[�{�[�h�֌W
// keycode�Ŏ������L�[�{�[�h��������Ă�����Atrue��Ԃ�
bool isKeyDown( int keycode )
{
	// �G���[����
	if( keycode >= MAX_KEYBUFFER_SIZE ){ return false; }
	if( keycode < 0 ) { return false; }
	// ��s�ŏ�����
	if( (keycode >= MAX_KEYBUFFER_SIZE) || (keycode < 0) ){ return false; }
	// ���ʂ�Ԃ�
	return keyBuf[ keycode ];
}

// keycode�Ŏ������L�[�{�[�h�������ꂽ�u�ԁAtrue��Ԃ�
bool isKeyOn( int keycode )
{
	// �G���[����
	if ( keycode >= MAX_KEYBUFFER_SIZE ) { return false; }
	if ( keycode < 0 ) { return false; }
	// ���ʂ�Ԃ�
	return keyOnBuf[ keycode ];
}

// keycode�Ŏ������L�[�{�[�h�������ꂽ�u�ԁAtrue��Ԃ�
bool isKeyRelease( int keycode )
{
	// �G���[����
	if ( keycode >= MAX_KEYBUFFER_SIZE ) { return false; }
	if ( keycode < 0 ) { return false; }
	// ���ʂ�Ԃ�
	return keyReleaseBuf[ keycode ];
}
