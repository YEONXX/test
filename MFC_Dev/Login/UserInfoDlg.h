#pragma once
#include "afxdialogex.h"
#include "LNSSQLiteDB.h"
#include "TypeCheck.h"
#include "Util.h"
#include "projectDefine.h"

// CUserInfoDlg 대화 상자

class CUserInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserInfoDlg)

public:
	CUserInfoDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CUserInfoDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INFO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetDbInit();
	void SetControlInit();
	void SetDbDestroy();

	BOOL SetTypeCheck();

private:
	CWnd* m_pParent;
	CLNSSQLiteDB* m_pDB;
	CTypeCheck		m_typeCheck;
	CUtil			m_util;

	CComboBox m_cbInfoRank;
	CComboBox m_cbInfoPower;

	CEdit m_edInfoID;
	CEdit m_edInfoPassword;
	CEdit m_edInfoPassCheck;
	CEdit m_edInfoEmail;
	CEdit m_edInfoPhone;
	CEdit m_edInfoMiliNo;

	CStatic m_ctrInfoText;
	CButton m_btInfoModify;
	CButton m_btInfoClose;

	CComboBox m_cbInfoPhone1;
	CEdit m_edInfoPhone2;
	CEdit m_edInfoPhone3;
	CString m_strInfoPhone;

	CEdit m_edInfoMiliNo1;
	CEdit m_edInfoMiliNo2;
	CString m_strInfoMiliNo;

	eDestroy m_eParent;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedbtinfoClose();
	afx_msg void OnBnClickedbtinfoSignup();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedbtinfoModify();
	afx_msg void OnEnChangeedinfoId();
	afx_msg void OnEnChangeedinfoPassword();
	afx_msg void OnEnChangeedinfoPasscheck();
};
