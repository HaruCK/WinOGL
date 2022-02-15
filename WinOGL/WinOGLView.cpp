
// WinOGLView.cpp : CWinOGLView クラスの実装
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "WinOGL.h"
#endif

#include "WinOGLDoc.h"
#include "WinOGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinOGLView

IMPLEMENT_DYNCREATE(CWinOGLView, CView)

BEGIN_MESSAGE_MAP(CWinOGLView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_XYZ, &CWinOGLView::OnXyz)
	ON_UPDATE_COMMAND_UI(ID_XYZ, &CWinOGLView::OnUpdateXyz)
	ON_COMMAND(ID_DRAWING, &CWinOGLView::OnDrawing)
	ON_COMMAND(ID_EDIT, &CWinOGLView::OnEdit)
	ON_UPDATE_COMMAND_UI(ID_DRAWING, &CWinOGLView::OnUpdateDrawing)
	ON_UPDATE_COMMAND_UI(ID_EDIT, &CWinOGLView::OnUpdateEdit)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_COLOR, &CWinOGLView::OnColor)
	ON_UPDATE_COMMAND_UI(ID_COLOR, &CWinOGLView::OnUpdateColor)
	ON_COMMAND(ID_ViewChange, &CWinOGLView::OnViewchange)
	ON_UPDATE_COMMAND_UI(ID_ViewChange, &CWinOGLView::OnUpdateViewchange)
END_MESSAGE_MAP()

// CWinOGLView コンストラクション/デストラクション

CWinOGLView::CWinOGLView() noexcept
{
	// TODO: 構築コードをここに追加します。
	ClickX = 0;
	ClickY = 0;

}

CWinOGLView::~CWinOGLView()
{
}

BOOL CWinOGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CWinOGLView 描画

void CWinOGLView::OnDraw(CDC* pDC)
{
	//描画系はぜんぶここでOKナス
	CWinOGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	wglMakeCurrent(pDC->m_hDC, m_hRC);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glLoadIdentity();

	if (AC.ModeFlag == 2)
	{
		//AC.solidZline();

		AC.persTrans();
		AC.persRotate();

	}
	else
	{
		AC.SumDiffRotateX = 0;
		AC.SumDiffRotateY = 0;
		AC.SumDiffTransX = 0;
		AC.SumDiffTransY = 0;
		AC.ZLine = 0.1;
		glLoadIdentity();
	}

	AC.Draw();
	
	glDisable(GL_DEPTH_TEST);

	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(pDC->m_hDC, NULL);
}


// CWinOGLView の診断

#ifdef _DEBUG
void CWinOGLView::AssertValid() const
{
	CView::AssertValid();
}

void CWinOGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWinOGLDoc* CWinOGLView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinOGLDoc)));
	return (CWinOGLDoc*)m_pDocument;
}
#endif //_DEBUG


// CWinOGLView メッセージ ハンドラー


void CWinOGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	nowLButton = true;
	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	rect.Width();
	rect.Height();

	ClickX = (double)point.x / rect.Width();
	ClickX = (ClickX * 2) - 1;

	ClickY = (double)point.y / rect.Height();
	ClickY = (ClickY -1 ) * -1;
	ClickY = (ClickY * 2) - 1;

	if (rect.Width() > rect.Height()) 
	{
		double hi = (double)rect.Width() / rect.Height();
		ClickX = ClickX * hi;
	}
	else
	{
		double hi = (double)rect.Height() / rect.Width();
		ClickY = ClickY * hi;
	}

	//CAdminControlへXYを受け渡し
	if (AC.ModeFlag == 0)
	{
		AC.CreateShape((float)ClickX, (float)ClickY);
	}

	if (AC.ModeFlag == 1)
	{
		AC.SelectPosition((float)ClickX, (float)ClickY);

		if (AC.SelectEdgeFlag && AC.SameSelect())
		{
			AC.AddVertex(ClickX, ClickY);
		}
	}


	RedrawWindow();
	CView::OnLButtonDown(nFlags, point);
}


int CWinOGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	PIXELFORMATDESCRIPTOR pfd =
	{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW |
	PFD_SUPPORT_OPENGL |
	PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	32,
	0,0,0,0,0,0,
	0,0,0,0,0,0,0,
	24,
	0,0,
	PFD_MAIN_PLANE,
	0,
	0,0,0
	};
	CClientDC clientDC(this);
	int pixelFormat = ChoosePixelFormat(clientDC.m_hDC,&pfd);
	SetPixelFormat(clientDC.m_hDC, pixelFormat, &pfd);
	m_hRC = wglCreateContext(clientDC.m_hDC);

	return 0;
}


void CWinOGLView::OnDestroy()
{
	CView::OnDestroy();
	wglDeleteContext(m_hRC);
}


BOOL CWinOGLView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	return true;
}


void CWinOGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CClientDC clientDC(this);
	wglMakeCurrent(clientDC.m_hDC, m_hRC);
	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double hi = 0;

	if (cx > cy) {

		hi = (double)cx / cy;
		glOrtho(-1*hi, 1*hi, -1, 1, -100, 100);
	}
	else
	{
		hi = (double)cy / cx;
		glOrtho(-1, 1, -1*hi, 1*hi, -100, 100);
	}
	glMatrixMode(GL_MODELVIEW);
	RedrawWindow();
	wglMakeCurrent(clientDC.m_hDC, NULL);
}

//XYZ軸の切り替え
void CWinOGLView::OnXyz()
{
	if (!AC.AxisFlag)
	{
		AC.AxisFlag = true;
	}
	else
	{
		AC.AxisFlag = false;
	}
	RedrawWindow();
}

void CWinOGLView::OnUpdateXyz(CCmdUI* pCmdUI)
{
	if (AC.AxisFlag)
	{
		pCmdUI->SetCheck(true);
	}
	else
	{
		pCmdUI->SetCheck(false);
	}
}


void CWinOGLView::OnDrawing()
{
	AC.ModeFlag = 0;

	RedrawWindow();
}

void CWinOGLView::OnUpdateDrawing(CCmdUI* pCmdUI)
{
	if (AC.ModeFlag == 0)
	{
		pCmdUI->SetCheck(true);
	}
	else
	{
		pCmdUI->SetCheck(false);
	}
}

void CWinOGLView::OnEdit()
{
	AC.ModeFlag = 1;

	RedrawWindow();
}

void CWinOGLView::OnUpdateEdit(CCmdUI* pCmdUI)
{
	if (AC.ModeFlag == 1)
	{
		pCmdUI->SetCheck(true);
	}
	else
	{
		pCmdUI->SetCheck(false);
	}
}

void CWinOGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	nowLButton = false;
	
	RedrawWindow();
	CView::OnLButtonUp(nFlags, point);
}


void CWinOGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	rect.Width();
	rect.Height();

	ClickX = (double)point.x / rect.Width();
	ClickX = (ClickX * 2) - 1;

	ClickY = (double)point.y / rect.Height();
	ClickY = (ClickY - 1) * -1;
	ClickY = (ClickY * 2) - 1;

	if (rect.Width() > rect.Height())
	{
		double hi = (double)rect.Width() / rect.Height();
		ClickX = ClickX * hi;
	}
	else
	{
		double hi = (double)rect.Height() / rect.Width();
		ClickY = ClickY * hi;
	}

	if (AC.ModeFlag == 1)
	{
		if (AC.SelectVertexFlag && nowLButton)
		{
			AC.MoveVertexJug(ClickX, ClickY, NULL);
		}
		else if(AC.SelectShapeFlag && nowLButton)
		{
			AC.MoveShapeJug(ClickX,ClickY,NULL);
		}

	}

	//視点変更のとき
	if (AC.ModeFlag == 2)
	{
		//前回のフレームと差分取る．
		if (nowLButton)
		{
			AC.DiffRotateX = ClickX - preMousemoveX;
			AC.DiffRotateY = ClickY - preMousemoveY;

			AC.SumDiffRotateX = AC.SumDiffRotateX + AC.DiffRotateX;
			AC.SumDiffRotateY = AC.SumDiffRotateY + AC.DiffRotateY;

			AC.DiffTransX = 0;
			AC.DiffTransY = 0;
		}
		else if (nowRButton)
		{
			AC.DiffTransX = ClickX - preMousemoveX;
			AC.DiffTransY = ClickY - preMousemoveY;

			AC.SumDiffTransX = AC.SumDiffTransX + AC.DiffTransX;
			AC.SumDiffTransY = AC.SumDiffTransY + AC.DiffTransY;

			AC.DiffRotateX = 0;
			AC.DiffRotateY = 0;
		}
		else
		{
			AC.DiffTransX = 0;
			AC.DiffTransY = 0;

			AC.DiffRotateX = 0;
			AC.DiffRotateY = 0;
		}
	}

	//保存
	preMousemoveX = ClickX;
	preMousemoveY = ClickY;

	RedrawWindow();

	CView::OnMouseMove(nFlags, point);
}


void CWinOGLView::OnRButtonDown(UINT nFlags, CPoint point)
{
	nowRButton = true;
	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	rect.Width();
	rect.Height();

	ClickX = (double)point.x / rect.Width();
	ClickX = (ClickX * 2) - 1;

	ClickY = (double)point.y / rect.Height();
	ClickY = (ClickY - 1) * -1;
	ClickY = (ClickY * 2) - 1;

	if (rect.Width() > rect.Height())
	{
		double hi = (double)rect.Width() / rect.Height();
		ClickX = ClickX * hi;
	}
	else
	{
		double hi = (double)rect.Height() / rect.Width();
		ClickY = ClickY * hi;
	}

	if (AC.ModeFlag == 1)
	{
		if (AC.SelectVertexFlag)
		{
			AC.DeleteVertex();
		}
	}

	RedrawWindow();
	CView::OnRButtonDown(nFlags, point);
}


void CWinOGLView::OnMButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	rect.Width();
	rect.Height();

	ClickX = (double)point.x / rect.Width();
	ClickX = (ClickX * 2) - 1;

	ClickY = (double)point.y / rect.Height();
	ClickY = (ClickY - 1) * -1;
	ClickY = (ClickY * 2) - 1;

	if (rect.Width() > rect.Height())
	{
		double hi = (double)rect.Width() / rect.Height();
		ClickX = ClickX * hi;
	}
	else
	{
		double hi = (double)rect.Height() / rect.Width();
		ClickY = ClickY * hi;
	}


	if (AC.ModeFlag == 1)
	{
		if (AC.SelectShapeFlag)
		{
			AC.BasePointSetting(ClickX, ClickY);
			AC.BasePointFlag = true;
		}
		else
		{
			AC.BasePointSetting(NULL, NULL);
			AC.BasePointFlag = false;
		}
	}

	RedrawWindow();
	CView::OnMButtonDown(nFlags, point);
}


BOOL CWinOGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

	if (AC.ModeFlag == 1)
	{
		if (AC.SelectShapeFlag && AC.BasePointFlag && !nowRButton)
		{
			AC.ResizeShape(zDelta);
		}
		else if (AC.SelectShapeFlag && AC.BasePointFlag && nowRButton)
		{
			AC.RollShape(zDelta);
		}
	}

	if (AC.ModeFlag == 2)
	{
		AC.DiffMouseWheel = (zDelta / 120) * 0.01;

		if (AC.ZLine + AC.DiffMouseWheel > 0.01 && AC.ZLine + AC.DiffMouseWheel < 1.0)
		{
			AC.ZLine = AC.ZLine + AC.DiffMouseWheel;
		}
	}

	RedrawWindow();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CWinOGLView::OnRButtonUp(UINT nFlags, CPoint point)
{
	nowRButton = false;

	RedrawWindow();
	CView::OnRButtonUp(nFlags, point);
}


void CWinOGLView::OnColor()
{
	if (!AC.ShapePaintFlag)
	{
		AC.ShapePaintFlag = true;
	}
	else
	{
		AC.ShapePaintFlag = false;
	}
	RedrawWindow();
}


void CWinOGLView::OnUpdateColor(CCmdUI* pCmdUI)
{
	if (AC.ShapePaintFlag)
	{
		pCmdUI->SetCheck(true);
	}
	else
	{
		pCmdUI->SetCheck(false);
	}
}


void CWinOGLView::OnViewchange()
{
	AC.ModeFlag = 2;
	
	RedrawWindow();
}


void CWinOGLView::OnUpdateViewchange(CCmdUI* pCmdUI)
{
	if (AC.ModeFlag == 2)
	{
		pCmdUI->SetCheck(true);
	}
	else
	{
		pCmdUI->SetCheck(false);
	}
}
