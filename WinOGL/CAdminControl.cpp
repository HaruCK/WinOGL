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

}

//�V����shape�ֈڂ�
void CAdminControl::inheritShape() {
	CShape* newShape = new CShape();
	newShape->SetSNext(shape_head);
	shape_head = newShape;
}

//�_�Ɠ_�̋���
float CAdminControl::Distance(CVertex* s, float x, float y)
{
	float d,X,Y;

	X = x - s->GetX();
	Y = y - s->GetY();

	d = sqrt(pow(X, 2) + pow(Y, 2));

	return d;
}

float CAdminControl::Distance(float vx, float vy, float x, float y)
{
	float d, X, Y;

	X = x - vx;
	Y = y - vy;

	d = sqrt(pow(X, 2) + pow(Y, 2));

	return d;
}

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

//�`�ʃ��[�h�̎��C�_��łĂ邩�̏����D���s��
void CAdminControl::CreateShape(float x, float y)
{
	if (ModeFlag == 0)//�`�惂�[�h
	{
		if (shape_head == NULL)//1-1�̂�
		{
			inheritShape();
		}

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

	else if (ModeFlag == 1)//�I�����[�h
	{
		/*
		if (SelectVertexFlag)
		{
			if (!JudCross(x, y) && !AltJudCross(x, y))
			{
				
			}
		}
		*/
		SelectPosition(x, y);
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

//�_�̎�����
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
		nowShape = shape_head->GetSNext();
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

void CAdminControl::SelectPosition(float x, float y)
{
	CShape* nowShape = shape_head;

	while (nowShape != NULL)
	{
		//�_������(�_,NULL,NULL)
		CVertex* nowVertex = nowShape->GetV();
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

		//�_�Ɛ�����(�_,�_,NULL)
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

		//�`������(NULL,NULL,�`)
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
}

void CAdminControl::NowSelect(int x)
{
	switch (x)
	{
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

//�}�E�X�̍��W�Ɉړ�������
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
	else if (!AltJudCross(x, y, preVertex, nowShape))
	{
		nowMoveXY->SetXY(OrigX, OrigY);
	}
	//����Ă���
	else if (AltInOutJug(nowShape))
	{
		nowMoveXY->SetXY(OrigX, OrigY);
	}
}
