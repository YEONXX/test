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
	{_T("ID가 없습니다.")},
	{_T("비밀번호가 틀렸습니다.")},
	{_T("같은 ID가 두 개 이상 있습니다.\n 관리자에게 문의하세요..")},
	{_T("중복된 ID가 있습니다.")},
	{_T("DB 접근 불가")},
	{_T("ID가 조건에 맞지 않습니다.")},
	{_T("Password가 조건에 맞지 않습니다.\n 조건 : 대,소문자 포함 / 숫자 포함 / 특수문자 하나 이상(!,@,#) / 8자리 이상")},
	{_T("Password가 일치하지 않습니다.")},
	{_T("Email이 형식에 맞지 않습니다.")},
	{_T("핸드폰 번호를 선택해 주세요.")},
	{_T("핸드폰 번호가 형식에 맞지 않습니다.")},
	{_T("계급을 선택해 주세요.")},
	{_T("군번이 형식에 맞지 않습니다.")},
	{_T("권한을 선택해 주세요.")},
	{_T("관리자 권한이 필요합니다.")},
	{_T("비밀번호 변경이 필요합니다.")},
	{_T("회원 가입 완료!.")},
	{_T("승인 완료!.")},
	{_T("삭제 완료!.")},
};