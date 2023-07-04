// CKeyboard_CheonJiInDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Keyboard.h"
#include "KeyboardDlg.h"
#include "afxdialogex.h"
#include "Keyboard_CheonJiInDlg.h"


// CKeyboard_CheonJiInDlg 대화 상자

IMPLEMENT_DYNAMIC(CKeyboard_CheonJiInDlg, CDialogEx)

CKeyboard_CheonJiInDlg::CKeyboard_CheonJiInDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_KEYBOARD_CHEON_JI_IN, pParent)
{
    m_pParent = pParent;
    m_bNotSpace = true;
    m_bHangul = true;
    m_bUpperCase = false;
}

CKeyboard_CheonJiInDlg::~CKeyboard_CheonJiInDlg()
{

}

void CKeyboard_CheonJiInDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(CKeyboard_CheonJiInDlg, CDialogEx)
    ON_COMMAND_RANGE(IDC_btVowel_In, IDC_btSymbol, &CKeyboard_CheonJiInDlg::OnButtonClick)
    ON_BN_CLICKED(IDC_btKE_Conversion, &CKeyboard_CheonJiInDlg::OnBnClickedbtkeConversion)
END_MESSAGE_MAP()

void CKeyboard_CheonJiInDlg::OnButtonClick(UINT nID)
{
    CKeyboardDlg* pDlg = (CKeyboardDlg*)m_pParent;
    CWnd* pButton = GetDlgItem(nID);
    if (pButton != NULL)
    {        
        // Edit 컨트롤에 입력되어 있는 문자
        CString strEdit = _T("");
        pDlg->GetTestDlg()->m_edtSample.GetWindowTextW(strEdit);

        // 버튼 캡션
        CString strButtonName;
        pButton->GetWindowTextW(strButtonName);

        // 입력 문자를 버튼 캡션의 제일 왼쪽 문자로 가져온다.
        CString strInputting = strButtonName.Left(1);

        if (strButtonName == "BackSpace")
        {
            PushBackSpace();
            m_bNotSpace = true;
        }
        else if (strButtonName == _T("Space"))
        {
            if (m_bNotSpace == false)
            {
                strInputting = _T(' ');
                ToEditSendConverText(0, strInputting);
            }
            m_bNotSpace = false;

            return;
        }
        else if (strButtonName == _T("Enter"))
        {

        }
        else if (strButtonName == _T("Shift"))
        {
            ConvertEngCase();
        }
        else
        {
            if (m_bHangul == true)
            {
                PushHangul(nID);
            }
            else
            {
                PushEnglish(nID);
            }
        }        
    }
}

// 추가 전에 edit 박스에서 뒤에 몇글자 지울건지 nDeleteCnt
// 기존 edit 박스 뒤에 붙일 텍스트 strAdd
void CKeyboard_CheonJiInDlg::ToEditSendConverText(int nDeleteCnt, CString strAdd)
{
    CKeyboardDlg* pDlg = (CKeyboardDlg*)m_pParent;

    CString strEdit = _T("");
    pDlg->GetTestDlg()->m_edtSample.GetWindowTextW(strEdit);
    strEdit.Delete(strEdit.GetLength() - nDeleteCnt, nDeleteCnt);
    strEdit = strEdit + strAdd;

    ::SendMessage(pDlg->GetTestDlg()->m_edtSample, WM_SETTEXT, 0, (LPARAM)strEdit.GetBuffer());
    pDlg->GetTestDlg()->m_edtSample.SetSel(strEdit.GetLength(), strEdit.GetLength() - 1);
    pDlg->GetTestDlg()->m_edtSample.SetFocus();
}

bool CKeyboard_CheonJiInDlg::ConvertHangulProcess(ConvertHangul callback, CString* strConvert, CString str1, CString str2, CString str3)
{
    bool bResult = true;
    // 콜백 함수 호출
    if ((m_cHangul.*callback)(str1, str2, str3, strConvert) == true)
    {
        bResult = true;
    }
    else
    {
        bResult =false;
    }
    return bResult;
}

void	CKeyboard_CheonJiInDlg::PushEnglish(UINT nID)
{
    CKeyboardDlg* pDlg = (CKeyboardDlg*)m_pParent;
    CWnd* pButton = GetDlgItem(nID);

    // Edit 컨트롤에 입력되어 있는 문자
    CString strEdit = _T("");
    pDlg->GetTestDlg()->m_edtSample.GetWindowTextW(strEdit);

    // 버튼 캡션
    CString strButtonName;
    pButton->GetWindowTextW(strButtonName);

    // 입력 문자를 버튼 캡션의 제일 왼쪽 문자로 가져온다.
    CString strInputting = strButtonName.Left(1);

    //일단 글자 추가
    ToEditSendConverText(0, strInputting);

    pDlg->GetTestDlg()->m_edtSample.GetWindowTextW(strEdit);
    if (strEdit.GetLength() - 2 >= 0 && m_bNotSpace == true)
    {
        CString strOld = strEdit.Mid(strEdit.GetLength() - 2, 1);
        CString strNew = strEdit.Right(1);

        if (m_cEng.IsOneKeyToMultiAlphabetLower(strOld, strNew, &strNew) == true)
        {
            ToEditSendConverText(2, strNew);
        }
        else if (m_cEng.IsOneKeyToMultiAlphabetUpper(strOld, strNew, &strNew) == true)
        {
            ToEditSendConverText(2, strNew);
        }
        else if (IsOneKeyToMultiSymbol(strOld, strNew, &strNew) == true)
        {
            ToEditSendConverText(2, strNew);
        }
    }
    m_bNotSpace = true;
}

void	CKeyboard_CheonJiInDlg::PushHangul(UINT nID)
{
    CKeyboardDlg* pDlg = (CKeyboardDlg*)m_pParent;
    CWnd* pButton = GetDlgItem(nID);

    // 이전 문자 초 중 종성 담기
    CString strOldCho = _T(" ");
    CString strOldJung = _T(" ");
    CString strOldJong = _T(" ");
    CString strOldCheon = _T(" ");

    // 입력 문자 초 중 종성 담기
    CString strNewCho = _T(" ");
    CString strNewJung = _T(" ");
    CString strNewJong = _T(" ");
    CString strNewCheon = _T(" ");

    CString strDevide1 = _T(" ");
    CString strDevide2 = _T(" ");
    CString strConvert = _T("");

    // Edit 컨트롤에 입력되어 있는 문자
    CString strEdit = _T("");
    pDlg->GetTestDlg()->m_edtSample.GetWindowTextW(strEdit);

    // 버튼 캡션
    CString strButtonName;
    pButton->GetWindowTextW(strButtonName);

    // 입력 문자를 버튼 캡션의 제일 왼쪽 문자로 가져온다.
    CString strInputting = strButtonName.Left(1);

    //일단 글자 추가
    ToEditSendConverText(0, strInputting);

    bool bLoop = m_bNotSpace;
    //추가 후 edit 박스의 마지막 두 문자를 확인한다.
    while (bLoop)
    {
        bLoop = false;
        pDlg->GetTestDlg()->m_edtSample.GetWindowTextW(strEdit);
        if (strEdit.GetLength() - 2 < 0)
        {
            break;
        }

        CString strOld = strEdit.Mid(strEdit.GetLength() - 2, 1);
        CString strNew = strEdit.Right(1);

        //마지막으로 생성된 문자가 무엇인지 확인한다. //자음, 모음, 천  
        eEntered eInputtingType = m_cHangul.DevideHangul(strNew, &strNewCho, &strNewJung, &strNewJong, &strNewCheon);

        //이전의 문자가 무엇인지 확인한다.
        eEntered eEnteredType = m_cHangul.DevideHangul(strOld, &strOldCho, &strOldJung, &strOldJong, &strOldCheon);

        if (eInputtingType == eEntered::ePerfectWord || eInputtingType == eEntered::eMissingProp)
        {
            break;
        }

        // 종성이 나뉠 수 있는 지 확인
        if (eEnteredType == eEntered::ePerfectWord)
        {
            // 이전 문자의 받침이 나뉘는 지 확인
            if (m_cHangul.IsDevideConsonant(strOldJong, &strDevide1, &strDevide2) == true)
            {
                if (eInputtingType == eEntered::eConsonant)
                {
                    // 롨 ㅅ -> 롫
                    // ㅅ ㅅ -> ㅎ
                    if (ConvertHangulProcess(&CHangulProcess::IsOneKeyToMultiCho, &strConvert, strDevide2, strNew, _T(" ")) == true)
                    {
                        // ㄹㅎ -> ㅀ
                        if (ConvertHangulProcess(&CHangulProcess::IsCombineConsonant, &strConvert, strDevide1, strConvert, _T(" ")) == true)
                        {
                            // 로 ㅀ -> 롫
                            ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strConvert, strOldCho, strOldJung, strConvert);
                            bLoop = true;
                        }
                        //랅 ㄱ -> 랄ㅋ
                        else
                        {
                            ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strOld, strOldCho, strOldJung, strDevide1);
                            strConvert = strOld + strConvert;
                            bLoop = true;
                        }
                    }
                }
                else if (eInputtingType == eEntered::eVowel)
                {
                    // 않 ㅏ -> 안하
                    ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strOld, strOldCho, strOldJung, strDevide1);
                    ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strNew, strDevide2, strNewJung, strNewJong);

                    strConvert = strOld + strNew;
                    bLoop = true;
                }
            }
            else
            {
                if (eInputtingType == eEntered::eConsonant)
                {         
                    // 안 ㅎ -> 않
                    if(ConvertHangulProcess(&CHangulProcess::IsCombineConsonant, &strConvert, strOldJong, strNew) == true)
                    {                        
                        ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strConvert, strOldCho, strOldJung, strConvert);
                        bLoop = true;
                    }
                    // 악 ㄱ -> 앜
                    else if(ConvertHangulProcess(&CHangulProcess::IsOneKeyToMultiCho, &strConvert, strOldJong, strNew) == true)
                    {
                        if (ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strConvert, strOldCho, strOldJung, strConvert) == true)
                        {
                            bLoop = true;
                        }
                        // 안만들어 지는 단어들이 있음 아 ㄸ, 아ㅉ 등등
                        else
                        {
                            ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strOld, strOldCho, strOldJung);
                            strConvert = strOld + strConvert;
                            bLoop = true;
                        }
                    }
                }
                else if (eInputtingType == eEntered::eVowel)
                {
                    // 안 ㅏ -> 아나
                    if(ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strNew, strOldJong, strNew) == true)
                    {
                        ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strOld, strOldCho, strOldJung);
                        strConvert = strOld + strNew;
                        bLoop = true;
                    }
                }
            }
        }
        else if (eEnteredType == eEntered::eMissingProp)
        {
            if (eInputtingType == eEntered::eConsonant)
            {
                // 아 ㄴ -> 안
                if (ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strConvert, strOldCho, strOldJung, strNew) == true)
                {
                    bLoop = true;
                }
            }
            else if (eInputtingType == eEntered::eVowel)
            {
                // 아 ㅣ -> 애
                if(ConvertHangulProcess(&CHangulProcess::IsCombineVowel, &strConvert, strOldJung, strNew) == true)
                {
                    ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strConvert, strOldCho, strConvert);
                    bLoop = true;
                }
            }
            else if (eInputtingType == eEntered::eCheon)
            {
                // 아 . -> 야 -> 야 . -> 아
                if (m_cHangul.IsDevideCheonJiIn(strOldJung, &strDevide1, &strDevide2) == true)
                {
                    if (ConvertHangulProcess(&CHangulProcess::IsOneKeyToMultiCheon, &strConvert, strDevide2, strNew) == true)
                    {
                        if (ConvertHangulProcess(&CHangulProcess::IsCombineCheonJiIn, &strConvert, strDevide1, strConvert) == true)
                        {
                            ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strConvert, strOldCho, strConvert, _T(" "));
                            bLoop = true;
                        }
                    }
                }
                // 애 . -> 아 ㅏ
                // ㅐ -> ㅏ ㅣ
                else if (m_cHangul.IsDevideVowel(strOldJung, &strDevide1, &strDevide2) == true)
                {
                    // ㅇ ㅏ
                    if (ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strOld, strOldCho, strDevide1) == true)
                    {
                        // ㅣ . -> ㅏ
                        if (ConvertHangulProcess(&CHangulProcess::IsCombineCheonJiIn, &strConvert, strDevide2, strNew) == true)
                        {
                            strConvert = strOld + strConvert;
                            bLoop = true;
                        }
                    }
                }
                // 이 . -> 아
                else if (ConvertHangulProcess(&CHangulProcess::IsCombineCheonJiIn, &strConvert, strOldJung, strNew) == true)
                {
                    ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strConvert, strOldCho, strConvert);
                    bLoop = true;
                }
            }
        }
        else if (eEnteredType == eEntered::eConsonant)
        {
            if (eInputtingType == eEntered::eConsonant)
            {
                // ㄳ ㅅ-> ㄱㅎ ㅅ-> ㄱㅆ ㅅ-> ㄳ
                if (m_cHangul.IsDevideConsonant(strOldJong, &strDevide1, &strDevide2) == true)
                {
                    if (ConvertHangulProcess(&CHangulProcess::IsOneKeyToMultiCho, &strConvert, strDevide2, strNew) == true)
                    {
                        strConvert = strDevide1 + strConvert;
                        bLoop = true;
                    }
                }
                // ㄱ ㄱ -> ㅋ
                else if(ConvertHangulProcess(&CHangulProcess::IsOneKeyToMultiCho, &strConvert, strOldCho, strNew) == true)
                {
                    bLoop = true;
                }
                // ㄱ ㅅ -> ㄳ
                else if(ConvertHangulProcess(&CHangulProcess::IsCombineConsonant, &strConvert, strOldCho, strNew) == true)
                {
                    bLoop = true;
                }
            }
            else if (eInputtingType == eEntered::eVowel)
            {
                // 이전 자음이 문자가 두글 자인지 확인
                // ㄳ ㅏ -> ㄱ사
                if (m_cHangul.IsDevideConsonant(strOldJong, &strDevide1, &strDevide2) == true)
                {
                    ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strNew, strDevide2, strNew);
                    strConvert = strDevide1 + strNew;
                    bLoop = true;
                }
                // ㄱ ㅏ -> 가
                else if(ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strConvert, strOldCho, strNew) == true)
                {
                    bLoop = true;
                }
            }
        }
        else if (eEnteredType == eEntered::eVowel)
        {
            if (eInputtingType == eEntered::eVowel)
            {
                // ㅏ ㅣ ->ㅐ
                if(ConvertHangulProcess(&CHangulProcess::IsCombineVowel, &strConvert, strOldJung, strNew) == true)
                {
                    bLoop = true;
                }
            }
            else if (eInputtingType == eEntered::eCheon)
            {
                // ㅏ . -> ㅑ . -> ㅏ
                if (m_cHangul.IsDevideCheonJiIn(strOldJung, &strDevide1, &strDevide2) == true)
                {
                    if (ConvertHangulProcess(&CHangulProcess::IsOneKeyToMultiCheon, &strConvert, strDevide2, strNew) == true)
                    {
                        if (ConvertHangulProcess(&CHangulProcess::IsCombineCheonJiIn, &strConvert, strDevide1, strConvert) == true)
                        {
                            bLoop = true;
                        }
                    }
                }
                // ㅐ  . -> ㅏ ㅏ
                // ㅐ -> ㅏ ㅣ
                else if (m_cHangul.IsDevideVowel(strOldJung, &strDevide1, &strDevide2) == true)
                {
                    if (ConvertHangulProcess(&CHangulProcess::IsCombineCheonJiIn, &strConvert, strDevide2, strNew) == true)
                    {
                        // ㅏ ㅏ 
                        strConvert = strDevide1 + strConvert;
                        bLoop = true;
                    }
                }

                // ㅣ . -> ㅏ
                else if (ConvertHangulProcess(&CHangulProcess::IsCombineCheonJiIn, &strConvert, strOldJung, strNew) == true)
                {
                    bLoop = true;
                }
            }
        }
        else if (eEnteredType == eEntered::eCheon)
        {
            if (eInputtingType == eEntered::eVowel)
            {
                // . ㅣ -> ㅓ
                if (ConvertHangulProcess(&CHangulProcess::IsCombineCheonJiIn, &strConvert, strOldCheon, strNew) == true)
                {
                    bLoop = true;
                }
            }
            else if (eInputtingType == eEntered::eCheon)
            {
                // . . -> ..
                if (ConvertHangulProcess(&CHangulProcess::IsOneKeyToMultiCheon, &strConvert, strOldCheon, strNew) == true)
                {
                    bLoop = true;
                }
            }
        }
        else
        {
            if (IsOneKeyToMultiSymbol(strOld, strNew, &strConvert) == true)
            {
                bLoop = true;
            }
        }

        if (bLoop == true)
        {
            ToEditSendConverText(2, strConvert);
        }
    }

    // 버튼을 눌렀다는 것을 인식 시켜줘야 함.
    m_bNotSpace = true;
}
void	CKeyboard_CheonJiInDlg::PushBackSpace()
{
    CKeyboardDlg* pDlg = (CKeyboardDlg*)m_pParent;

    // Edit 컨트롤에 입력되어 있는 문자
    CString strEdit = _T("");
    pDlg->GetTestDlg()->m_edtSample.GetWindowTextW(strEdit);

    // 마지막 문자 가져오기
    CString strOld = strEdit.Right(1);
    
    // 문자 초 중 종성 담기
    CString strOldCho = _T(" ");
    CString strOldJung = _T(" ");
    CString strOldJong = _T(" ");
    CString strDevide1 = _T(" ");
    CString strDevide2 = _T(" ");
    CString strOldCheon = _T(" ");

    // 변경될 문자 담는 버퍼
    CString strConvert = _T("");

    // 이전의 문자가 무엇인지 확인한다.
    eEntered eEnteredType = m_cHangul.DevideHangul(strOld, &strOldCho, &strOldJung, &strOldJong, &strOldCheon);

    if (eEnteredType == eEntered::ePerfectWord)
    {
        // 이전 문자의 받침이 나뉘는 지 확인
        // 않 backspace -> 안
        if (m_cHangul.IsDevideConsonant(strOldJong, &strDevide1, &strDevide2) == true)
        {
            ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strConvert, strOldCho, strOldJung, strDevide1);
        }
        // 안 -> 아
        else
        {
            ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strConvert, strOldCho, strOldJung);
        }
    }
    else if (eEnteredType == eEntered::eMissingProp)
    {
        //애 -> 아
        if (m_cHangul.IsDevideVowel(strOldJung, &strDevide1, &strDevide2) == true)
        {
            ConvertHangulProcess(&CHangulProcess::CombineConsonantAndVowel, &strConvert, strOldCho, strDevide1);
        }
        // 아 -> ㅇ
        else
        {
            strConvert = strOldCho;
        }
    }
    else if (eEnteredType == eEntered::eConsonant)
    {
        // ㄳ -> ㄱ
        if (m_cHangul.IsDevideConsonant(strOldJong, &strDevide1, &strDevide2) == true)
        {
            strConvert = strDevide1;
        }
    }
    else if (eEnteredType == eEntered::eVowel)
    {
        // ㅐ -> ㅏ
        if (m_cHangul.IsDevideVowel(strOldJung, &strDevide1, &strDevide2) == true)
        {
            strConvert = strDevide1;
        }
    }

    ToEditSendConverText(1, strConvert);

    if (m_bHangul == false)
    {
        m_bNotSpace = false;
    }
}



BOOL CKeyboard_CheonJiInDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
    CKeyboardDlg* pDlg = (CKeyboardDlg*)m_pParent;
    if (pMsg->message == WM_KEYDOWN)
    {
        // 키보드 입력 메시지 처리
        // 예: 특정 키에 대한 동작 수행
        switch (pMsg->wParam)
        {
        case VK_NUMPAD7:
            OnButtonClick(IDC_btVowel_In);
            break;
        case VK_NUMPAD8:
            OnButtonClick(IDC_btVowel_Cheon);
            break;
        case VK_NUMPAD9:
            OnButtonClick(IDC_btVowel_Ji);
            break;
        case VK_NUMPAD4:
            OnButtonClick(IDC_btConsonant1);
            break;
        case VK_NUMPAD5:
            OnButtonClick(IDC_btConsonant2);
            break;
        case VK_NUMPAD6:
            OnButtonClick(IDC_btConsonant3);
            break;
        case VK_NUMPAD1:
            OnButtonClick(IDC_btConsonant4);
            break;
        case VK_NUMPAD2:
            OnButtonClick(IDC_btConsonant5);
            break;
        case VK_NUMPAD3:
            OnButtonClick(IDC_btConsonant6);
            break;
        case VK_NUMPAD0:
            OnButtonClick(IDC_btConsonant7);
            break;
        case VK_SUBTRACT:
            OnButtonClick(IDC_btBackSpace);
            break;
        case VK_DECIMAL:
            OnButtonClick(IDC_btSpace);
            break;
        }
        this->SetFocus();
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}


void CKeyboard_CheonJiInDlg::OnBnClickedbtkeConversion()
{
    CString str;
    GetDlgItemText(IDC_btVowel_In, str);
    if(m_bHangul == true)
    {
        SetDlgItemText(IDC_btVowel_In, _T(".,?!"));
        SetDlgItemText(IDC_btVowel_Cheon, _T("a b c"));
        SetDlgItemText(IDC_btVowel_Ji, _T("d e f"));
        SetDlgItemText(IDC_btConsonant1, _T("g h i"));
        SetDlgItemText(IDC_btConsonant2, _T("j k l"));
        SetDlgItemText(IDC_btConsonant3, _T("m n o"));
        SetDlgItemText(IDC_btConsonant4, _T("p q r s"));
        SetDlgItemText(IDC_btConsonant5, _T("t u v"));
        SetDlgItemText(IDC_btConsonant6, _T("w x y z"));
        SetDlgItemText(IDC_btConsonant7, _T(""));
        SetDlgItemText(IDC_btSymbol, _T("Shift"));
        m_bHangul = false;
    }
    else
    {
        SetDlgItemText(IDC_btVowel_In, _T("ㅣ"));
        SetDlgItemText(IDC_btVowel_Cheon, _T("·"));
        SetDlgItemText(IDC_btVowel_Ji, _T("ㅡ"));
        SetDlgItemText(IDC_btConsonant1, _T("ㄱ ㅋ"));
        SetDlgItemText(IDC_btConsonant2, _T("ㄴ ㄹ"));
        SetDlgItemText(IDC_btConsonant3, _T("ㄷ ㅌ"));
        SetDlgItemText(IDC_btConsonant4, _T("ㅂ ㅍ"));
        SetDlgItemText(IDC_btConsonant5, _T("ㅅ ㅎ"));
        SetDlgItemText(IDC_btConsonant6, _T("ㅈ ㅊ"));
        SetDlgItemText(IDC_btConsonant7, _T("ㅇ ㅁ"));
        SetDlgItemText(IDC_btSymbol, _T(".,?!"));
        m_bHangul = true;
    }
}

void CKeyboard_CheonJiInDlg::ConvertEngCase()
{
    CString str;
    GetDlgItemText(IDC_btVowel_In, str);
    if (m_bUpperCase == true)
    {
        //SetDlgItemText(IDC_btVowel_In, _T(".,?!"));
        SetDlgItemText(IDC_btVowel_Cheon, _T("a b c"));
        SetDlgItemText(IDC_btVowel_Ji, _T("d e f"));
        SetDlgItemText(IDC_btConsonant1, _T("g h i"));
        SetDlgItemText(IDC_btConsonant2, _T("j k l"));
        SetDlgItemText(IDC_btConsonant3, _T("m n o"));
        SetDlgItemText(IDC_btConsonant4, _T("p q r s"));
        SetDlgItemText(IDC_btConsonant5, _T("t u v"));
        SetDlgItemText(IDC_btConsonant6, _T("w x y z"));
        SetDlgItemText(IDC_btConsonant7, _T(""));
        SetDlgItemText(IDC_btSymbol, _T("Shift"));
        m_bUpperCase = false;
    }
    else
    {
       // SetDlgItemText(IDC_btVowel_In, _T("ㅣ"));
        SetDlgItemText(IDC_btVowel_Cheon, _T("A B C"));
        SetDlgItemText(IDC_btVowel_Ji, _T("D E F"));
        SetDlgItemText(IDC_btConsonant1, _T("G H I"));
        SetDlgItemText(IDC_btConsonant2, _T("J K L"));
        SetDlgItemText(IDC_btConsonant3, _T("M N O"));
        SetDlgItemText(IDC_btConsonant4, _T("P Q R S"));
        SetDlgItemText(IDC_btConsonant5, _T("T U V"));
        SetDlgItemText(IDC_btConsonant6, _T("W X Y Z"));
        SetDlgItemText(IDC_btConsonant7, _T(""));
        //SetDlgItemText(IDC_btSymbol, _T(".,?!"));
        m_bUpperCase = true;
    }

}


bool		CKeyboard_CheonJiInDlg::IsOneKeyToMultiSymbol(const CString str1, const CString str2, CString* str)
{
    bool bResult = false;
    const WCHAR				wWord[4] = { L'.', L',', L'?', L'!' };

    int n = 0;
    for (int i = 0; i < 4; i++)
    {
        if (str1 == wWord[i])
        {
            bResult = true;
            n = i;
            break;
        }
    }

    if (bResult == true)
    {
        if (str2 == wWord[0])
        {
            *str = wWord[(n + 1) % 4];
            bResult = true;
        }
        else
        {
            *str = str2;
            bResult = false;
        }
    }
    return bResult;
}