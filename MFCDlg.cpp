
// oglMFCCubeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC.h"
#include "MFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
// 对话框数据
	enum { IDD = IDD_ABOUTBOX };//对话框资源的ID

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)//添加新的变量
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CoglMFCCubeDlg 对话框



CoglMFCCubeDlg::CoglMFCCubeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CoglMFCCubeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CoglMFCCubeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FRAME, m_Cube);
}

BEGIN_MESSAGE_MAP(CoglMFCCubeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RANDOM, &CoglMFCCubeDlg::OnBnClickedRandom)//对应消息响应，消息映射
	ON_BN_CLICKED(IDC_RETURN, &CoglMFCCubeDlg::OnBnClickedReturn)
	ON_BN_CLICKED(IDC_RESET, &CoglMFCCubeDlg::OnBnClickedReset)
	ON_BN_CLICKED(IDC_FILE, &CoglMFCCubeDlg::OnBnClickedFile)
	ON_WM_SIZE()//可变大小
	//ON_BN_CLICKED(IDC_FILE, &CoglMFCCubeDlg::OnBnClickedFile)
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CoglMFCCubeDlg)
	EASYSIZE(IDC_FRAME, ES_BORDER, ES_BORDER, ES_BORDER, ES_BORDER, 0)	//对控件IDC_FRAME设置，响应EASYSIZE
	EASYSIZE(IDC_RETURN, ES_BORDER, ES_KEEPSIZE, ES_KEEPSIZE, ES_BORDER, 0)
	EASYSIZE(IDC_RESET, ES_BORDER, ES_KEEPSIZE, ES_KEEPSIZE, ES_BORDER, 0)
	EASYSIZE(IDC_RANDOM, ES_BORDER, ES_KEEPSIZE, ES_KEEPSIZE, ES_BORDER, 0)
	EASYSIZE(IDC_FILE, ES_BORDER, ES_KEEPSIZE, ES_KEEPSIZE, ES_BORDER, 0)
END_EASYSIZE_MAP


// CoglMFCCubeDlg 消息处理程序

BOOL CoglMFCCubeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	INIT_EASYSIZE;		//EASYSIZE初始化
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_Cube.InitGL();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CoglMFCCubeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CoglMFCCubeDlg::OnPaint()
{
	if (IsIconic())//判断窗口是否最小化
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CoglMFCCubeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT CoglMFCCubeDlg::CtrlThreadFunc(LPVOID pParam)
{
	CoglMFCCubeDlg *p = (CoglMFCCubeDlg*)pParam;

	p->GetDlgItem(IDC_RANDOM)->EnableWindow(false);
	p->GetDlgItem(IDC_RETURN)->EnableWindow(false);
	p->GetDlgItem(IDC_RESET)->EnableWindow(false);
	p->GetDlgItem(IDC_FILE)->EnableWindow(false);
	CString Preset;
	CString Order;
	CString LastOrder = "";
	CString chk1, chk2;
	int num;
	srand(GetTickCount() + rand());				//随机种子
	num = rand()/(RAND_MAX/30) + 30;
	int atpos = 0;
	Preset = "F+R+U-L-X+Y-R-L+X-B+D-Z-Y+F-U+X-D-Y+X-R-B-D+U-Z-Y+X-U+F-L-U-R-Z+R+X+Y-Z+D-F+F+X+Y-B-X-B+Z-L-Y+B-F-U+";
	Preset += "X+Y+D-B-X+B-Z-R+Y-D+U-F-L+B-L+Y-L-F+Z-D-F-R+B-U-D+Z-X+R-D-Y-B-L+B+U+Z-F+R-D+B+U+L-F-R-Z+X-U-D+F-D-R+";
	for(int i=0; i<num; i++)
	{
		chk1 = LastOrder.Left(1);
		atpos = rand()/(RAND_MAX/90);
		if(atpos%2 == 0)	Order = Preset.Mid(atpos, 2);
		chk2 = Order.Left(1);
		if(chk1 == chk2)	continue;
		else if(chk2 != "")		
		{
			p->m_Cube.Rotate3D(Order);		//旋转
			LastOrder = Order;
		}
	}

	p->GetDlgItem(IDC_RANDOM)->EnableWindow(true);
	p->GetDlgItem(IDC_RETURN)->EnableWindow(true);
	p->GetDlgItem(IDC_RESET)->EnableWindow(true);
	p->GetDlgItem(IDC_FILE)->EnableWindow(true);
	return 0;
}

//LPVOID是一个没有类型的指针
UINT CoglMFCCubeDlg::AutoThreadFunc(LPVOID pParam)
{
	CoglMFCCubeDlg *p = (CoglMFCCubeDlg*)pParam;
	HWND hcwnd;
	hcwnd = p->GetDlgItem(IDC_FRAME)->GetSafeHwnd();
	::SendMessage(hcwnd, WM_MYTIMER, 1, 1);
	p->GetDlgItem(IDC_FILE)->EnableWindow(false);
	p->GetDlgItem(IDC_RANDOM)->EnableWindow(false);
	p->GetDlgItem(IDC_RETURN)->EnableWindow(false);
	p->GetDlgItem(IDC_RESET)->EnableWindow(false);

	if(!p->m_Cube.m_3DCube.IsRestore())
	{
		p->m_Cube.BottomCrossSolve();
		p->m_Cube.BottomCornerSolve();
		p->m_Cube.SecondSolve();
		p->m_Cube.TopCrossFaceSolve();
		p->m_Cube.TopCrossPosSolve();
		p->m_Cube.TopLastSolve();
	}
	p->GetDlgItem(IDC_RANDOM)->EnableWindow(true);
	p->GetDlgItem(IDC_RETURN)->EnableWindow(true);
	p->GetDlgItem(IDC_RESET)->EnableWindow(true);
	p->GetDlgItem(IDC_FILE)->EnableWindow(true);
	::SendMessage(hcwnd, WM_MYTIMER, 1, 0);
	return 0;
}

UINT CoglMFCCubeDlg::FileThreadFunc(LPVOID pParam)
{
	CoglMFCCubeDlg *p = (CoglMFCCubeDlg*)pParam;
	p->GetDlgItem(IDC_FILE)->EnableWindow(false);
	p->GetDlgItem(IDC_RANDOM)->EnableWindow(false);
	p->GetDlgItem(IDC_RETURN)->EnableWindow(false);
	p->GetDlgItem(IDC_RESET)->EnableWindow(false);
	CString strFile = _T("");
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Preset Files (*.txt)|*.txt|All Files (*.*)|*.*||"), NULL);
	if (dlgFile.DoModal()){
		strFile = dlgFile.GetPathName();
	}
	CStdioFile file;
	if (!file.Open(strFile, CFile::modeRead))
	{
		return FALSE;
	}
	CString Preset = _T("");
	CString Order = _T("");
	CString Check = _T("");
	file.ReadString(Preset);
	int len = Preset.GetLength();
	for(int i=0; i<len; i+=2){
		Order = Preset.Mid(i, 2);
		Check = Order.Mid(0, 1);
		if(Check == "F" || Check == "U" || Check == "R" ||
			Check == "L" || Check == "D" || Check == "B" ||
			Check == "X" || Check == "Y" || Check == "Z"){
			p->m_Cube.Rotate3D(Order);
		}
	}

	file.Close();
	p->GetDlgItem(IDC_RANDOM)->EnableWindow(true);
	p->GetDlgItem(IDC_RETURN)->EnableWindow(true);
	p->GetDlgItem(IDC_RESET)->EnableWindow(true);
	p->GetDlgItem(IDC_FILE)->EnableWindow(true);
	return 0;
}

void CoglMFCCubeDlg::OnBnClickedRandom()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxBeginThread(CtrlThreadFunc, this);
}



void CoglMFCCubeDlg::OnBnClickedReturn()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxBeginThread(AutoThreadFunc, this);
}


void CoglMFCCubeDlg::OnBnClickedReset()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Cube.Reset();
}

void CoglMFCCubeDlg::OnBnClickedFile()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxBeginThread(FileThreadFunc, this);
}



void CoglMFCCubeDlg::OnSize(UINT nType, int cx, int cy)
{
	// TODO: 在此处添加消息处理程序代码
	CDialogEx::OnSize(nType, cx, cy);
	UPDATE_EASYSIZE;
}


