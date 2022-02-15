
// WinOGLView.h : CWinOGLView クラスのインターフェイス
//
#pragma once
#include <gl/GL.h>
#include "CAdminControl.h"

class CWinOGLView : public CView
{
protected: // シリアル化からのみ作成します。
	CWinOGLView() noexcept;
	DECLARE_DYNCREATE(CWinOGLView)
	

// 属性
public:
	CWinOGLDoc* GetDocument() const;
	//double ClickX, ClickY;
private:
	double ClickX, ClickY;

// 操作
public:
	bool nowLButton = false;
	bool nowRButton = false;

	float preMousemoveX = 0;
	float preMousemoveY = 0;

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 実装
public:
	virtual ~CWinOGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
	HGLRC m_hRC;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
private:
	CAdminControl AC;
public:
	//int ModeFlag = 1;

	afx_msg void OnXyz();
	afx_msg void OnUpdateXyz(CCmdUI* pCmdUI);
	afx_msg void OnDrawing();

	afx_msg void OnEdit();
	afx_msg void OnUpdateDrawing(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEdit(CCmdUI* pCmdUI);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnColor();
	afx_msg void OnUpdateColor(CCmdUI* pCmdUI);
	afx_msg void OnViewchange();
	afx_msg void OnUpdateViewchange(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // WinOGLView.cpp のデバッグ バージョン
inline CWinOGLDoc* CWinOGLView::GetDocument() const
   { return reinterpret_cast<CWinOGLDoc*>(m_pDocument); }
#endif

