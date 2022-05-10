#pragma once

#include "CubeControl.h"

// CCubeWnd

class CCubeWnd : public CWnd//���м̳�
{
	DECLARE_DYNAMIC(CCubeWnd)
public:
	CCubeWnd();
	virtual ~CCubeWnd();
	int InitGL();
	virtual void Reset();

	CCubeControl m_3DCube;
	void Rotate3D(CString Rotrun);
	void BottomCrossSolve();	//����ʮ��
	void BottomCornerSolve();	//����ǿ�
	void SecondSolve();			//�����λ
	void TopCrossFaceSolve();	//����ʮ��
	void TopCrossPosSolve();	//������λ
	void TopLastSolve();		//��β

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
	CPoint	m_LastPointR;	//��������Ҽ��ƶ���ת��ͼ
	float	m_RotateX;		//����3D��ͼ��ת
	float	m_RotateY;		//����3D��ͼ��ת
	HDC		m_hDC;			//������hDC
	HGLRC	m_hRC;			//����OpenGL��hRc
	
	void	RotateSide();	//��������ת
	void	ReSizeGLScene( int width, int height);		//OpenGL��ReSize����
	BOOL	SetPixelformat(HDC hdc);					//OpenGL�����ص�����
	struct Coord3D				//����һ����ά����ϵ
	{
		double x;
		double y;
		double z;
	};
	Coord3D	LdwCoord;			//����������ʱ����ά����
	Coord3D LupCoord;			//������̧��ʱ����ά����
	void	Get3DCoord(Coord3D &lCoord, CPoint point);	//��ȡ��ά����

	void CenterPosSolve();		//���Ŀ��λ����Ϊ�ף�ǰΪ������Ϊħ����ԭ�Ļ����ο�
	void SecondMethod(int Pos);	//�ڶ���Ĺ̶�ת��
	void TopCrossFace();		//����ʮ�����ת��
	void TopCrossPos();			//����ʮ��λ��ת����ǰ����ʮ���������
	void TopConer2Rotate(char Face);		//����ͬ��Խ���ת
	void TopConer3Change(char Face);		//����3�ǽ�������ת
	int  FindYellow();			//�Ҷ���ǿ鶥��Ϊ��ɫ�Ŀ�
	void DisplayTimer();
	int timeCounter;			//��ʱ��
};


