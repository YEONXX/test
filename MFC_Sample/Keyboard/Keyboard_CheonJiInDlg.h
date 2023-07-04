#pragma once
#include "afxdialogex.h"
#include "HangulProcess.h"
#include "EnglishProcess.h"
#include "projectDefine.h"


// CKeyboard_CheonJiInDlg 대화 상자

class CKeyboard_CheonJiInDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKeyboard_CheonJiInDlg)

private:
	CWnd*				m_pParent;
	bool				m_bNotSpace;
	CHangulProcess		m_cHangul;		// 한
	CEnglishProcess		m_cEng;			// 영
	bool				m_bHangul;		// 한 / 영
	bool				m_bUpperCase;	// 영어 대소문자

	typedef bool(CHangulProcess::*ConvertHangul)(const CString, const CString, const CString, CString*);

public:

	CKeyboard_CheonJiInDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CKeyboard_CheonJiInDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_KEYBOARD_CHEON_JI_IN };

public :
	void	ToEditSendConverText(int nDeleteCnt, CString strAdd);
	void	PushBackSpace();
	void	PushHangul(UINT nID);
	void	PushEnglish(UINT nID);
	void	ConvertEngCase();

	bool	ConvertHangulProcess(ConvertHangul callback, CString* strConvert, CString str1, CString str2, CString str3 = _T(" "));
	bool	IsOneKeyToMultiSymbol(const CString str1, const CString str2, CString* str);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg void OnButtonClick(UINT nID);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedbtkeConversion();
};
