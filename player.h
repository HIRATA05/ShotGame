#pragma once
/*
* Player�Ɋւ��鏈��
* 
* 
* 
* 
* 
*/

enum {
	PLSTATE_INIT = 0,		// �������
	PLSTATE_NORMAL,			// �ʏ���
	PLSTATE_HIT,			// ���������u��
	PLSTATE_EXPLOSION,		// ������
	PLSTATE_MUTEKI,			// �񕜏��
	PLSTATE_GAMEOVER,		// �Q�[���I�[�o�[��

};

int Player_Init( void );
int Player_Dsp( void );
int Player_End( void );


void SetPlayerState( int state );			/* player�̏�Ԃ�ݒ肷�� */
int GetPlayerState( );						/* player�̏�Ԃ��擾���� */
void GetPlayerPosition( int* x, int* y );	/* player�̈ʒu���擾���� */
void GetPlayerOldPosition(int* x, int* y);
void GetPlayerSize( int* size );			/* player�̑傫�����擾���� */
void ClearSlashFlag( void );
