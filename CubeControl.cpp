#include "stdafx.h"
#include "CubeControl.h"
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
//在.h头文件中，我们声明了如下变量：
//CCube* m_Cubes[3][3][3];
//CCube* m_SavedCubes[3][3][3];
//float m_RotateX[3], m_RotateY[3], m_RotateZ[3];
//float m_CubeX, m_CubeY, m_CubeZ;	//旋转角度的控制
//定义了如下函数：
//public:
//	void Render();
//	void RotateX(unsigned int block, int degrees);
//	void RotateY(unsigned int block, int degrees);
//	void RotateZ(unsigned int block, int degrees);
//	BOOL IsSameFace(int Face, int color);	//Face面颜色是否全部为color
//	BOOL IsRestore();	//魔方是否已经还原
//
//	void Reset();
//	void Save();
//	void Restore();
//	int	 GetFaceColor(int Face, int Pos);	//获取Pos位置的方块Face面的颜色
//	int  GetFace(int color);	//颜色为color的中心块所在的面
//	int	 GetCurPos(int OrgPos);	//输入为原始位置，输出为当前位置
//	int  GetOrgPos(int CurPos);	//输入为当前位置，输出为原始位置
//private:
//	void InitDraw();	//位置及颜色信息的初始化
//	int  FindCenterPos(int color);	//返回当前所在位置，输入为某个中心块颜色，中心块主要用于该面定色判断

CCubeControl::CCubeControl(void)
:m_CubeX(0)
,m_CubeY(0)
,m_CubeZ(0)
{
	//初始化旋转角度为0
	for (int i = 0; i < 3;i++)
	{
		m_RotateX[i] = 0;
		m_RotateY[i] = 0;
		m_RotateZ[i] = 0;
	}
	for (int i = 0; i < 3; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			for (int k = 0; k < 3; k++) 
			{
				m_Cubes[i][j][k] = new CCube();
				m_SavedCubes[i][j][k] = new CCube();
			}
		}
	}
	InitDraw();
}

CCubeControl::~CCubeControl(void)//析构函数
{
	for (int i = 0; i < 3; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			for (int k = 0; k < 3; k++) 
			{
				delete m_Cubes[i][j][k];
				delete m_SavedCubes[i][j][k];
			}
		}
	}
}

void CCubeControl::InitDraw()
{
	for (int i = 0; i < 3; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			for (int k = 0; k < 3; k++) 
			{
				m_Cubes[i][j][k]->Position = i*9 + j*3 + k + 1;		//位置信息初始化
				m_Cubes[i][j][k]->SetFaceColor();					//颜色信息初始化
			}
		}
	}
}

void CCubeControl::Render()
{
	//旋转角度在本函数内部控制，旋转函数改变的是m_Rotate
	glRotatef(m_CubeX, 1, 0, 0);
	glRotatef(m_CubeY, 0, 1, 0);
	glRotatef(m_CubeZ, 0, 0, 1);

	//循环绘制Cube
	for (int i = 0; i < 3; i++) 
	{
		glRotatef(m_RotateX[i], 1, 0, 0);
		for (int j = 0; j < 3; j++) 
		{
			glRotatef(m_RotateY[j], 0, 1, 0);
			for (int k = 0; k < 3; k++) 
			{
				glRotatef(m_RotateZ[k], 0, 0, 1);
				glTranslatef(i-1.0f, j-1.0f, k-1.0f);
				m_Cubes[i][j][k]->Render();
				glTranslatef(1.0f-i, 1.0f-j, 1.0f-k);

				glRotatef(-m_RotateZ[k], 0, 0, 1);
			}
			glRotatef(-m_RotateY[j], 0, 1, 0);
		}
		glRotatef(-m_RotateX[i], 1, 0, 0);
	}
	glRotatef(-m_CubeZ, 0, 0, 1);
	glRotatef(-m_CubeY, 0, 1, 0);
	glRotatef(-m_CubeX, 1, 0, 0);
}

void CCubeControl::RotateX(unsigned int block, int degrees)
{
		m_RotateX[block] += degrees;
   
	if (degrees < 0 && m_RotateX[block] == -90)		//旋转角度达到90度就要更新位置信息
	{
		m_RotateX[block] = 0;						//旋转90度后，必须清除角度，否则在其他轴旋转时会再次旋转，魔方模型就错误了
		CCube *tempcube = m_Cubes[block][0][0];
		m_Cubes[block][0][0] = m_Cubes[block][2][0];
		m_Cubes[block][2][0] = m_Cubes[block][2][2];
		m_Cubes[block][2][2] = m_Cubes[block][0][2];
		m_Cubes[block][0][2] = tempcube;
    
		tempcube = m_Cubes[block][1][0];
	  
		m_Cubes[block][1][0] = m_Cubes[block][2][1];
		m_Cubes[block][2][1] = m_Cubes[block][1][2];
		m_Cubes[block][1][2] = m_Cubes[block][0][1];
		m_Cubes[block][0][1] = tempcube;

		for (int j = 0; j < 3; j++) 
		{
			for (int k = 0; k < 3; k++) 
			{
				m_Cubes[block][j][k]->RotateFaceRV(1);	//-90度对应反向旋转面
			}
		}
	} 
	else if (degrees > 0 && m_RotateX[block] == 90)		//正向
	{
		m_RotateX[block] = 0;
    
		CCube *tempcube = m_Cubes[block][0][2];
		m_Cubes[block][0][2] = m_Cubes[block][2][2];
		m_Cubes[block][2][2] = m_Cubes[block][2][0];
		m_Cubes[block][2][0] = m_Cubes[block][0][0];
		m_Cubes[block][0][0] = tempcube;

		tempcube = m_Cubes[block][0][1];
		m_Cubes[block][0][1] = m_Cubes[block][1][2];
		m_Cubes[block][1][2] = m_Cubes[block][2][1];
		m_Cubes[block][2][1] = m_Cubes[block][1][0];
		m_Cubes[block][1][0] = tempcube;
		for (int j = 0; j < 3; j++) 
		{
			for (int k = 0; k < 3; k++) 
			{
				m_Cubes[block][j][k]->RotateFaceFW(1);	//90度对应正向旋转面
			}
		}
	}

	if ( (m_RotateX[0] == m_RotateX[1] ||
         m_RotateX[0] == m_RotateX[1]-90 ||
         m_RotateX[0] == m_RotateX[1]+90) &&
        (m_RotateX[1] == m_RotateX[2] ||
         m_RotateX[1] == m_RotateX[2]-90 ||
         m_RotateX[1] == m_RotateX[2]+90)) 
	{
		m_CubeX += m_RotateX[0];
		m_RotateX[0] = m_RotateX[1] = m_RotateX[2] = 0;	//旋转角度复位为0
	}
}

void CCubeControl::RotateY(unsigned int block, int degrees)
{
	m_RotateY[block] += degrees;

	if (degrees < 0 && m_RotateY[block] == -90) 
	{
		m_RotateY[block] = 0;
       
		CCube *tempcube = m_Cubes[0][block][2];
		m_Cubes[0][block][2] = m_Cubes[2][block][2];
		m_Cubes[2][block][2] = m_Cubes[2][block][0];
		m_Cubes[2][block][0] = m_Cubes[0][block][0];
		m_Cubes[0][block][0] = tempcube;
                                                
		tempcube = m_Cubes[0][block][1];
		m_Cubes[0][block][1] = m_Cubes[1][block][2];
		m_Cubes[1][block][2] = m_Cubes[2][block][1];
		m_Cubes[2][block][1] = m_Cubes[1][block][0];
		m_Cubes[1][block][0] = tempcube;
       
		for (int i = 0; i < 3; i++) 
		{
			for (int k = 0; k < 3; k++) 
			{
				m_Cubes[i][block][k]->RotateFaceRV(2);
			}
		}
	} 
	else if (degrees > 0 && m_RotateY[block] == 90) 
	{
		m_RotateY[block] = 0;
       
		CCube *tempcube = m_Cubes[0][block][0];
		m_Cubes[0][block][0] = m_Cubes[2][block][0];
		m_Cubes[2][block][0] = m_Cubes[2][block][2];
		m_Cubes[2][block][2] = m_Cubes[0][block][2];
		m_Cubes[0][block][2] = tempcube;
       
		tempcube = m_Cubes[1][block][0];
		m_Cubes[1][block][0] = m_Cubes[2][block][1];
		m_Cubes[2][block][1] = m_Cubes[1][block][2];
		m_Cubes[1][block][2] = m_Cubes[0][block][1];
		m_Cubes[0][block][1] = tempcube;
       
		for (int i = 0; i < 3; i++) 
		{
			for (int k = 0; k < 3; k++) 
			{
				m_Cubes[i][block][k]->RotateFaceFW(2);
			}
		}
	}

	if ( (m_RotateY[0] == m_RotateY[1] ||
         m_RotateY[0] == m_RotateY[1]-90 ||
         m_RotateY[0] == m_RotateY[1]+90) &&
        (m_RotateY[1] == m_RotateY[2] ||
         m_RotateY[1] == m_RotateY[2]-90 ||
         m_RotateY[1] == m_RotateY[2]+90)) 
	{
		m_CubeY += m_RotateY[0];
		m_RotateY[0] = m_RotateY[1] = m_RotateY[2] = 0;
	}
}

void CCubeControl::RotateZ(unsigned int block, int degrees) 
{
	m_RotateZ[block] += degrees;

	if (degrees < 0 && m_RotateZ[block] == -90) 
	{
		m_RotateZ[block] = 0;
       
		CCube *tmpcube = m_Cubes[0][0][block];
		m_Cubes[0][0][block]   = m_Cubes[2][0][block];
		m_Cubes[2][0][block]   = m_Cubes[2][2][block];
		m_Cubes[2][2][block]   = m_Cubes[0][2][block];
		m_Cubes[0][2][block]   = tmpcube;
       
		tmpcube            = m_Cubes[1][0][block];
		m_Cubes[1][0][block] = m_Cubes[2][1][block];
		m_Cubes[2][1][block] = m_Cubes[1][2][block];
		m_Cubes[1][2][block] = m_Cubes[0][1][block];
		m_Cubes[0][1][block] = tmpcube;
      
		for (int i = 0; i < 3; i++) 
		{
			for (int j = 0; j < 3; j++) 
			{
				m_Cubes[i][j][block]->RotateFaceRV(3);
			}
		}
	} 
	else if (degrees > 0 && m_RotateZ[block] == 90) 
	{
		m_RotateZ[block] = 0;
       
		CCube *tmpcube = m_Cubes[0][2][block];
		m_Cubes[0][2][block] = m_Cubes[2][2][block];
		m_Cubes[2][2][block] = m_Cubes[2][0][block];
		m_Cubes[2][0][block] = m_Cubes[0][0][block];
		m_Cubes[0][0][block] = tmpcube;
                                                
		tmpcube = m_Cubes[0][1][block];
		m_Cubes[0][1][block] = m_Cubes[1][2][block];
		m_Cubes[1][2][block] = m_Cubes[2][1][block];
		m_Cubes[2][1][block] = m_Cubes[1][0][block];
		m_Cubes[1][0][block] = tmpcube;
       
		for (int i = 0; i < 3; i++) 
		{
			for (int j = 0; j < 3; j++) 
			{
				m_Cubes[i][j][block]->RotateFaceFW(3);
			}
		}
	}

	if ( (m_RotateZ[0] == m_RotateZ[1] ||
         m_RotateZ[0] == m_RotateZ[1]-90 ||
         m_RotateZ[0] == m_RotateZ[1]+90) &&
        (m_RotateZ[1] == m_RotateZ[2] ||
         m_RotateZ[1] == m_RotateZ[2]-90 ||
         m_RotateZ[1] == m_RotateZ[2]+90)) 
	{
		m_CubeZ += m_RotateZ[0];
		m_RotateZ[0] = m_RotateZ[1] = m_RotateZ[2] = 0;
	}
}

void CCubeControl::Reset()
{
	InitDraw();
}

void CCubeControl::Save()
{
	for (int i = 0; i < 3; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			for (int k = 0; k < 3; k++) 
			{
				m_SavedCubes[i][j][k]->Copy(m_Cubes[i][j][k]);
			}
		}
	}
}

void CCubeControl::Restore()
{
	for (int i = 0; i < 3; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			for (int k = 0; k < 3; k++) 
			{
				m_Cubes[i][j][k]->Copy(m_SavedCubes[i][j][k]);
			}
		}
	}
}

int CCubeControl::FindCenterPos(int color)	
{
	int pos = 0;
	int atpos = 0;

	switch(color)
	{
	case YELLOW:
		pos = 17;	break;
	case BLUE:
		pos = 15;	break;
	case ORANGE:
		pos = 5;	break;
	case RED:
		pos = 23;	break;
	case GREEN:
		pos = 13;	break;
	case WHITE:
		pos = 11;	break;
	}
		
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			for(int k=0; k<3; k++)
			{
				if(m_Cubes[i][j][k]->Position == pos)
					atpos = i*9 + j*3 + k + 1;	//当前所在位置
			}
	return atpos;
}

int CCubeControl::GetFace(int color)
{
	int face;
	int pos = FindCenterPos(color);		//先确定color颜色中心块当前在哪个位置
	switch(pos)
	{
	case 5:
		face = 4;	break;
	case 11:
		face = 2;	break;
	case 13:
		face = 3;	break;
	case 15:
		face = 1;	break;
	case 17:
		face = 0;	break;
	case 23:
		face = 5;	break;
	}
	return face;		//再确定color颜色中心块当前位置所在面
}

BOOL CCubeControl::IsSameFace(int Face, int color)	//确定color颜色中心块当前所在面的颜色是否全部一样
{
	BOOL result;
	result = FALSE;
	int nCount = 0;
	switch(Face)
	{
	case 0:		//顶面,[i][2][k]
		{
			for(int i=0; i<3; i++)
				for(int k=0; k<3; k++)
				{
					if(m_Cubes[i][2][k]->FaceColor[Face] == color )
						nCount++;				//开始用result=TRUE;犯了逻辑错误
				}
			if(nCount == 9)	result = TRUE;		//9个方块颜色全部相等
			break;
		}
	case 1: 	//前面,[i][j][2]
		{
			for(int i=0; i<3; i++)
				for(int j=0; j<3; j++)
				{
					if(m_Cubes[i][j][2]->FaceColor[Face] == color)
						nCount++;
				}
			if(nCount == 9)	result = TRUE;
			break;
		}
	case 2:		//底面,[i][0][k]
		{
			for(int i=0; i<3; i++)
				for(int k=0; k<3; k++)
				{
					if(m_Cubes[i][0][k]->FaceColor[Face] == color)
						nCount++;
				}
			if(nCount == 9)	result = TRUE;
			break;
		}
	case 3:		//后面,[i][j][0]
		{
			for(int i=0; i<3; i++)
				for(int j=0; j<3; j++)
				{
					if(m_Cubes[i][j][0]->FaceColor[Face] == color)
						nCount++;
				}
			if(nCount == 9)	result = TRUE;
			break;
		}
	case 4:		//左面,[0][j][k]
		{
			for(int j=0; j<3; j++)
				for(int k=0; k<3; k++)
				{
					if(m_Cubes[0][j][k]->FaceColor[Face] == color)
						nCount++;
				}
			if(nCount == 9)	result = TRUE;
			break;
		}
	case 5:		//右面,[2][j][k]
		{
			for(int j=0; j<3; j++)
				for(int k=0; k<3; k++)
				{
					if(m_Cubes[2][j][k]->FaceColor[Face] == color)
						nCount++;
				}
			if(nCount == 9)	result = TRUE;
			break;
		}
	}
	return result;
}

BOOL CCubeControl::IsRestore()
{
	BOOL result;
	result = FALSE;
	int nCount = 0;
	int face;
	for(int color=1; color<7; color++)
	{
		face = GetFace(color);
		if(IsSameFace(face, color))	
			nCount++;
		if(nCount == 6)	result = TRUE;
	}
	return result;
}

int CCubeControl::GetCurPos(int OldPos)
{
	int result;
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			for(int k=0; k<3; k++)
			{
				if(m_Cubes[i][j][k]->Position == OldPos)
				{
					result = i*9 + j*3 + k + 1;
				}
			}
	return result;
}

int CCubeControl::GetOrgPos(int CurPos)
{
	int result;
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			for(int k=0; k<3; k++)
			{
				if( (i*9+j*3+k+1)== CurPos)
				{
					result = m_Cubes[i][j][k]->Position;
				}
			}
	return result;
}

int CCubeControl::GetFaceColor(int Face, int Pos)
{
	int result;
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			for(int k=0; k<3; k++)
			{
				if(Pos == i*9+j*3+k+1)
					result = m_Cubes[i][j][k]->FaceColor[Face];
			}
	return result;
}