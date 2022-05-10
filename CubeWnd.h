#pragma once

#include "CubeControl.h"

// CCubeWnd

class CCubeWnd : public CWnd//公有继承
{
	DECLARE_DYNAMIC(CCubeWnd)
public:
	CCubeWnd();
	virtual ~CCubeWnd();
	int InitGL();
	virtual void Reset();

	CCubeControl m_3DCube;
	void Rotate3D(CString Rotrun);
	void BottomCrossSolve();	//底面十字
	void BottomCornerSolve();	//底面角块
	void SecondSolve();			//中棱归位
	void TopCrossFaceSolve();	//顶面十字
	void TopCrossPosSolve();	//顶角面位
	void TopLastSolve();		//收尾

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);	
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
	virtual void SaveCube();
	virtual void RestoreCube();
private:
	CPoint	m_LastPointR;	//用于鼠标右键移动旋转视图
	float	m_RotateX;		//用于3D视图旋转
	float	m_RotateY;		//用于3D视图旋转
	HDC		m_hDC;			//本窗口hDC
	HGLRC	m_hRC;			//用于OpenGL的hRc
	
	void	RotateSide();	//鼠标左键旋转
	void	ReSizeGLScene( int width, int height);		//OpenGL的ReSize设置
	BOOL	SetPixelformat(HDC hdc);					//OpenGL的像素等设置
	struct Coord3D				//定义一个三维坐标系
	{
		double x;
		double y;
		double z;
	};
	Coord3D	LdwCoord;			//鼠标左键按下时的三维坐标
	Coord3D LupCoord;			//鼠标左键抬起时的三维坐标
	void	Get3DCoord(Coord3D &lCoord, CPoint point);	//获取三维坐标

	void CenterPosSolve();		//中心块归位，白为底，前为蓝，作为魔方还原的基础参考
	void SecondMethod(int Pos);	//第二层的固定转法
	void TopCrossFace();		//顶层十字面的转法
	void TopCrossPos();			//顶层十字位的转法，前提是十字面已完成
	void TopConer2Rotate(char Face);		//顶层同侧对角旋转
	void TopConer3Change(char Face);		//顶层3角交换、旋转
	int  FindYellow();			//找顶层角块顶面为黄色的块
	void DisplayTimer();
	int timeCounter;			//计时器
};


