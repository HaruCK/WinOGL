#pragma once
#include <gl/GL.h>
#include "CVertex.h"

class CAdminControl
{
public:
	CAdminControl();
	~CAdminControl();

	void Draw();
	CVertex CV;

	CVertex* inheritVertex(float x, float y);

private:
	CVertex* vertex_head = NULL;
};

