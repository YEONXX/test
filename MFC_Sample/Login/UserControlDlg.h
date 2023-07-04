#pragma once
#include "afxdialogex.h"


// CUserControlDlg 대화 상자

class CUserControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserControlDlg)

public:
	CUserControlDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CUserControlDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_USER_CONTROL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pParent;

public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedbtuserModify();
	afx_msg void OnBnClickedbtucClose();


};
