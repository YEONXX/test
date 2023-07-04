#pragma once

enum class eDestroy
{
	eMain = 0,
	eLogin,
	eUserInfo,
	eUserManager,
	eUserControl,
}; 

enum class eFailCode : int
{
	ePass = 0,
	eLoginIdFail = 100,
	eLoginPasswordFail,
	eLoginPassDateFail,
};

enum class eInfoMode : int
{
	eNone = -1,
	eInfo_SignUp,
	eInfo_ChangePass,
	eInfo_Modify,
};

enum class eLoginPower : int
{
	eNone = -1,
	eLP_General,
	eLP_Operator,
	eLP_Admin,
};

enum class eStatusLogin
{
	eNoID,
	eWrongPassword,
	eTwoID,
	eSameID,
	eDbDiconnect,
	eIdCondition,
	ePassCondition,
	eNotSamePass,
	eEmailCondition,
	ePhoneSelect,
	ePhoneCondition,
	eRankSelect,
	eMiliCondition,
	ePowerSelect,
	eNeedAdmin,
	eNeedPassChange,
	eCompSignUp,
	eCompApproval,
	eCompDelete,
};

static const struct stStatusLogin
{
	TCHAR* text;
} stStatusLoginText[] = {
	{_T("ID�� �����ϴ�.")},
	{_T("��й�ȣ�� Ʋ�Ƚ��ϴ�.")},
	{_T("���� ID�� �� �� �̻� �ֽ��ϴ�.\n �����ڿ��� �����ϼ���..")},
	{_T("�ߺ��� ID�� �ֽ��ϴ�.")},
	{_T("DB ���� �Ұ�")},
	{_T("ID�� ���ǿ� ���� �ʽ��ϴ�.")},
	{_T("Password�� ���ǿ� ���� �ʽ��ϴ�.\n ���� : ��,�ҹ��� ���� / ���� ���� / Ư������ �ϳ� �̻�(!,@,#) / 8�ڸ� �̻�")},
	{_T("Password�� ��ġ���� �ʽ��ϴ�.")},
	{_T("Email�� ���Ŀ� ���� �ʽ��ϴ�.")},
	{_T("�ڵ��� ��ȣ�� ������ �ּ���.")},
	{_T("�ڵ��� ��ȣ�� ���Ŀ� ���� �ʽ��ϴ�.")},
	{_T("����� ������ �ּ���.")},
	{_T("������ ���Ŀ� ���� �ʽ��ϴ�.")},
	{_T("������ ������ �ּ���.")},
	{_T("������ ������ �ʿ��մϴ�.")},
	{_T("��й�ȣ ������ �ʿ��մϴ�.")},
	{_T("ȸ�� ���� �Ϸ�!.")},
	{_T("���� �Ϸ�!.")},
	{_T("���� �Ϸ�!.")},
};