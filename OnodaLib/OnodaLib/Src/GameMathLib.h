#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define AngleToRadian(angle) (angle/180.0*3.14)
#define RadianToAngle(radian) (radian*180.0/3.14)

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <random>
namespace GameMath {
	class Pos2 {
	public:
		float p[2];//xy
		Pos2 operator+(Pos2 pos);
		Pos2 operator+(float arg);

		Pos2 operator-(Pos2 pos);
		Pos2 operator-(float arg);

		Pos2 operator/(Pos2 pos);
		Pos2 operator/(float arg);

	};
	class Pos3 {
	public:
		float p[3];//xyz
	};
	//四角判定のクラス
	class HitObject {
	private:
		int x0, x1, y0, y1;
	public:
		HitObject(int _x0, int _y0, int _x1, int _y1) {
			x0 = _x0;
			x1 = _x1;
			y0 = _y0;
			y1 = _y1;
		}
		bool Hit(int px, int py) {
			if (x0 <= px && px <= x1 && y0 <= py && py <= y1)
				return true;
			return false;
		}
	};

	float GetRand(int min, int max);
	//三角関数//////////////////////////////
	////原点と角度と半径を送ると位置が返ってくる 右回り
	void GetCirclePoint(float posX, float posY, float r, float  angle, float* px, float* py);
	Pos2 GetCirclePoint(float posX, float posY, float r, float  angle);
	Pos2 GetCirclePoint_negativeY(float posX, float posY, float r, float  angle);
	Pos2 GetCirclePoint_positiveY(float posX, float posY, float r, float  angle);
	//位置から角度を算出
	float GetAngle(Pos2 pos0, Pos2 pos1);
	//底辺と高さから斜辺を取り出す
	float GetHypotenuse(Pos2 pos0, Pos2 pos1);
	float GetHypotenuse(float x, float y);
	float GetCosWave(float angle);
	float GetSinWave(float angle);
	//移動等
	float TransformPosition1(float nowX, float endX, float speed);
	Pos2 TransformPosition2(Pos2 nowPos, Pos2 endPos, float speed);
	
	void TransformSize(float* nowSize, float endSize, float speed);
	float TransformSize(float nowSize, float endSize, float speed);
	////////////////////////////////////////////////////////////
	//当たり判定//////////////////////////////
	
	void GetRectVectorR(Pos2 center, Pos2 size, Pos2* returnRect);
	//中心点を描画軸とする四角画像の当たり判定
	bool HitBar(float center, float size, float point);
	bool HitRect(Pos2 center, Pos2 size, Pos2 point);//矩形to点
	bool HitRectDouble(Pos2 center1, Pos2 size1, Pos2 center2, Pos2 size2);//矩形to矩形
	bool HitCircle(Pos2 center1, float r1, Pos2 center2, float r2);//円to点

		//内積外積を使った高度な計算、傾いた図形でも使える
		//引数のベクトル（座標）は時計回りにおいていく
	bool HitTriangle(Pos2 pos0, Pos2 pos1, Pos2 pos2, Pos2 point);
	bool HitRectVectorR(Pos2 pos0, Pos2 pos1, Pos2 pos2, Pos2 pos3, Pos2 point);

	//座標移動をしたものまで返す四角形判定の完成系
	Pos2 HitRectAllProcess(Pos2 center, Pos2 size, Pos2 nowPoint, Pos2 oldPoint,Pos2 size2);

	//当たり判定の処理３D/////////////////////////////////////////////////
	bool HitRect3D(Pos3 center, Pos3 size, Pos3 point);
	bool HitCircle3D(Pos3 center1, float r1, Pos3 center2, float r2);
	////////////////////////////////////////////////////////////

	//移動系
	//void EaseIn(Pos2 startPos,Pos2 endPos,float speed,int nowTime,int endTime,)

	//ファイル操作//////////////////////////////


	////////////////////////////////////////////////////////////


}