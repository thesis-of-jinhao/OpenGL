// CubeWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "CubeWnd.h"

// CCubeWnd
IMPLEMENT_DYNAMIC(CCubeWnd, CWnd)

CCubeWnd::CCubeWnd()
{
	//初始化
	m_hRC = NULL;
	m_RotateX = 25.0f;
	m_RotateY = -45.0f;
	timeCounter = 0;
}

CCubeWnd::~CCubeWnd()
{
	wglMakeCurrent(NULL,NULL);//指定函数操作表
	wglDeleteContext(m_hRC);//删除函数操作表
}

BEGIN_MESSAGE_MAP(CCubeWnd, CWnd)//ccubewnd为所属类的名字，cwnd为基类的名字
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()//左键摁下
	ON_WM_LBUTTONUP()//左键抬起
	ON_WM_LBUTTONDBLCLK()//左键双击
	ON_WM_RBUTTONDOWN()
	ON_WM_ENABLE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


void CCubeWnd::OnEnable(BOOL bEnable)
{
	CWnd::OnEnable(bEnable);
	Invalidate();
	UpdateWindow();
}


void CCubeWnd::OnPaint()//绘制函数
{
	CPaintDC dc(this);	// device context for painting
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		glLoadIdentity();									
		glTranslated(0.0,0.0,-7.0);							
		glRotatef(m_RotateX,1,0,0);
		glRotatef(m_RotateY,0,1,0);

		m_3DCube.Render();	//调用函数，绘制3D魔方
		glFlush();//将GL命令队列中的命令发送给显卡并清空命令队列，发送完立即返回；
		SwapBuffers(m_hDC);//OpenGL绘图上下文传给windows绘图上下文
		DisplayTimer();
	}
}


void CCubeWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDblClk(nFlags, point);
}


void CCubeWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	//m_LasePointL = point;
	Get3DCoord(LdwCoord, point);	//获取鼠标左键按下时的三维坐标
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CCubeWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	Get3DCoord(LupCoord, point);	//获取鼠标左键抬起时的三维坐标
	RotateSide();
	Invalidate();
	UpdateWindow();
	CWnd::OnLButtonUp(nFlags, point);
}

void CCubeWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	timeCounter++;
	DisplayTimer();
	CWnd::OnTimer(nIDEvent);
}
void CCubeWnd::RotateSide()
{
	double lx,bx;
	double ly,by;
	double lz,bz;
	double dist = 0.2;	//鼠标移动最小响应距离
	double xdist, ydist, zdist;
	lx = LdwCoord.x;
	ly = LdwCoord.y;
	lz = LdwCoord.z;
	bx = LupCoord.x;
	by = LupCoord.y;
	bz = LupCoord.z;
	xdist = bx - lx;
	ydist = by - ly;
	zdist = bz - lz;
	
	//在Cube.h中设定了方块的边长为1.0，坐标识别计算以此为准
	//判断鼠标所在坐标位置用变量来替代具体数值，方便修改
	double SideLength = 1.0;
	double AtFaceLow = SideLength + SideLength/2 - 0.01;		//1.49
	double AtFaceHig = SideLength + SideLength/2 + 0.01;		//1.51
	double AtQuadLow = SideLength/2 - 0.01;						//0.49
	double AtQuadHig = SideLength/2 + 0.01;						//0.51
	if(!m_3DCube.IsRestore() && fabs(xdist)<2*AtFaceHig && fabs(ydist)<2*AtFaceHig && fabs(zdist)<2*AtFaceHig)
		SetTimer(1, 1000, NULL);


	//------------------右面x=1.49<<>>1.51---------------------------------------------


	if(	lx>AtFaceLow && lx<AtFaceHig && ly>AtQuadLow && ly<AtFaceHig && lz>-AtFaceHig && lz<AtFaceHig &&
		bx>AtFaceLow && bx<AtFaceHig && by>AtQuadLow && by<AtFaceHig && bz>-AtFaceHig && bz<AtFaceHig)		//鼠标在右面z方向滑动，顶层
	{
		if(bz-lz>dist)			//鼠标往z方向移动
		{ 
			Rotate3D("U+");
		}
		else if(bz-lz<-dist)	//鼠标往z反方向移动
			Rotate3D("U-");
	}
	else if(lx>AtFaceLow && lx<AtFaceHig && ly>-AtQuadHig && ly<AtQuadHig && lz>-AtFaceHig && lz<AtFaceHig &&
		bx>AtFaceLow && bx<AtFaceHig && by>-AtQuadHig && by<AtQuadHig && bz>-AtFaceHig && bz<AtFaceHig)		//鼠标在右面z方向滑动，中层	
	{
		if(bz-lz>dist)			//鼠标往z方向移动
			Rotate3D("Y+");
		else if(bz-lz<-dist)	//鼠标往z反方向移动
			Rotate3D("Y-");
	}
	else if( lx>AtFaceLow && lx<AtFaceHig && ly>-AtFaceHig && ly<-AtQuadLow && lz>-AtFaceHig && lz<AtFaceHig &&
		bx>AtFaceLow && bx<AtFaceHig && by>-AtFaceHig && by<-AtQuadLow && bz>-AtFaceHig && bz<AtFaceHig)		//鼠标在右面z方向滑动，底层	
	{
		if(bz-lz>dist)			//鼠标往z方向移动
			Rotate3D("D-");
		else if(bz-lz<-dist)	//鼠标往z反方向移动
			Rotate3D("D+");
	}
	else if( lx>AtFaceLow && lx<AtFaceHig && ly>-AtFaceHig && ly<AtFaceHig && lz>AtQuadLow && lz<AtFaceHig &&
		bx>AtFaceLow && bx<AtFaceHig && by>-AtFaceHig && by<AtFaceHig && bz>AtQuadLow && bz<AtFaceHig)		//鼠标在右面y方向滑动，前层	
	{
		if(by-ly>dist)			//鼠标往y方向移动
			Rotate3D("F-");
		else if(by-ly<-dist)	//鼠标往y反方向移动
			Rotate3D("F+");
	}
	else if( lx>AtFaceLow && lx<AtFaceHig && ly>-AtFaceHig && ly<AtFaceHig && lz>-AtQuadHig && lz<AtQuadHig &&
		bx>AtFaceLow && bx<AtFaceHig && by>-AtFaceHig && by<AtFaceHig && bz>-AtQuadHig && bz<AtQuadHig)		//鼠标在右面y方向滑动，中层	
	{
		if(by-ly>dist)			//鼠标往y方向移动
			Rotate3D("Z-");
		else if(by-ly<-dist)	//鼠标往y反方向移动
			Rotate3D("Z+");
	}
	else if( lx>AtFaceLow && lx<AtFaceHig && ly>-AtFaceHig && ly<AtFaceHig && lz>-AtFaceHig && lz<-AtQuadLow &&
		bx>AtFaceLow && bx<AtFaceHig && by>-AtFaceHig && by<AtFaceHig && bz>-AtFaceHig && bz<-AtQuadLow)		//鼠标在右面y方向滑动，后层	
	{
		if(by-ly>dist)			//鼠标往y方向移动
			Rotate3D("B+");
		else if(by-ly<-dist)	//鼠标往y反方向移动
			Rotate3D("B-");
	}


	//------------------前面z=1.49<<>>1.51---------------------------------------------


	else if( lx>-AtFaceHig && lx<-AtQuadHig && ly>-AtFaceHig && ly<AtFaceHig && lz>AtFaceLow && lz<AtFaceHig &&
		bx>-AtFaceHig && bx<-AtQuadHig && by>-AtFaceHig && by<AtFaceHig && bz>AtFaceLow && bz<AtFaceHig)		//鼠标在右面y方向滑动，左层	
	{
		if(by-ly>dist)			//鼠标往y方向移动
			Rotate3D("L-");
		else if(by-ly<-dist)	//鼠标往y反方向移动
			Rotate3D("L+");
	}
	else if( lx>-AtQuadHig && lx<AtQuadHig && ly>-AtFaceHig && ly<AtFaceHig && lz>AtFaceLow && lz<AtFaceHig &&
		bx>-AtQuadHig && bx<AtQuadHig && by>-AtFaceHig && by<AtFaceHig && bz>AtFaceLow && bz<AtFaceHig)		//鼠标在右面y方向滑动，中层	
	{
		if(by-ly>dist)			//鼠标往y方向移动
			Rotate3D("X+");
		else if(by-ly<-dist)	//鼠标往y反方向移动
			Rotate3D("X-");
	}
	else if( lx>AtQuadLow && lx<AtFaceHig && ly>-AtFaceHig && ly<AtFaceHig && lz>AtFaceLow && lz<AtFaceHig &&
		bx>AtQuadLow && bx<AtFaceHig && by>-AtFaceHig && by<AtFaceHig && bz>AtFaceLow && bz<AtFaceHig)		//鼠标在右面y方向滑动，右层	
	{
		if(by-ly>dist)			//鼠标往y方向移动
			Rotate3D("R+");
		else if(by-ly<-dist)	//鼠标往y反方向移动
			Rotate3D("R-");
	}
	else if( lx>-AtFaceHig && lx<AtFaceHig && ly>AtQuadLow && ly<AtFaceHig && lz>AtFaceLow && lz<AtFaceHig &&
		bx>-AtFaceHig && bx<AtFaceHig && by>AtQuadLow && by<AtFaceHig && bz>AtFaceLow && bz<AtFaceHig)		//鼠标在右面x方向滑动，顶层	
	{
		if(bx-lx>dist)			//鼠标往x方向移动
			Rotate3D("U-");
		else if(bx-lx<-dist)	//鼠标往x反方向移动
			Rotate3D("U+");
	}
	else if( lx>-AtFaceHig && lx<AtFaceHig && ly>-AtQuadHig && ly<AtQuadHig && lz>AtFaceLow && lz<AtFaceHig &&
		bx>-AtFaceHig && bx<AtFaceHig && by>-AtQuadHig && by<AtQuadHig && bz>AtFaceLow && bz<AtFaceHig)		//鼠标在右面x方向滑动，中层	
	{
		if(bx-lx>dist)			//鼠标往x方向移动
			Rotate3D("Y-");
		else if(bx-lx<-dist)	//鼠标往x反方向移动
			Rotate3D("Y+");
	}
	else if( lx>-AtFaceHig && lx<AtFaceHig && ly>-AtFaceHig && ly<-AtQuadLow && lz>AtFaceLow && lz<AtFaceHig &&
		bx>-AtFaceHig && bx<AtFaceHig && by>-AtFaceHig && by<-AtQuadLow && bz>AtFaceLow && bz<AtFaceHig)		//鼠标在右面x方向滑动，底层	
	{
		if(bx-lx>dist)			//鼠标往x方向移动
			Rotate3D("D+");
		else if(bx-lx<-dist)	//鼠标往x反方向移动
			Rotate3D("D-");
	}

	//------------------顶面y=1.49<<>>1.51---------------------------------------------


	else if( lx>-AtFaceHig && lx<AtFaceHig && ly>AtFaceLow && ly<AtFaceHig && lz>AtQuadLow && lz<AtFaceHig &&
		bx>-AtFaceHig && bx<AtFaceHig && by>AtFaceLow && by<AtFaceHig && bz>AtQuadLow && bz<AtFaceHig)		//鼠标在右面x方向滑动，前层	
	{
		if(bx-lx>dist)			//鼠标往x方向移动
			Rotate3D("F+");
		else if(bx-lx<-dist)	//鼠标往x反方向移动
			Rotate3D("F-");
	}
	else if( lx>-AtFaceHig && lx<AtFaceHig && ly>AtFaceLow && ly<AtFaceHig && lz>-AtQuadHig && lz<AtQuadHig &&
		bx>-AtFaceHig && bx<AtFaceHig && by>AtFaceLow && by<AtFaceHig && bz>-AtQuadHig && bz<AtQuadHig)		//鼠标在右面x方向滑动，中层	
	{
		if(bx-lx>dist)			//鼠标往x方向移动
			Rotate3D("Z+");
		else if(bx-lx<-dist)	//鼠标往x反方向移动
			Rotate3D("Z-");
	}
	else if( lx>-AtFaceHig && lx<AtFaceHig && ly>AtFaceLow && ly<AtFaceHig && lz>-AtFaceHig && lz<-AtQuadLow &&
		bx>-AtFaceHig && bx<AtFaceHig && by>AtFaceLow && by<AtFaceHig && bz>-AtFaceHig && bz<-AtQuadLow)		//鼠标在右面x方向滑动，后层	
	{
		if(bx-lx>dist)			//鼠标往x方向移动
			Rotate3D("B-");
		else if(bx-lx<-dist)	//鼠标往x反方向移动
			Rotate3D("B+");
	}
	else if( lx>AtQuadLow && lx<AtFaceHig && ly>AtFaceLow && ly<AtFaceHig && lz>-AtFaceHig && lz<AtFaceHig &&
		bx>AtQuadLow && bx<AtFaceHig && by>AtFaceLow && by<AtFaceHig && bz>-AtFaceHig && bz<AtFaceHig)		//鼠标在右面z方向滑动，右层	
	{
		if(bz-lz>dist)			//鼠标往z方向移动
			Rotate3D("R-");
		else if(bz-lz<-dist)	//鼠标往z反方向移动
			Rotate3D("R+");
	}
	else if( lx>-AtQuadHig && lx<AtQuadHig && ly>AtFaceLow && ly<AtFaceHig && lz>-AtFaceHig && lz<AtFaceHig &&
		bx>-AtQuadHig && bx<AtQuadHig && by>AtFaceLow && by<AtFaceHig && bz>-AtFaceHig && bz<AtFaceHig)		//鼠标在右面z方向滑动，中层	
	{
		if(bz-lz>dist)			//鼠标往z方向移动
			Rotate3D("X-");
		else if(bz-lz<-dist)	//鼠标往z反方向移动
			Rotate3D("X+");
	}
	else if( lx>-AtFaceHig && lx<-AtQuadLow && ly>AtFaceLow && ly<AtFaceHig && lz>-AtFaceHig && lz<AtFaceHig &&
		bx>-AtFaceHig && bx<-AtQuadLow && by>AtFaceLow && by<AtFaceHig && bz>-AtFaceHig && bz<AtFaceHig)		//鼠标在右面z方向滑动，左层	
	{
		if(bz-lz>dist)			//鼠标往z方向移动
			Rotate3D("L+");
		else if(bz-lz<-dist)	//鼠标往z反方向移动
			Rotate3D("L-");
	}

	//--------------------------------------后面z=-1.51<<>>-1.49---------------------------------------------

	else if( lx>-AtFaceHig && lx<AtFaceHig && ly>AtQuadLow && ly<AtFaceHig && lz>-AtFaceHig && lz<-AtFaceLow &&
		bx>-AtFaceHig && bx<AtFaceHig && by>AtQuadLow && by<AtFaceHig && bz>-AtFaceHig && bz<-AtFaceLow)		//鼠标在右面x方向滑动，顶层	
	{
		if(bx-lx>dist)			//鼠标往x方向移动
			Rotate3D("U+");
		else if(bx-lx<-dist)	//鼠标往x反方向移动
			Rotate3D("U-");
	}
	else if( lx>-AtFaceHig && lx<AtFaceHig && ly>-AtQuadHig && ly<AtQuadHig && lz>-AtFaceHig && lz<-AtFaceLow &&
		bx>-AtFaceHig && bx<AtFaceHig && by>-AtQuadHig && by<AtQuadHig && bz>-AtFaceHig && bz<-AtFaceLow)		//鼠标在右面x方向滑动，中层	
	{
		if(bx-lx>dist)			//鼠标往x方向移动
			Rotate3D("Y+");
		else if(bx-lx<-dist)	//鼠标往x反方向移动
			Rotate3D("Y-");
	}
	if( lx>-AtFaceHig && lx<AtFaceHig && ly>-AtFaceHig && ly<-AtQuadLow && lz>-AtFaceHig && lz<-AtFaceLow &&
		bx>-AtFaceHig && bx<AtFaceHig && by>-AtFaceHig && by<-AtQuadLow && bz>-AtFaceHig && bz<-AtFaceLow)	//鼠标在右面x方向滑动，底层	
	{
		if(bx-lx>dist)			//鼠标往x方向移动
			Rotate3D("D-");
		else if(bx-lx<-dist)	//鼠标往x反方向移动
			Rotate3D("D+");
	}
	else if( lx>AtQuadLow && lx<AtFaceHig && ly>-AtFaceHig && ly<AtFaceHig && lz>-AtFaceHig && lz<-AtFaceLow &&
		bx>AtQuadLow && bx<AtFaceHig && by>-AtFaceHig && by<AtFaceHig && bz>-AtFaceHig && bz<-AtFaceLow)		//鼠标在右面y方向滑动，左层	
	{
		if(by-ly>dist)			//鼠标往y方向移动
			Rotate3D("R-");
		else if(by-ly<-dist)	//鼠标往y反方向移动
			Rotate3D("R+");
	}
	else if( lx>-AtQuadHig && lx<AtQuadHig && ly>-AtFaceHig && ly<AtFaceHig && lz>-AtFaceHig && lz<-AtFaceLow &&
		bx>-AtQuadHig && bx<AtQuadHig && by>-AtFaceHig && by<AtFaceHig && bz>-AtFaceHig && bz<-AtFaceLow)		//鼠标在右面y方向滑动，中层	
	{
		if(by-ly>dist)			//鼠标往y方向移动
			Rotate3D("X-");
		else if(by-ly<-dist)	//鼠标往y反方向移动
			Rotate3D("X+");
	}
	else if( lx>-AtFaceHig && lx<-AtQuadLow && ly>-AtFaceHig && ly<AtFaceHig && lz>-AtFaceHig && lz<-AtFaceLow &&
		bx>-AtFaceHig && bx<-AtQuadLow && by>-AtFaceHig && by<AtFaceHig && bz>-AtFaceHig && bz<-AtFaceLow)	//鼠标在右面y方向滑动，右层	
	{
		if(by-ly>dist)			//鼠标往y方向移动
			Rotate3D("L+");
		else if(by-ly<-dist)	//鼠标往y反方向移动
			Rotate3D("L-");
	}
	//------------------左面x=-1.51<<>>-1.49---------------------------------------------
	else if( lx>-AtFaceHig && lx<-AtFaceLow && ly>AtQuadLow && ly<AtFaceHig && lz>-AtFaceHig && lz<AtFaceHig &&
		bx>-AtFaceHig && bx<-AtFaceLow && by>AtQuadLow && by<AtFaceHig && bz>-AtFaceHig && bz<AtFaceHig)		//鼠标在右面z方向滑动，顶层	
	{
		if(bz-lz>dist)			//鼠标往z方向移动
			Rotate3D("U-");
		else if(bz-lz<-dist)	//鼠标往z反方向移动
			Rotate3D("U+");
	}
	else if( lx>-AtFaceHig && lx<-AtFaceLow && ly>-AtQuadHig && ly<AtQuadHig && lz>-AtFaceHig && lz<AtFaceHig &&
		bx>-AtFaceHig && bx<-AtFaceLow && by>-AtQuadHig && by<AtQuadHig && bz>-AtFaceHig && bz<AtFaceHig)		//鼠标在右面z方向滑动，中层	
	{
		if(bz-lz>dist)			//鼠标往z方向移动
			Rotate3D("Y-");
		else if(bz-lz<-dist)	//鼠标往z反方向移动
			Rotate3D("Y+");
	}
	else if( lx>-AtFaceHig && lx<-AtFaceLow && ly>-AtFaceHig && ly<-AtQuadLow && lz>-AtFaceHig && lz<AtFaceHig &&
		bx>-AtFaceHig && bx<-AtFaceLow && by>-AtFaceHig && by<-AtQuadLow && bz>-AtFaceHig && bz<AtFaceHig)	//鼠标在右面z方向滑动，底层	
	{
		if(bz-lz>dist)			//鼠标往z方向移动
			Rotate3D("D+");
		else if(bz-lz<-dist)	//鼠标往z反方向移动
			Rotate3D("D-");
	}
	else if( lx>-AtFaceHig && lx<-AtFaceLow && ly>-AtFaceHig && ly<AtFaceHig && lz>-AtFaceHig && lz<-AtQuadLow &&
		bx>-AtFaceHig && bx<-AtFaceLow && by>-AtFaceHig && by<AtFaceHig && bz>-AtFaceHig && bz<-AtQuadLow)	//鼠标在右面y方向滑动，左层	
	{
		if(by-ly>dist)			//鼠标往y方向移动
			Rotate3D("B-");
		else if(by-ly<-dist)	//鼠标往y反方向移动
			Rotate3D("B+");
	}
	else if( lx>-AtFaceHig && lx<-AtFaceLow && ly>-AtFaceHig && ly<AtFaceHig && lz>-AtQuadHig && lz<AtQuadHig &&
		bx>-AtFaceHig && bx<-AtFaceLow && by>-AtFaceHig && by<AtFaceHig && bz>-AtQuadHig && bz<AtQuadHig)		//鼠标在右面y方向滑动，中层	
	{
		if(by-ly>dist)			//鼠标往y方向移动
			Rotate3D("Z+");
		else if(by-ly<-dist)	//鼠标往y反方向移动
			Rotate3D("Z-");
	}
	else if( lx>-AtFaceHig && lx<-AtFaceLow && ly>-AtFaceHig && ly<AtFaceHig && lz>AtQuadLow && lz<AtFaceHig &&
		bx>-AtFaceHig && bx<-AtFaceLow && by>-AtFaceHig && by<AtFaceHig && bz>AtQuadLow && bz<AtFaceHig)		//鼠标在右面y方向滑动，中层	
	{
		if(by-ly>dist)			//鼠标往y方向移动
			Rotate3D("F+");
		else if(by-ly<-dist)	//鼠标往y反方向移动
			Rotate3D("F-");
	}
	//------------------底面y=-1.51<<>>-1.49---------------------------------------------
	else if( lx>-AtFaceHig && lx<-AtQuadLow && ly>-AtFaceHig && ly<-AtFaceLow && lz>-AtFaceHig && lz<AtFaceHig &&
		bx>-AtFaceHig && bx<-AtQuadLow && by>-AtFaceHig && by<-AtFaceLow && bz>-AtFaceHig && bz<AtFaceHig)	//鼠标在右面z方向滑动，左层	
	{
		if(bz-lz>dist)			//鼠标往z方向移动
			Rotate3D("L-");
		else if(bz-lz<-dist)	//鼠标往z反方向移动
			Rotate3D("L+");
	}
	else if( lx>-AtQuadHig && lx<AtQuadHig && ly>-AtFaceHig && ly<-AtFaceLow && lz>-AtFaceHig && lz<AtFaceHig &&
		bx>-AtQuadHig && bx<AtQuadHig && by>-AtFaceHig && by<-AtFaceLow && bz>-AtFaceHig && bz<AtFaceHig)		//鼠标在右面z方向滑动，中层	
	{
		if(bz-lz>dist)			//鼠标往z方向移动
			Rotate3D("X+");
		else if(bz-lz<-dist)	//鼠标往z反方向移动
			Rotate3D("X-");
	}
	else if( lx>AtQuadLow && lx<AtFaceHig && ly>-AtFaceHig && ly<-AtFaceLow && lz>-AtFaceHig && lz<AtFaceHig &&
		bx>AtQuadLow && bx<AtFaceHig && by>-AtFaceHig && by<-AtFaceLow && bz>-AtFaceHig && bz<AtFaceHig)		//鼠标在右面z方向滑动，右层	
	{
		if(bz-lz>dist)			//鼠标往z方向移动
			Rotate3D("R+");
		else if(bz-lz<-dist)	//鼠标往z反方向移动
			Rotate3D("R-");
	}
	else if( lx>-AtFaceHig && lx<AtFaceHig && ly>-AtFaceHig && ly<-AtFaceLow && lz>AtQuadLow && lz<AtFaceHig &&
		bx>-AtFaceHig && bx<AtFaceHig && by>-AtFaceHig && by<-AtFaceLow && bz>AtQuadLow && bz<AtFaceHig)		//鼠标在右面x方向滑动，上层	
	{
		if(bx-lx>dist)			//鼠标往x方向移动
			Rotate3D("F-");
		else if(bx-lx<-dist)	//鼠标往x反方向移动
			Rotate3D("F+");
	}
	else if( lx>-AtFaceHig && lx<AtFaceHig && ly>-AtFaceHig && ly<-AtFaceLow && lz>-AtQuadHig && lz<AtQuadHig &&
		bx>-AtFaceHig && bx<AtFaceHig && by>-AtFaceHig && by<-AtFaceLow && bz>-AtQuadHig && bz<AtQuadHig)		//鼠标在右面x方向滑动，中层	
	{
		if(bx-lx>dist)			//鼠标往x方向移动
			Rotate3D("Z-");
		else if(bx-lx<-dist)	//鼠标往x反方向移动
			Rotate3D("Z+");
	}
	else if( lx>-AtFaceHig && lx<AtFaceHig && ly>-AtFaceHig && ly<-AtFaceLow && lz>-AtFaceHig && lz<-AtQuadLow &&
		bx>-AtFaceHig && bx<AtFaceHig && by>-AtFaceHig && by<-AtFaceLow && bz>-AtFaceHig && bz<-AtQuadLow)	//鼠标在右面x方向滑动，下层	
	{
		if(bx-lx>dist)			//鼠标往x方向移动
			Rotate3D("B+");
		else if(bx-lx<-dist)	//鼠标往x反方向移动
			Rotate3D("B-");
	}
}
void CCubeWnd::Get3DCoord(Coord3D &lCoord, CPoint point)
{
	float fdepth; 
	GLdouble ObjectX, ObjectY, ObjectZ; 
	GLint iViewPort[4]; 
	GLdouble dProjMatrix[16]; 
	GLdouble dModelMatrix[16]; 
	
	glGetIntegerv(GL_VIEWPORT, iViewPort); 
	glGetDoublev(GL_MODELVIEW_MATRIX, dModelMatrix); 
	glGetDoublev(GL_PROJECTION_MATRIX, dProjMatrix); 
	glReadPixels(point.x, iViewPort[3]-point.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &fdepth);
	gluUnProject((GLdouble)point.x, (GLdouble)(iViewPort[3]-point.y), (GLdouble)fdepth, dModelMatrix, dProjMatrix, iViewPort, &ObjectX, &ObjectY, &ObjectZ);
	lCoord.x = ObjectX; 
	lCoord.y = ObjectY; 
	lCoord.z = ObjectZ; 
}

BOOL CCubeWnd::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CCubeWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	ReSizeGLScene(cx,cy);
}

void CCubeWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_LastPointR = point;
 //  SetCapture();
	CWnd::OnRButtonDown(nFlags, point);
}

void CCubeWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	CWnd::OnRButtonUp(nFlags, point);
}

//鼠标在窗体内移动处理
void CCubeWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_RBUTTON)		//按住右键实现旋转功能
	{
		SetCursor(LoadCursor(NULL, IDC_HAND));		//修改鼠标为手型
		m_RotateX += (point.y - m_LastPointR.y);
		int TempRotate = (int)m_RotateX;
		if (TempRotate< 0)
		{
			TempRotate += 360;
		}
		TempRotate = TempRotate % 360;
		if (TempRotate > 90 && TempRotate < 270)
		{
			m_RotateY -= (point.x - m_LastPointR.x);
		}
		else
		{
			m_RotateY += (point.x - m_LastPointR.x);
		}
			
		m_LastPointR = point;
		Invalidate();
		UpdateWindow();
	}
	if(nFlags & MK_LBUTTON)
	{
		Coord3D CurCoord;
		Get3DCoord(CurCoord, point);
		if(CurCoord.x > -1.6 && CurCoord.x < 1.6  && CurCoord.y > -1.6 && CurCoord.y < 1.6 && CurCoord.z > -1.6 && CurCoord.z < 1.6)
		{
			SetCursor(LoadCursor(NULL, IDC_HAND));		//修改鼠标为手型
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}

int CCubeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	InitGL();
	return 0;
}

void CCubeWnd::ReSizeGLScene( int width, int height)	// Resize初始化绘图窗口
{
	if (height==0)										
	{
		height=1;										
	}
	GLdouble nRange=15.0;	
	glViewport(0,0,width,height);						
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();									
	
	gluPerspective(45.0, (float)width/(float)height, 1, 100.0);
	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();									
}

int CCubeWnd::InitGL()							//初始化OpenGL
{
	m_hDC = ::GetDC(this->m_hWnd);
	if(!SetPixelformat(m_hDC))		//设置像素格式
	{
		::MessageBox(::GetFocus(), "设置像素格式失败!", "Error", MB_OK);
	}

	m_hRC = wglCreateContext(m_hDC);	// Create an OpenGL rendering context 
	int i= wglMakeCurrent(m_hDC,m_hRC);	// Make rendering context current

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.1f, 0.1f, 0.1f, 0.5f);				
	glClearDepth(1.0);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	GLfloat light_ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };		// faint white light
	GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0 };		// white diffuse
	GLfloat light_specular[] = { 0.35f, 0.35f, 0.35f, 1.0 };	// highlights
	GLfloat light_position[] = { 0.0, 10.0, 20.0, 1.0 };

	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
   
	CRect Rect;
	GetClientRect(&Rect);
	ReSizeGLScene(Rect.Width(),Rect.Height());
	return TRUE;
}
//来自网上
BOOL CCubeWnd::SetPixelformat(HDC hdc)		
{
    PIXELFORMATDESCRIPTOR *ppfd; 
    int pixelformat; 
 
    PIXELFORMATDESCRIPTOR pfd = { 
    sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
    1,                     // version number 
    PFD_DRAW_TO_WINDOW |   // support window 
    PFD_SUPPORT_OPENGL |   // support OpenGL 
    PFD_GENERIC_FORMAT |
    PFD_DOUBLEBUFFER,      // double buffered 
    PFD_TYPE_RGBA,         // RGBA type 
    32,                    // 32-bit color depth 
    0, 0, 0, 0, 0, 0,      // color bits ignored 
    8,                     // no alpha buffer 
    0,                     // shift bit ignored 
    8,                     // no accumulation buffer 
    0, 0, 0, 0,            // accum bits ignored 
    64,                    // 64-bit z-buffer	 
    8,                     // stencil buffer 
    8,                     // auxiliary buffer 
    PFD_MAIN_PLANE,        // main layer 
    0,                     // reserved 
    0, 0, 0                // layer masks ignored 
    }; 
    ppfd = &pfd;
 
    if ( (pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0 ) 
    { 
        ::MessageBox(NULL, "选择像素格式失败！", "Error", MB_OK); 
        return FALSE; 
    } 
 
    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE) 
    { 
        ::MessageBox(NULL, "设置像素格式失败！", "Error", MB_OK); 
        return FALSE; 
    } 
 
    return TRUE; 
}

void CCubeWnd::Reset()
{
	m_3DCube.Reset();
	Invalidate();
	UpdateWindow();
}


void CCubeWnd::SaveCube()
{
	m_3DCube.Save();
}

void CCubeWnd::RestoreCube()
{
	m_3DCube.Restore();
}

void CCubeWnd::Rotate3D(CString Rotrun)
{
	int RotateNum = 9;
	int angle = 90/RotateNum;
	CString RotateSolve;
	RotateSolve = Rotrun.Mid(0, 2);
	for(int i = 1; i <= RotateNum; i++)
	{
		if(RotateSolve == "F+")	m_3DCube.RotateZ(2, -angle);
		if(RotateSolve == "F-")	m_3DCube.RotateZ(2, angle);
		if(RotateSolve == "U+")	m_3DCube.RotateY(2, -angle);
		if(RotateSolve == "U-")	m_3DCube.RotateY(2, angle);

		if(RotateSolve == "R+")	m_3DCube.RotateX(2, -angle);
		if(RotateSolve == "R-")	m_3DCube.RotateX(2, angle);

		if(RotateSolve == "L+")	m_3DCube.RotateX(0, angle);		//L方向一致
		if(RotateSolve == "L-")	m_3DCube.RotateX(0, -angle);

		if(RotateSolve == "D+")	m_3DCube.RotateY(0, angle);		//D方向一致
		if(RotateSolve == "D-")	m_3DCube.RotateY(0, -angle);

		if(RotateSolve == "B+")	m_3DCube.RotateZ(0, angle);		//B方向一致
		if(RotateSolve == "B-")	m_3DCube.RotateZ(0, -angle);
		//增加中间块的旋转以保证真实性
		if(RotateSolve == "X+")	m_3DCube.RotateX(1, -angle);	
		if(RotateSolve == "X-")	m_3DCube.RotateX(1, angle);

		if(RotateSolve == "Y+")	m_3DCube.RotateY(1, -angle);	
		if(RotateSolve == "Y-")	m_3DCube.RotateY(1, angle);

		if(RotateSolve == "Z+")	m_3DCube.RotateZ(1, -angle);
		if(RotateSolve == "Z-")	m_3DCube.RotateZ(1, angle);
		Sleep(15);
		Invalidate();
		UpdateWindow();
	}
	if(m_3DCube.IsRestore())
	{
		KillTimer(1);
		::MessageBox(NULL, "魔方还原成功！", "完成", MB_OK);
	}
}

LRESULT CCubeWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case WM_MYTIMER:
		
		if(lParam == 1)	
		{
			timeCounter = 0;
			SetTimer(1, 1000, NULL);
		}
		if(lParam == 0) 
		{
			KillTimer(1);
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}
void CCubeWnd::CenterPosSolve()
{
	//将白色放在底面，蓝色放在前面，可以确定其余中心块颜色
	int PosWhite;
	int PosBlue;
	PosWhite = m_3DCube.GetFace(WHITE);
	if(PosWhite != 2)	//白色中心块放在底面
	{
		switch(PosWhite)
		{
		case 0:	
			Rotate3D("X+");
			Rotate3D("X+");
			break;
		case 1:
			Rotate3D("X-");
			break;
		case 3:
			Rotate3D("X+");
			break;
		case 4:
			Rotate3D("Z-");
			break;
		case 5:
			Rotate3D("Z+");
			break;
		}
	}
	PosBlue  = m_3DCube.GetFace(BLUE);
	if(PosBlue != 1)	//蓝色中心块放在前面
	{
		switch(PosBlue)
		{
		case 3:
			Rotate3D("Y-");
			Rotate3D("Y-");
			break;
		case 4:
			Rotate3D("Y-");
			break;
		case 5:
			Rotate3D("Y+");
			break;
		}
	}
}
void CCubeWnd::BottomCrossSolve()
{
	CenterPosSolve();	//中心块到位
	//底面十字
	int PosBlueWhite;
	int color;
	CString stemp;
	PosBlueWhite = m_3DCube.GetCurPos(12);	
	if(PosBlueWhite != 12)	
	{
		switch(PosBlueWhite)
		{
		case 6:	
			Rotate3D("F-");
			break;
		case 24:
			Rotate3D("F+");
			break;
		case 18:
			Rotate3D("F+");
			Rotate3D("F+");
			break;
		case 26:
			Rotate3D("U+");
			Rotate3D("F+");
			Rotate3D("F+");
			break;
		case 16:
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("F+");
			Rotate3D("F+");
			break;
		case 8:
			Rotate3D("U-");
			Rotate3D("F+");
			Rotate3D("F+");
			break;
		case 20:
			Rotate3D("R+");
			Rotate3D("F+");
			Rotate3D("R-");
			break;
		case 2:
			Rotate3D("L-");
			Rotate3D("F-");
			Rotate3D("L+");
			break;
		case 10:
			Rotate3D("B+");
			Rotate3D("B+");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("B-");
			Rotate3D("B-");
			Rotate3D("F+");
			Rotate3D("F+");
			break;
		case 22:
			Rotate3D("B+");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("B-");
			Rotate3D("F+");
			Rotate3D("F+");
			break;
		case 4:
			Rotate3D("L+");
			Rotate3D("U-");
			Rotate3D("L-");
			Rotate3D("F+");
			Rotate3D("F+");
			break;
		}
	}
	color = m_3DCube.GetFaceColor(1, 12);
	if(color != BLUE)
	{
		Rotate3D("F-");
		Rotate3D("R+");
		Rotate3D("U+");
		Rotate3D("R-");
		Rotate3D("F+");
		Rotate3D("F+");
	}
	int PosRedWhite;
	PosRedWhite = m_3DCube.GetCurPos(20);
	if(PosRedWhite != 20)
	{
		switch(PosRedWhite)
		{
		case 22:
			Rotate3D("R+");
			break;
		case 24:
			Rotate3D("R-");
			break;
		case 26:
			Rotate3D("R+");
			Rotate3D("R+");
			break;
		case 16:
			Rotate3D("U+");
			Rotate3D("R+");
			Rotate3D("R+");
			break;
		case 18:
			Rotate3D("U-");
			Rotate3D("R+");
			Rotate3D("R+");
			break;
		case 6:
			Rotate3D("L-");
			Rotate3D("U-");
			Rotate3D("U-");
			Rotate3D("L+");
			Rotate3D("R+");
			Rotate3D("R+");
			break;
		case 4:
			Rotate3D("L+");
			Rotate3D("U-");
			Rotate3D("U-");
			Rotate3D("L-");
			Rotate3D("R+");
			Rotate3D("R+");
			break;
		case 2:
			Rotate3D("L+");
			Rotate3D("L+");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("R+");
			Rotate3D("R+");
			Rotate3D("L-");
			Rotate3D("L-");
			break;
		case 8:
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("R+");
			Rotate3D("R+");
			break;
		case 10:
			Rotate3D("B+");
			Rotate3D("R+");
			Rotate3D("B-");
			break;
		}
	}

	color = m_3DCube.GetFaceColor(5, 20);
	if(color != RED)	//颜色不对，就需要再转
	{
		Rotate3D("R-");
		Rotate3D("B+");
		Rotate3D("U+");
		Rotate3D("B-");
		Rotate3D("R+");
		Rotate3D("R+");
	}

	int PosGreenWhite;	//绿白
	PosGreenWhite = m_3DCube.GetCurPos(10);
	if(PosGreenWhite != 10)
	{
		switch(PosGreenWhite)
		{
		case 16:
			Rotate3D("B+");
			Rotate3D("B+");
			break;
		case 22:
			Rotate3D("B-");
			break;
		case 4:
			Rotate3D("B+");
			break;
		case 8:
			Rotate3D("U+");
			Rotate3D("B+");
			Rotate3D("B+");
			break;
		case 18:
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("B+");
			Rotate3D("B+");
			break;
		case 26:
			Rotate3D("U-");
			Rotate3D("B+");
			Rotate3D("B+");
			break;
		case 2:
			Rotate3D("L+");
			Rotate3D("B+");
			Rotate3D("L-");
			break;
		case 6:
			Rotate3D("L-");
			Rotate3D("L-");
			Rotate3D("B+");
			break;
		case 24:
			Rotate3D("R+");
			Rotate3D("U-");
			Rotate3D("R-");
			Rotate3D("B+");
			Rotate3D("B+");
			break;
		}
	}

	color = m_3DCube.GetFaceColor(3, 10);
	if(color != GREEN)	//颜色不对，就需要再转
	{
		Rotate3D("B-");
		Rotate3D("L+");
		Rotate3D("U+");
		Rotate3D("L-");
		Rotate3D("B+");
		Rotate3D("B+");
	}

	int PosOrangeWhite;	//橙白
	PosOrangeWhite = m_3DCube.GetCurPos(2);

	
	if(PosOrangeWhite != 2)
	{
		switch(PosOrangeWhite)
		{
		case 4:
			Rotate3D("L-");
			break;
		case 6:
			Rotate3D("L+");
			break;
		case 8:
			Rotate3D("L+");
			Rotate3D("L+");
			break;
		case 16:
			Rotate3D("U-");
			Rotate3D("L+");
			Rotate3D("L+");
			break;
		case 18:
			Rotate3D("U+");
			Rotate3D("L+");
			Rotate3D("L+");
			break;
		case 26:
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("L+");
			Rotate3D("L+");
			break;
		case 22:
			Rotate3D("R-");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("R+");
			Rotate3D("L+");
			Rotate3D("L+");
			break;
		case 24:
			Rotate3D("R+");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("R-");
			Rotate3D("L+");
			Rotate3D("L+");
			break;
		}
	}
	color = m_3DCube.GetFaceColor(4, 2);
	if(color != ORANGE)	//颜色不对，就需要再转
	{
		Rotate3D("L-");
		Rotate3D("F+");
		Rotate3D("U+");
		Rotate3D("F-");
		Rotate3D("L+");
		Rotate3D("L+");
	}
}

void CCubeWnd::BottomCornerSolve()
{
	//底面复原
	int OrangeGreen;
	int color;
	OrangeGreen = m_3DCube.GetCurPos(1);	//橙绿角块位置
	if(OrangeGreen != 1)
	{
		switch(OrangeGreen)
		{
		case 3:
			Rotate3D("F+");
			Rotate3D("U+");
			Rotate3D("F-");
			Rotate3D("U+");
			Rotate3D("L+");
			Rotate3D("U-");
			Rotate3D("L-");
			break;
		case 7:
			Rotate3D("U+");
			Rotate3D("L+");
			Rotate3D("U-");
			Rotate3D("L-");
			break;
		case 9:
			Rotate3D("U-");
			Rotate3D("L+");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("L-");
			break;
		case 19:
			Rotate3D("R-");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("R+");
			Rotate3D("U-");
			Rotate3D("L+");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("L-");
			break;
		case 21:
			Rotate3D("R+");
			Rotate3D("U+");
			Rotate3D("R-");
			Rotate3D("U-");
			Rotate3D("L+");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("L-");
			break;
		case 25:
			Rotate3D("U+");
			Rotate3D("L+");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("L-");
			break;
		case 27:
			Rotate3D("L+");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("L-");
			break;
		}
	}
	color = m_3DCube.GetFaceColor(2, 1);	//获取底面颜色
	if(color != WHITE)
	{
		if(color == GREEN)
		{
			Rotate3D("L+");
			Rotate3D("U+");
			Rotate3D("L-");
			Rotate3D("U-");
			Rotate3D("U-");
			Rotate3D("B-");
			Rotate3D("U+");
			Rotate3D("B+");
		}
		if(color == ORANGE)
		{
			Rotate3D("B-");
			Rotate3D("U-");
			Rotate3D("B+");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("L+");
			Rotate3D("U-");
			Rotate3D("L-");
		}
	}
	int BlueOrange;
	BlueOrange = m_3DCube.GetCurPos(3);		//蓝橙角块位置
	if(BlueOrange != 3)
	{
		switch(BlueOrange)
		{
		case 7:
			Rotate3D("F+");
			Rotate3D("U-");
			Rotate3D("F-");
			break;
		case 9:
			Rotate3D("U+");
			Rotate3D("F+");
			Rotate3D("U-");
			Rotate3D("F-");
			break;
		case 19:
			Rotate3D("R-");
			Rotate3D("U-");
			Rotate3D("R+");
			Rotate3D("U+");
			Rotate3D("F+");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("F-");
			break;
		case 21:
			Rotate3D("R+");
			Rotate3D("U+");
			Rotate3D("R-");
			Rotate3D("U+");
			Rotate3D("F+");
			Rotate3D("U-");
			Rotate3D("F-");
			break;
		case 25:
			Rotate3D("F+");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("F-");
			break;
		case 27:
			Rotate3D("U-");
			Rotate3D("F+");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("F-");
			break;
		}
	}
	color = m_3DCube.GetFaceColor(2, 3);	//获取底面颜色
	if(color != WHITE)
	{
		if(color == BLUE)
		{
			Rotate3D("L-");
			Rotate3D("U-");
			Rotate3D("L+");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("F+");
			Rotate3D("U-");
			Rotate3D("F-");
		}
		if(color == ORANGE)
		{
			Rotate3D("F+");
			Rotate3D("U+");
			Rotate3D("F-");
			Rotate3D("U-");
			Rotate3D("U-");
			Rotate3D("L-");
			Rotate3D("U+");
			Rotate3D("L+");
		}
	}
	int RedBlue;
	RedBlue = m_3DCube.GetCurPos(21);		//红蓝角块位置
	if(RedBlue != 21)
	{
		switch(RedBlue)
		{
		case 7:
			Rotate3D("R+");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("R-");
			break;
		case 9:
			Rotate3D("R+");
			Rotate3D("U-");
			Rotate3D("R-");
			break;
		case 19:
			Rotate3D("R-");
			Rotate3D("U-");
			Rotate3D("R+");
			Rotate3D("U-");
			Rotate3D("R+");
			Rotate3D("U-");
			Rotate3D("R-");
			break;
		case 25:
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("R+");
			Rotate3D("U-");
			Rotate3D("R-");
			break;
		case 27:
			Rotate3D("U+");
			Rotate3D("R+");
			Rotate3D("U-");
			Rotate3D("R-");
			break;
		}
	}
	color = m_3DCube.GetFaceColor(2, 21);	//获取底面颜色
	if(color != WHITE)
	{
		if(color == RED)
		{
			Rotate3D("F-");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("F+");
			Rotate3D("U-");
			Rotate3D("R+");
			Rotate3D("U-");
			Rotate3D("R-");
		}
		if(color == BLUE)
		{
			Rotate3D("R+");
			Rotate3D("U+");
			Rotate3D("R-");
			Rotate3D("U-");
			Rotate3D("U-");
			Rotate3D("F-");
			Rotate3D("U+");
			Rotate3D("F+");
		}
	}
	int GreenRed;
	GreenRed = m_3DCube.GetCurPos(19);		//绿红角块
	if(GreenRed != 19)
	{
		switch(GreenRed)
		{
		case 7:
			Rotate3D("R-");
			Rotate3D("U+");
			Rotate3D("R+");
			break;
		case 9:
			Rotate3D("R-");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("R+");
			break;
		case 25:
			Rotate3D("U-");
			Rotate3D("R-");
			Rotate3D("U+");
			Rotate3D("R+");
			break;
		case 27:
			Rotate3D("U+");
			Rotate3D("R-");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("R+");
			break;
		}
	}
	color = m_3DCube.GetFaceColor(2, 19);
	if(color != WHITE)
	{
		if(color == GREEN)
		{
			Rotate3D("R-");
			Rotate3D("U+");
			Rotate3D("U+");
			Rotate3D("R+");
			Rotate3D("U-");
			Rotate3D("B+");
			Rotate3D("U-");
			Rotate3D("B-");
		}
		if(color == RED)
		{
			Rotate3D("B+");
			Rotate3D("U+");
			Rotate3D("B-");
			Rotate3D("U-");
			Rotate3D("U-");
			Rotate3D("R-");
			Rotate3D("U+");
			Rotate3D("R+");
		}
	}
}
void CCubeWnd::SecondMethod(int Pos)	
{
	//底角归位
	switch(Pos)
	{
	case 4:		//第4个
		Rotate3D("B-");
		Rotate3D("U+");
		Rotate3D("B+");
		Rotate3D("U+");
		Rotate3D("L+");
		Rotate3D("U-");
		Rotate3D("L-");
		break;
	case 6:
		Rotate3D("L-");
		Rotate3D("U+");
		Rotate3D("L+");
		Rotate3D("U+");
		Rotate3D("F+");
		Rotate3D("U-");
		Rotate3D("F-");
		break;
	case 24:
		Rotate3D("F-");
		Rotate3D("U+");
		Rotate3D("F+");
		Rotate3D("U+");
		Rotate3D("R+");
		Rotate3D("U-");
		Rotate3D("R-");
		break;
	case 22:
		Rotate3D("R-");
		Rotate3D("U+");
		Rotate3D("R+");
		Rotate3D("U+");
		Rotate3D("B+");
		Rotate3D("U-");
		Rotate3D("B-");
		break;
	}
}

void CCubeWnd::SecondSolve()
{
	//底棱归位
	
	int OrangeGreen;
	int color;
	OrangeGreen = m_3DCube.GetCurPos(4);	//二层棱块第1个，目标位置4
	if(OrangeGreen != 4)	//先到位
	{
		switch(OrangeGreen)
		{
		case 6:
			SecondMethod(6);	//不在顶层就要用固定方法先转到顶层
			Rotate3D("U+");
			Rotate3D("U+");
			SecondMethod(4);
			break;
		case 8:
			Rotate3D("U-");
			SecondMethod(4);
			break;
		case 16:
			Rotate3D("U+");	
			Rotate3D("U+");	
			SecondMethod(4);
			break;
		case 18:	//参考位置
			SecondMethod(4);
			break;
		case 22:
			SecondMethod(22);	//不在顶层就要用固定方法先转到顶层
			SecondMethod(4);
			break;
		case 24:
			SecondMethod(24);	//不在顶层就要用固定方法先转到顶层
			Rotate3D("U-");
			SecondMethod(4);
			break;
		case 26:
			Rotate3D("U+");
			SecondMethod(4);
			break;
		}
	}
	color = m_3DCube.GetFaceColor(4, 4);
	if(color != ORANGE)
	{
		SecondMethod(4);
		Rotate3D("U+");
		SecondMethod(4);
	}

	int BlueOrange;
	BlueOrange = m_3DCube.GetCurPos(6);	
	if(BlueOrange != 6)
	{
		switch(BlueOrange)
		{
		case 8:
			Rotate3D("U+");
			Rotate3D("U+");
			SecondMethod(6);
			break;
		case 16:
			Rotate3D("U+");
			SecondMethod(6);
			break;
		case 18:
			Rotate3D("U-");
			SecondMethod(6);
			break;
		case 22:
			SecondMethod(22);		//不在顶层就要用固定方法先转到顶层
			Rotate3D("U-");
			SecondMethod(6);
			break;
		case 24:
			SecondMethod(24);		//不在顶层就要用固定方法先转到顶层
			Rotate3D("U+");
			Rotate3D("U+");
			SecondMethod(6);
			break;
		case 26:	//参考位置
			SecondMethod(6);
			break;
		}
	}
	color = m_3DCube.GetFaceColor(1, 6);
	if(color != BLUE)
	{
		SecondMethod(6);
		Rotate3D("U+");
		SecondMethod(6);
	}

	int RedBlue;
	RedBlue = m_3DCube.GetCurPos(24);		//二层棱块第3个，目标位置24
	if(RedBlue != 24)
	{
		switch(RedBlue)
		{
		case 8:
			Rotate3D("U+");
			SecondMethod(24);
			break;
		case 16:
			SecondMethod(24);		//参考位置
			break;
		case 18:
			Rotate3D("U+");
			Rotate3D("U+");
			SecondMethod(24);
			break;
		case 22:
			SecondMethod(22);		//不在顶层就要用固定方法先转到顶层
			Rotate3D("U+");
			Rotate3D("U+");
			SecondMethod(24);
			break;
		case 26:
			Rotate3D("U-");
			SecondMethod(24);
			break;
		}
	}
	color = m_3DCube.GetFaceColor(5, 24);
	if(color != RED)
	{
		SecondMethod(24);
		Rotate3D("U+");
		SecondMethod(24);
	}

	int GreenRed;
	GreenRed = m_3DCube.GetCurPos(22);			//二层棱块第4个，目标位置22
	if(GreenRed != 22)
	{
		switch(GreenRed)
		{
		case 8:
			SecondMethod(22);		//参考位置
			break;
		case 16:
			Rotate3D("U-");
			SecondMethod(22);
			break;
		case 18:
			Rotate3D("U+");
			SecondMethod(22);
			break;
		case 26:
			Rotate3D("U+");
			Rotate3D("U+");
			SecondMethod(22);
			break;
		}
	}
	color = m_3DCube.GetFaceColor(3, 22);
	if(color != GREEN)
	{
		SecondMethod(22);
		Rotate3D("U+");
		SecondMethod(22);
	}		
}

void CCubeWnd::TopCrossFace()
{
	Rotate3D("F-");
	Rotate3D("U-");
	Rotate3D("L-");
	Rotate3D("U+");
	Rotate3D("L+");
	Rotate3D("F+");
}
void CCubeWnd::TopCrossFaceSolve()
{
	int YellowCross = 0;
	BOOL Pos8  = FALSE;
	BOOL Pos16 = FALSE;
	BOOL Pos18 = FALSE;
	BOOL Pos26 = FALSE;
	if(m_3DCube.GetFaceColor(0, 8) == YELLOW)
	{
		Pos8 = TRUE;
		YellowCross++;
	}
	if(m_3DCube.GetFaceColor(0, 16) == YELLOW)
	{
		Pos16 = TRUE;
		YellowCross++;
	}
	if(m_3DCube.GetFaceColor(0, 18) == YELLOW)
	{
		Pos18 = TRUE;
		YellowCross++;
	}
	if(m_3DCube.GetFaceColor(0, 26) == YELLOW)
	{
		Pos26 = TRUE;
		YellowCross++;
	}
	if(YellowCross == 0)	//一个黄色都没有
	{
		TopCrossFace();		//摆成一字，后中前为黄色
		TopCrossFace();		//摆成L字，左中前为黄色
		Rotate3D("U-");
		Rotate3D("U-");		//L字换方向，变成后中右为黄色
		TopCrossFace();		//摆成十字
	}
	if(YellowCross == 2)	//2个黄色可能会L字和一字
	{
		if(Pos8 && Pos18)	//L字，左后前，需转方向
		{
			Rotate3D("U-");
			Rotate3D("U-");
			TopCrossFace();	//摆十字
		}
		if(Pos16 && Pos26)
		{
			TopCrossFace();
		}
		if(Pos26 && Pos18)
		{
			Rotate3D("U-");
			TopCrossFace();
		}
		if(Pos8 && Pos16)
		{
			Rotate3D("U+");
			/*Rotate3D("F-");
			Rotate3D("U-");
			Rotate3D("L-");
			Rotate3D("U+");
			Rotate3D("L+");
			Rotate3D("F+");*/
			TopCrossFace();
		}
		if(Pos8 && Pos26)	//一字，先要转成L再转成十字
		{
			Rotate3D("U-");
			TopCrossFace();
			Rotate3D("U-");
			Rotate3D("U-");
			TopCrossFace();
		}
		if(Pos16 && Pos18)
		{
			TopCrossFace();
			Rotate3D("U-");
			Rotate3D("U-");
			TopCrossFace();
		}
	}
}

void CCubeWnd::TopCrossPos()
{
	//交换右面和后面棱块位置，顶面颜色保持黄色
	Rotate3D("F-");
	Rotate3D("U-");
	Rotate3D("F+");
	Rotate3D("U-");
	Rotate3D("F-");
	Rotate3D("U-");
	Rotate3D("U-");
	Rotate3D("F+");

	Rotate3D("U-");
}
void CCubeWnd::TopCrossPosSolve()//顶边面位
{
	int color18;
	int color16;
	int color26;
	int YellowOrange;
	YellowOrange = m_3DCube.GetCurPos(8);
	if(YellowOrange != 8)	//到位，也是以该位为参考方向
	{
		switch(YellowOrange)
		{
		case 16:
			Rotate3D("U-");
			break;
		case 18:
			Rotate3D("U+");
			break;
		case 26:
			Rotate3D("U+");
			Rotate3D("U+");
			break;
		}
	}
	color18 = m_3DCube.GetFaceColor(1, 18);
	color16 = m_3DCube.GetFaceColor(3, 16);
	color26 = m_3DCube.GetFaceColor(5, 26);
	if(color18 == BLUE && color16 == RED)	TopCrossPos();//顶层十字位的转法
	if(color18 == GREEN && color16 == BLUE)
	{
		Rotate3D("U+");
		//Rotate3D("F-");
		/*Rotate3D("U-");
		Rotate3D("L-");
		Rotate3D("U+");
		Rotate3D("L+");
		Rotate3D("F+")*/;
		TopCrossPos();
		Rotate3D("U-");
		Rotate3D("U-");
		TopCrossPos();
	}
	if(color18 == GREEN && color16 == RED)
	{
		Rotate3D("U+");
		Rotate3D("U+");
		TopCrossPos();
		Rotate3D("U-");
	}
	if(color18 == RED && color16 == GREEN)
	{
		Rotate3D("U-");
		TopCrossPos();
		Rotate3D("U+");
	}
	if(color18 == RED && color16 == BLUE)
	{
		TopCrossPos();
		Rotate3D("U-");
		TopCrossPos();
		Rotate3D("U+");
	}
}

void CCubeWnd::TopConer2Rotate(char Face) //顶层同侧对角旋转
{	
	//相对面改变
	//常规记忆为"R-U-R+U-R-U-U-R+","L+U+L-U+L+U+U+L-"
	switch(Face)
	{
	case 'F':
		Rotate3D("R-");
		Rotate3D("U-");
		Rotate3D("R+");
		Rotate3D("U-");
		Rotate3D("R-");
		Rotate3D("U-");
		Rotate3D("U-");
		Rotate3D("R+");

		Rotate3D("L+");
		Rotate3D("U+");
		Rotate3D("L-");
		Rotate3D("U+");
		Rotate3D("L+");
		Rotate3D("U+");
		Rotate3D("U+");
		Rotate3D("L-");
		break;
	case 'R':
		Rotate3D("B-");
		Rotate3D("U-");
		Rotate3D("B+");
		Rotate3D("U-");
		Rotate3D("B-");
		Rotate3D("U-");
		Rotate3D("U-");
		Rotate3D("B+");

		Rotate3D("F+");
		Rotate3D("U+");
		Rotate3D("F-");
		Rotate3D("U+");
		Rotate3D("F+");
		Rotate3D("U+");
		Rotate3D("U+");
		Rotate3D("F-");
		break;
	case 'L':
		Rotate3D("F-");
		Rotate3D("U-");
		Rotate3D("F+");
		Rotate3D("U-");
		Rotate3D("F-");
		Rotate3D("U-");
		Rotate3D("U-");
		Rotate3D("F+");

		Rotate3D("B+");
		Rotate3D("U+");
		Rotate3D("B-");
		Rotate3D("U+");
		Rotate3D("B+");
		Rotate3D("U+");
		Rotate3D("U+");
		Rotate3D("B-");
		break;
	case 'B':
		Rotate3D("L-");
		Rotate3D("U-");
		Rotate3D("L+");
		Rotate3D("U-");
		Rotate3D("L-");
		Rotate3D("U-");
		Rotate3D("U-");
		Rotate3D("L+");

		Rotate3D("R+");
		Rotate3D("U+");
		Rotate3D("R-");
		Rotate3D("U+");
		Rotate3D("R+");
		Rotate3D("U+");
		Rotate3D("U+");
		Rotate3D("R-");
		break;
	}
	
}
void CCubeWnd::TopConer3Change(char Face)////顶层3角交换、旋转
{
	//"L+R-U+L-U-R+U+L+U-L-"
	switch(Face)
	{
	case 'F':	//正面朝向自己
		Rotate3D("L+");
		Rotate3D("R-");
		Rotate3D("U+");
		Rotate3D("L-");
		Rotate3D("U-");
		Rotate3D("R+");
		Rotate3D("U+");
		Rotate3D("L+");
		Rotate3D("U-");
		Rotate3D("L-");
		break;
	case 'R':	//右面朝向自己
		Rotate3D("F+");
		Rotate3D("B-");
		Rotate3D("U+");
		Rotate3D("F-");
		Rotate3D("U-");
		Rotate3D("B+");
		Rotate3D("U+");
		Rotate3D("F+");
		Rotate3D("U-");
		Rotate3D("F-");
		break;
	case 'L':	//左面朝向自己
		Rotate3D("B+");
		Rotate3D("F-");
		Rotate3D("U+");
		Rotate3D("B-");
		Rotate3D("U-");
		Rotate3D("F+");
		Rotate3D("U+");
		Rotate3D("B+");
		Rotate3D("U-");
		Rotate3D("B-");
		break;
	case 'B':	//后面朝向自己
		Rotate3D("R+");
		Rotate3D("L-");
		Rotate3D("U+");
		Rotate3D("R-");
		Rotate3D("U-");
		Rotate3D("L+");
		Rotate3D("U+");
		Rotate3D("R+");
		Rotate3D("U-");
		Rotate3D("R-");
		break;
	}
}

int CCubeWnd::FindYellow()
{
	//找顶面为黄色的角块
	int repeater = 0;
	int result = 0;
	int OrgPos = 0;
	int TopConer = 0;
	int TopColor7, TopColor9, TopColor25, TopColor27;
ReFind:		TopColor7 = m_3DCube.GetFaceColor(0, 7);
	TopColor9 = m_3DCube.GetFaceColor(0, 9);
	TopColor25 = m_3DCube.GetFaceColor(0, 25);
	TopColor27 = m_3DCube.GetFaceColor(0, 27);
	if(TopColor7 == YELLOW)
	{
		TopConer++;
		result = 7;
		OrgPos = m_3DCube.GetOrgPos(7);
		if(OrgPos == 7) return 7;
	}
	if(TopColor9 == YELLOW)
	{
		TopConer++;
		result = 9;
		OrgPos = m_3DCube.GetOrgPos(9);
		if(OrgPos == 9) return 9;
	}
	if(TopColor25 == YELLOW)
	{
		TopConer++;
		result = 25;
		OrgPos = m_3DCube.GetOrgPos(25);
		if(OrgPos == 25) return 25;
	}
	if(TopColor27 == YELLOW)
	{
		TopConer++;
		result = 27;
		OrgPos = m_3DCube.GetOrgPos(27);
		if(OrgPos == 27) return 27;
	}
	if(TopConer == 0)
	{
		repeater++;
		TopConer3Change('L');	//如果用TopConer3Change方法没找到黄色，就换方向转一次
		if(repeater == 2)	TopConer2Rotate('R');	//如果已经用了TopConer3Change两次还是没找到就换TopConer2Rotate方法
		goto ReFind;	//用了goto
	}
	return result;
}

void CCubeWnd::TopLastSolve()
{
	//顶面四个角块
	int RefPos;
	int OrgPos;
	RefPos = FindYellow();
find:	OrgPos = m_3DCube.GetOrgPos(RefPos);	
	if(RefPos != OrgPos)
	{
		switch(RefPos)
		{
		case 7:
			TopConer3Change('B');	//顶面为黄色且没到位的需要放在右前位置继续转动
			RefPos = 25;			//转动后参考位置更新
			break;
		case 9:
			TopConer3Change('L');
			RefPos = 7;
			break;
		case 25:
			TopConer3Change('R');
			RefPos = 27;
			break;
		case 27:
			TopConer3Change('F');
			RefPos = 9;
			break;
		}
		goto find;	//为了定位，不得已用了goto
	}
	int cbleft;	
	int rbleft;
	switch(RefPos)
	{
	case 7:	//'F'
		cbleft = 9;
		break;
	case 9:	//'R'
		cbleft = 27;
		break;
	case 25: //'L'
		cbleft = 7;
		break;
	case 27: //'B'
		cbleft = 25;
		break;
	}
	rbleft = m_3DCube.GetOrgPos(cbleft);
	for(int i=0; i<2; i++)
	{
		if(cbleft != rbleft)
		{
			switch(cbleft)
			{
			case 7:
				TopConer3Change('L');
				rbleft = m_3DCube.GetOrgPos(cbleft);	//转动后重新获取该位置的方块原始位置信息
				break;
			case 9:
				TopConer3Change('F');
				rbleft = m_3DCube.GetOrgPos(cbleft);
				break;
			case 25:
				TopConer3Change('B');
				rbleft = m_3DCube.GetOrgPos(cbleft);
				break;
			case 27:
				TopConer3Change('R');
				rbleft = m_3DCube.GetOrgPos(cbleft);
				break;
			}
		}
		else break;
	}
	int backleftcolor;		//颜色自然是以顶面为准了
	int backleftpos;
	switch(RefPos)
	{
	case 7:		//对应TopConer2Rotate方法的面是'R'
		backleftpos = 9;
		break;
	case 9:		//'B'
		backleftpos = 27;
		break;
	case 25:	//'F'
		backleftpos = 7;
		break;
	case 27:	//'L'
		backleftpos = 25;
		break;
	}
	backleftcolor = m_3DCube.GetFaceColor(0, backleftpos);
	for(int m=0; m<2; m++)
	{
		if(backleftcolor != YELLOW)
		{
			switch(backleftpos)
			{
			case 7:
				TopConer2Rotate('F');
				backleftcolor = m_3DCube.GetFaceColor(0, backleftpos);	//转动后更新颜色信息
				break;
			case 9:
				TopConer2Rotate('R');
				backleftcolor = m_3DCube.GetFaceColor(0, backleftpos);
				break;
			case 25:
				TopConer2Rotate('L');
				backleftcolor = m_3DCube.GetFaceColor(0, backleftpos);
				break;
			case 27:
				TopConer2Rotate('B');
				backleftcolor = m_3DCube.GetFaceColor(0, backleftpos);
				break;
			}
		}
		else break;
	}
	int lastleftpos;
	int lastleftcolor;
	switch(RefPos)
	{
	case 7:		//'B'
		lastleftpos = 25;
		break;
	case 9:		//'L'
		lastleftpos = 7;
		break;
	case 25:	//'R'
		lastleftpos = 27;
		break;
	case 27:	//'F'
		lastleftpos = 9;
		break;
	}
	lastleftcolor = m_3DCube.GetFaceColor(0, lastleftpos);
	for(int j=0; j<2; j++)
	{
		if(lastleftcolor != YELLOW)
		{
			switch(lastleftpos)
			{
			case 7:
				TopConer2Rotate('L');
				lastleftcolor = m_3DCube.GetFaceColor(0, lastleftpos);
				break;
			case 9:
				TopConer2Rotate('F');
				lastleftcolor = m_3DCube.GetFaceColor(0, lastleftpos);
				break;
			case 25:
				TopConer2Rotate('B');
				lastleftcolor = m_3DCube.GetFaceColor(0, lastleftpos);
				break;
			case 27:
				TopConer2Rotate('R');
				lastleftcolor = m_3DCube.GetFaceColor(0, lastleftpos);
				break;
			}
		}
		else break;
	}
}

void CCubeWnd::DisplayTimer()
{
	HWND hwnd =  this->m_hWnd;
	HDC hdc= ::GetDC(hwnd);
	int hour = 0;
	int minute = 0;
	int second = 0;
	hour = timeCounter/3600;
	minute = timeCounter/60%60;
	second = timeCounter%60;
	
	CString stimer,stemp;
	if(hour > 24)
	{
		timeCounter = 0;
		DisplayTimer();
	}
	//小时定义
	if(hour < 10) 
	{
		stemp.Format("%d", hour);
		stemp = '0' + stemp;
	}
	else stemp.Format("%d", hour);
	stimer += stemp;
	stimer += ":";
	//分定义
	if(minute < 10) 
	{
		stemp.Format("%d", minute);
		stemp = '0' + stemp;
	}
	else stemp.Format("%d", minute);
	stimer += stemp;
	stimer += ":";
	//秒定义
	if(second < 10) 
	{
		stemp.Format("%d", second);
		stemp = '0' + stemp;
	}
	else stemp.Format("%d", second);
	stimer += stemp;

	SetBkColor(hdc, COLOR_WINDOWTEXT);
	SetTextColor(hdc, RGB(0,255,0));
	TextOut(hdc, 7, 7, stimer, strlen(stimer));
	::ReleaseDC(hwnd,hdc);
}
