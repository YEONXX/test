#include "pch.h"
#include "Util.h"

std::vector<CString> CUtil::GetExSubString(CString strIndata, TCHAR szToken) 
{
    std::vector<CString> strOut;

    while (1)
    {
        CString strToken;
        int nTokenPos = strIndata.Find(szToken);
        if (nTokenPos != -1)
        {
            strToken = strIndata.Left(nTokenPos);
            strIndata = strIndata.Mid(nTokenPos + 1);
        }
        else
        {
            strToken = strIndata;
            strIndata.Empty();
        }

        if (!strToken.IsEmpty())
            strOut.push_back(strToken);
        else
            break;
    }

    return strOut;
}