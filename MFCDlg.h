
// oglMFCCubeDlg.h : 头文件
//

#pragma once

#include "CubeWnd.h"
#include "EasySize.h"
#include "stdafx.h"

// CoglMFCCubeDlg 对话框
class CoglMFCCubeDlg : public CDialogEx
{
// 构造
public:
	CoglMFCCubeDlg(CWnd* pParent = NULL);	// 标准构造函数
	static UINT CtrlThreadFunc(LPVOID pParam);
	static UINT AutoThreadFunc(LPVOID pParam);
	static UINT FileThreadFunc(LPVOID pParam);

// 对话框数据
	enum { IDD = IDD_OGLMFCCUBE_DIALOG };
	CCubeWnd 	m_Cube;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_EASYSIZE	//声明添加EASYSIZE
public:
	afx_msg void OnBnClickedRandom();
	afx_msg void OnBnClickedReturn();
	afx_msg void OnBnClickedReset();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedFile();
};
