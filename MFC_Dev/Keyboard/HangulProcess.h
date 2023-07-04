#pragma once
#include "projectDefine.h"

class CHangulProcess
{
public:
	bool		IsOneKeyToMultiCho			(const CString str1, const CString str2, const CString str3, CString* str);
	bool		IsOneKeyToMultiCheon		(const CString str1, const CString str2, const CString str3, CString* str);

	bool		IsCombineConsonant			(const CString str1, const CString str2, const CString str3, CString* str);
	bool		IsCombineVowel				(const CString str1, const CString str2, const CString str3, CString* str);
	bool		IsCombineCheonJiIn			(const CString str1, const CString str2, const CString str3, CString* str);
	bool		CombineConsonantAndVowel	(const CString consonant, const CString vowel, const CString consonant2, CString* str);

	bool		IsDevideConsonant			(const CString str, CString* str1, CString* str2);
	bool		IsDevideVowel				(const CString str, CString* str1, CString* str2);
	bool		IsDevideCheonJiIn			(const CString str, CString* str1, CString* str2);


	eEntered	DevideHangul				(const CString str, CString* strCho, CString* strJung, CString* strJong, CString* strCheon);
};

