#include "pch.h"
#include "EnglishProcess.h"


bool		CEnglishProcess::IsOneKeyToMultiAlphabetUpper(const CString str1, const CString str2, CString* str)
{
    bool bResult = false;
    const WCHAR				wWord[8][4] = { {L'A', L'B', L'C', }, {L'D', L'E', L'F', }, {L'G', L'H', L'I', },
                                            {L'J', L'K', L'L', }, {L'M', L'N', L'O', }, {L'P', L'Q', L'R', L'S'},
                                            {L'T', L'U', L'V', }, {L'W', L'X', L'Y', 'Z'} };
    int nDevide = 0;
    typedef struct _stARRAY
    {
        int  i;
        int  j;
    } ARRAY;

    ARRAY stArray;
    memset(&stArray, 0, sizeof(ARRAY));
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 4; j++)
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
        if (stArray.i == 5 || stArray.i == 7) //p q r s , w x y z
        {
            nDevide = 4;
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


bool		CEnglishProcess::IsOneKeyToMultiAlphabetLower(const CString str1, const CString str2, CString* str)
{
    bool bResult = false;
    const WCHAR				wWord[8][4] = { {L'a', L'b', L'c', }, {L'd', L'e', L'f', }, {L'g', L'h', L'i', },
                                            {L'j', L'k', L'l', }, {L'm', L'n', L'o', }, {L'p', L'q', L'r', L's'},
                                            {L't', L'u', L'v', }, {L'w', L'x', L'y', 'z'} };
    int nDevide = 0;
    typedef struct _stARRAY
    {
        int  i;
        int  j;
    } ARRAY;

    ARRAY stArray;
    memset(&stArray, 0, sizeof(ARRAY));
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 4; j++)
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
        if (stArray.i == 5 || stArray.i == 7) //p q r s , w x y z
        {
            nDevide = 4;
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