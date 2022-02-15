#include "pch.h"
#include "CAdminControl.h"
#include <math.h>

#define PI 3.14159265359

CAdminControl::CAdminControl()
{
}

CAdminControl::~CAdminControl()
{
	shape_head->FreeShape();
}

//�`�ʃ��[�h
void CAdminControl::Draw()
{
	if (AxisFlag)//XYZ���̕`�ʁC�^�Ȃ�`��
	{
		DrawAxis();
	}

	CShape* nowShape = shape_head;

	CVertex* nowVertex;

	while (nowShape != NULL)
	{
		//���Ă��烋�[�v�C�ŐV�ŕ��ĂȂ��z�͐��ŕ`��
		if (nowShape != shape_head)
		{
			glColor3f(1.0, 1.0, 0.0);
			glLineWidth(3);
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
			glColor3f(0.0, 1.0, 1.0);
			glLineWidth(3);
			glBegin(GL_LINE_STRIP);

			nowVertex = nowShape->GetV();

			while (nowVertex != NULL)
			{
				glVertex2f(nowVertex->GetX(), nowVertex->GetY());
				nowVertex = nowVertex->GetNext();
			}
			glEnd();
		}

		//�_��ł�
		glColor3f(1.0, 1.0, 1.0);
		glPointSize(10);
		glBegin(GL_POINTS);

		nowVertex = nowShape->GetV();

		while (nowVertex != NULL)
		{
			glVertex2f(nowVertex->GetX(), nowVertex->GetY());
			nowVertex = nowVertex->GetNext();
		}
		glEnd();

		nowShape = nowShape->GetSNext();
	}

	//��������I�������_��`��Ȃǂ�����Ε`��
	if (select_vertex != NULL)
	{
		//�_�̂Ƃ�(�ӂ̈�_��)
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_POINTS);
		glVertex2f(select_vertex->GetX(), select_vertex->GetY());
		glEnd();

		//�ӂ̂Ƃ�
		if (select_vertexNext != NULL)
		{
			//��_��
			glColor3f(1.0, 0.0, 1.0);
			glBegin(GL_POINTS);
			glVertex2f(select_vertex->GetX(), select_vertex->GetY());
			glVertex2f(select_vertexNext->GetX(), select_vertexNext->GetY());
			glEnd();

			//��(��)
			glColor3f(1.0, 0.0, 1.0);
			glLineWidth(3);
			glBegin(GL_LINES);
			glVertex2f(select_vertex->GetX(), select_vertex->GetY());
			glVertex2f(select_vertexNext->GetX(), select_vertexNext->GetY());
			glEnd();
		}
	}
	
	//�`�̂Ƃ�
	if (select_shape != NULL)
	{
		nowVertex = select_shape->GetV();
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_POINTS);
		while (nowVertex != NULL)
		{
			glVertex2f(nowVertex->GetX(), nowVertex->GetY());
			nowVertex = nowVertex->GetNext();
		}
		glEnd();

		nowVertex = select_shape->GetV();
		glColor3f(1.0, 0.0, 1.0);
		glLineWidth(3);
		glBegin(GL_LINE_LOOP);
		while (nowVertex != NULL)
		{
			glVertex2f(nowVertex->GetX(), nowVertex->GetY());
			nowVertex = nowVertex->GetNext();
		}
		glEnd();
	}

	//��_�̕`��
	if (BasePointFlag)
	{
		glColor3f(0.4, 1.0, 0.7);
		glPointSize(10);
		glBegin(GL_POINTS);

		glVertex2f(basePointX, basePointY);

		glEnd();
	}

	//�`�ɐF��h��
	if (ShapePaintFlag)
	{
		ShapePaint();
	}

	if (ModeFlag == 2)
	{
		solidZline();
	}

}

//�V����shape�ֈڂ�
void CAdminControl::inheritShape() {
	CShape* newShape = new CShape();
	newShape->SetSNext(shape_head);
	shape_head = newShape;
}

//�_�Ɠ_(x,y)�̋���
float CAdminControl::Distance(CVertex* s, float x, float y)
{
	float d,X,Y;

	X = x - s->GetX();
	Y = y - s->GetY();

	d = sqrt(pow(X, 2) + pow(Y, 2));

	return d;
}

//�_(x,y)�Ɠ_(x,y)�̋���
float CAdminControl::Distance(float vx, float vy, float x, float y)
{
	float d, X, Y;

	X = x - vx;
	Y = y - vy;

	d = sqrt(pow(X, 2) + pow(Y, 2));

	return d;
}

//
float CAdminControl::Distance(float nx, float ny, float ex, float ey, float x, float y)
{
	float d = 1,s;
	CVertex ba;
	CVertex bp;
	CVertex ap;

	ba.SetXY(nx - ex, ny - ey);
	bp.SetXY(x - ex, y - ey);
	ap.SetXY(x- nx,y-ny);

	s = nisk(bp.GetX(), bp.GetY(), ba.GetX(), ba.GetY()) / nisk(ba.GetX(), ba.GetY(), ba.GetX(), ba.GetY());

	if (s <= 1 && s >= 0)
	{
		d = fabs(gisk(ap.GetX(), ap.GetY(), ba.GetX(), ba.GetY()) / Distance(ex, ey, nx, ny));
	}
	
	return d;
}

//�`�ʃ��[�h�̎��C�_��łĂ邩�̏������s��
void CAdminControl::CreateShape(float x, float y)
{
	if (ModeFlag == 0)//�`�惂�[�h
	{
		if (shape_head == NULL)//1-1�̂�
		{
			inheritShape();
		}
		else if(shape_head->GetV() != NULL)
		{
			CVertex* nowVertex = shape_head->GetV();
			while (nowVertex->GetNext() != NULL)
			{
				nowVertex = nowVertex->GetNext();
			}
			if (Distance(nowVertex, x, y) == 0)
			{
				return;
			}
		}
		AllShape = false;

		//�O�̓_��x,y�����������ׂ�
		
		
		if (shape_head->CountVertex() < 3)//�O�_�����̂Ƃ�
		{
			if (shape_head->CountVertex() == 0)//��_��
			{
				if (!InOutJug(x, y))//���O���胈�V
				{
					shape_head->inheritVertex(x, y);
				}
			}
			else if (!AltJudCross(x, y))//���������ĂȂ�
			{
				shape_head->inheritVertex(x, y);
			}
		}
		else if (Distance(shape_head->GetV(), x, y) <= 0.1)//���鎞
		{
			float MrX = shape_head->GetV()->GetX();
			float MsY = shape_head->GetV()->GetY();
			if (!JudCross(MrX, MsY) && !AltJudCross(MrX, MsY) && !AltInOutJug())//�������C�������C����Ȃ�
			{
				inheritShape();
				AllShape = true;
			}
			else if (!JudCross(x, y) && !AltJudCross(x, y))//���������ĂȂ��C���������ĂȂ�
			{
				shape_head->inheritVertex(x, y);
			}
		}
		else//���Ȃ��Ƃ�
		{
			if (!JudCross(x, y) && !AltJudCross(x, y))
			{
				shape_head->inheritVertex(x, y);
			}
		}
	}
}

//���όv�Z
float CAdminControl::nisk(float ax, float ay, float bx, float by)
{
	return ax * bx + ay * by;
}

//�O�όv�Z
float CAdminControl::gisk(float ax, float ay, float bx, float by)
{
	return ax * by - bx * ay;
}

//������
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

//�_�̎������F�o�O�^�f
bool CAdminControl::JudCross(float vx, float vy, CVertex* preV, CShape* nowS)
{
	CVertex* As = preV;

	for (int i = 0; i < 2; i++)
	{
		if (As->GetNext() != NULL)
		{
			As = As->GetNext();
		}
		else
		{
			As = nowS->GetV();
		}
	}
	CVertex* Ae = As->GetNext();
	if (Ae == NULL)
	{
		Ae = nowS->GetV();
	}

	CVertex* Bs = preV;
	CVertex* Be = Bs->GetNext();

	if (Ae == Bs)
	{
		return false;
	}

	while (Ae != Bs)
	{
		if (LastJudgment(vx, vy, As, Ae, Bs))
		{
			return true;
		}
		As = Ae;
		Ae = Ae->GetNext();
		if (Ae == NULL)
		{
			Ae = nowS->GetV();
		}
	}

	//��{��
	As = preV;
	for (int i = 0; i < 3; i++)
	{
		if (As->GetNext() != NULL)
		{
			As = As->GetNext();
		}
		else
		{
			As = nowS->GetV();
		}
	}
	Ae = As->GetNext();
	if (Ae == NULL)
	{
		Ae = nowS->GetV();
	}

	Be = preV;
	for (int i = 0; i < 2; i++)
	{
		if (Be->GetNext() != NULL)
		{
			Be = Be->GetNext();
		}
		else
		{
			Be = nowS->GetV();
		}
	}
	Bs = preV->GetNext();
	if (Bs == NULL) 
	{
		Bs = nowS->GetV();
	}

	while (Ae != Bs)
	{
		if (LastJudgment(vx, vy, As, Ae, Be))
		{
			return true;
		}
		As = Ae;
		Ae = Ae->GetNext();
		if (Ae == NULL)
		{
			Ae = nowS->GetV();
		}
	}
	return false;
}

//������
bool CAdminControl::AltJudCross(float Bex, float Bey)
{
	CShape* nowS;

	if (shape_head->GetSNext() != NULL)//�`�󂪂P�ȏ゠��
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

//�ړ��_�̑�����
bool CAdminControl::AltJudCross(float vx, float vy, CVertex* preV, CShape* nowS)
{
	CShape* nowShape = shape_head;
	if (nowShape->GetV() == NULL)
	{
		nowShape = nowShape->GetSNext();
	}
	if (nowS == nowShape)
	{
		nowShape = nowShape->GetSNext();
	}
	if (nowShape == NULL)
	{
		return true;
	}

	CVertex* As = nowShape->GetV();
	CVertex* Ae = As->GetNext();

	CVertex* Bs = preV;
	//CVertex* Be = Bs->GetNext();

	while (nowShape != NULL)
	{
		As = nowShape->GetV();
		Ae = As->GetNext();

		while (Ae != NULL)
		{
			if (LastJudgment(vx, vy, As, Ae, Bs))
			{
				return false;
			}
			As = Ae;
			Ae = Ae->GetNext();
		}

		Ae = nowShape->GetV();
		if (LastJudgment(vx, vy, As, Ae, Bs))
		{
			return false;
		}
		
		nowShape = nowShape->GetSNext();

		if (nowS == nowShape)
		{
			nowShape = nowShape->GetSNext();
		}
	}

	//��{��
	nowShape = shape_head;
	if (nowShape->GetV() == NULL)
	{
		nowShape = nowShape->GetSNext();
	}
	if (nowS == nowShape)
	{
		nowShape = nowShape->GetSNext();
	}

	As = nowShape->GetV();
	Ae = As->GetNext();

	Bs = preV->GetNext();
	if (Bs == NULL)
	{
		Bs = nowS->GetV();
	}
	CVertex* Be = Bs->GetNext();
	if (Be == NULL)
	{
		Be = nowS->GetV();
	}
	
	while (nowShape != NULL)
	{
		As = nowShape->GetV();
		Ae = As->GetNext();

		while (Ae != NULL)
		{
			if (LastJudgment(vx, vy, As, Ae, Be))
			{
				return false;
			}
			As = Ae;
			Ae = Ae->GetNext();
		}

		Ae = nowShape->GetV();
		if (LastJudgment(vx, vy, As, Ae, Be))
		{
			return false;
		}

		nowShape = nowShape->GetSNext();
		if (nowS == nowShape)
		{
			nowShape = nowShape->GetSNext();
		}
	}
	return true;
}

//�`�̑�����
bool CAdminControl::AltJugCross(CShape* nowMoveShape)
{
	CShape* nowShape = shape_head;
	if (nowShape->GetV() == NULL)
	{
		nowShape = nowShape->GetSNext();
	}
	if (nowShape == nowMoveShape)
	{
		nowShape = nowShape->GetSNext();
	}
	if (nowShape == NULL)
	{
		return false;
	}

	CVertex* As = nowShape->GetV();
	CVertex* Ae = As->GetNext();

	CVertex* Bs = nowMoveShape->GetV();
	CVertex* Be = Bs->GetNext();
	float Bex = Be->GetX();
	float Bey = Be->GetY();

	while (nowShape != NULL)
	{
		As = nowShape->GetV();
		Ae = As->GetNext();

		while (Ae != NULL)
		{
			Bs = nowMoveShape->GetV();
			Be = Bs->GetNext();
			Bex = Be->GetX();
			Bey = Be->GetY();

			while (Be != NULL)
			{
				Bex = Be->GetX();
				Bey = Be->GetY();

				if (LastJudgment(Bex, Bey, As, Ae, Bs))
				{
					return true;
				}
				Bs = Be;
				Be = Be->GetNext();
			}

			Be = nowMoveShape->GetV();
			Bex = Be->GetX();
			Bey = Be->GetY();
			if (LastJudgment(Bex, Bey, As, Ae, Bs))
			{
				return true;
			}

			As = Ae;
			Ae = Ae->GetNext();
		}
		Ae = nowShape->GetV();

		Bs = nowMoveShape->GetV();
		Be = Bs->GetNext();
		Bex = Be->GetX();
		Bey = Be->GetY();

		while (Be != NULL)
		{
			Bex = Be->GetX();
			Bey = Be->GetY();

			if (LastJudgment(Bex, Bey, As, Ae, Bs))
			{
				return true;
			}
			Bs = Be;
			Be = Be->GetNext();
		}

		Be = nowMoveShape->GetV();
		Bex = Be->GetX();
		Bey = Be->GetY();
		if (LastJudgment(Bex, Bey, As, Ae, Bs))
		{
			return true;
		}

		nowShape = nowShape->GetSNext();

		if (nowShape == nowMoveShape)
		{
			nowShape = nowShape->GetSNext();
		}
	}
	return false;
}

//��������
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

//���O����
bool CAdminControl::InOutJug(float x, float y)
{
	CShape* nowS;
	
	if (shape_head->GetSNext() != NULL)//�`�󂪂P�ȏ゠��
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

			//�p�̌v�Z
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

//���_�폜�̓��O����
bool CAdminControl::InOutJug(CVertex* preVertex, CVertex* nowVertex, CVertex* nextVertex)
{
	CShape* nowShape = shape_head;
	if (nowShape->GetV() == NULL)
	{
		nowShape = nowShape->GetSNext();
	}

	float stockAng = 0;
	float totalAng = 0;

	CVertex* attV = nowShape->GetV();

	while (nowShape != NULL)
	{
		attV = nowShape->GetV();

		while (attV != NULL)
		{
			if (attV == preVertex || attV == nowVertex || attV == nextVertex)
			{
				attV = attV->GetNext();
			}
			else
			{
				stockAng = 0;
				totalAng = 0;

				float x = attV->GetX();
				float y = attV->GetY();

				float preX = preVertex->GetX() - x;
				float preY = preVertex->GetY() - y;
				float nowX = nowVertex->GetX() - x;
				float nowY = nowVertex->GetY() - y;
				float nextX = nextVertex->GetX() - x;
				float nextY = nextVertex->GetY() - y;

				//�p�̌v�Z
				stockAng = atan2(gisk(preX, preY, nowX, nowY), nisk(preX, preY, nowX, nowY));
				totalAng = totalAng + stockAng;
				stockAng = atan2(gisk(nowX, nowY, nextX, nextY), nisk(nowX, nowY, nextX, nextY));
				totalAng = totalAng + stockAng;
				stockAng = atan2(gisk(nextX, nextY, preX, preY), nisk(nextX, nextY, preX, preY));
				totalAng = totalAng + stockAng;

				if (2 * PI - fabs(totalAng) <= 0.0001)
				{
					return true;
				}
				attV = attV->GetNext();
			}
		}
		nowShape = nowShape->GetSNext();
	}
	
	return false;
}

//�F�h��̓��O����
bool CAdminControl::InOutJugTriangle(CShape* targetS,CVertex* cV, CVertex* cVN, CVertex* cVNN)
{
	float stockAng = 0;
	float totalAng = 0;

	CVertex* attV = targetS->GetV();

	while (attV != NULL)
	{
		if (attV == cV || attV == cVN || attV == cVNN)
		{
			attV = attV->GetNext();
		}
		else
		{
			stockAng = 0;
			totalAng = 0;

			float x = attV->GetX();
			float y = attV->GetY();

			float preX = cV->GetX() - x;
			float preY = cV->GetY() - y;
			float nowX = cVN->GetX() - x;
			float nowY = cVN->GetY() - y;
			float nextX = cVNN->GetX() - x;
			float nextY = cVNN->GetY() - y;

			//�p�̌v�Z
			stockAng = atan2(gisk(preX, preY, nowX, nowY), nisk(preX, preY, nowX, nowY));
			totalAng = totalAng + stockAng;
			stockAng = atan2(gisk(nowX, nowY, nextX, nextY), nisk(nowX, nowY, nextX, nextY));
			totalAng = totalAng + stockAng;
			stockAng = atan2(gisk(nextX, nextY, preX, preY), nisk(nextX, nextY, preX, preY));
			totalAng = totalAng + stockAng;

			if (2 * PI - fabs(totalAng) <= 0.0001)
			{
				return true;
			}
			attV = attV->GetNext();
		}
	}

	return false;
}

//�����
bool CAdminControl::AltInOutJug()
{
	if (shape_head->GetSNext() == NULL)//�`�󂪂P�ȏ゠��
	{
		return false;
	}

	CVertex* As = shape_head->GetV();
	CVertex* Ae = As->GetNext();

	CShape* nowS = shape_head->GetSNext();
	CVertex* attV;

	float ax, ay, bx, by;

	float totalAng = 0;
	float stockAng = 0;

	while (nowS != NULL)
	{
		attV = nowS->GetV();

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

			//�p�̌v�Z
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

//�����(��_���`�ɓ���Ă��邩)
bool CAdminControl::AltInOutJug(CShape* nowShape, float x, float y)
{
	CVertex* As = nowShape->GetV();
	CVertex* Ae = As->GetNext();

	float ax, ay, bx, by;

	float totalAng = 0;
	float stockAng = 0;

	while (As != NULL)
	{
		stockAng = 0;

		ax = As->GetX() - x;
		ay = As->GetY() - y;
		bx = Ae->GetX() - x;
		by = Ae->GetY() - y;

		//�p�̌v�Z
		stockAng = atan2(gisk(ax, ay, bx, by), nisk(ax, ay, bx, by));

		As = As->GetNext();
		Ae = Ae->GetNext();

		totalAng = totalAng + stockAng;

		if (Ae == NULL)
		{
			Ae = nowShape->GetV();
		}
	}
	if (2 * PI - fabs(totalAng) <= 0.0001)
	{
		return true;
	}
	return false;
}

//�ړ��_�̓����
bool CAdminControl::AltInOutJug(CShape* nowS)
{
	CVertex* As = nowS->GetV();
	CVertex* Ae = As->GetNext();

	CShape* nowShape = shape_head;
	if (nowShape->GetV() == NULL)
	{
		nowShape = shape_head->GetSNext();
	}
	if (nowShape == nowS)
	{
		nowShape = nowShape->GetSNext();
	}
	if (nowShape == NULL)
	{
		return false;
	}
	CVertex* attV;

	float ax, ay, bx, by;

	float totalAng = 0;
	float stockAng = 0;

	while (nowShape != NULL)
	{
		attV = nowShape->GetV();

		As = nowS->GetV();
		Ae = As->GetNext();
		totalAng = 0;

		while (As != NULL)
		{
			stockAng = 0;

			ax = As->GetX() - attV->GetX();
			ay = As->GetY() - attV->GetY();
			bx = Ae->GetX() - attV->GetX();
			by = Ae->GetY() - attV->GetY();

			//�p�̌v�Z
			stockAng = atan2(gisk(ax, ay, bx, by), nisk(ax, ay, bx, by));

			As = As->GetNext();
			Ae = Ae->GetNext();

			totalAng = totalAng + stockAng;

			if (Ae == NULL)
			{
				Ae = nowS->GetV();
			}
		}

		if (2 * PI - fabs(totalAng) <= 0.0001)
		{
			return true;
		}
		nowShape = nowShape->GetSNext();
		if (nowS == nowShape)
		{
			nowShape = nowShape->GetSNext();
		}
	}
	return false;
}

//����ꂽ�Ƃ�
bool CAdminControl::AltOutInJug(CShape* nowMoveShape)
{
	CShape* nowShape = shape_head;
	if (nowShape->GetV() == NULL)
	{
		nowShape = shape_head->GetSNext();
	}
	if (nowShape == nowMoveShape)
	{
		nowShape = nowShape->GetSNext();
	}
	if (nowShape == NULL)
	{
		return false;
	}

	CVertex* As = nowShape->GetV();
	CVertex* Ae = As->GetNext();
	
	CVertex* attV;

	float ax, ay, bx, by;

	float totalAng = 0;
	float stockAng = 0;

	while (nowShape != NULL)
	{
		attV = nowMoveShape->GetV();

		As = nowShape->GetV();
		Ae = As->GetNext();
		totalAng = 0;

		while (As != NULL)
		{
			stockAng = 0;

			ax = As->GetX() - attV->GetX();
			ay = As->GetY() - attV->GetY();
			bx = Ae->GetX() - attV->GetX();
			by = Ae->GetY() - attV->GetY();

			//�p�̌v�Z
			stockAng = atan2(gisk(ax, ay, bx, by), nisk(ax, ay, bx, by));

			As = As->GetNext();
			Ae = Ae->GetNext();

			totalAng = totalAng + stockAng;

			if (Ae == NULL)
			{
				Ae = nowShape->GetV();
			}
		}

		if (2 * PI - fabs(totalAng) <= 0.0001)
		{
			return true;
		}

		nowShape = nowShape->GetSNext();
		if (nowMoveShape == nowShape)
		{
			nowShape = nowShape->GetSNext();
		}
	}
	return false;
}

//XYZ���̕`��
void CAdminControl::DrawAxis()
{
	glBegin(GL_LINES);

	//X�� R
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);

	//Y�� G
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	
	//Z�� B
 	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -1.0);
	glVertex3f(0.0, 0.0, 1.0);

	glEnd();
}

//�ҏW���[�h
void CAdminControl::SelectPosition(float x, float y)
{
	CShape* nowShape = shape_head;
	CVertex* nowVertex = NULL;

	preSelect_shape = select_shape;
	preSelect_vertex = select_vertex;
	preSelect_vertexNext = select_vertexNext;

	//�_������(�_,NULL,NULL)
	while (nowShape != NULL)
	{
		nowVertex = nowShape->GetV();
		while (nowVertex != NULL)
		{
			if (Distance(nowVertex->GetX(), nowVertex->GetY(), x, y) <= 0.03)
			{
				select_vertex = nowVertex;
				select_shape = NULL;
				select_vertexNext = NULL;
				NowSelect(1);
				return;
			}
			nowVertex = nowVertex->GetNext();
		}
		nowShape = nowShape->GetSNext();
	}

	nowShape = shape_head;

	//�_�Ɛ�����(�_,�_,NULL)
	while (nowShape != NULL)
	{
		nowVertex = nowShape->GetV();
		float ex, ey;
		while (nowVertex != NULL)
		{
			if (nowVertex->GetNext() != NULL)
			{
				ex = nowVertex->GetNext()->GetX();
				ey = nowVertex->GetNext()->GetY();
			}
			else
			{
				ex = nowShape->GetV()->GetX();
				ey = nowShape->GetV()->GetY();
			}

			if (Distance(nowVertex->GetX(), nowVertex->GetY(), ex, ey, x, y) <= 0.03)
			{
				select_vertex = nowVertex;
				if (nowVertex->GetNext() != NULL)
				{
					select_vertexNext = nowVertex->GetNext();
				}
				else
				{
					select_vertexNext = nowShape->GetV();
				}
				select_shape = NULL;
				NowSelect(2);
				return;
			}
			nowVertex = nowVertex->GetNext();
		}
		nowShape = nowShape->GetSNext();
	}

	nowShape = shape_head;

	//�`������(NULL,NULL,�`)
	while (nowShape != NULL)
	{
		if (nowShape != shape_head)
		{
			if (AltInOutJug(nowShape, x, y))
			{
				select_shape = nowShape;
				select_vertex = NULL;
				select_vertexNext = NULL;
				NowSelect(3);
				return;
			}
		}
		nowShape = nowShape->GetSNext();
	}

	select_shape = NULL;
	select_vertex = NULL;
	select_vertexNext = NULL;
	NowSelect(0);
	return;
}

//SelectFlag�̃��Z�b�g
void CAdminControl::NowSelect(int x)
{
	switch (x)
	{
	case 0:
		SelectVertexFlag = false;
		SelectEdgeFlag = false;
		SelectShapeFlag = false;
		break;
	case 1:
		SelectVertexFlag = true;
		SelectEdgeFlag = false;
		SelectShapeFlag = false;
		break;
	case 2:
		SelectVertexFlag = false;
		SelectEdgeFlag = true;
		SelectShapeFlag = false;
		break;
	case 3:
		SelectVertexFlag = false;
		SelectEdgeFlag = false;
		SelectShapeFlag = true;
		break;
	default:
		break;
	}
}

//�_���}�E�X�̍��W�Ɉړ�������
void CAdminControl::MoveVertexJug(float x, float y, CVertex* nowMoveXY)
{
	CShape* nowShape = shape_head;
	if (nowShape->GetV() == NULL)
	{
		nowShape = nowShape->GetSNext();
	}

	CVertex* nowVertex = nowShape->GetV();
	CVertex* preVertex = NULL;

	if (nowMoveXY == NULL)
	{
		nowMoveXY = select_vertex;
	}
	float OrigX = nowMoveXY->GetX();
	float OrigY = nowMoveXY->GetY();

	while (nowMoveXY != nowVertex)
	{	
		if (nowVertex->GetNext() != NULL)
		{
			preVertex = nowVertex;
			nowVertex = nowVertex->GetNext();
		}
		else
		{
			nowShape = nowShape->GetSNext();
			preVertex = NULL;
			nowVertex = nowShape->GetV();
		}
	}
	if (preVertex == NULL)
	{
		preVertex = nowVertex;
		while (preVertex->GetNext() != NULL)
		{
			preVertex = preVertex->GetNext();
		}
	}

	//nowMove��x,y�������
	nowMoveXY->SetXY(x, y);
	
	//���������Ă���
	if (JudCross(x, y, preVertex, nowShape))
	{
		nowMoveXY->SetXY(OrigX, OrigY);
	}
	//���������Ă���
	if (!AltJudCross(x, y, preVertex, nowShape))
	{
		nowMoveXY->SetXY(OrigX, OrigY);
	}
	//����Ă���
	if (AltInOutJug(nowShape))
	{
		nowMoveXY->SetXY(OrigX, OrigY);
	}
	

}

//�`���}�E�X�̍��W�Ɉړ�������
void CAdminControl::MoveShapeJug(float x, float y, CShape* nowMoveShape)
{
	CShape* nowShape = shape_head;
	if (nowShape->GetV() == NULL)
	{
		nowShape = nowShape->GetSNext();
	}

	if (nowMoveShape == NULL)
	{
		nowMoveShape = select_shape;
	}

	while (nowShape != nowMoveShape)
	{
		nowShape = nowShape->GetSNext();
	}

	CVertex* nowVertex = nowShape->GetV();

	CVertex* OrigVertex = nowShape->GetV();
	float OrigX = OrigVertex->GetX();
	float OrigY = OrigVertex->GetY();

	//�ړ��ʂ�(x,y)�֓����D
	x = x - OrigX;
	y = y - OrigY;

	//(x,y)���e���W��
	while (nowVertex != NULL)
	{
		nowVertex->SetXY(nowVertex->GetX() + x, nowVertex->GetY() + y);
		nowVertex = nowVertex->GetNext();
	}

	//���������Ă���
	if (AltJugCross(nowMoveShape))
	{
		nowVertex = nowShape->GetV();

		while (nowVertex != NULL)
		{
			nowVertex->SetXY(nowVertex->GetX() - x, nowVertex->GetY() - y);
			nowVertex = nowVertex->GetNext();
		}
	}

	//����Ă���
	else if (AltInOutJug(nowMoveShape))
	{
		nowVertex = nowShape->GetV();

		while (nowVertex != NULL)
		{
			nowVertex->SetXY(nowVertex->GetX() - x, nowVertex->GetY() - y);
			nowVertex = nowVertex->GetNext();
		}
	}

	//�����Ă���
	else if (AltOutInJug(nowMoveShape))
	{
		nowVertex = nowShape->GetV();

		while (nowVertex != NULL)
		{
			nowVertex->SetXY(nowVertex->GetX() - x, nowVertex->GetY() - y);
			nowVertex = nowVertex->GetNext();
		}
	}
}

//�O��I�񂾓_�C���C�`�Ɠ�����
bool CAdminControl::SameSelect()
{
	if (preSelect_shape == select_shape && preSelect_vertex == select_vertex && preSelect_vertexNext == select_vertexNext)
	{
		return true;
	}
	return false;
}

//�_��ǉ�
void CAdminControl::AddVertex(float x, float y)
{
	CShape* nowShape = shape_head;
	if (nowShape->GetV() == NULL)
	{
		nowShape = nowShape->GetSNext();
	}

	CVertex* nowEdgeS = select_vertex;
	float sx = nowEdgeS->GetX();
	float sy = nowEdgeS->GetY();

	CVertex* nowEdgeE = select_vertexNext;
	float ex = nowEdgeE->GetX();
	float ey = nowEdgeE->GetY();
	
	float s ,t;
	CVertex ba;
	CVertex bp;

	ba.SetXY(sx - ex, sy - ey);//P1->P2
	bp.SetXY(x - ex, y - ey);//P2->P3

	s = nisk(bp.GetX(), bp.GetY(), ba.GetX(), ba.GetY()) / nisk(ba.GetX(), ba.GetY(), ba.GetX(), ba.GetY());
	t = 1 - s;

	x = nowEdgeS->GetX() + ba.GetX() * -t;
	y = nowEdgeS->GetY() + ba.GetY() * -t;


	CVertex* addV = new CVertex(x, y);

	if (nowEdgeS->GetNext() == NULL)
	{
		nowEdgeS->SetNext(addV);
	}
	else
	{
		nowEdgeS->SetNext(addV);
		addV->SetNext(nowEdgeE);
	}

	select_shape = NULL;
	select_vertex = NULL;
	select_vertexNext = NULL;
	NowSelect(0);
}

//�_�������D
void CAdminControl::DeleteVertex()
{
	CVertex* preVertex = NULL;
	CVertex* nowVertex = NULL;
	CVertex* nextVertex = NULL;

	CShape* nowShape = shape_head;
	if (nowShape->GetV() == NULL)
	{
		nowShape = nowShape->GetSNext();
	}

	while (nowShape != NULL)
	{
		nowVertex = nowShape->GetV();
		nextVertex = nowVertex->GetNext();

		while (nowVertex != NULL)
		{

			if (nowVertex == select_vertex && nowShape->CountVertex() > 3)
			{
				//�I�������_=�n�_
				if (nowVertex == nowShape->GetV())
				{
					preVertex = nowShape->GetV();
					while (preVertex->GetNext() != NULL)
					{
						preVertex = preVertex->GetNext();
					}
					nextVertex = nowVertex->GetNext();

					//���O�Ȃ�(�I�_,�n�_,���̓_)
					if (!InOutJug(preVertex, nowVertex, nextVertex))
					{
						nowShape->SetV(nowVertex->GetNext());

						delete nowVertex;

						select_shape = NULL;
						select_vertex = NULL;
						select_vertexNext = NULL;
						NowSelect(0);
					}
				}
				//�I�������_=�n�_�ȊO
				else
				{
					preVertex = nowShape->GetV();
					while (preVertex->GetNext() != nowVertex)
					{
						preVertex = preVertex->GetNext();
					}

					nextVertex = nowVertex->GetNext();
					if (nextVertex == NULL)
					{
						nextVertex = nowShape->GetV();
					}
					//���O�Ȃ�(�n�_or���̓_,���̓_,���̓_)��(�O�̓_�C�I�_�C�n�_)
					if (!InOutJug(preVertex, nowVertex, nextVertex))
					{
						preVertex->SetNext(nowVertex->GetNext());

						delete nowVertex;

						select_shape = NULL;
						select_vertex = NULL;
						select_vertexNext = NULL;
						NowSelect(0);
					}
				}
				return;
			}
			nowVertex = nowVertex->GetNext();
		}
		nowShape = nowShape->GetSNext();
	}
}

//��_�̒ǉ�
void CAdminControl::BasePointSetting(float x,float y)
{
	if (x == NULL && y == NULL)
	{
		BasePointFlag = false;
	}
	else
	{
		BasePointFlag = true;
		basePointX = x;
		basePointY = y;
	}
}

//�`�̊g��k��
void CAdminControl::ResizeShape(float s)
{
	CShape* nowShape = shape_head;
	CShape* nowMoveShape = select_shape;

	if (nowShape->GetV() == NULL)
	{
		nowShape = nowShape->GetSNext();
	}

	while (nowShape != nowMoveShape)
	{
		nowShape = nowShape->GetSNext();
	}

	CVertex* nowVertex = nowShape->GetV();

	float x = 0;
	float y = 0;

	if (s > 0)
	{
		s = 1.05;
	}
	else
	{
		s = 0.95;
	}
	
	//(x,y)���e���W��
	while (nowVertex != NULL)
	{
		x = s * (nowVertex->GetX() - basePointX) + basePointX;
		y = s * (nowVertex->GetY() - basePointY) + basePointY;
		nowVertex->SetXY(x,y);

		nowVertex = nowVertex->GetNext();
	}

	//�߂����Ⴟ������
	float totalEdge = 0;
	nowVertex = nowShape->GetV();
	while (nowVertex->GetNext() != NULL)
	{
		CVertex* nextVertex = nowVertex->GetNext();
		totalEdge = totalEdge + Distance(nowShape->GetV(),nextVertex->GetX(), nextVertex->GetY());
		nowVertex = nowVertex->GetNext();
	}
	if (totalEdge <= 0.3)
	{
		nowVertex = nowShape->GetV();

		while (nowVertex != NULL)
		{
			float disX = (nowVertex->GetX() - basePointX) / s + basePointX;
			float disY = (nowVertex->GetY() - basePointY) / s + basePointY;

			nowVertex->SetXY(disX, disY);
			nowVertex = nowVertex->GetNext();
		}
	}
	
	//���������Ă���
	if (AltJugCross(nowMoveShape))
	{
		nowVertex = nowShape->GetV();

		while (nowVertex != NULL)
		{
			float disX = (nowVertex->GetX() - basePointX) / s + basePointX;
			float disY = (nowVertex->GetY() - basePointY) / s + basePointY;

			nowVertex->SetXY(disX,disY);
			nowVertex = nowVertex->GetNext();
		}
	}

	//����Ă���
	else if (AltInOutJug(nowMoveShape))
	{
		nowVertex = nowShape->GetV();

		while (nowVertex != NULL)
		{
			float disX = (nowVertex->GetX() - basePointX) / s + basePointX;
			float disY = (nowVertex->GetY() - basePointY) / s + basePointY;

			nowVertex->SetXY(nowVertex->GetX() - disX, nowVertex->GetY() - disY);
			nowVertex = nowVertex->GetNext();
		}
	}

	//�����Ă���
	else if (AltOutInJug(nowMoveShape))
	{
		nowVertex = nowShape->GetV();

		while (nowVertex != NULL)
		{
			float disX = (nowVertex->GetX() - basePointX) / s + basePointX;
			float disY = (nowVertex->GetY() - basePointY) / s + basePointY;

			nowVertex->SetXY(nowVertex->GetX() - disX, nowVertex->GetY() - disY);
			nowVertex = nowVertex->GetNext();
		}
	}
}

//�`�̉�]
void CAdminControl::RollShape(float s)
{
	s = s / 1080;
	CShape* nowShape = shape_head;
	CShape* nowMoveShape = select_shape;

	if (nowShape->GetV() == NULL)
	{
		nowShape = nowShape->GetSNext();
	}

	while (nowShape != nowMoveShape)
	{
		nowShape = nowShape->GetSNext();
	}

	CVertex* nowVertex = nowShape->GetV();

	float x = 0;
	float y = 0;

	//(x,y)���e���W��
	while (nowVertex != NULL)
	{
		x = (nowVertex->GetX() - basePointX) * cos(s) - (nowVertex->GetY() - basePointY) * sin(s) + basePointX;
		y = (nowVertex->GetX() - basePointX) * sin(s) + (nowVertex->GetY() - basePointY) * cos(s) + basePointY;

		nowVertex->SetXY(x, y);
		nowVertex = nowVertex->GetNext();
	}

	//���������Ă���
	if (AltJugCross(nowMoveShape))
	{
		nowVertex = nowShape->GetV();
		s = -1 * s;
		while (nowVertex != NULL)
		{
			float disX = (nowVertex->GetX() - basePointX) * cos(s) - (nowVertex->GetY() - basePointY) * sin(s) + basePointX;
			float disY = (nowVertex->GetX() - basePointX) * sin(s) + (nowVertex->GetY() - basePointY) * cos(s) + basePointY;

			nowVertex->SetXY(disX, disY);
			nowVertex = nowVertex->GetNext();
		}
	}

	//����Ă���
	else if (AltInOutJug(nowMoveShape))
	{
		nowVertex = nowShape->GetV();
		s = -1 * s;
		while (nowVertex != NULL)
		{
			float disX = (nowVertex->GetX() - basePointX) * cos(s) - (nowVertex->GetY() - basePointY) * sin(s) + basePointX;
			float disY = (nowVertex->GetX() - basePointX) * sin(s) + (nowVertex->GetY() - basePointY) * cos(s) + basePointY;

			nowVertex->SetXY(disX,disY);
			nowVertex = nowVertex->GetNext();
		}
	}

	//�����Ă���
	else if (AltOutInJug(nowMoveShape))
	{
		nowVertex = nowShape->GetV();
		s = -1 * s;
		while (nowVertex != NULL)
		{
			float disX = (nowVertex->GetX() - basePointX) * cos(s) - (nowVertex->GetY() - basePointY) * sin(s) + basePointX;
			float disY = (nowVertex->GetX() - basePointX) * sin(s) + (nowVertex->GetY() - basePointY) * cos(s) + basePointY;

			nowVertex->SetXY(disX,disY);
			nowVertex = nowVertex->GetNext();
		}
	}
}

//����̌`��h��(���J��)
void CAdminControl::ShapePainting(float x, float y)
{
	CShape* paintShape = select_shape;
	CVertex* copyVertex = paintShape->GetV();

	if (!AltInOutJug(paintShape, x, y))
	{
		return;
	}

	/*
	while (paintShape->CountVertex() > 3)
	{


		glBegin(GL_TRIANGLES);
		glVertex2f(copyVertex->GetX(), copyVertex->GetY());
		glVertex2f(copyVertex->GetNext()->GetX(), copyVertex->GetNext()->GetY());
		glVertex2f(copyVertex->GetNext()->GetNext()->GetX(), copyVertex->GetNext()->GetNext()->GetY());
		glEnd();

		copyVertex = copyVertex->GetNext();

		delete paintShape->GetV();
	}
	
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 1.0);
	glVertex2f(copyVertex->GetX(), copyVertex->GetY());
	glVertex2f(0,0);
	glVertex2f(1,1);
	glEnd();

	*/

	

}

//�`��h��
void CAdminControl::ShapePaint()
{
	//�I���W�i��
	CShape* paintShape = shape_head;

	//���������}�`���Ȃ�
	if (paintShape == NULL)
	{
		return;
	}

	paintShape = paintShape->GetSNext();

	while (paintShape != NULL)
	{
		CShape* paintShapeCopy = new CShape();
		CVertex* nowV = paintShape->GetV();

		while (nowV != NULL)
		{
			paintShapeCopy->inheritVertex(nowV->GetX(), nowV->GetY());
			nowV = nowV->GetNext();
		}

		CVertex* cV = paintShapeCopy->GetV();
		CVertex* cVN = cV->GetNext();
		CVertex* cVNN = cVN->GetNext();

		while (paintShapeCopy->CountVertex() > 3)
		{

			if(AltInOutJug(paintShapeCopy, ((cV->GetX()+ cVN->GetX() + cVNN->GetX()) / 3), ((cV->GetY() + cVN->GetY() + cVNN->GetY()) / 3)) && !InOutJugTriangle(paintShapeCopy,cV,cVN,cVNN))
			{
				glBegin(GL_TRIANGLES);
				glColor3f(0.0, 0.5, 0.5);
				glVertex2f(cV->GetX(), cV->GetY());
				glVertex2f(cVN->GetX(), cVN->GetY());
				glVertex2f(cVNN->GetX(), cVNN->GetY());
				glEnd();

				if (cVN->GetNext() == NULL)//�����̂��I�_ 123456N��5"6"1��5->NULL
				{
					cV->SetNext(NULL);
				}
				else if(cV->GetNext() == NULL)//�����̂��n�_ 123456N��6"1"2��
				{
					paintShapeCopy->SetV(cVNN);
				}
				else//�����̂͘A�`����
				{
					cV->SetNext(cVNN);
				}
				CVertex* deleV = cVN;

				delete deleV;

				//cVN = cV;

				if (cV->GetNext() == NULL)
				{
					cVN = paintShapeCopy->GetV();
				}
				else
				{
					cVN = cVNN;
				}

				if (cVN->GetNext() == NULL)
				{
					cVNN = paintShapeCopy->GetV();
				}
				else
				{
					cVNN = cVNN->GetNext();
				}
			}
			//�h��Ȃ��Ƃ�
			else
			{
				cV = cVN;
				cVN = cVNN;
				if (cVNN->GetNext() == NULL)
				{
					cVNN = paintShapeCopy->GetV();
				}
				else
				{
					cVNN = cVNN->GetNext();
				}
			}
		}

		//�c�����O�p�`
		glBegin(GL_TRIANGLES);
		glColor3f(0.0, 0.5, 0.5);
		glVertex2f(cV->GetX(), cV->GetY());
		glVertex2f(cVN->GetX(), cVN->GetY());
		glVertex2f(cVNN->GetX(), cVNN->GetY());
		glEnd();

		delete cVNN;
		delete cVN;
		delete cV;
		paintShapeCopy = NULL;
		delete paintShapeCopy;

		paintShape = paintShape->GetSNext();
	}
}

//���_�̉�]
void CAdminControl::persRotate()
{
	glRotatef(SumDiffRotateX * 200.0, 0.0, 1.0, 0.0);
	glRotatef(SumDiffRotateY * 200.0, 1.0, 0.0, 0.0);


}

//���_�̕��s�ړ�
void CAdminControl::persTrans()
{
	glTranslatef(1.3 * SumDiffTransX, 1.3 * SumDiffTransY, 0.0);
}

//���_�̊g��k��...���J��
void CAdminControl::persScale()
{
	//glScalef(1.0, 1.0, 1.0 * DiffScale);
}

//���̂ɂ���
void CAdminControl::solidZline()
{
	//���C�e�B���O
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_AMBIENT, dif);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);


	//�I���W�i��
	CShape* paintShape = shape_head;

	//���������}�`���Ȃ�
	if (paintShape == NULL)
	{
		return;
	}

	paintShape = paintShape->GetSNext();

	//����
	while (paintShape != NULL)
	{
		CShape* paintShapeCopy = new CShape();
		CVertex* nowV = paintShape->GetV();

		while (nowV != NULL)
		{
			paintShapeCopy->inheritVertex(nowV->GetX(), nowV->GetY());
			nowV = nowV->GetNext();
		}

		CVertex* cV = paintShapeCopy->GetV();
		CVertex* cVN = cV->GetNext();
		CVertex* cVNN = cVN->GetNext();

		while (paintShapeCopy->CountVertex() > 3)
		{

			if (AltInOutJug(paintShapeCopy, ((cV->GetX() + cVN->GetX() + cVNN->GetX()) / 3), ((cV->GetY() + cVN->GetY() + cVNN->GetY()) / 3)) && !InOutJugTriangle(paintShapeCopy, cV, cVN, cVNN))
			{
				//����
				glBegin(GL_TRIANGLES);
				glColor3f(0.0, 0.5, 0.5);
				glVertex2f(cV->GetX(), cV->GetY());
				glVertex2f(cVN->GetX(), cVN->GetY());
				glVertex2f(cVNN->GetX(), cVNN->GetY());
				glEnd();

				//���
				glBegin(GL_TRIANGLES);
				glColor3f(0.0, 0.5, 0.5);
				glVertex3f(cV->GetX(), cV->GetY(),ZLine);
				glVertex3f(cVN->GetX(), cVN->GetY(),ZLine);
				glVertex3f(cVNN->GetX(), cVNN->GetY(),ZLine);
				glEnd();

				if (cVN->GetNext() == NULL)//�����̂��I�_ 123456N��5"6"1��5->NULL
				{
					cV->SetNext(NULL);
				}
				else if (cV->GetNext() == NULL)//�����̂��n�_ 123456N��6"1"2��
				{
					paintShapeCopy->SetV(cVNN);
				}
				else//�����̂͘A�`����
				{
					cV->SetNext(cVNN);
				}
				CVertex* deleV = cVN;

				delete deleV;

				//cVN = cV;

				if (cV->GetNext() == NULL)
				{
					cVN = paintShapeCopy->GetV();
				}
				else
				{
					cVN = cVNN;
				}

				if (cVN->GetNext() == NULL)
				{
					cVNN = paintShapeCopy->GetV();
				}
				else
				{
					cVNN = cVNN->GetNext();
				}
			}
			//�h��Ȃ��Ƃ�
			else
			{
				cV = cVN;
				cVN = cVNN;
				if (cVNN->GetNext() == NULL)
				{
					cVNN = paintShapeCopy->GetV();
				}
				else
				{
					cVNN = cVNN->GetNext();
				}
			}
		}

		//�c�����O�p�`
		glBegin(GL_TRIANGLES);
		glColor3f(0.0, 0.5, 0.5);
		glVertex2f(cV->GetX(), cV->GetY());
		glVertex2f(cVN->GetX(), cVN->GetY());
		glVertex2f(cVNN->GetX(), cVNN->GetY());
		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3f(0.0, 0.5, 0.5);
		glVertex3f(cV->GetX(), cV->GetY(), ZLine);
		glVertex3f(cVN->GetX(), cVN->GetY(), ZLine);
		glVertex3f(cVNN->GetX(), cVNN->GetY(), ZLine);
		glEnd();

		delete cVNN;
		delete cVN;
		delete cV;
		paintShapeCopy = NULL;
		delete paintShapeCopy;

		paintShape = paintShape->GetSNext();
	}

	//����
	paintShape = shape_head;
	paintShape = paintShape->GetSNext();

	while (paintShape != NULL)
	{
		CVertex* V = paintShape->GetV();
		CVertex* VN = V->GetNext();

		while (VN != NULL)
		{
			glBegin(GL_POLYGON);
			glColor3f(0.0, 0.5, 0.5);
			glVertex3f(V->GetX(),V->GetY(), 0);
			glVertex3f(VN->GetX(),VN->GetY(), 0);
			glVertex3f(VN->GetX(), VN->GetY(), ZLine);
			glVertex3f(V->GetX(), V->GetY(), ZLine);
			glEnd();

			V = VN;
			VN = VN->GetNext();
		}

		VN = paintShape->GetV();

		glBegin(GL_POLYGON);
		glColor3f(0.0, 0.5, 0.5);
		glVertex3f(V->GetX(), V->GetY(), 0);
		glVertex3f(VN->GetX(), VN->GetY(), 0);
		glVertex3f(VN->GetX(), VN->GetY(), ZLine);
		glVertex3f(V->GetX(), V->GetY(), ZLine);
		glEnd();

		paintShape = paintShape->GetSNext();
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
}



