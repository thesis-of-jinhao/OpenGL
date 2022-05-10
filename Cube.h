#pragma once
class CCube
{
public:
	CCube(float SideLength = 0.9, float RoundWidth = 0.05, int RoundSteps = 2); //���鳤��0.9+0.05*2=1
	~CCube(void);
	float m_Sides[6][3];		//�������ɫ
	void Render();
	//1Ϊx��; 2Ϊy��, 3Ϊz��
	void RotateFaceFW(int Axis);	//����
	void RotateFaceRV(int Axis);	//����
	void Reset();
	void SetColor(int Face,int Color);
	void Copy(CCube *Src);
	int  Position;		//����λ��,��27��
	void SetFaceColor();
	int  FaceColor[6];//С��6�������ɫ

private:
	float m_SideLength;//ħ��С��߳�
	float m_RoundWidth; 
	int   m_RoundSteps;
	float m_SideMaterial[4];
	float m_Shininess[1];
	float m_BevelDiffuse[4];
	float m_BevelMaterial[4];
	float m_BevelShininess[1];

	void RotateFaceMe(int a, int b, int c, int d);	//��ת�棬������ɫ��ֵ��Ҳ�൱����ת��ɫ
	void MakeRoundedSide();
	void MakeCornerSphere();
};
