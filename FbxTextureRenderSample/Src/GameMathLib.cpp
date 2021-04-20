
#include "GameMathLib.h"


namespace GameMath {
	Pos2 Pos2::operator+(Pos2 pos) {
		Pos2 tmp;
		tmp.p[0] = p[0] + pos.p[0];
		tmp.p[1] = p[1] + pos.p[1];
		return tmp;
	}	
	Pos2 Pos2::operator+(float arg) {
		Pos2 tmp;
		tmp.p[0] = p[0] + arg;
		tmp.p[1] = p[1] + arg;
		return tmp;
	}
	Pos2 Pos2::operator-(Pos2 pos) {
		Pos2 tmp;
		tmp.p[0] = p[0] - pos.p[0];
		tmp.p[1] = p[1] - pos.p[1];
		return tmp;
	}
	Pos2 Pos2::operator-(float arg) {
		Pos2 tmp;
		tmp.p[0] = p[0] - arg;
		tmp.p[1] = p[1] - arg;
		return tmp;
	}
	Pos2 Pos2::operator/(Pos2 pos) {
		Pos2 tmp;
		tmp.p[0] = p[0] / pos.p[0];
		tmp.p[1] = p[1] / pos.p[1];
		return tmp;
	}
	Pos2 Pos2::operator/(float arg) {
		Pos2 tmp;
		tmp.p[0] = p[0] / arg;
		tmp.p[1] = p[1] / arg;
		return tmp;
	}

	int GetRand(int min, int max) {
		std::random_device rnd;
		std::mt19937 mt(rnd());
		std::uniform_int_distribution<> randA(min, max);

		return randA(mt);
	}
	//三角関数系の処理//////////////////////////////
	////原点と角度と半径を送ると位置が返ってくる 　上を０度に時計回り
	void GetCirclePoint(float posX, float posY, float r, float  angle, float* px, float* py) {

		float rad = ((double)angle-90.0) / 180 * 3.1415;

		*px = posX + r * cos(rad);
		*py = posY + r * sin(rad);
	}
	Pos2 GetCirclePoint(float posX, float posY, float r, float  angle) {
		Pos2 pos;
		GetCirclePoint(posX, posY, r, angle, &pos.p[0], &pos.p[1]);
		return pos;
	}
	//二つの位置から角度を求める　上を０度に時計回り
	float GetAngle(Pos2 pos0, Pos2 pos1) {
		int big[2];//0 左右　//1上下
		for (int i = 0; i < 2; i++) {
			if (pos0.p[i] < pos1.p[i])
				big[i] = 0;
			else if (pos0.p[i] > pos1.p[i])
				big[i] = 1;
			else
				big[i] = 2;
		}

		float differenceX = abs((int)(pos1.p[0] - pos0.p[0]));
		float differenceY = abs((int)(pos1.p[1] - pos0.p[1]));

		float tan = differenceY / differenceX;

		float angle = atan(tan) * 180.0 / 3.1415;
		float returnAngle;
		switch (big[0]) {
		case 0:
			switch (big[1]) {
			case 0:
				return 90 + angle;
				break;
			case 1:
				return 90 - angle;
				break;
			case 2:
				return 90;
				break;
			}
			break;
		case 1:
			switch (big[1]) {
			case 0:
				return 180 + (90 - angle);
				break;
			case 1:
				return 270 + angle;
				break;
			case 2:
				return 270;
				break;
			}
			break;
		case 2:
			switch (big[1]) {
			case 0:
				return 180;
				break;
			case 1:
				return 360;
				break;
			case 2:
				return 0;
				break;
			}
			break;
		}
		//バグった時に返す
		return -1;
	}

	float GetHypotenuse(Pos2 pos0, Pos2 pos1) {
		Pos2 tmp = pos0 - pos1;
		tmp.p[0] = abs((int)tmp.p[0]);
		tmp.p[1] = abs((int)tmp.p[1]);

		return GetHypotenuse(tmp.p[0], tmp.p[1]);
	}
	//底辺と高さから斜辺を取り出す
	float GetHypotenuse(int x, int y) {
		return pow(pow(x, 2) + pow(y, 2), 0.5);
	}

	float GetCosWave(float angle) {
		return 	cos((double)angle / 180 * 3.1415);
	}
	float GetSinWave(float angle) {
		return 	sin((double)angle / 180 * 3.1415);
	}

	//移動等
	float TransformPosition1(float nowX, float endX, float speed) {
		if (nowX > endX) {
			nowX -= speed;
			if (nowX < endX)
				nowX = endX;
		}
		else if (nowX < endX) {
			nowX += speed;
			if (nowX > endX)
				nowX = endX;
		}
		return nowX;
	}
	Pos2 TransformPosition2(Pos2 nowPos, Pos2 endPos, float speed) {
		float angle = GetAngle(nowPos, endPos);

		Pos2 oldPos = nowPos;
		nowPos = GetCirclePoint(nowPos.p[0], nowPos.p[1], speed, angle);
		for (int i = 0; i < 2; i++) {
			if (oldPos.p[i] > endPos.p[i]) {
				if (nowPos.p[i] < endPos.p[i]) {
					nowPos.p[i] = endPos.p[i];
				}
			}
			else if (oldPos.p[i] < endPos.p[i]) {
				if (nowPos.p[i] > endPos.p[i]) {
					nowPos.p[i] = endPos.p[i];
				}
			}
		}
		return nowPos;
	}

	void TransformSize(float* nowSize, float endSize, float speed) {
		*nowSize = TransformSize(*nowSize, endSize, speed);
	}
	float TransformSize(float nowSize, float endSize, float speed) {
		if (nowSize > endSize) {
			nowSize -= speed;
			if (nowSize < endSize)
				nowSize = endSize;
		}
		else if (nowSize < endSize) {
			nowSize += speed;
			if (nowSize > endSize)
				nowSize = endSize;
		}
		return nowSize;
	}
	////////////////////////////////////////////////////////////
	//当たり判定の処理2D//////////////////////////////

	//中心点を縦横サイズで四角形座標配列を左上から時計回りで返してくれる
	void GetRectVectorR(Pos2 center, Pos2 size, Pos2* returnRect) {
		for (int i = 0; i < 4; i++) {
			if (i == 0 || i == 3) {
				returnRect[i].p[0] = center.p[0] - size.p[0] / 2;
			}
			else {
				returnRect[i].p[0] = center.p[0] + size.p[0] / 2;
			}

			if (i < 2) {
				returnRect[i].p[1] = center.p[1] - size.p[1] / 2;
			}
			else {
				returnRect[i].p[1] = center.p[1] + size.p[1] / 2;
			}
		}
	}
	//線上と点の当たり判定
	bool HitBar(float center, float size, float point) {
		if (center - size / 2 <= point && point <= center + size / 2) {
			return true;
		}
		return false;
	}
	//四角と点の当たり判定
	bool HitRect(Pos2 center, Pos2 size, Pos2 point) {
		bool tmp;
		for (int i = 0; i < 2; i++) {
			tmp = HitBar(center.p[i], size.p[i], point.p[i]);
			if (!tmp)
				break;
			if (i == 1)
				return true;
		}
		return false;
	}
	//四角同士の当たり判定
	bool HitRectDouble(Pos2 center1, Pos2 size1, Pos2 center2, Pos2 size2) {
		Pos2 rect1[4];
		Pos2 rect2[4];
		GetRectVectorR(center1, size1, rect1);
		GetRectVectorR(center2, size2,rect2);

		bool check = false;
		for (int i = 0; i < 4; i++) {
			if (HitRect(center1, size1, rect2[i])) {
				check = true;
			}
			if (HitRect(center2, size2, rect1[i])) {
				check = true;
			}
		}
		return check;
	}
	//円同士の当たり判定(円と点も可能
	bool HitCircle(Pos2 center1, float r1, Pos2 center2, float r2) {

		float distX, distY;//x&yの距離
		float dist;//点同士の斜辺の距離

		distX = center1.p[0] - center2.p[0];//距離を求める
		distY = center1.p[1] - center2.p[1];

		dist = sqrt(distX*distX + distY * distY);//斜辺の長さを求める　

		if (r1 + r2 > dist) {
			return true;//当たっている
		}
		return false;
	}
	//三角と点の当たり判定
	Pos2 sub_vector(Pos2 pos0, Pos2 pos1)
	{
		Pos2 ret;
		ret.p[0] = pos0.p[0] - pos1.p[0];
		ret.p[1] = pos0.p[1] - pos1.p[1];
		return ret;
	}
	bool HitTriangle(Pos2 pos0, Pos2 pos1, Pos2 pos2, Pos2 point) {
		//線上は外とみなします。

		Pos2 pos[6];
		pos[0] = sub_vector(pos1, pos0);
		pos[1] = sub_vector(point, pos1);

		pos[2] = sub_vector(pos2, pos1);
		pos[3] = sub_vector(point, pos2);

		pos[4] = sub_vector(pos0, pos2);
		pos[5] = sub_vector(point, pos0);

		//外積    Z成分だけ計算すればよいです
		double c1 = pos[0].p[0] *  pos[1].p[1] - pos[0].p[1] *  pos[1].p[0];
		double c2 = pos[2].p[0] *  pos[3].p[1] - pos[2].p[1] *  pos[3].p[0];
		double c3 = pos[4].p[0] *  pos[5].p[1] - pos[4].p[1] *  pos[5].p[0];

		if ((c1 > 0 && c2 > 0 && c3 > 0) || (c1 < 0 && c2 < 0 && c3 < 0)) {
			//三角形の内側に点がある
			return true;
		}
		//三角形の外側に点がある
		return false;
	}

	//時計回りの座標を入れないとダメ
	bool HitRectVectorR(Pos2 pos0, Pos2 pos1, Pos2 pos2, Pos2 pos3, Pos2 point) {

		//座標は時計回りにすること

		//(vx2 - vx1)*(yp - vy1) - (xp - vx1)*(vy2 - vy1)

		Pos2 pos[8];
		pos[0] = sub_vector(pos1, pos0);//辺０のベクトル
		pos[1] = sub_vector(point, pos0);

		pos[2] = sub_vector(pos2, pos1);//辺１のベクトル1
		pos[3] = sub_vector(point, pos1);

		pos[4] = sub_vector(pos3, pos2);//辺２のベクトル
		pos[5] = sub_vector(point, pos2);

		pos[6] = sub_vector(pos0, pos3);//辺３のベクトル
		pos[7] = sub_vector(point, pos3);



		//外積    Z成分だけ計算すればよいです
		double c1 = pos[0].p[0] * pos[1].p[1] - pos[1].p[0] * pos[0].p[1];

		double c2 = pos[2].p[0] * pos[3].p[1] - pos[3].p[0] * pos[2].p[1];

		double c3 = pos[4].p[0] * pos[5].p[1] - pos[5].p[0] * pos[4].p[1];

		double c4 = pos[6].p[0] * pos[7].p[1] - pos[7].p[0] * pos[6].p[1];

		if ((c1 > 0 && c2 > 0 && c3 > 0 && c4 > 0)) {
			//外側に点がある
			return true;
		}
		//内側に点がある
		return false;
	}

	//四角形の当たり判定のすべてを行う
	Pos2 HitRectAllProcess(Pos2 center, Pos2 size, Pos2 nowPoint, Pos2 oldPoint,Pos2 size2) {
		
		Pos2 rect[4];
		Pos2 dist = nowPoint - oldPoint;
		
		Pos2 pos2 = nowPoint;
		//１回の移動量をabs２以下にする
		int max = 2;
		while (!((dist.p[0] <= max && dist.p[0] >= -max) && (dist.p[1] <= max && dist.p[1] >= -max))) {
			if(dist.p[0]!=0)
				dist.p[0] /= 2;

			if (dist.p[1] != 0)
				dist.p[1] /= 2;
		}

		while (HitRectDouble(center, size, pos2, size2)==true) {
			pos2.p[0] -= dist.p[0];
			pos2.p[1] -= dist.p[1];

			if ((dist.p[0]<1 && dist.p[0] > -1) && (dist.p[1]<1 && dist.p[1] > -1)) {
				break;
			}
		}
		
		return pos2;
	}

	//当たり判定の処理３D/////////////////////////////////////////////////
	//四角と点の当たり判定
	bool HitRect3D(Pos3 center, Pos3 size, Pos3 point) {
		bool tmp;
		for (int i = 0; i < 3; i++) {
			tmp = HitBar(center.p[i], size.p[i], point.p[i]);
			if (!tmp)
				break;
			if (i == 2)
				return true;
		}
		return false;
	}
	//円同士の当たり判定(円と点も可能
	bool HitCircle3D(Pos3 center1, float r1, Pos3 center2, float r2) {
		Pos2 xy[2];
		xy[0].p[0] = center1.p[0];
		xy[0].p[1] = center1.p[1];

		xy[1].p[0] = center2.p[0];
		xy[1].p[1] = center2.p[1];

		Pos2 xz[2];
		xz[0].p[0] = center1.p[0];
		xz[0].p[1] = center1.p[2];

		xz[1].p[0] = center2.p[0];
		xz[1].p[1] = center2.p[2];

		bool tmp[2];
		tmp[0] = HitCircle(xy[0], r1, xy[1], r2);
		tmp[1] = HitCircle(xz[0], r1, xz[1], r2);

		if (tmp[0] && tmp[1]) {
			return true;
		}
		return false;
	}


	////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////

}