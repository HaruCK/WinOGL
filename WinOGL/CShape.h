#pragma once
#include "CVertex.h"

class CShape
{
public:
	CShape();
	CShape(CVertex* new_v);
	~CShape();

	void SetV(CVertex* v);
	CVertex* GetV();

	CShape* GetSNext();
	CShape* SetSNext(CShape* new_Snext);

	void FreeShape();

	CVertex* inheritVertex(float x, float y);
	int CountVertex();

private:
	CShape* shape_next;
	CVertex* vertex_head = NULL;
};

