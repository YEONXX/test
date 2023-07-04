// MainDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Login.h"
#include "afxdialogex.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainDlg 대화 상자

IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

CMainDlg::CMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN_DIALOG, pParent)
{
	m_pLoginDlg			= NULL;
	m_pUserControl		= NULL;
	m_pUserInfo			= NULL;
}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
END_MESSAGE_MAP()


// CMainDlg 메시지 처리기
BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//ShowWindow(SW_SHOWMINIMIZED);//! 최소화후 숨겨야 화면에 나타나지 않음
	//PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);

	LoginDlgCreate();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMainDlg::LoginDlgCreate()
{
	if (m_pLoginDlg == NULL)
	{
		m_pLoginDlg = new CLoginDlg(this);
		m_pLoginDlg->Create(CLoginDlg::IDD, this);

		m_pLoginDlg->CenterWindow(this);
		m_pLoginDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pLoginDlg->CenterWindow(this);
		m_pLoginDlg->ShowWindow(SW_SHOW);
	}
}

void CMainDlg::UserControlDlgCreate()
{
	if (m_pUserControl == NULL)
	{
		m_pUserControl = new CUserControlDlg(this);
		m_pUserControl->Create(CUserControlDlg::IDD, this);

		m_pUserControl->CenterWindow(this);
		m_pUserControl->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pUserControl->CenterWindow(this);
		m_pUserControl->ShowWindow(SW_SHOW);
	}
	LoginDlgDestroy();
}

void CMainDlg::UserInfoDlgCreate()
{
	if (m_pUserInfo == NULL)
	{
		m_pUserInfo = new CUserInfoDlg(this);
		m_pUserInfo->Create(CUserInfoDlg::IDD, this);

		m_pUserInfo->CenterWindow(this);
		m_pUserInfo->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pUserInfo->CenterWindow(this);
		m_pUserInfo->ShowWindow(SW_SHOW);
	}
}

void CMainDlg::UserManageDlgCreate()
{
	if (m_pUserManage == NULL)
	{
		m_pUserManage = new CUserManageDlg(this);
		m_pUserManage->Create(CUserManageDlg::IDD, this);

		m_pUserManage->CenterWindow(this);
		m_pUserManage->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pUserManage->CenterWindow(this);
		m_pUserManage->ShowWindow(SW_SHOW);
	}
	LoginDlgDestroy();
}

void CMainDlg::LoginDlgDestroy(BOOL bAll)
{
	if (m_pLoginDlg != NULL)
	{
		m_pLoginDlg->DestroyWindow();
		delete m_pLoginDlg;
		m_pLoginDlg = NULL;
	}
	if (bAll == TRUE)
	{
		SystemDestroy();
	}
}

void CMainDlg::SystemDestroy()
{
	::SendMessage(m_hWnd, WM_CLOSE, NULL, NULL);
}
void CMainDlg::UserControlDlgDestroy()
{
	if (m_pUserControl != NULL)
	{
		m_pUserControl->DestroyWindow();
		delete m_pUserControl;
		m_pUserControl = NULL;
	}
}

void CMainDlg::UserManageDlgDestroy()
{
	if (m_pUserManage != NULL)
	{
		m_pUserManage->DestroyWindow();
		delete m_pUserManage;
		m_pUserManage = NULL;
	}
}


void CMainDlg::UserInfoDlgDestroy()
{
	if (m_pUserInfo != NULL)
	{
		m_pUserInfo->DestroyWindow();
		delete m_pUserInfo;
		m_pUserInfo = NULL;
	}
}

