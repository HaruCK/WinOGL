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
	float Distance(float nx, float ny,float ex,float ey, float x, float y);
	void CreateShape(float x, float y);

	float nisk(float ax,float ay,float bx,float by);
	float gisk(float ax, float ay, float bx, float by);

	bool ShapeFlag = false;

	bool JudCross(float Bex, float Bey);
	bool JudCross(float vx,float vy,CVertex* preV,CShape* nowS);
	bool AltJudCross(float Bex, float Bey);
	bool AltJudCross(float vx, float vy, CVertex* preV, CShape* nowS);
	bool AltJugCross(CShape* nowMoveShape);
	bool LastJudgment(float Bex, float Bey, CVertex* As, CVertex* Ae, CVertex* Bs);

	bool InOutJug(float x, float y);
	bool InOutJug(CVertex* preVertex, CVertex* nowVertex, CVertex* nextVertex);
	bool InOutJugTriangle(CShape* targetS,CVertex* cV, CVertex* cVN, CVertex* cVNN);
	bool AltInOutJug();
	bool AltInOutJug(CShape* nowShape, float x, float y);
	bool AltInOutJug(CShape* nowS);
	bool AltOutInJug(CShape* nowMoveShape);

	void DrawAxis();
	bool AxisFlag = false;
	int ModeFlag = 0;
	void SelectPosition(float x, float y);

	void NowSelect(int x);
	bool SelectVertexFlag = false;
	bool SelectEdgeFlag = false;
	bool SelectShapeFlag = false;
	bool BasePointFlag = false;

	void MoveVertexJug(float x,float y, CVertex* nowMoveXY);
	void MoveShapeJug(float x, float y, CShape* nowMoveShape);

	bool SameSelect();
	void AddVertex(float x, float y);
	void DeleteVertex();

	void BasePointSetting(float x,float y);
	void ResizeShape(float s);
	void RollShape(float s);

	bool AllShape = false;

	void ShapePainting(float x, float y);
	//void CenterPoint(CVertex* a, CVertex* b, CVertex* c);

	void ShapePaint();
	bool ShapePaintFlag = false;

	void persRotate();
	void persTrans();
	void persScale();

	float DiffRotateX = 0;
	float DiffRotateY = 0;
	float DiffTransX = 0;
	float DiffTransY = 0;
	float DiffMouseWheel = 0;

	float SumDiffRotateX = 0;
	float SumDiffRotateY = 0;
	float SumDiffTransX = 0;
	float SumDiffTransY = 0;

	float ZLine = 0.1;
	void solidZline();

	float dif[4] = { 1.0,1.0,1.0,1.0 };
	float naturalDif[4] = { 0.0,0.0,0.0,1.0 };

private:
	CShape* shape_head = NULL;
	CShape* shape_change = NULL;

	CShape* select_shape = NULL;
	CVertex* select_vertex = NULL;
	CVertex* select_vertexNext = NULL;

	CShape* preSelect_shape = NULL;
	CVertex* preSelect_vertex = NULL;
	CVertex* preSelect_vertexNext = NULL;

	float basePointX = NULL;
	float basePointY = NULL;
};

