#pragma once
class CCube
{
public:
	CCube(float SideLength = 0.9, float RoundWidth = 0.05, int RoundSteps = 2); //方块长度0.9+0.05*2=1
	~CCube(void);
	float m_Sides[6][3];		//各面的颜色
	void Render();
	//1为x轴; 2为y轴, 3为z轴
	void RotateFaceFW(int Axis);	//正向
	void RotateFaceRV(int Axis);	//反向
	void Reset();
	void SetColor(int Face,int Color);
	void Copy(CCube *Src);
	int  Position;		//方块位置,共27个
	void SetFaceColor();
	int  FaceColor[6];//小块6个面的颜色

private:
	float m_SideLength;//魔方小块边长
	float m_RoundWidth; 
	int   m_RoundSteps;
	float m_SideMaterial[4];
	float m_Shininess[1];
	float m_BevelDiffuse[4];
	float m_BevelMaterial[4];
	float m_BevelShininess[1];

	void RotateFaceMe(int a, int b, int c, int d);	//旋转面，进行颜色赋值，也相当于旋转颜色
	void MakeRoundedSide();
	void MakeCornerSphere();
};
