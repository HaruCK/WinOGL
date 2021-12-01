
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
	CWinOGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	wglMakeCurrent(pDC->m_hDC, m_hRC);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT*/);

	AC.Draw();
	
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


	if (AC.ModeFlag == 1 && AC.SelectVertexFlag)
	{
		if (nowLButton)
		{
			AC.MoveVertexJug(ClickX, ClickY, NULL);
		}

	}

	
	RedrawWindow();
	CView::OnMouseMove(nFlags, point);

}


void CWinOGLView::OnRButtonDown(UINT nFlags, CPoint point)
{
	//nowRButton = true;
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
