#include "pch.h"
#include "HangulProcess.h"
#include <regex>
bool CHangulProcess::CombineConsonantAndVowel(const CString consonant, const CString vowel, const CString consonant2, CString *str)
{
    bool bResult = true;
    const wchar_t choSungs[] = { L'ㄱ', L'ㄲ', L'ㄴ', L'ㄷ', L'ㄸ', L'ㄹ', L'ㅁ', L'ㅂ', L'ㅃ', L'ㅅ', L'ㅆ', L'ㅇ', L'ㅈ', L'ㅉ', L'ㅊ', L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ' };
    const wchar_t jungSungs[] = { L'ㅏ', L'ㅐ', L'ㅑ', L'ㅒ', L'ㅓ', L'ㅔ', L'ㅕ', L'ㅖ', L'ㅗ', L'ㅘ', L'ㅙ', L'ㅚ', L'ㅛ', L'ㅜ', L'ㅝ', L'ㅞ', L'ㅟ', L'ㅠ', L'ㅡ', L'ㅢ', L'ㅣ' };
    const wchar_t jongSungs[] = { L' ', L'ㄱ', L'ㄲ', L'ㄳ', L'ㄴ', L'ㄵ', L'ㄶ', L'ㄷ', L'ㄹ', L'ㄺ', L'ㄻ', L'ㄼ', L'ㄽ', L'ㄾ', L'ㄿ', L'ㅀ', L'ㅁ', L'ㅂ', L'ㅄ', L'ㅅ', L'ㅆ', L'ㅇ', L'ㅈ', L'ㅊ', L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ' };

    int choIndex = -1;
    int jungIndex = -1;
    int jongIndex = -1;

    // 초성 인덱스 찾기
    for (int i = 0; i < sizeof(choSungs) / sizeof(wchar_t); i++)
    {
        if (consonant == choSungs[i])
        {
            choIndex = i;
            break;
        }
    }

    // 중성 인덱스 찾기
    for (int i = 0; i < sizeof(jungSungs) / sizeof(wchar_t); i++)
    {
        if (vowel == jungSungs[i])
        {
            jungIndex = i;
            break;
        }
    }

    // 종성 인덱스 찾기
    if (consonant2.IsEmpty() != true)
    {
        for (int i = 0; i < sizeof(jongSungs) / sizeof(wchar_t); i++)
        {
            if (consonant2 == jongSungs[i])
            {
                jongIndex = i;
                break;
            }
        }
    }
    


    if (choIndex == -1 || jungIndex == -1 || jongIndex == -1)
    {
        bResult = false;
    }
    else
    {
        // 한글 유니코드 계산
        wchar_t unicode = /*0xAC00*/ _T('가') + ((choIndex * 21 * 28) + (jungIndex * 28) + jongIndex);

        // 유니코드 값을 문자열로 변환하여 반환
        //*(str).Format(_T("%c"), unicode);
        *str = unicode;
        bResult = true;
    }
    

    return bResult;
}

// 자음 같은 것 눌렀을 때, ㄱ ㄱ -> ㅋ
bool		CHangulProcess::IsOneKeyToMultiCho(const CString str1, const CString str2, const CString str3, CString* str)
{
    bool bResult = false;
    const WCHAR				wWord[7][3] = { {L'ㄱ', L'ㅋ', L'ㄲ'}, {L'ㄴ', L'ㄹ',}, {L'ㄷ', L'ㅌ', L'ㄸ'}, 
                                                {L'ㅂ', L'ㅍ', L'ㅃ'}, {L'ㅅ', L'ㅎ', L'ㅆ'}, {L'ㅈ', L'ㅊ', L'ㅉ'} ,{L'ㅇ', L'ㅁ',} };
    
    int nDevide = 0;

    typedef struct _stARRAY
    {
        int  i;
        int  j;
    } ARRAY;

    ARRAY stArray;
    memset(&stArray, 0, sizeof(ARRAY));
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (str1 == wWord[i][j])
            {
                stArray.i = i;
                stArray.j = j;

                bResult = true;
                break;
            }
        }
        if (bResult == true)
        {
            break;
        }
    }

    if (bResult == true)
    {
        if (stArray.i == 1 || stArray.i == 6) //ㄴ ㄹ , ㅇ ㅁ
        {
            nDevide = 2;
        }
        else
        {
            nDevide = 3;
        }

        if (str2 == wWord[stArray.i][0])
        {
            *str = wWord[stArray.i][(stArray.j + 1) % nDevide];
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

// 천 같은 것 눌렀을 때 '·', '‥' 전환
bool		CHangulProcess::IsOneKeyToMultiCheon(const CString str1, const CString str2, const CString str3, CString* str)
{
    bool bResult = false;
    const WCHAR				wWord[2] = { L'·', L'‥' };

    int n = 0;
    for (int i = 0; i < 2; i++)
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
            *str = wWord[(n + 1) % 2];
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


// 자음끼리 합쳐지는 것 ㄴ ㅎ -> ㄶ
bool		CHangulProcess::IsCombineConsonant(const CString str1, const CString str2, const CString str3, CString* str)
{
    bool bResult = true;
    const CString strWord[] = { L"ㄱㅅ", L"ㄴㅈ", L"ㄴㅎ", L"ㄹㄱ", L"ㄹㅁ", L"ㄹㅂ", L"ㄹㅅ", L"ㄹㅌ", L"ㄹㅍ", L"ㄹㅎ", L"ㅂㅅ"};
    CString strCombine;
    strCombine = str1 + str2;

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (strCombine == strWord[0])             { *str = _T('ㄳ'); }
        else if (strCombine == strWord[1])        { *str = _T('ㄵ'); }
        else if (strCombine == strWord[2])        { *str = _T('ㄶ'); }
        else if (strCombine == strWord[3])        { *str = _T('ㄺ'); }
        else if (strCombine == strWord[4])        { *str = _T('ㄻ'); }
        else if (strCombine == strWord[5])        { *str = _T('ㄼ'); }
        else if (strCombine == strWord[6])        { *str = _T('ㄽ'); }
        else if (strCombine == strWord[7])        { *str = _T('ㄾ'); }
        else if (strCombine == strWord[8])        { *str = _T('ㄿ'); }
        else if (strCombine == strWord[9])        { *str = _T('ㅀ'); }
        else if (strCombine == strWord[10])       { *str = _T('ㅄ'); }
        else
        {
            bResult = false;
        }
    }
    return bResult;
}

bool		CHangulProcess::IsCombineVowel(const CString str1, const CString str2, const CString str3, CString* str)
{
    bool bResult = true;
    const CString strWord[] = { L"ㅏㅣ", L"ㅑㅣ", L"ㅓㅣ", L"ㅕㅣ", L"ㅗㅏ", L"ㅘㅣ", L"ㅗㅣ", L"ㅠㅣ", L"ㅝㅣ", L"ㅜㅣ", L"ㅡㅣ" };
    CString strCombine;
    strCombine = str1 + str2;

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (strCombine == strWord[0]) { *str = _T('ㅐ'); }
        else if (strCombine == strWord[1]) { *str = _T('ㅒ'); }
        else if (strCombine == strWord[2]) { *str = _T('ㅔ'); }
        else if (strCombine == strWord[3]) { *str = _T('ㅖ'); }
        else if (strCombine == strWord[4]) { *str = _T('ㅘ'); }
        else if (strCombine == strWord[5]) { *str = _T('ㅙ'); }
        else if (strCombine == strWord[6]) { *str = _T('ㅚ'); }
        else if (strCombine == strWord[7]) { *str = _T('ㅝ'); }
        else if (strCombine == strWord[8]) { *str = _T('ㅞ'); }
        else if (strCombine == strWord[9]) { *str = _T('ㅟ'); }
        else if (strCombine == strWord[10]) { *str = _T('ㅢ'); }
        else
        {
            bResult = false;
        }
    }
    return bResult;
}

bool		CHangulProcess::IsCombineCheonJiIn(const CString str1, const CString str2, const CString str3, CString* str)
{

    bool bResult = true;
    const CString strWord[] = { L"ㅣ·", L"ㅏ·", L"ㅣ‥", L"·ㅣ", L"‥ㅣ", L"·ㅡ", L"ㅚ·", L"‥ㅡ", L"ㅡ·", L"ㅡ‥", L"ㅜ·" };
    CString strCombine;
    strCombine = str1 + str2;

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (strCombine == strWord[0])         { *str = _T('ㅏ'); }
        else if (strCombine == strWord[1]) { *str = _T('ㅑ'); }
        else if (strCombine == strWord[2]) { *str = _T('ㅑ'); }
        else if (strCombine == strWord[3]) { *str = _T('ㅓ'); }
        else if (strCombine == strWord[4]) { *str = _T('ㅕ'); }
        else if (strCombine == strWord[5]) { *str = _T('ㅗ'); }
        else if (strCombine == strWord[6]) { *str = _T('ㅘ'); }
        else if (strCombine == strWord[7]) { *str = _T('ㅛ'); }
        else if (strCombine == strWord[8]) { *str = _T('ㅜ'); }
        else if (strCombine == strWord[9]) { *str = _T('ㅠ'); }
        else if (strCombine == strWord[10]) { *str = _T('ㅠ'); }
        else
        {
            bResult = false;
        }
    }
    return bResult;
}

bool		CHangulProcess::IsDevideConsonant(const CString str, CString* str1, CString* str2)
{
    bool bResult = true;
    const CString strWord[] = { L"ㄳ", L"ㄵ", L"ㄶ", L"ㄺ", L"ㄻ", L"ㄼ", L"ㄽ", L"ㄾ", L"ㄿ", L"ㅀ", L"ㅄ" };

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (str == strWord[0])             { *str1 = _T('ㄱ'); *str2 = _T('ㅅ');}
        else if (str == strWord[1])        { *str1 = _T('ㄴ'); *str2 = _T('ㅈ');}
        else if (str == strWord[2])        { *str1 = _T('ㄴ'); *str2 = _T('ㅎ');}
        else if (str == strWord[3])        { *str1 = _T('ㄹ'); *str2 = _T('ㄱ');}
        else if (str == strWord[4])        { *str1 = _T('ㄹ'); *str2 = _T('ㅁ');}
        else if (str == strWord[5])        { *str1 = _T('ㄹ'); *str2 = _T('ㅂ');}
        else if (str == strWord[6])        { *str1 = _T('ㄹ'); *str2 = _T('ㅅ');}
        else if (str == strWord[7])        { *str1 = _T('ㄹ'); *str2 = _T('ㅌ');}
        else if (str == strWord[8])        { *str1 = _T('ㄹ'); *str2 = _T('ㅍ');}
        else if (str == strWord[9])        { *str1 = _T('ㄹ'); *str2 = _T('ㅎ');}
        else if (str == strWord[10])       { *str1 = _T('ㅂ'); *str2 = _T('ㅅ');}
        else
        {
            bResult = false;
        }
    }
    return bResult;
}

bool		CHangulProcess::IsDevideVowel(const CString str, CString* str1, CString* str2)
{
    bool bResult = true;
    const CString strWord[] = { L"ㅐ", L"ㅒ", L"ㅔ", L"ㅖ", L"ㅘ", L"ㅙ", L"ㅚ", L"ㅝ", L"ㅞ", L"ㅟ", L"ㅢ" };

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (str == strWord[0])         { *str1 = _T('ㅏ'); *str2 = _T('ㅣ'); }
        else if (str == strWord[1])    { *str1 = _T('ㅑ'); *str2 = _T('ㅣ'); }
        else if (str == strWord[2])    { *str1 = _T('ㅓ'); *str2 = _T('ㅣ'); }
        else if (str == strWord[3])    { *str1 = _T('ㅕ'); *str2 = _T('ㅣ'); }
        else if (str == strWord[4])    { *str1 = _T('ㅗ'); *str2 = _T('ㅏ'); }
        else if (str == strWord[5])    { *str1 = _T('ㅗ'); *str2 = _T('ㅐ'); }
        else if (str == strWord[6])    { *str1 = _T('ㅗ'); *str2 = _T('ㅣ'); }
        else if (str == strWord[7])    { *str1 = _T('ㅜ'); *str2 = _T('ㅓ'); }
        else if (str == strWord[8])    { *str1 = _T('ㅜ'); *str2 = _T('ㅔ'); }
        else if (str == strWord[9])    { *str1 = _T('ㅜ'); *str2 = _T('ㅣ'); }
        else if (str == strWord[10])   { *str1 = _T('ㅡ'); *str2 = _T('ㅣ'); }
        else
        {
            bResult = false;
        }
    }
    return bResult;
}

bool		CHangulProcess::IsDevideCheonJiIn(const CString str, CString* str1, CString* str2)
{

    bool bResult = true;
    const CString strWord[] = { L"ㅏ", L"ㅑ",  L"ㅓ", L"ㅕ", L"ㅗ", L"ㅘ", L"ㅛ", L"ㅜ", L"ㅠ"};
    //· ‥

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (str == strWord[0])          { *str1 = _T('ㅣ'); *str2 = _T('·'); }
        else if (str == strWord[1])     { *str1 = _T('ㅣ'); *str2 = _T('‥'); }
        else if (str == strWord[2])     { *str1 = _T('·'); *str2 = _T('ㅣ'); }
        else if (str == strWord[3])     { *str1 = _T('‥'); *str2 = _T('ㅣ'); }
        else if (str == strWord[4])     { *str1 = _T('·'); *str2 = _T('ㅡ'); }
        else if (str == strWord[5])     { *str1 = _T('ㅚ'); *str2 = _T('·'); }
        else if (str == strWord[6])     { *str1 = _T('‥'); *str2 = _T('ㅡ'); }
        else if (str == strWord[7])     { *str1 = _T('ㅡ'); *str2 = _T('·'); }
        else if (str == strWord[8])     { *str1 = _T('ㅡ'); *str2 = _T('‥'); }
        else
        {
            bResult = false;
        }
    }
    return bResult;
}


eEntered CHangulProcess::DevideHangul(const CString str, CString* strCho, CString* strJung, CString* strJong, CString* strCheon)
{
    const wchar_t choSungs[] = { L'ㄱ', L'ㄲ', L'ㄴ', L'ㄷ', L'ㄸ', L'ㄹ', L'ㅁ', L'ㅂ', L'ㅃ', L'ㅅ', L'ㅆ', L'ㅇ', L'ㅈ', L'ㅉ', L'ㅊ', L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ' };
    const wchar_t jungSungs[] = { L'ㅏ', L'ㅐ', L'ㅑ', L'ㅒ', L'ㅓ', L'ㅔ', L'ㅕ', L'ㅖ', L'ㅗ', L'ㅘ', L'ㅙ', L'ㅚ', L'ㅛ', L'ㅜ', L'ㅝ', L'ㅞ', L'ㅟ', L'ㅠ', L'ㅡ', L'ㅢ', L'ㅣ' };
    const wchar_t jongSungs[] = { L' ', L'ㄱ', L'ㄲ', L'ㄳ', L'ㄴ', L'ㄵ', L'ㄶ', L'ㄷ', L'ㄹ', L'ㄺ', L'ㄻ', L'ㄼ', L'ㄽ', L'ㄾ', L'ㄿ', L'ㅀ', L'ㅁ', L'ㅂ', L'ㅄ', L'ㅅ', L'ㅆ', L'ㅇ', L'ㅈ', L'ㅊ', L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ' };
    const wchar_t cheonSungs[] = { L'·', L'‥' };

    if (str.IsEmpty() != true)
    {
        std::wstring strRegex;
        //strRegex = CT2CA(strWord);
        std::wregex re(L"([가-힣])");
        
        if (std::regex_search(str.GetString(), re))
        {
            int unicode = str.GetAt(0) - _T('가');
            int nChoSungs = (int)((((unicode - (unicode % 28)) / 28) / 21));
            int nJungSungs = (int)((((unicode - (unicode % 28)) / 28) % 21));
            int nJongSungs = (int)((unicode % 28));

            *strCho = choSungs[nChoSungs];
            *strJung = jungSungs[nJungSungs];
            *strJong = jongSungs[nJongSungs];

            if (nJongSungs == 0)
            {
                return eEntered::eMissingProp;
            }
            else
            {
                return eEntered::ePerfectWord;
            }
        }
        else
        {
            // 초성 인덱스 찾기
            for (int i = 0; i < sizeof(choSungs) / sizeof(wchar_t); i++)
            {
                if (str == choSungs[i])
                {
                    *strCho = choSungs[i];
                    return eEntered::eConsonant;
                }
            }

            // 중성 인덱스 찾기
            for (int i = 0; i < sizeof(jungSungs) / sizeof(wchar_t); i++)
            {
                if (str == jungSungs[i])
                {
                    *strJung = jungSungs[i];
                    return eEntered::eVowel;
                }
            }

            // 종성 인덱스 찾기
            for (int i = 1; i < sizeof(jongSungs) / sizeof(wchar_t); i++)
            {
                if (str == jongSungs[i])
                {
                    *strJong = jongSungs[i];
                    return eEntered::eConsonant;
                }
            }

            // 천 인덱스 찾기
            for (int i = 0; i < sizeof(cheonSungs) / sizeof(wchar_t); i++)
            {
                if (str == cheonSungs[i])
                {
                    *strCheon = cheonSungs[i];
                    return eEntered::eCheon;
                }
            }

        }
    }
    return eEntered::eNone;
}

