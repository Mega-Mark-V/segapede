#ifndef _PROTO_H_
#define _PROTO_H_

/* ANIM.C */
void AnimateCreature(CreaturePtr t);
void BeginAnimation(CreaturePtr t, int activity);

/* DRAW.C */
void DrawCreatures(void);

/* IO.C */
void DoIO(void);

/* LOGO.C */
int SegaLogo(void);

/* MATH.C */
short atan2(long x, long y);

/* PLAYER.C */
void InitPlayer(void);
void OperatePlayer(void);

/* SCREEN.H */
void UpdateScreen(short x, short y);
void InitScreen(void);

/* SEGAPEDE.C */
void main(void);
void Game(void);
int GameCore(void);
void InitGameVariables(void);

/* SEGMENTS.C */
void InitSegments(void);
void OperateSegments(void);
int DoDamping(int v1, int v2);
void SetSegmentShape(int i, int j);
int GetElevation(int h, int v);
void FollowTerrain(CreaturePtr c);

/* STARTUP.C */
void start(void);
void InitVDP(void);
void VBLHandler(void);
void HBLHandler(void);
void ErrorHandler(void);
void WaitTOF(void);
void ReadHWControllers(void);

/* TITLE.C */
int Title(void);

/* UTILS.C */
void InterpolateColor(short target, short time, short duration, short *color);
void InterpolatePalette(short target, short time, short duration, short *palette);
void InterpolateFullPalette(short target, short time, short duration, short *palette);
void Interpolate2Palettes(short *target, short time, short duration, short *palette);
void Interpolate2FullPalettes(short *target, short time, short duration, short *palette);
void FadeIn(short *palette, short time);
void FadeOut(short *palette, short time);

#endif
