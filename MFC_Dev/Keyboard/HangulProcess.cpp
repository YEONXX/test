#include "pch.h"
#include "HangulProcess.h"
#include <regex>
bool CHangulProcess::CombineConsonantAndVowel(const CString consonant, const CString vowel, const CString consonant2, CString *str)
{
    bool bResult = true;
    const wchar_t choSungs[] = { L'¤¡', L'¤¢', L'¤¤', L'¤§', L'¤¨', L'¤©', L'¤±', L'¤²', L'¤³', L'¤µ', L'¤¶', L'¤·', L'¤¸', L'¤¹', L'¤º', L'¤»', L'¤¼', L'¤½', L'¤¾' };
    const wchar_t jungSungs[] = { L'¤¿', L'¤À', L'¤Á', L'¤Â', L'¤Ã', L'¤Ä', L'¤Å', L'¤Æ', L'¤Ç', L'¤È', L'¤É', L'¤Ê', L'¤Ë', L'¤Ì', L'¤Í', L'¤Î', L'¤Ï', L'¤Ð', L'¤Ñ', L'¤Ò', L'¤Ó' };
    const wchar_t jongSungs[] = { L' ', L'¤¡', L'¤¢', L'¤£', L'¤¤', L'¤¥', L'¤¦', L'¤§', L'¤©', L'¤ª', L'¤«', L'¤¬', L'¤­', L'¤®', L'¤¯', L'¤°', L'¤±', L'¤²', L'¤´', L'¤µ', L'¤¶', L'¤·', L'¤¸', L'¤º', L'¤»', L'¤¼', L'¤½', L'¤¾' };

    int choIndex = -1;
    int jungIndex = -1;
    int jongIndex = -1;

    // ÃÊ¼º ÀÎµ¦½º Ã£±â
    for (int i = 0; i < sizeof(choSungs) / sizeof(wchar_t); i++)
    {
        if (consonant == choSungs[i])
        {
            choIndex = i;
            break;
        }
    }

    // Áß¼º ÀÎµ¦½º Ã£±â
    for (int i = 0; i < sizeof(jungSungs) / sizeof(wchar_t); i++)
    {
        if (vowel == jungSungs[i])
        {
            jungIndex = i;
            break;
        }
    }

    // Á¾¼º ÀÎµ¦½º Ã£±â
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
        // ÇÑ±Û À¯´ÏÄÚµå °è»ê
        wchar_t unicode = /*0xAC00*/ _T('°¡') + ((choIndex * 21 * 28) + (jungIndex * 28) + jongIndex);

        // À¯´ÏÄÚµå °ªÀ» ¹®ÀÚ¿­·Î º¯È¯ÇÏ¿© ¹ÝÈ¯
        //*(str).Format(_T("%c"), unicode);
        *str = unicode;
        bResult = true;
    }
    

    return bResult;
}

// ÀÚÀ½ °°Àº °Í ´­·¶À» ¶§, ¤¡ ¤¡ -> ¤»
bool		CHangulProcess::IsOneKeyToMultiCho(const CString str1, const CString str2, const CString str3, CString* str)
{
    bool bResult = false;
    const WCHAR				wWord[7][3] = { {L'¤¡', L'¤»', L'¤¢'}, {L'¤¤', L'¤©',}, {L'¤§', L'¤¼', L'¤¨'}, 
                                                {L'¤²', L'¤½', L'¤³'}, {L'¤µ', L'¤¾', L'¤¶'}, {L'¤¸', L'¤º', L'¤¹'} ,{L'¤·', L'¤±',} };
    
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
        if (stArray.i == 1 || stArray.i == 6) //¤¤ ¤© , ¤· ¤±
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

// Ãµ °°Àº °Í ´­·¶À» ¶§ '¡¤', '¡¥' ÀüÈ¯
bool		CHangulProcess::IsOneKeyToMultiCheon(const CString str1, const CString str2, const CString str3, CString* str)
{
    bool bResult = false;
    const WCHAR				wWord[2] = { L'¡¤', L'¡¥' };

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


// ÀÚÀ½³¢¸® ÇÕÃÄÁö´Â °Í ¤¤ ¤¾ -> ¤¦
bool		CHangulProcess::IsCombineConsonant(const CString str1, const CString str2, const CString str3, CString* str)
{
    bool bResult = true;
    const CString strWord[] = { L"¤¡¤µ", L"¤¤¤¸", L"¤¤¤¾", L"¤©¤¡", L"¤©¤±", L"¤©¤²", L"¤©¤µ", L"¤©¤¼", L"¤©¤½", L"¤©¤¾", L"¤²¤µ"};
    CString strCombine;
    strCombine = str1 + str2;

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (strCombine == strWord[0])             { *str = _T('¤£'); }
        else if (strCombine == strWord[1])        { *str = _T('¤¥'); }
        else if (strCombine == strWord[2])        { *str = _T('¤¦'); }
        else if (strCombine == strWord[3])        { *str = _T('¤ª'); }
        else if (strCombine == strWord[4])        { *str = _T('¤«'); }
        else if (strCombine == strWord[5])        { *str = _T('¤¬'); }
        else if (strCombine == strWord[6])        { *str = _T('¤­'); }
        else if (strCombine == strWord[7])        { *str = _T('¤®'); }
        else if (strCombine == strWord[8])        { *str = _T('¤¯'); }
        else if (strCombine == strWord[9])        { *str = _T('¤°'); }
        else if (strCombine == strWord[10])       { *str = _T('¤´'); }
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
    const CString strWord[] = { L"¤¿¤Ó", L"¤Á¤Ó", L"¤Ã¤Ó", L"¤Å¤Ó", L"¤Ç¤¿", L"¤È¤Ó", L"¤Ç¤Ó", L"¤Ð¤Ó", L"¤Í¤Ó", L"¤Ì¤Ó", L"¤Ñ¤Ó" };
    CString strCombine;
    strCombine = str1 + str2;

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (strCombine == strWord[0]) { *str = _T('¤À'); }
        else if (strCombine == strWord[1]) { *str = _T('¤Â'); }
        else if (strCombine == strWord[2]) { *str = _T('¤Ä'); }
        else if (strCombine == strWord[3]) { *str = _T('¤Æ'); }
        else if (strCombine == strWord[4]) { *str = _T('¤È'); }
        else if (strCombine == strWord[5]) { *str = _T('¤É'); }
        else if (strCombine == strWord[6]) { *str = _T('¤Ê'); }
        else if (strCombine == strWord[7]) { *str = _T('¤Í'); }
        else if (strCombine == strWord[8]) { *str = _T('¤Î'); }
        else if (strCombine == strWord[9]) { *str = _T('¤Ï'); }
        else if (strCombine == strWord[10]) { *str = _T('¤Ò'); }
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
    const CString strWord[] = { L"¤Ó¡¤", L"¤¿¡¤", L"¤Ó¡¥", L"¡¤¤Ó", L"¡¥¤Ó", L"¡¤¤Ñ", L"¤Ê¡¤", L"¡¥¤Ñ", L"¤Ñ¡¤", L"¤Ñ¡¥", L"¤Ì¡¤" };
    CString strCombine;
    strCombine = str1 + str2;

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (strCombine == strWord[0])         { *str = _T('¤¿'); }
        else if (strCombine == strWord[1]) { *str = _T('¤Á'); }
        else if (strCombine == strWord[2]) { *str = _T('¤Á'); }
        else if (strCombine == strWord[3]) { *str = _T('¤Ã'); }
        else if (strCombine == strWord[4]) { *str = _T('¤Å'); }
        else if (strCombine == strWord[5]) { *str = _T('¤Ç'); }
        else if (strCombine == strWord[6]) { *str = _T('¤È'); }
        else if (strCombine == strWord[7]) { *str = _T('¤Ë'); }
        else if (strCombine == strWord[8]) { *str = _T('¤Ì'); }
        else if (strCombine == strWord[9]) { *str = _T('¤Ð'); }
        else if (strCombine == strWord[10]) { *str = _T('¤Ð'); }
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
    const CString strWord[] = { L"¤£", L"¤¥", L"¤¦", L"¤ª", L"¤«", L"¤¬", L"¤­", L"¤®", L"¤¯", L"¤°", L"¤´" };

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (str == strWord[0])             { *str1 = _T('¤¡'); *str2 = _T('¤µ');}
        else if (str == strWord[1])        { *str1 = _T('¤¤'); *str2 = _T('¤¸');}
        else if (str == strWord[2])        { *str1 = _T('¤¤'); *str2 = _T('¤¾');}
        else if (str == strWord[3])        { *str1 = _T('¤©'); *str2 = _T('¤¡');}
        else if (str == strWord[4])        { *str1 = _T('¤©'); *str2 = _T('¤±');}
        else if (str == strWord[5])        { *str1 = _T('¤©'); *str2 = _T('¤²');}
        else if (str == strWord[6])        { *str1 = _T('¤©'); *str2 = _T('¤µ');}
        else if (str == strWord[7])        { *str1 = _T('¤©'); *str2 = _T('¤¼');}
        else if (str == strWord[8])        { *str1 = _T('¤©'); *str2 = _T('¤½');}
        else if (str == strWord[9])        { *str1 = _T('¤©'); *str2 = _T('¤¾');}
        else if (str == strWord[10])       { *str1 = _T('¤²'); *str2 = _T('¤µ');}
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
    const CString strWord[] = { L"¤À", L"¤Â", L"¤Ä", L"¤Æ", L"¤È", L"¤É", L"¤Ê", L"¤Í", L"¤Î", L"¤Ï", L"¤Ò" };

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (str == strWord[0])         { *str1 = _T('¤¿'); *str2 = _T('¤Ó'); }
        else if (str == strWord[1])    { *str1 = _T('¤Á'); *str2 = _T('¤Ó'); }
        else if (str == strWord[2])    { *str1 = _T('¤Ã'); *str2 = _T('¤Ó'); }
        else if (str == strWord[3])    { *str1 = _T('¤Å'); *str2 = _T('¤Ó'); }
        else if (str == strWord[4])    { *str1 = _T('¤Ç'); *str2 = _T('¤¿'); }
        else if (str == strWord[5])    { *str1 = _T('¤Ç'); *str2 = _T('¤À'); }
        else if (str == strWord[6])    { *str1 = _T('¤Ç'); *str2 = _T('¤Ó'); }
        else if (str == strWord[7])    { *str1 = _T('¤Ì'); *str2 = _T('¤Ã'); }
        else if (str == strWord[8])    { *str1 = _T('¤Ì'); *str2 = _T('¤Ä'); }
        else if (str == strWord[9])    { *str1 = _T('¤Ì'); *str2 = _T('¤Ó'); }
        else if (str == strWord[10])   { *str1 = _T('¤Ñ'); *str2 = _T('¤Ó'); }
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
    const CString strWord[] = { L"¤¿", L"¤Á",  L"¤Ã", L"¤Å", L"¤Ç", L"¤È", L"¤Ë", L"¤Ì", L"¤Ð"};
    //¡¤ ¡¥

    for (int i = 0; i < sizeof(strWord) / sizeof(CString); i++)
    {
        if (str == strWord[0])          { *str1 = _T('¤Ó'); *str2 = _T('¡¤'); }
        else if (str == strWord[1])     { *str1 = _T('¤Ó'); *str2 = _T('¡¥'); }
        else if (str == strWord[2])     { *str1 = _T('¡¤'); *str2 = _T('¤Ó'); }
        else if (str == strWord[3])     { *str1 = _T('¡¥'); *str2 = _T('¤Ó'); }
        else if (str == strWord[4])     { *str1 = _T('¡¤'); *str2 = _T('¤Ñ'); }
        else if (str == strWord[5])     { *str1 = _T('¤Ê'); *str2 = _T('¡¤'); }
        else if (str == strWord[6])     { *str1 = _T('¡¥'); *str2 = _T('¤Ñ'); }
        else if (str == strWord[7])     { *str1 = _T('¤Ñ'); *str2 = _T('¡¤'); }
        else if (str == strWord[8])     { *str1 = _T('¤Ñ'); *str2 = _T('¡¥'); }
        else
        {
            bResult = false;
        }
    }
    return bResult;
}


eEntered CHangulProcess::DevideHangul(const CString str, CString* strCho, CString* strJung, CString* strJong, CString* strCheon)
{
    const wchar_t choSungs[] = { L'¤¡', L'¤¢', L'¤¤', L'¤§', L'¤¨', L'¤©', L'¤±', L'¤²', L'¤³', L'¤µ', L'¤¶', L'¤·', L'¤¸', L'¤¹', L'¤º', L'¤»', L'¤¼', L'¤½', L'¤¾' };
    const wchar_t jungSungs[] = { L'¤¿', L'¤À', L'¤Á', L'¤Â', L'¤Ã', L'¤Ä', L'¤Å', L'¤Æ', L'¤Ç', L'¤È', L'¤É', L'¤Ê', L'¤Ë', L'¤Ì', L'¤Í', L'¤Î', L'¤Ï', L'¤Ð', L'¤Ñ', L'¤Ò', L'¤Ó' };
    const wchar_t jongSungs[] = { L' ', L'¤¡', L'¤¢', L'¤£', L'¤¤', L'¤¥', L'¤¦', L'¤§', L'¤©', L'¤ª', L'¤«', L'¤¬', L'¤­', L'¤®', L'¤¯', L'¤°', L'¤±', L'¤²', L'¤´', L'¤µ', L'¤¶', L'¤·', L'¤¸', L'¤º', L'¤»', L'¤¼', L'¤½', L'¤¾' };
    const wchar_t cheonSungs[] = { L'¡¤', L'¡¥' };

    if (str.IsEmpty() != true)
    {
        std::wstring strRegex;
        //strRegex = CT2CA(strWord);
        std::wregex re(L"([°¡-ÆR])");
        
        if (std::regex_search(str.GetString(), re))
        {
            int unicode = str.GetAt(0) - _T('°¡');
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
            // ÃÊ¼º ÀÎµ¦½º Ã£±â
            for (int i = 0; i < sizeof(choSungs) / sizeof(wchar_t); i++)
            {
                if (str == choSungs[i])
                {
                    *strCho = choSungs[i];
                    return eEntered::eConsonant;
                }
            }

            // Áß¼º ÀÎµ¦½º Ã£±â
            for (int i = 0; i < sizeof(jungSungs) / sizeof(wchar_t); i++)
            {
                if (str == jungSungs[i])
                {
                    *strJung = jungSungs[i];
                    return eEntered::eVowel;
                }
            }

            // Á¾¼º ÀÎµ¦½º Ã£±â
            for (int i = 1; i < sizeof(jongSungs) / sizeof(wchar_t); i++)
            {
                if (str == jongSungs[i])
                {
                    *strJong = jongSungs[i];
                    return eEntered::eConsonant;
                }
            }

            // Ãµ ÀÎµ¦½º Ã£±â
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

