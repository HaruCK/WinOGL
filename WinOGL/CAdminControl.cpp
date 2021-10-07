#include "pch.h"
#include "CAdminControl.h"

CAdminControl::CAdminControl()
{
}

CAdminControl::~CAdminControl()
{
	vertex_head->FreeVertex();
}

void CAdminControl::Draw()
{
	CVertex* nowVertex = vertex_head;

	glColor3f(1.0, 1.0, 1.0);
	glPointSize(10);

	/*lŠpŒ`‚Ì•`Ê
	glBegin(GL_LINE_LOOP);
	glVertex2f(0.5, 0.5);
	glVertex2f(-0.5, 0.5);
	glVertex2f(-0.5, -0.5);
	glVertex2f(0.5, -0.5);
	*/

	glBegin(GL_POINTS);

	while(nowVertex != NULL) {
		glVertex2f(nowVertex->GetX(), nowVertex->GetY());
		nowVertex = nowVertex->GetNext();
	}

	glEnd();
}

CVertex* CAdminControl::inheritVertex(float x, float y) {
	CVertex* nowVertex = vertex_head;
	CVertex* newVertex = new CVertex(x, y);

	if (nowVertex != NULL)
	{
		while (nowVertex->GetNext() != NULL) 
		{
			nowVertex = nowVertex->GetNext();
		}
		nowVertex->SetNext(newVertex);
	}
	else
	{
		vertex_head = newVertex;
	}

	return newVertex;
}
