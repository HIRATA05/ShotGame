/*
* �L�[�{�[�h��}�E�X�̓��͂ɑ΂���֐���p�ӂ���
*
*
*
*
*
*/
#pragma once


bool InputDevice_Init( );	//���̓f�o�C�X�֐��̏�����
bool InputDevice_End( );	//���̓f�o�C�X�֐��̏I����
bool InputDevice_Update( );	//���̓f�o�C�X�֌W�̃f�[�^�̍X�V


// �}�E�X�֌W
int	 GetMouseInputAll( );		// MOUSE_INPUT_�̏�Ԃ�Ԃ�
int	 GetMouseInputOn();			// MOUSE_INPUT_�̉����ꂽ�u�Ԃ̏�Ԃ�Ԃ�
int	 GetMouseInputRelease( );	// MOUSE_INPUT_�̗����ꂽ�u�Ԃ̏�Ԃ�Ԃ�
int  GetMousePosition( int *x, int *y );// Mouse�̈ʒu���擾����

bool isMouseInputDown( int mouse_input );	// MOUSE_INPUT_ ��������Ă�����Atrue��Ԃ�
bool isMouseInputOn( int mouse_input );		// MOUSE_INPUT_ �������ꂽ�u�ԁAtrue��Ԃ�
bool isMouseInputRelease( int mouse_input );// MOUSE_INPUT_ �������ꂽ�u�ԁAtrue��Ԃ�

// �L�[�{�[�h�֌W
bool isKeyDown( int keycode );		// keycode�Ŏ������L�[�{�[�h��������Ă�����Atrue��Ԃ�
bool isKeyOn( int keycode );		// keycode�Ŏ������L�[�{�[�h�������ꂽ�u�ԁAtrue��Ԃ�
bool isKeyRelease( int keycode );	// keycode�Ŏ������L�[�{�[�h�������ꂽ�u�ԁAtrue��Ԃ�

