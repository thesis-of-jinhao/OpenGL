#pragma once

#include "Cube.h"

class CCubeControl
{
public:
	CCubeControl(void);
	~CCubeControl(void);
	
	void Render();
	void RotateX(unsigned int block, int degrees);
	void RotateY(unsigned int block, int degrees);
	void RotateZ(unsigned int block, int degrees);
	BOOL IsSameFace(int Face, int color);	//Face����ɫ�Ƿ�ȫ��Ϊcolor
	BOOL IsRestore();	//ħ���Ƿ��Ѿ���ԭ
	
	void Reset();
	void Save();
	void Restore();
	int	 GetFaceColor(int Face, int Pos);	//��ȡPosλ�õķ���Face�����ɫ
	int  GetFace(int color);	//��ɫΪcolor�����Ŀ����ڵ���
	int	 GetCurPos(int OrgPos);	//����Ϊԭʼλ�ã����Ϊ��ǰλ��
	int  GetOrgPos(int CurPos);	//����Ϊ��ǰλ�ã����Ϊԭʼλ��

private:
	void InitDraw();
	CCube *m_Cubes[3][3][3];
	CCube *m_SavedCubes[3][3][3];
	float m_RotateX[3], m_RotateY[3], m_RotateZ[3];
	float m_CubeX, m_CubeY, m_CubeZ;
	int  FindCenterPos(int color);	//���ص�ǰ����λ�ã�����Ϊĳ�����Ŀ���ɫ�����Ŀ���Ҫ���ڸ��涨ɫ�ж�
};

