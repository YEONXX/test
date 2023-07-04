#pragma once
#include "Login.h"
#include "afxdialogex.h"
#include "LoginDlg.h"
#include "UserControlDlg.h"
#include "UserInfoDlg.h"
#include "UserManageDlg.h"


// CMainDlg 대화 상자

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMainDlg();

// 대화 상자 데이터입니다.
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_DIALOG };
//#endif

private:
	CLoginDlg*			m_pLoginDlg;
	CUserControlDlg*	m_pUserControl;
	CUserInfoDlg*		m_pUserInfo;
	CUserManageDlg*		m_pUserManage;

	CString					m_strLoginID;	//로그인 된 ID
	eInfoMode				m_eInfoMode;	//Info Dialog를 띄울 때 가입인지, 수정인지
	eLoginPower				m_eLoginPower;	//로그인 모드 0 : 일반 , 1 : 운용, 2: 관리자

public:
	inline CLoginDlg* GetLoginDlg()
	{
		return m_pLoginDlg;
	}

	inline CUserControlDlg* GetUserControlDlg()
	{
		return m_pUserControl;
	}

	inline CUserInfoDlg* GetInfoDlg()
	{
		return m_pUserInfo;
	}

	inline CUserManageDlg* GetUserManageDlg()
	{
		return m_pUserManage;
	}

	inline CString GetLoginID()
	{
		return m_strLoginID;
	}
	inline void SetLoginID(CString strID)
	{
		m_strLoginID = strID;
	}
	inline eInfoMode GetInfoMode()
	{
		return m_eInfoMode;
	}
	inline void SetInfoMode(eInfoMode eInfoMode)
	{
		m_eInfoMode = eInfoMode;
	}

	inline eLoginPower GetLoginPower()
	{
		return m_eLoginPower;
	}
	inline void SetLoginPower(eLoginPower eLoginPower)
	{
		m_eLoginPower = eLoginPower;
	}
public:
	void		LoginDlgCreate();
	void		UserControlDlgCreate();
	void		UserInfoDlgCreate();
	void		UserManageDlgCreate();

	void		LoginDlgDestroy(BOOL bAll = FALSE);
	void		UserControlDlgDestroy();
	void		UserManageDlgDestroy();
	void		UserInfoDlgDestroy();

	void		SystemDestroy();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
