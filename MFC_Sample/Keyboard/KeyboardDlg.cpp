
// KeyboardDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Keyboard.h"
#include "KeyboardDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKeyboardDlg 대화 상자

CKeyboardDlg::CKeyboardDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KEYBOARD_DIALOG, pParent)
{
	m_pTestDlg = NULL;
	m_pCnJiInDlg = NULL;
}

void CKeyboardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKeyboardDlg, CDialogEx)
END_MESSAGE_MAP()


// CKeyboardDlg 메시지 처리기

BOOL CKeyboardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	TestDlgCreate();
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


void CKeyboardDlg::TestDlgCreate()
{
	if (m_pTestDlg == NULL)
	{
		m_pTestDlg = new CTestDlg(this);
		m_pTestDlg->Create(CTestDlg::IDD);

		m_pTestDlg->CenterWindow(this);
		m_pTestDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pTestDlg->CenterWindow(this);
		m_pTestDlg->ShowWindow(SW_SHOW);
	}
}

void CKeyboardDlg::TestDlgDestroy()
{
	if (m_pTestDlg != NULL)
	{
		m_pTestDlg->DestroyWindow();
		delete m_pTestDlg;
		m_pTestDlg = NULL;
	}
}

void CKeyboardDlg::CnJiInDlgCreate()
{
	if (m_pCnJiInDlg == NULL)
	{
		m_pCnJiInDlg = new CKeyboard_CheonJiInDlg(this);
		m_pCnJiInDlg->Create(CKeyboard_CheonJiInDlg::IDD);

		m_pCnJiInDlg->CenterWindow(this);
		m_pCnJiInDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pCnJiInDlg->ShowWindow(SW_SHOW);
	}
}

void CKeyboardDlg::CnJiInDlgDestroy()
{
	if (m_pCnJiInDlg != NULL)
	{
		m_pCnJiInDlg->DestroyWindow();
		delete m_pCnJiInDlg;
		m_pCnJiInDlg = NULL;
	}
}

CTestDlg* CKeyboardDlg::GetTestDlg()
{
	return m_pTestDlg;
}

CKeyboard_CheonJiInDlg* CKeyboardDlg::GetCnJiInDlg()
{
	return m_pCnJiInDlg;
}

void CKeyboardDlg::SystemDestroy()
{
	::SendMessage(m_hWnd, WM_CLOSE, NULL, NULL);
}
