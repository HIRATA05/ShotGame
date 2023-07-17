/*
* 
*/


#include "DxLib.h"
#include "sfx.h"

static int hnsfx_gameover=0;
static int hnsfx_plhit=0;
static int hnsfx_enmhit=0;
static int hnsfx_plshoot=0;

void SFX_Init()
{

	hnsfx_gameover	=	LoadSoundMem( SFX_GAMEOVER );
	hnsfx_plshoot	=	LoadSoundMem( SFX_SHOOT );
	hnsfx_plhit		=	LoadSoundMem( SFX_PLAYER_EXPLOSION );
	hnsfx_enmhit	=	LoadSoundMem( SFX_ENEMY_EXPLOSION );
}
void SFX_End()
{
	DeleteSoundMem(hnsfx_gameover);
	DeleteSoundMem(hnsfx_plshoot);
	DeleteSoundMem(hnsfx_plhit);
	DeleteSoundMem(hnsfx_enmhit);

	hnsfx_gameover	=	0;//LoadSoundMem(SFX_GAMEOVER);
	hnsfx_plshoot	=	0;//LoadSoundMem(SFX_SHOOT);
	hnsfx_plhit		=	0;//LoadSoundMem(SFX_PLAYER_EXPLOSION);
	hnsfx_enmhit	=	0;//LoadSoundMem(SFX_ENEMY_EXPLOSION);

}

void sfx_GaneOver()	{ if( hnsfx_gameover ){	PlaySoundMem( hnsfx_gameover, DX_PLAYTYPE_BACK, TRUE);	}}
void sfx_PlShoot()	{ if( hnsfx_plshoot  ){	PlaySoundMem( hnsfx_plshoot,  DX_PLAYTYPE_BACK, TRUE);	}}
void sfx_PlHit()	{ if( hnsfx_plhit    ){	PlaySoundMem( hnsfx_plhit,    DX_PLAYTYPE_BACK, TRUE);	}}
void sfx_EnmHit()	{ if( hnsfx_enmhit   ){	PlaySoundMem( hnsfx_enmhit,   DX_PLAYTYPE_BACK, TRUE);	}}
