// UserControlDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include "UserControlDlg.h"


// CUserControlDlg 대화 상자

IMPLEMENT_DYNAMIC(CUserControlDlg, CDialogEx)

CUserControlDlg::CUserControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USER_CONTROL_DIALOG, pParent)
{
	m_pParent = pParent;
}

CUserControlDlg::~CUserControlDlg()
{
}

void CUserControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserControlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_btUC_CLOSE, &CUserControlDlg::OnBnClickedbtucClose)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btUSER_MODIFY, &CUserControlDlg::OnBnClickedbtuserModify)
END_MESSAGE_MAP()


// CUserControlDlg 메시지 처리기
void CUserControlDlg::OnBnClickedbtucClose()
{
	CMainDlg* pDlg = (CMainDlg*)m_pParent;
	pDlg->UserControlDlgDestroy();
	pDlg->SystemDestroy();
}


void CUserControlDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	CMainDlg* pDlg = (CMainDlg*)m_pParent;

	eLoginPower eLoginPower = pDlg->GetLoginPower();

	if (eLoginPower == eLoginPower::eLP_General)
	{
		GetDlgItem(IDC_btUC_CLOSE)->SetWindowTextW(_T("일반 사용자"));
	}
	else if(eLoginPower == eLoginPower::eLP_Operator)
	{
		GetDlgItem(IDC_btUC_CLOSE)->SetWindowTextW(_T("운용자"));
	}
	else
	{
		GetDlgItem(IDC_btUC_CLOSE)->SetWindowTextW(_T("관리자"));
	}
}


void CUserControlDlg::OnBnClickedbtuserModify()
{
	CMainDlg* pDlg = (CMainDlg*)m_pParent;
	pDlg->SetInfoMode(eInfoMode::eInfo_Modify);
	pDlg->UserInfoDlgCreate();
}
