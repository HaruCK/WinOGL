#pragma once
#include <gl/GL.h>
#include "CVertex.h"
#include "CShape.h"

class CAdminControl
{
public:
	CAdminControl();
	~CAdminControl();

	void Draw();
	//CVertex CV;

	void inheritShape();
	float Distance(CVertex* s, float x, float y);
	void CreateShape(float x, float y);

	bool ShapeFlag = false;

	bool JudCross(float Bex, float Bey);
	bool AltJudCross(float Bex, float Bey);
	bool LastJudgment(float Bex, float Bey, CVertex* As, CVertex* Ae, CVertex* Bs);


private:
	CShape* shape_head = NULL;
	CShape* shape_change = NULL;
};

