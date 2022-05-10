#include "stdafx.h"
#include "Cube.h"
#include <gl\gl.h>
#include <math.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#define M_PI 3.1415926535897932f

CCube::CCube(float SideLength, float RoundWidth, int RoundSteps)
: m_SideLength(SideLength)//ħ��С��߳�0.9
, m_RoundSteps(RoundSteps)
, m_RoundWidth(RoundWidth)
{
	m_SideMaterial[0] = m_SideMaterial[1] = m_SideMaterial[2] = 0.15f;
	m_SideMaterial[3] = 1.0;
	m_Shininess[0] = 2; // The sides have small highlights.
	m_BevelDiffuse[0] = m_BevelDiffuse[1] = m_BevelDiffuse[2] = 0.45f;
	m_BevelDiffuse[3] = 1.0;
	m_BevelMaterial[0] = m_BevelMaterial[1] = m_BevelMaterial[2] = 1.0;
	m_BevelMaterial[3] = 1.0;
	m_BevelShininess[0] = 115;
	Reset();
}

CCube::~CCube(void)
{
}
//CCube(float SideLength = 0.9, float RoundWidth = 0.05, int RoundSteps = 2); //���鳤��0.9+0.05*2=1
//float m_Sides[6][3];		//�������ɫ
//void Render();
////1Ϊx��; 2Ϊy��, 3Ϊz��
//void RotateFaceFW(int Axis);	//����
//void RotateFaceRV(int Axis);	//����
//void Reset();
//void SetColor(int Face, int Color);
//void Copy(CCube* Src);
//int  Position;		//����λ��,��27��
//void SetFaceColor();
//int  FaceColor[6];//С��6�������ɫ
//float m_BevelShininess[1];

void CCube::Render() //��һ�����ߡ����ǵ������壬��������ɫ����
{
	glPushMatrix();

	/* ��ʼ����6���� */
  
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_SideMaterial);
	glMaterialfv(GL_FRONT, GL_SHININESS, m_Shininess);

	glBegin(GL_QUADS);	//ѡ���ı���

	//���棺��������ƣ�������ӦΪ(1, 0, 0)��x������
	glColor3fv(m_Sides[5]);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_Sides[5]);//ָ��������ɫ�Ĺ�ᱻ���䣬�൱����������ɫ
	glNormal3f(1, 0, 0);		//����������Ϊx������

	glVertex3f(m_SideLength/2 + m_RoundWidth,  m_SideLength/2,  m_SideLength/2);//����һ����
	glVertex3f(m_SideLength/2 + m_RoundWidth,  m_SideLength/2, -m_SideLength/2);
	glVertex3f(m_SideLength/2 + m_RoundWidth, -m_SideLength/2, -m_SideLength/2);
	glVertex3f(m_SideLength/2 + m_RoundWidth, -m_SideLength/2,  m_SideLength/2);

	//���棺��������ƣ�������ӦΪ(0, 1, 0)��y������
	glColor3fv(m_Sides[0]);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_Sides[0]);
	glNormal3f(0, 1, 0);

	glVertex3f(-m_SideLength/2,  m_SideLength/2 + m_RoundWidth, -m_SideLength/2);
	glVertex3f(-m_SideLength/2,  m_SideLength/2 + m_RoundWidth,  m_SideLength/2);
	glVertex3f( m_SideLength/2,  m_SideLength/2 + m_RoundWidth,  m_SideLength/2);
	glVertex3f( m_SideLength/2,  m_SideLength/2 + m_RoundWidth, -m_SideLength/2);

	//ǰ�棺��������ƣ�������ӦΪ(0, 0, 1)��z������
	glColor3fv(m_Sides[1]);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_Sides[1]);
	glNormal3f(0, 0, 1);

	glVertex3f( m_SideLength/2,  m_SideLength/2,  m_SideLength/2 + m_RoundWidth);
	glVertex3f(-m_SideLength/2,  m_SideLength/2,  m_SideLength/2 + m_RoundWidth);
	glVertex3f(-m_SideLength/2, -m_SideLength/2,  m_SideLength/2 + m_RoundWidth);
	glVertex3f( m_SideLength/2, -m_SideLength/2,  m_SideLength/2 + m_RoundWidth);

	//���棺��������ƣ�������ӦΪ(0, 0, -1)��z������
	glColor3fv(m_Sides[3]);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_Sides[3]);
	glNormal3f(0, 0, -1); 

	glVertex3f( m_SideLength/2,  m_SideLength/2, -m_SideLength/2 - m_RoundWidth);
	glVertex3f(-m_SideLength/2,  m_SideLength/2, -m_SideLength/2 - m_RoundWidth);
	glVertex3f(-m_SideLength/2, -m_SideLength/2, -m_SideLength/2 - m_RoundWidth);
	glVertex3f( m_SideLength/2, -m_SideLength/2, -m_SideLength/2 - m_RoundWidth);

	//���棺��������ƣ�������ӦΪ(0, -1, 0)��y������
	glColor3fv(m_Sides[2]);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_Sides[2]);
	glNormal3f(0, -1, 0);

	glVertex3f(-m_SideLength/2, -m_SideLength/2 - m_RoundWidth, -m_SideLength/2);
	glVertex3f(-m_SideLength/2, -m_SideLength/2 - m_RoundWidth,  m_SideLength/2);
	glVertex3f( m_SideLength/2, -m_SideLength/2 - m_RoundWidth,  m_SideLength/2);
	glVertex3f( m_SideLength/2, -m_SideLength/2 - m_RoundWidth, -m_SideLength/2);

	//���棺��������ƣ�������ӦΪ(-1, 0, 0)��x������
	glColor3fv(m_Sides[4]);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_Sides[4]);
	glNormal3f(-1, 0, 0);

	glVertex3f(-m_SideLength/2 - m_RoundWidth,  m_SideLength/2,  m_SideLength/2);
	glVertex3f(-m_SideLength/2 - m_RoundWidth,  m_SideLength/2, -m_SideLength/2);
	glVertex3f(-m_SideLength/2 - m_RoundWidth, -m_SideLength/2, -m_SideLength/2);
	glVertex3f(-m_SideLength/2 - m_RoundWidth, -m_SideLength/2,  m_SideLength/2);

	glEnd();

	/* ����8������ */

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_BevelDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_BevelMaterial);
	glMaterialfv(GL_FRONT, GL_SHININESS, m_BevelShininess);
	//glColor3f(0.2, 0.2, 0.2);

	//�ϡ��ҡ�ǰ���潻��
	glTranslatef(m_SideLength/2, m_SideLength/2, m_SideLength/2);
	MakeCornerSphere();

	//�ϡ���ǰ���潻��
	glTranslatef(-m_SideLength, 0, 0);
	glRotatef(90, 0, 0, 1);
	MakeCornerSphere();
	glRotatef(-90, 0, 0, 1);

	//�¡���ǰ���潻��
	glTranslatef(0, -m_SideLength, 0);
	glRotatef(180, 0, 0, 1);
	MakeCornerSphere();
	glRotatef(-180, 0, 0, 1);

	//�¡��ҡ�ǰ���潻��
	glTranslatef(m_SideLength, 0, 0);
	glRotatef(270, 0, 0, 1);
	MakeCornerSphere();
	glRotatef(-270, 0, 0, 1);

	//�ϡ��ҡ������潻��
	glTranslatef(0, m_SideLength, -m_SideLength);
	glRotatef(90, 0, 1, 0);
	MakeCornerSphere();
	glRotatef(-90, 0, 1, 0);


	//�ϡ��󡢺����潻��
	glTranslatef(-m_SideLength, 0, 0);
	glRotatef(180, 0, 1, 0);
	MakeCornerSphere();
	glRotatef(-180, 0, 1, 0);

	//�¡��󡢺����潻��
	glTranslatef(0, -m_SideLength, 0);
	glRotatef(180, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	MakeCornerSphere();
	glRotatef(-90, 1, 0, 0);
	glRotatef(-180, 0, 1, 0);

	//�¡��ҡ������潻��
	glTranslatef(m_SideLength, 0, 0);
	glRotatef(180, 1, 0, 0);
	MakeCornerSphere();
	glRotatef(-180, 1, 0, 0);

	/* ���Ƶ��ߣ���12���� */

	//�ϡ������潻��
	glTranslatef(0, m_SideLength, m_SideLength/2);
  
	MakeRoundedSide();

	//�ϡ������潻��
	glTranslatef(-m_SideLength, 0, 0);
	glRotatef(90, 0, 0, 1);
	MakeRoundedSide();
	glRotatef(-90, 0, 0, 1);

	//�¡������潻��
	glTranslatef(0, -m_SideLength, 0);
	glRotatef(180, 0, 0, 1);
	MakeRoundedSide();
	glRotatef(-180, 0, 0, 1);

	//�¡������潻��
	glTranslatef(m_SideLength, 0, 0);
	glRotatef(270, 0, 0, 1);
	MakeRoundedSide();
	glRotatef(-270, 0, 0, 1);

	//ǰ�������潻��
	glTranslatef(0, m_SideLength/2, m_SideLength/2);
	glRotatef(90, 1, 0, 0);
	MakeRoundedSide();

	//ǰ�������潻��
	glTranslatef(-m_SideLength, 0, 0);
	glRotatef(90, 0, 0, 1);
	MakeRoundedSide();

	//�������潻��
	glTranslatef(-m_SideLength, 0, 0);
	glRotatef(90, 0, 0, 1);
	MakeRoundedSide();

	//�������潻��
	glTranslatef(-m_SideLength, 0, 0);
	glRotatef(90, 0, 0, 1);
	MakeRoundedSide();

	//�������潻��
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, m_SideLength/2, -m_SideLength/2);
	MakeRoundedSide();

	//ǰ�������潻��
	glTranslatef(-m_SideLength, 0, 0);
	glRotatef(90, 0, 0, 1);
	MakeRoundedSide();

	//ǰ�������潻��
	glTranslatef(-m_SideLength, 0, 0);
	glRotatef(90, 0, 0, 1);
	MakeRoundedSide();

	//�������潻��
	glTranslatef(-m_SideLength, 0, 0);
	glRotatef(90, 0, 0, 1);
	MakeRoundedSide();

	glPopMatrix();

}

void CCube::SetFaceColor()
{
	if(Position == 1)
	{
		FaceColor[0] = 0;
		FaceColor[1] = 0;
		FaceColor[2] = WHITE;
		FaceColor[3] = GREEN;
		FaceColor[4] = ORANGE;
		FaceColor[5] = 0;
	}
	if(Position == 2)
	{
		FaceColor[0] = 0;
		FaceColor[1] = 0;
		FaceColor[2] = WHITE;
		FaceColor[3] = 0;
		FaceColor[4] = ORANGE;
		FaceColor[5] = 0;
	}
	if(Position == 3)
	{
		FaceColor[0] = 0;
		FaceColor[1] = BLUE;
		FaceColor[2] = WHITE;
		FaceColor[3] = 0;
		FaceColor[4] = ORANGE;
		FaceColor[5] = 0;
	}
	if(Position == 4)
	{
		FaceColor[0] = 0;
		FaceColor[1] = 0;
		FaceColor[2] = 0;
		FaceColor[3] = GREEN;
		FaceColor[4] = ORANGE;
		FaceColor[5] = 0;
	}
	if(Position == 5)
	{
		FaceColor[0] = 0;
		FaceColor[1] = 0;
		FaceColor[2] = 0;
		FaceColor[3] = 0;
		FaceColor[4] = ORANGE;
		FaceColor[5] = 0;
	}
	if(Position == 6)
	{
		FaceColor[0] = 0;
		FaceColor[1] = BLUE;
		FaceColor[2] = 0;
		FaceColor[3] = 0;
		FaceColor[4] = ORANGE;
		FaceColor[5] = 0;
	}
	if(Position == 7)
	{
		FaceColor[0] = YELLOW;
		FaceColor[1] = 0;
		FaceColor[2] = 0;
		FaceColor[3] = GREEN;
		FaceColor[4] = ORANGE;
		FaceColor[5] = 0;
	}
	if(Position == 8)
	{
		FaceColor[0] = YELLOW;
		FaceColor[1] = 0;
		FaceColor[2] = 0;
		FaceColor[3] = 0;
		FaceColor[4] = ORANGE;
		FaceColor[5] = 0;
	}
	if(Position == 9)
	{
		FaceColor[0] = YELLOW;
		FaceColor[1] = BLUE;
		FaceColor[2] = 0;
		FaceColor[3] = 0;
		FaceColor[4] = ORANGE;
		FaceColor[5] = 0;
	}
	if(Position == 10)
	{
		FaceColor[0] = 0;
		FaceColor[1] = 0;
		FaceColor[2] = WHITE;
		FaceColor[3] = GREEN;
		FaceColor[4] = 0;
		FaceColor[5] = 0;
	}
	if(Position == 11)
	{
		FaceColor[0] = 0;
		FaceColor[1] = 0;
		FaceColor[2] = WHITE;
		FaceColor[3] = 0;
		FaceColor[4] = 0;
		FaceColor[5] = 0;
	}
	if(Position == 12)
	{
		FaceColor[0] = 0;
		FaceColor[1] = BLUE;
		FaceColor[2] = WHITE;
		FaceColor[3] = 0;
		FaceColor[4] = 0;
		FaceColor[5] = 0;
	}
	if(Position == 13)
	{
		FaceColor[0] = 0;
		FaceColor[1] = 0;
		FaceColor[2] = 0;
		FaceColor[3] = GREEN;
		FaceColor[4] = 0;
		FaceColor[5] = 0;
	}
	if(Position == 14)
	{
		FaceColor[0] = 0;
		FaceColor[1] = 0;
		FaceColor[2] = 0;
		FaceColor[3] = 0;
		FaceColor[4] = 0;
		FaceColor[5] = 0;
	}
	if(Position == 15)
	{
		FaceColor[0] = 0;
		FaceColor[1] = BLUE;
		FaceColor[2] = 0;
		FaceColor[3] = 0;
		FaceColor[4] = 0;
		FaceColor[5] = 0;
	}
	if(Position == 16)
	{
		FaceColor[0] = YELLOW;
		FaceColor[1] = 0;
		FaceColor[2] = 0;
		FaceColor[3] = GREEN;
		FaceColor[4] = 0;
		FaceColor[5] = 0;
	}
	if(Position == 17)
	{
		FaceColor[0] = YELLOW;
		FaceColor[1] = 0;
		FaceColor[2] = 0;
		FaceColor[3] = 0;
		FaceColor[4] = 0;
		FaceColor[5] = 0;
	}
	if(Position == 18)
	{
		FaceColor[0] = YELLOW;
		FaceColor[1] = BLUE;
		FaceColor[2] = 0;
		FaceColor[3] = 0;
		FaceColor[4] = 0;
		FaceColor[5] = 0;
	}
	if(Position == 19)
	{
		FaceColor[0] = 0;
		FaceColor[1] = 0;
		FaceColor[2] = WHITE;
		FaceColor[3] = GREEN;
		FaceColor[4] = 0;
		FaceColor[5] = RED;
	}
	if(Position == 20)
	{
		FaceColor[0] = 0;
		FaceColor[1] = 0;
		FaceColor[2] = WHITE;
		FaceColor[3] = 0;
		FaceColor[4] = 0;
		FaceColor[5] = RED;
	}
	if(Position == 21)
	{
		FaceColor[0] = 0;
		FaceColor[1] = BLUE;
		FaceColor[2] = WHITE;
		FaceColor[3] = 0;
		FaceColor[4] = 0;
		FaceColor[5] = RED;
	}
	if(Position == 22)
	{
		FaceColor[0] = 0;
		FaceColor[1] = 0;
		FaceColor[2] = 0;
		FaceColor[3] = GREEN;
		FaceColor[4] = 0;
		FaceColor[5] = RED;
	}
	if(Position == 23)
	{
		FaceColor[0] = 0;
		FaceColor[1] = 0;
		FaceColor[2] = 0;
		FaceColor[3] = 0;
		FaceColor[4] = 0;
		FaceColor[5] = RED;
	}
	if(Position == 24)
	{
		FaceColor[0] = 0;
		FaceColor[1] = BLUE;
		FaceColor[2] = 0;
		FaceColor[3] = 0;
		FaceColor[4] = 0;
		FaceColor[5] = RED;
	}
	if(Position == 25)
	{
		FaceColor[0] = YELLOW;
		FaceColor[1] = 0;
		FaceColor[2] = 0;
		FaceColor[3] = GREEN;
		FaceColor[4] = 0;
		FaceColor[5] = RED;
	}
	if(Position == 26)
	{
		FaceColor[0] = YELLOW;
		FaceColor[1] = 0;
		FaceColor[2] = 0;
		FaceColor[3] = 0;
		FaceColor[4] = 0;
		FaceColor[5] = RED;
	}
	if(Position == 27)
	{
		FaceColor[0] = YELLOW;
		FaceColor[1] = BLUE;
		FaceColor[2] = 0;
		FaceColor[3] = 0;
		FaceColor[4] = 0;
		FaceColor[5] = RED;
	}//*/
	for(int i=0; i<6; i++)
		SetColor(i, FaceColor[i]);
}

void CCube::MakeRoundedSide() //��Բ��
{
	float StepSize = M_PI/2/m_RoundSteps;
	float v, vv;
  
	glBegin(GL_QUADS);
	glColor3f(0.5, 0.5, 0.5);//��ɫ
  
	for (int i = 0; i < m_RoundSteps; i++) 
	{
		v = i * StepSize;
		vv = (i+1)*StepSize;

		glNormal3f((float)cos(v),(float)sin(v), 0);
		glVertex3f((float)cos(v) * m_RoundWidth, (float)sin(v) * m_RoundWidth,  m_SideLength/2);
		glVertex3f((float)cos(v) * m_RoundWidth, (float)sin(v) * m_RoundWidth, -m_SideLength/2);

		glNormal3f((float)cos(vv), (float)sin(vv), 0);
		glVertex3f((float)cos(vv) * m_RoundWidth, (float)sin(vv) * m_RoundWidth, -m_SideLength/2);
		glVertex3f((float)cos(vv) * m_RoundWidth, (float)sin(vv) * m_RoundWidth,  m_SideLength/2);
	}

	glEnd();
}

void CCube::MakeCornerSphere() //��Բ��
{
	float StepSize = M_PI/2/m_RoundSteps;
	float v, w, vv, ww;

	glBegin(GL_QUADS);
	glColor3f(0.5, 0.5, 0.5);

	for (int i = 0; i < m_RoundSteps; i++) 
	{
		v = i * StepSize;
		vv = (i+1)*StepSize;

		for (int j = 0; j < m_RoundSteps; j++) 
		{
			w = j * StepSize;
			ww = (j+1) * StepSize;

			glNormal3f((float)cos(v) * (float)cos(w), (float)cos(v) * (float)sin(w), (float)sin(v));
			glVertex3f((float)cos(v) * (float)cos(w) * m_RoundWidth,
					  (float)cos(v) * (float)sin(w) * m_RoundWidth,
					  (float)sin(v) * m_RoundWidth);

			glNormal3f((float)cos(vv) * (float)cos(w), (float)cos(vv) * (float)sin(w), (float)sin(vv));
			glVertex3f((float)cos(vv) * (float)cos(w) * m_RoundWidth,
					  (float)cos(vv) * (float)sin(w) * m_RoundWidth,
					  (float)sin(vv) * m_RoundWidth);

			glNormal3f((float)cos(vv) * (float)cos(ww), (float)cos(vv) * (float)sin(ww), (float)sin(vv));
			glVertex3f((float)cos(vv) * (float)cos(ww) * m_RoundWidth,
					  (float)cos(vv) * (float)sin(ww) * m_RoundWidth,
					  (float)sin(vv) * m_RoundWidth);

			glNormal3f((float)cos(v) * (float)cos(ww), (float)cos(v) * (float)sin(ww), (float)sin(v));
			glVertex3f((float)cos(v) * (float)cos(ww) * m_RoundWidth,
					  (float)cos(v) * (float)sin(ww) * m_RoundWidth,
					  (float)sin(v) * m_RoundWidth);
		}
	}
	glEnd();
}

void CCube::RotateFaceMe(int a, int b, int c, int d)
{
	int TempFace = FaceColor[a];
	FaceColor[a] = FaceColor[b];
	FaceColor[b] = FaceColor[c];
	FaceColor[c] = FaceColor[d];
	FaceColor[d] = TempFace;
	for(int i=0; i<6; i++)
		SetColor(i, FaceColor[i]);
}
void CCube::RotateFaceFW(int Axis)	//������������ת�����Ƕ�Ϊ��
{
	switch (Axis) //1Ϊx�ᣬ2Ϊy�ᣬ3Ϊz��
	{
	case 1:	RotateFaceMe(3, 2, 1, 0);	//���ֶ�ӦFace
            break;
	case 2:	RotateFaceMe(5, 1, 4, 3); 
            break;
	case 3:	RotateFaceMe(5, 2, 4, 0);
            break;
	}
}
void CCube::RotateFaceRV(int Axis)	//���ᷴ������ת�����Ƕ�Ϊ��
{
	switch (Axis) //1Ϊx�ᣬ2Ϊy�ᣬ3Ϊz��
	{
	case 1:	RotateFaceMe(0, 1, 2, 3);	//���ֶ�ӦFace
            break;
	case 2:	RotateFaceMe(3, 4, 1, 5);
            break;
	case 3:	RotateFaceMe(0, 4, 2, 5);
            break;
	}
}

void CCube::Reset()
{
	for (int i = 0; i < 6; i++)
	{
		m_Sides[i][0] = m_Sides[i][1] = m_Sides[i][2] = 0.25;	//��λ��ɫ
	}
}

void CCube::SetColor(int Face,int Color)		//������ɫ
{
	//�ڴ˶�������ɫ
	switch (Color)
	{
	case YELLOW :	m_Sides[Face][0] = 1;			//R
					m_Sides[Face][1] = 1;			//G
					m_Sides[Face][2] = 0;			//B
					break;
	case BLUE   :	m_Sides[Face][0] = 0;
					m_Sides[Face][1] = 0;
					m_Sides[Face][2] = 1;
					break;
	case RED    :	m_Sides[Face][0] = 1;
					m_Sides[Face][1] = 0;
					m_Sides[Face][2] = 0;
					break;
	case ORANGE :	m_Sides[Face][0] = 1;
					m_Sides[Face][1] = 0.35f;
					m_Sides[Face][2] = 0;
					break;
	case GREEN  :	m_Sides[Face][0] = 0;
					m_Sides[Face][1] = 1;
					m_Sides[Face][2] = 0;
					break;
	case WHITE  :	m_Sides[Face][0] = 1;
					m_Sides[Face][1] = 1;
					m_Sides[Face][2] = 1;
					break;
	case NOCOLOR:	m_Sides[Face][0] = 0.25f;
					m_Sides[Face][1] = 0.25f;
					m_Sides[Face][2] = 0.25f;
					break;
	}
}

void CCube::Copy(CCube *Src)
{
	for (int i = 0; i < 6;i++)
	{
		m_Sides[i][0] = Src->m_Sides[i][0];
		m_Sides[i][1] = Src->m_Sides[i][1];
		m_Sides[i][2] = Src->m_Sides[i][2];
	}
}