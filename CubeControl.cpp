#include "stdafx.h"
#include "CubeControl.h"
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
//��.hͷ�ļ��У��������������±�����
//CCube* m_Cubes[3][3][3];
//CCube* m_SavedCubes[3][3][3];
//float m_RotateX[3], m_RotateY[3], m_RotateZ[3];
//float m_CubeX, m_CubeY, m_CubeZ;	//��ת�ǶȵĿ���
//���������º�����
//public:
//	void Render();
//	void RotateX(unsigned int block, int degrees);
//	void RotateY(unsigned int block, int degrees);
//	void RotateZ(unsigned int block, int degrees);
//	BOOL IsSameFace(int Face, int color);	//Face����ɫ�Ƿ�ȫ��Ϊcolor
//	BOOL IsRestore();	//ħ���Ƿ��Ѿ���ԭ
//
//	void Reset();
//	void Save();
//	void Restore();
//	int	 GetFaceColor(int Face, int Pos);	//��ȡPosλ�õķ���Face�����ɫ
//	int  GetFace(int color);	//��ɫΪcolor�����Ŀ����ڵ���
//	int	 GetCurPos(int OrgPos);	//����Ϊԭʼλ�ã����Ϊ��ǰλ��
//	int  GetOrgPos(int CurPos);	//����Ϊ��ǰλ�ã����Ϊԭʼλ��
//private:
//	void InitDraw();	//λ�ü���ɫ��Ϣ�ĳ�ʼ��
//	int  FindCenterPos(int color);	//���ص�ǰ����λ�ã�����Ϊĳ�����Ŀ���ɫ�����Ŀ���Ҫ���ڸ��涨ɫ�ж�

CCubeControl::CCubeControl(void)
:m_CubeX(0)
,m_CubeY(0)
,m_CubeZ(0)
{
	//��ʼ����ת�Ƕ�Ϊ0
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

CCubeControl::~CCubeControl(void)//��������
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
				m_Cubes[i][j][k]->Position = i*9 + j*3 + k + 1;		//λ����Ϣ��ʼ��
				m_Cubes[i][j][k]->SetFaceColor();					//��ɫ��Ϣ��ʼ��
			}
		}
	}
}

void CCubeControl::Render()
{
	//��ת�Ƕ��ڱ������ڲ����ƣ���ת�����ı����m_Rotate
	glRotatef(m_CubeX, 1, 0, 0);
	glRotatef(m_CubeY, 0, 1, 0);
	glRotatef(m_CubeZ, 0, 0, 1);

	//ѭ������Cube
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
   
	if (degrees < 0 && m_RotateX[block] == -90)		//��ת�Ƕȴﵽ90�Ⱦ�Ҫ����λ����Ϣ
	{
		m_RotateX[block] = 0;						//��ת90�Ⱥ󣬱�������Ƕȣ���������������תʱ���ٴ���ת��ħ��ģ�;ʹ�����
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
				m_Cubes[block][j][k]->RotateFaceRV(1);	//-90�ȶ�Ӧ������ת��
			}
		}
	} 
	else if (degrees > 0 && m_RotateX[block] == 90)		//����
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
				m_Cubes[block][j][k]->RotateFaceFW(1);	//90�ȶ�Ӧ������ת��
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
		m_RotateX[0] = m_RotateX[1] = m_RotateX[2] = 0;	//��ת�Ƕȸ�λΪ0
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
					atpos = i*9 + j*3 + k + 1;	//��ǰ����λ��
			}
	return atpos;
}

int CCubeControl::GetFace(int color)
{
	int face;
	int pos = FindCenterPos(color);		//��ȷ��color��ɫ���Ŀ鵱ǰ���ĸ�λ��
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
	return face;		//��ȷ��color��ɫ���Ŀ鵱ǰλ��������
}

BOOL CCubeControl::IsSameFace(int Face, int color)	//ȷ��color��ɫ���Ŀ鵱ǰ���������ɫ�Ƿ�ȫ��һ��
{
	BOOL result;
	result = FALSE;
	int nCount = 0;
	switch(Face)
	{
	case 0:		//����,[i][2][k]
		{
			for(int i=0; i<3; i++)
				for(int k=0; k<3; k++)
				{
					if(m_Cubes[i][2][k]->FaceColor[Face] == color )
						nCount++;				//��ʼ��result=TRUE;�����߼�����
				}
			if(nCount == 9)	result = TRUE;		//9��������ɫȫ�����
			break;
		}
	case 1: 	//ǰ��,[i][j][2]
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
	case 2:		//����,[i][0][k]
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
	case 3:		//����,[i][j][0]
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
	case 4:		//����,[0][j][k]
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
	case 5:		//����,[2][j][k]
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