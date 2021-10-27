#include "pch.h"
#include "CAdminControl.h"
#include <math.h>

#define PI 3.141592

CAdminControl::CAdminControl()
{
}

CAdminControl::~CAdminControl()
{
	shape_head->FreeShape();
}

void CAdminControl::Draw()
{
	CShape* nowShape = shape_head;

	while (nowShape != NULL)
	{
		//点を打つ
		glColor3f(1.0, 1.0, 1.0);
		glPointSize(10);
		glBegin(GL_POINTS);

		CVertex* nowVertex = nowShape->GetV();

		while (nowVertex != NULL)
		{
			glVertex2f(nowVertex->GetX(), nowVertex->GetY());
			nowVertex = nowVertex->GetNext();
		}
		glEnd();
		
		//閉じてたらループ，最新で閉じてない奴は線で描写
		if (nowShape != shape_head)
		{
			glColor3f(1.0, 1.0, 1.0);
			glPointSize(10);
			glBegin(GL_LINE_LOOP);

			nowVertex = nowShape->GetV();

			while (nowVertex != NULL)
			{
				glVertex2f(nowVertex->GetX(), nowVertex->GetY());
				nowVertex = nowVertex->GetNext();
			}
			glEnd();
		}
		else
		{
			glColor3f(1.0, 1.0, 1.0);
			glPointSize(10);
			glBegin(GL_LINE_STRIP);

			nowVertex = nowShape->GetV();

			while (nowVertex != NULL)
			{
				glVertex2f(nowVertex->GetX(), nowVertex->GetY());
				nowVertex = nowVertex->GetNext();
			}
			glEnd();
		}

		nowShape = nowShape->GetSNext();
	}
}

void CAdminControl::inheritShape() {
	CShape* newShape = new CShape();
	newShape->SetSNext(shape_head);
	shape_head = newShape;
}

float CAdminControl::Distance(CVertex* s, float x, float y)
{
	float d,X,Y;

	X = x - s->GetX();
	Y = y - s->GetY();

	d = sqrt(pow(X, 2) + pow(Y, 2));

	return d;
}

void CAdminControl::CreateShape(float x, float y)
{
	//1-1のみ
	if (shape_head == NULL)
	{
		inheritShape();
	}
	
	if (shape_head->CountVertex() < 3)//三点未満のとき
	{
		if (shape_head->CountVertex() == 0 )//一点目
		{
			if (!InOutJug(x, y))//内外判定ヨシ
			{
				shape_head->inheritVertex(x, y);
			}
		}
		else if (!AltJudCross(x, y))//他交差してない
		{
			shape_head->inheritVertex(x, y);
		}
	}
	else if (Distance(shape_head->GetV(), x, y) <= 0.07)//閉じる時
	{
		float MrX = shape_head->GetV()->GetX();
		float MsY = shape_head->GetV()->GetY();
		if (!JudCross(MrX,MsY) && !AltJudCross(MrX,MsY) && !AltInOutJug())//自交差，他交差，内包なし
		{
			inheritShape();
		}
		else if (!JudCross(x, y) && !AltJudCross(x, y))//自交差してない，他交差してない
		{
			shape_head->inheritVertex(x, y);
		}
	}
	else//閉じないとき
	{
		if (!JudCross(x, y) && !AltJudCross(x, y))
		{
			shape_head->inheritVertex(x, y);
		}
	}
}

//内積計算
float CAdminControl::nisk(float ax, float ay, float bx, float by)
{
	return ax * bx + ay * by;
}

//外積計算
float CAdminControl::gisk(float ax, float ay, float bx, float by)
{
	return ax * by - bx * ay;
}

//自交差
bool CAdminControl::JudCross(float Bex, float Bey)
{

	CVertex* As = shape_head->GetV();
	CVertex* Ae = As->GetNext();

	CVertex* Bs = Ae->GetNext();
	while (Bs->GetNext() != NULL)
	{
		Bs = Bs->GetNext();
	}

	if (As->GetX() == Bex && As->GetY() == Bey)
	{
		As = Ae;
		Ae = Ae->GetNext();
	}

	while (Ae != Bs)
	{
		if (LastJudgment(Bex,Bey,As,Ae,Bs))
		{
			return true;
		}
		As = Ae;
		Ae = Ae->GetNext();

	}
	return false;
}

//他交差
bool CAdminControl::AltJudCross(float Bex, float Bey)
{
	CShape* nowS;

	if (shape_head->GetSNext() != NULL)//形状が１つ以上ある
	{
		nowS = shape_head->GetSNext();
	}
	else
	{
		return false;
	}

	CVertex* As = nowS->GetV();
	CVertex* Ae = As->GetNext();

	CVertex* Bs = shape_head->GetV();
	while (Bs->GetNext() != NULL)
	{
		Bs = Bs->GetNext();
	}

	while (nowS != NULL)
	{
		As = nowS->GetV();
		Ae = As->GetNext();

		while (Ae != NULL)
		{
			if (LastJudgment(Bex, Bey, As, Ae, Bs))
			{
				return true;
			}
			As = Ae;
			Ae = Ae->GetNext();
		}
		Ae = nowS->GetV();
		if (LastJudgment(Bex, Bey, As, Ae, Bs))
		{
			return true;
		}
		nowS = nowS->GetSNext();
	}
	return false;
}


bool CAdminControl::LastJudgment(float Bex, float Bey, CVertex* As, CVertex* Ae, CVertex* Bs)
{
	float c1, c2, c3, c4;
	CVertex a, b, a1, b1, a2, b2;

	a.SetXY(Ae->GetX() - As->GetX(), Ae->GetY() - As->GetY());
	b.SetXY(Bex - Bs->GetX(), Bey - Bs->GetY());

	a1.SetXY(Bs->GetX() - As->GetX(), Bs->GetY() - As->GetY());
	b1.SetXY(As->GetX() - Bs->GetX(), As->GetY() - Bs->GetY());

	a2.SetXY(Bex - As->GetX(), Bey - As->GetY());
	b2.SetXY(Ae->GetX() - Bs->GetX(), Ae->GetY() - Bs->GetY());

	c1 = a.GetX() * a1.GetY() - a1.GetX() * a.GetY();
	c2 = a.GetX() * a2.GetY() - a2.GetX() * a.GetY();
	c3 = b.GetX() * b1.GetY() - b1.GetX() * b.GetY();
	c4 = b.GetX() * b2.GetY() - b2.GetX() * b.GetY();
	if (c1 * c2 <= 0 && c3 * c4 <= 0)
	{
		return true;
	}

	return false;
}

//内外判定
bool CAdminControl::InOutJug(float x, float y)
{
	CShape* nowS;
	
	if (shape_head->GetSNext() != NULL)//形状が１つ以上ある
	{
		nowS = shape_head->GetSNext();
	}
	else
	{
		return false;
	}
	
	CVertex* As = nowS->GetV();
	CVertex* Ae = As->GetNext();
	float ax, ay, bx, by;

	float totalAng = 0;
	float stockAng = 0;

	while (nowS != NULL)
	{
		As = nowS->GetV();
		Ae = As->GetNext();
		totalAng = 0;

		while (As != NULL)
		{
			stockAng = 0;

			ax = As->GetX() - x;
			ay = As->GetY() - y;
			bx = Ae->GetX() - x;
			by = Ae->GetY() - y;

			//角の計算
			stockAng = atan2(gisk(ax, ay, bx, by),nisk(ax, ay, bx, by));

			As = As->GetNext();
			Ae = Ae->GetNext();

			totalAng = totalAng + stockAng;

			if (Ae == NULL)
			{
				Ae = nowS->GetV();
			}
		}

		if ( 2 * PI - fabs(totalAng) <= 0.0001)
		{
			return true;
		}

		nowS = nowS->GetSNext();
	}
	return false;
}

//内包判定
bool CAdminControl::AltInOutJug()
{
	if (shape_head->GetSNext() == NULL)//形状が１つ以上ある
	{
		return false;
	}

	CVertex* As = shape_head->GetV();
	CVertex* Ae = As->GetNext();

	CShape* nowS = shape_head->GetSNext();
	CVertex* attV = shape_head->GetSNext()->GetV();

	float ax, ay, bx, by;

	float totalAng = 0;
	float stockAng = 0;

	while (nowS != NULL)
	{
		As = shape_head->GetV();
		Ae = As->GetNext();
		totalAng = 0;

		while (As != NULL)
		{
			stockAng = 0;

			ax = As->GetX() - attV->GetX();
			ay = As->GetY() - attV->GetY();
			bx = Ae->GetX() - attV->GetX();
			by = Ae->GetY() - attV->GetY();

			//角の計算
			stockAng = atan2(gisk(ax, ay, bx, by), nisk(ax, ay, bx, by));

			As = As->GetNext();
			Ae = Ae->GetNext();

			totalAng = totalAng + stockAng;

			if (Ae == NULL)
			{
				Ae = shape_head->GetV();
			}
		}

		if (2 * PI - fabs(totalAng) <= 0.0001)
		{
			return true;
		}

		nowS = nowS->GetSNext();
	}
	return false;
}



