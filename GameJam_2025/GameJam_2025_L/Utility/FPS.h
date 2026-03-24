#pragma once
#ifndef FPS_H_
#define FPS_H_
#include"DxLib.h"
#include<math.h>

class FPS
{
private:
	int count;
	//FPSの計測と表示を行うローカル変数の宣言
	LONGLONG nowTime;
	LONGLONG oldTime;
	LONGLONG fpsCheckTime;
	double deltaTime;
	int fpsCounter;
	int Fps;
public:

	int Time;


	FPS();
	~FPS();

	void Wait_Fanc();//FPS固定処理の関数
	void Reset_FPS();//ループ前にFPS計測を初期化する関数
	void Keisoku_FPS();//FPSの計測する関数
	void Display_FPS()const;//計測したFPSを表示する関数

	void FPSUpdate();
};

#endif