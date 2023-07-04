#pragma once
#include "afxdialogex.h"
#include "LNSSQLiteDB.h"
#include "Util.h"
#include <string>


// CUserManageDlg 대화 상자

class CUserManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserManageDlg)

public:
	CUserManageDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CUserManageDlg();

// 대화 상자 데이터입니다.
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USER_MANAGE_DIALOG };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
		
private:
	CWnd* m_pParent;

	CLNSSQLiteDB* m_pDB;
	CUtil		  m_util;

public:
	void SetDbInit();
	void SetDbDestroy();
	void GetSearchData(CString strData);
	void SetControlInit();

private:
	CComboBox	m_cbApprovalStatus;
	CEdit		m_edID;
	CEdit		m_edRank;
	CEdit		m_edPower;

	CListCtrl m_listInfo;
	CButton m_btApproval;
	CButton m_btDelete;
	CButton m_btClose;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedbtumClose();
	afx_msg void OnBnClickedbtumSearch();
	afx_msg void OnNMDblclklistumInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedbtumApproval();
public:
	afx_msg void OnBnClickedbtumDelete();
};
