
// oglMFCCubeDlg.h : ͷ�ļ�
//

#pragma once

#include "CubeWnd.h"
#include "EasySize.h"
#include "stdafx.h"

// CoglMFCCubeDlg �Ի���
class CoglMFCCubeDlg : public CDialogEx
{
// ����
public:
	CoglMFCCubeDlg(CWnd* pParent = NULL);	// ��׼���캯��
	static UINT CtrlThreadFunc(LPVOID pParam);
	static UINT AutoThreadFunc(LPVOID pParam);
	static UINT FileThreadFunc(LPVOID pParam);

// �Ի�������
	enum { IDD = IDD_OGLMFCCUBE_DIALOG };
	CCubeWnd 	m_Cube;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_EASYSIZE	//�������EASYSIZE
public:
	afx_msg void OnBnClickedRandom();
	afx_msg void OnBnClickedReturn();
	afx_msg void OnBnClickedReset();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedFile();
};
