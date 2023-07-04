#include "pch.h"
#include "TypeCheck.h"
#include <regex>

BOOL CTypeCheck::CheckID(CString strID)
{
	std::string strCheck;
	strCheck = CT2CA(strID);
	std::regex re("([a-zA-Z0-9._-]).{5,}$");
	//std::regex re(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

	if (!std::regex_match(strCheck, re))
	{
		// 비밀번호가 조건에 맞지 않으면 처리할 코드를 작성합니다.
		return FALSE;
	}
	return TRUE;
}


BOOL CTypeCheck::CheckPassword(CString strPassword)
{
	std::string strCheck;
	strCheck = CT2CA(strPassword);
	std::regex re("(?=.*[A-Z])(?=.*[0-9])(?=.*[!@#]).{8,}$");

	if (!std::regex_match(strCheck, re))
	{
		// 비밀번호가 조건에 맞지 않으면 처리할 코드를 작성합니다.
		return FALSE;
	}
	return TRUE;
}


BOOL CTypeCheck::CheckEmail(CString strEmail)
{
	std::string strCheck;
	strCheck = CT2CA(strEmail);
	std::regex re(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

	if (!std::regex_match(strCheck, re))
	{
		// 비밀번호가 조건에 맞지 않으면 처리할 코드를 작성합니다.
		return FALSE;
	}

	return TRUE;
}

BOOL CTypeCheck::CheckPhoneNumber(CString strPhone)
{
	std::string strCheck;
	strCheck = CT2CA(strPhone);
	std::regex re(R"(\d{3}-\d{3,4}-\d{4})");

	if (!std::regex_match(strCheck, re))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CTypeCheck::CheckMiliNo(CString strMiliNo)
{
	std::string strCheck;
	strCheck = CT2CA(strMiliNo);
	std::regex re(R"(\d{2}-\d{8})");

	if (!std::regex_match(strCheck, re))
	{
		return FALSE;
	}
	return TRUE;
}