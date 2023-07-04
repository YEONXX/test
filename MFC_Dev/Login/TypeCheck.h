#pragma once

class CTypeCheck
{
public:
	BOOL CheckID(CString strID);
	BOOL CheckPassword(CString strPassword);
	BOOL CheckEmail(CString strEmail);
	BOOL CheckPhoneNumber(CString strPhone);
	BOOL CheckMiliNo(CString strMiliNo);
};

