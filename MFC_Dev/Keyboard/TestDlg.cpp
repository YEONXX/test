// TestDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Keyboard.h"
#include "KeyboardDlg.h"
#include "afxdialogex.h"
#include "TestDlg.h"


// CTestDlg 대화 상자

IMPLEMENT_DYNAMIC(CTestDlg, CDialogEx)

CTestDlg::CTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEST_DIALOG, pParent)
{
	m_pParent = pParent;
}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_edSample, m_edtSample);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
	ON_EN_SETFOCUS(IDC_edSample, &CTestDlg::OnEnSetfocusedsample)
	ON_BN_CLICKED(IDC_btTest_Close, &CTestDlg::OnBnClickedbttestClose)
END_MESSAGE_MAP()


// CTestDlg 메시지 처리기


void CTestDlg::OnEnSetfocusedsample()
{
	CKeyboardDlg* pDlg = (CKeyboardDlg*)m_pParent;
	pDlg->CnJiInDlgCreate();
}



void CTestDlg::OnBnClickedbttestClose()
{
	CKeyboardDlg* pDlg = (CKeyboardDlg*)m_pParent;
	pDlg->CnJiInDlgDestroy();
	pDlg->TestDlgDestroy();
	pDlg->SystemDestroy();
}
