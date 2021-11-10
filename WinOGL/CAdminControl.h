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
	float Distance(float vx, float vy, float x, float y);
	void CreateShape(float x, float y);

	float nisk(float ax,float ay,float bx,float by);
	float gisk(float ax, float ay, float bx, float by);

	bool ShapeFlag = false;

	bool JudCross(float Bex, float Bey);
	bool AltJudCross(float Bex, float Bey);
	bool LastJudgment(float Bex, float Bey, CVertex* As, CVertex* Ae, CVertex* Bs);

	bool InOutJug(float x, float y);
	bool AltInOutJug();
	bool AltInOutJug(CShape* nowShape, float x, float y);

	void DrawAxis();
	bool AxisFlag = false;
	int ModeFlag = 0;

	void SelectPosition(float x, float y);

private:
	CShape* shape_head = NULL;
	CShape* shape_change = NULL;

	CShape* select_shape = NULL;
	CVertex* select_vertex = NULL;
};

