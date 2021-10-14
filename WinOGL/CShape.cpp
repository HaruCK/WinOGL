#include "pch.h"
#include "CShape.h"

CShape::CShape()
{
}

CShape::CShape(CVertex* new_v)
{
	vertex_head = new_v;
}

CShape::~CShape()
{
	vertex_head->FreeVertex();
}


void CShape::SetV(CVertex* v)
{
	vertex_head = v;
}

CVertex* CShape::GetV()
{
	return vertex_head;
}

CShape* CShape::GetSNext()
{
	return shape_next;
}

CShape* CShape::SetSNext(CShape* new_Snext)
{
	return shape_next = new_Snext;
}

void CShape::FreeShape()
{
	CShape* nowS = this;
	while (nowS != NULL)
	{
		CShape* del_cell = nowS;
		nowS = nowS->GetSNext();
		delete del_cell;
	}
}

CVertex* CShape::inheritVertex(float x, float y) {
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

int CShape::CountVertex()
{
	int cnt = 0;

	for (CVertex* vp = vertex_head; vp != NULL; vp = vp->GetNext())
	{
		cnt++;
	}

	return cnt;
}



