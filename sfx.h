#pragma once
/*
* 
* Sound Effect
* 
* 
* 
*/


/* https://taira-komori.jpn.org/arms01.html */
#define	SFXDIR	"sfx/"
#define	SFX_GAMEOVER	SFXDIR ## "powerdown07.mp3"
#define	SFX_SHOOT		SFXDIR ## "select09.mp3"
#define	SFX_ENEMY_EXPLOSION	SFXDIR ## "game_explosion9.mp3"
#define	SFX_PLAYER_EXPLOSION	SFXDIR ## "pyo2.mp3"

void SFX_Init();
void SFX_End();
void sfx_GaneOver();
void sfx_PlShoot();
void sfx_PlHit();
void sfx_EnmHit();