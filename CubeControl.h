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
	BOOL IsSameFace(int Face, int color);	//Face面颜色是否全部为color
	BOOL IsRestore();	//魔方是否已经还原
	
	void Reset();
	void Save();
	void Restore();
	int	 GetFaceColor(int Face, int Pos);	//获取Pos位置的方块Face面的颜色
	int  GetFace(int color);	//颜色为color的中心块所在的面
	int	 GetCurPos(int OrgPos);	//输入为原始位置，输出为当前位置
	int  GetOrgPos(int CurPos);	//输入为当前位置，输出为原始位置

private:
	void InitDraw();
	CCube *m_Cubes[3][3][3];
	CCube *m_SavedCubes[3][3][3];
	float m_RotateX[3], m_RotateY[3], m_RotateZ[3];
	float m_CubeX, m_CubeY, m_CubeZ;
	int  FindCenterPos(int color);	//返回当前所在位置，输入为某个中心块颜色，中心块主要用于该面定色判断
};

