#pragma once
#include "afxdialogex.h"


// CTestDlg 대화 상자

class CTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTestDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TEST_DIALOG };

private:
	CWnd* m_pParent;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtSample;
	afx_msg void OnEnSetfocusedsample();
	afx_msg void OnBnClickedbttestClose();
};
