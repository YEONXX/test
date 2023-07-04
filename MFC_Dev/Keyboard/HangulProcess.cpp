#include "pch.h"
#include "HangulProcess.h"
#include <regex>
bool CHangulProcess::CombineConsonantAndVowel(const CString consonant, const CString vowel, const CString consonant2, CString *str)
{
    bool bResult = true;
    const wchar_t choSungs[] = { L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��' };
    const wchar_t jungSungs[] = { L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��' };
    const wchar_t jongSungs[] = { L' ', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��' };

    int choIndex = -1;
    int jungIndex = -1;
    int jongIndex = -1;

    // �ʼ� �ε��� ã��
    for (int i = 0; i < sizeof(choSungs) / sizeof(wchar_t); i++)
    {
        if (consonant == choSungs[i])
        {
            choIndex = i;
            break;
        }
    }

    // �߼� �ε��� ã��
    for (int i = 0; i < sizeof(jungSungs) / sizeof(wchar_t); i++)
    {
        if (vowel == jungSungs[i])
        {
            jungIndex = i;
            break;
        }
    }

    // ���� �ε��� ã��
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
        // �ѱ� �����ڵ� ���
        wchar_t unicode = /*0xAC00*/ _T('��') + ((choIndex * 21 * 28) + (jungIndex * 28) + jongIndex);

        // �����ڵ� ���� ���ڿ��� ��ȯ�Ͽ� ��ȯ
        //*(str).Format(_T("%c"), unicode);
        *str = unicode;
        bResult = true;
    }
    

    return bResult;
}

// ���� ���� �� ������ ��, �� �� -> ��
bool		CHangulProcess::IsOneKeyToMultiCho(const CString str1, const CString str2, const CString str3, CString* str)
{
    bool bResult = false;
    const WCHAR				wWord[7][3] = { {L'��', L'��', L'��'}, {L'��', L'��',}, {L'��', L'��', L'��'}, 
                                                {L'��', L'��', L'��'}, {L'��', L'��', L'��'}, {L'��', L'��', L'��'} ,{L'��', L'��',} };
    
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
        if (stArray.i == 1 || stArray.i == 6) //�� �� , �� ��
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

// õ ���� �� ������ �� '��', '��' ��ȯ
bool		CHangulProcess::IsOneKeyToMultiCheon(const CString str1, const CString str2, const CString str3, CString* str)
{
    bool bResult = false;
    const WCHAR				wWord[2] = { L'��', L'��' };

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


// �������� �������� �� �� �� -> ��
bool		CHangulProcess::IsCombineConsonant(const CString str1, const CString str2, const CString str3, CString* str)
{
    bool bResult = true;
    const CString strWord[] = { L"����", L"����", L"����", L"����", L"����", L"����", L"����", L"����", L"����", L"����", L"����"};
    CString strCombine;
    strCombine = str1 + str2;

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (strCombine == strWord[0])             { *str = _T('��'); }
        else if (strCombine == strWord[1])        { *str = _T('��'); }
        else if (strCombine == strWord[2])        { *str = _T('��'); }
        else if (strCombine == strWord[3])        { *str = _T('��'); }
        else if (strCombine == strWord[4])        { *str = _T('��'); }
        else if (strCombine == strWord[5])        { *str = _T('��'); }
        else if (strCombine == strWord[6])        { *str = _T('��'); }
        else if (strCombine == strWord[7])        { *str = _T('��'); }
        else if (strCombine == strWord[8])        { *str = _T('��'); }
        else if (strCombine == strWord[9])        { *str = _T('��'); }
        else if (strCombine == strWord[10])       { *str = _T('��'); }
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
    const CString strWord[] = { L"����", L"����", L"�ä�", L"�Ť�", L"�Ǥ�", L"�Ȥ�", L"�Ǥ�", L"�Ф�", L"�ͤ�", L"�̤�", L"�Ѥ�" };
    CString strCombine;
    strCombine = str1 + str2;

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (strCombine == strWord[0]) { *str = _T('��'); }
        else if (strCombine == strWord[1]) { *str = _T('��'); }
        else if (strCombine == strWord[2]) { *str = _T('��'); }
        else if (strCombine == strWord[3]) { *str = _T('��'); }
        else if (strCombine == strWord[4]) { *str = _T('��'); }
        else if (strCombine == strWord[5]) { *str = _T('��'); }
        else if (strCombine == strWord[6]) { *str = _T('��'); }
        else if (strCombine == strWord[7]) { *str = _T('��'); }
        else if (strCombine == strWord[8]) { *str = _T('��'); }
        else if (strCombine == strWord[9]) { *str = _T('��'); }
        else if (strCombine == strWord[10]) { *str = _T('��'); }
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
    const CString strWord[] = { L"�ӡ�", L"����", L"�ӡ�", L"����", L"����", L"����", L"�ʡ�", L"����", L"�ѡ�", L"�ѡ�", L"�̡�" };
    CString strCombine;
    strCombine = str1 + str2;

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (strCombine == strWord[0])         { *str = _T('��'); }
        else if (strCombine == strWord[1]) { *str = _T('��'); }
        else if (strCombine == strWord[2]) { *str = _T('��'); }
        else if (strCombine == strWord[3]) { *str = _T('��'); }
        else if (strCombine == strWord[4]) { *str = _T('��'); }
        else if (strCombine == strWord[5]) { *str = _T('��'); }
        else if (strCombine == strWord[6]) { *str = _T('��'); }
        else if (strCombine == strWord[7]) { *str = _T('��'); }
        else if (strCombine == strWord[8]) { *str = _T('��'); }
        else if (strCombine == strWord[9]) { *str = _T('��'); }
        else if (strCombine == strWord[10]) { *str = _T('��'); }
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
    const CString strWord[] = { L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��" };

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (str == strWord[0])             { *str1 = _T('��'); *str2 = _T('��');}
        else if (str == strWord[1])        { *str1 = _T('��'); *str2 = _T('��');}
        else if (str == strWord[2])        { *str1 = _T('��'); *str2 = _T('��');}
        else if (str == strWord[3])        { *str1 = _T('��'); *str2 = _T('��');}
        else if (str == strWord[4])        { *str1 = _T('��'); *str2 = _T('��');}
        else if (str == strWord[5])        { *str1 = _T('��'); *str2 = _T('��');}
        else if (str == strWord[6])        { *str1 = _T('��'); *str2 = _T('��');}
        else if (str == strWord[7])        { *str1 = _T('��'); *str2 = _T('��');}
        else if (str == strWord[8])        { *str1 = _T('��'); *str2 = _T('��');}
        else if (str == strWord[9])        { *str1 = _T('��'); *str2 = _T('��');}
        else if (str == strWord[10])       { *str1 = _T('��'); *str2 = _T('��');}
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
    const CString strWord[] = { L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��" };

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (str == strWord[0])         { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[1])    { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[2])    { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[3])    { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[4])    { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[5])    { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[6])    { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[7])    { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[8])    { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[9])    { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[10])   { *str1 = _T('��'); *str2 = _T('��'); }
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
    const CString strWord[] = { L"��", L"��",  L"��", L"��", L"��", L"��", L"��", L"��", L"��"};
    //�� ��

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (str == strWord[0])          { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[1])     { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[2])     { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[3])     { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[4])     { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[5])     { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[6])     { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[7])     { *str1 = _T('��'); *str2 = _T('��'); }
        else if (str == strWord[8])     { *str1 = _T('��'); *str2 = _T('��'); }
        else
        {
            bResult = false;
        }
    }
    return bResult;
}


eEntered CHangulProcess::DevideHangul(const CString str, CString* strCho, CString* strJung, CString* strJong, CString* strCheon)
{
    const wchar_t choSungs[] = { L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��' };
    const wchar_t jungSungs[] = { L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��' };
    const wchar_t jongSungs[] = { L' ', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��', L'��' };
    const wchar_t cheonSungs[] = { L'��', L'��' };

    if (str.IsEmpty() != true)
    {
        std::wstring strRegex;
        //strRegex = CT2CA(strWord);
        std::wregex re(L"([��-�R])");
        
        if (std::regex_search(str.GetString(), re))
        {
            int unicode = str.GetAt(0) - _T('��');
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
            // �ʼ� �ε��� ã��
            for (int i = 0; i < sizeof(choSungs) / sizeof(wchar_t); i++)
            {
                if (str == choSungs[i])
                {
                    *strCho = choSungs[i];
                    return eEntered::eConsonant;
                }
            }

            // �߼� �ε��� ã��
            for (int i = 0; i < sizeof(jungSungs) / sizeof(wchar_t); i++)
            {
                if (str == jungSungs[i])
                {
                    *strJung = jungSungs[i];
                    return eEntered::eVowel;
                }
            }

            // ���� �ε��� ã��
            for (int i = 1; i < sizeof(jongSungs) / sizeof(wchar_t); i++)
            {
                if (str == jongSungs[i])
                {
                    *strJong = jongSungs[i];
                    return eEntered::eConsonant;
                }
            }

            // õ �ε��� ã��
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

