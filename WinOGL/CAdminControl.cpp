#include "pch.h"
#include "CAdminControl.h"
#include <math.h>

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
	if (shape_head == NULL)
	{
		inheritShape();
	}

	if (shape_head->CountVertex() < 3)
	{
		shape_head->inheritVertex(x, y);
	}
	else if (Distance(shape_head->GetV(), x, y) <= 0.1)
	{
		shape_head->inheritVertex(shape_head->GetV()->GetX(), shape_head->GetV()->GetY());
		inheritShape();
	}
	else
	{
		shape_head->inheritVertex(x, y);
	}
}
