
// LoginDlg.h: 헤더 파일
//

#pragma once
#include "UserInfoDlg.h"
#include "UserManageDlg.h"
#include "UserControlDlg.h"
#include "afxdialogex.h"
#include "LNSSQLiteDB.h"
#include "projectDefine.h"
// CLoginDlg 대화 상자

class CLoginDlg : public CDialogEx
{
// 생성입니다.
public:
	CLoginDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOGIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
private:
	CWnd*					m_pParent;
	CLNSSQLiteDB*			m_pDB;

public:
	void					SetDbInit();
	void					SetVarInit();
	void					SetDbDestroy();

	void					SqlCreateTable();
	void					SqlInsertTable();
	eFailCode				SqlLoginCheck();

protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CStatic					m_ctrProject;
	CEdit					m_edLoginID;
	CEdit					m_edLoginPassword;
	CButton					m_btLogin;
	CButton					m_btSignUp;
	CButton					m_btManage;
	CButton					m_btClose;

	afx_msg void OnBnClickedbtsignup();
	afx_msg void OnBnClickedbtlogin();
	afx_msg void OnBnClickedbtadminmange();
	afx_msg void OnBnClickedbtclose();

	CStatic m_ctrLoginText;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
