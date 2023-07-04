
// KeyboardDlg.h: 헤더 파일
//

#pragma once
#include "TestDlg.h"
#include "Keyboard_CheonJiInDlg.h"

// CKeyboardDlg 대화 상자
class CKeyboardDlg : public CDialogEx
{
// 생성입니다.
public:
	CKeyboardDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KEYBOARD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

private:
	CTestDlg* m_pTestDlg;
	CKeyboard_CheonJiInDlg* m_pCnJiInDlg;

public:
	void TestDlgCreate();
	void TestDlgDestroy();

	void CnJiInDlgCreate();
	void CnJiInDlgDestroy();
	
	void SystemDestroy();

	CTestDlg* GetTestDlg();
	CKeyboard_CheonJiInDlg* GetCnJiInDlg();

// 구현입니다.
protected:
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
