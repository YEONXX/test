
// LoginDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include "LoginDlg.h"



// CLoginDlg 대화 상자
CLoginDlg::CLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN_DIALOG, pParent)
{
	m_pParent = pParent;
	m_pDB = NULL;
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_edLOGIN_ID, m_edLoginID);
	DDX_Control(pDX, IDC_edLOGIN_PASSWORD, m_edLoginPassword);

	DDX_Control(pDX, IDC_ptPROJECT, m_ctrProject);
	DDX_Control(pDX, IDC_btLOGIN, m_btLogin);
	DDX_Control(pDX, IDC_btSIGNUP, m_btSignUp);
	DDX_Control(pDX, IDC_btADMINMANGE, m_btManage);
	DDX_Control(pDX, IDC_btCLOSE, m_btClose);
	DDX_Control(pDX, IDC_stLOGIN_TEXT, m_ctrLoginText);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_btSIGNUP, &CLoginDlg::OnBnClickedbtsignup)
	ON_BN_CLICKED(IDC_btADMINMANGE, &CLoginDlg::OnBnClickedbtadminmange)
	ON_BN_CLICKED(IDC_btCLOSE, &CLoginDlg::OnBnClickedbtclose)
	ON_BN_CLICKED(IDC_btLOGIN, &CLoginDlg::OnBnClickedbtlogin)
END_MESSAGE_MAP()

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDbInit();
	SetVarInit();

	SqlCreateTable();
	SqlInsertTable();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CLoginDlg::SetDbInit()
{
	if (m_pDB == NULL)
	{
		m_pDB = new CLNSSQLiteDB(_T("D:\\Work\\MFC\\MFC_Dev\\output_x86\\DB\\TEST.DB"));
		m_pDB->InitDB();
	}
}

void CLoginDlg::SetVarInit()
{
	CMainDlg* pDlg = (CMainDlg*)m_pParent;
	pDlg->SetLoginID(_T(""));
	pDlg->SetInfoMode(eInfoMode::eInfo_SignUp);
	pDlg->SetLoginPower(eLoginPower::eLP_General);
	m_ctrLoginText.SetWindowTextW(_T(""));
}

void CLoginDlg::SqlCreateTable()
{
	if (m_pDB != NULL)
	{
		CString strQry;

		//Login Table
		strQry.Format(_T("create table LOGIN( "));
		strQry.Format(strQry + _T("Approval				boolean,"));
		strQry.Format(strQry + _T("ID					char(128) PRIMARY KEY,"));
		strQry.Format(strQry + _T("Password				char,"));
		strQry.Format(strQry + _T("Email				char(150),"));
		strQry.Format(strQry + _T("Phone				char,"));
		strQry.Format(strQry + _T("RankCode				integer DEFAULT 0,"));
		strQry.Format(strQry + _T("MiliNo				char,"));
		strQry.Format(strQry + _T("D_Reporting			DATE,"));
		strQry.Format(strQry + _T("D_Approval			DATE,"));
		strQry.Format(strQry + _T("D_Password			DATE,"));
		strQry.Format(strQry + _T("PowerCode			integer DEFAULT 0"));
		strQry.Format(strQry + _T(");"));
		m_pDB->InsUpDel(strQry);

		//codeInfo Table
		strQry.Format(_T("create table codeInfo( "));
		strQry.Format(strQry + _T("ID					integer PRIMARY KEY,"));
		strQry.Format(strQry + _T("Class				char(150),"));
		strQry.Format(strQry + _T("Code					integer DEFAULT 0,"));
		strQry.Format(strQry + _T("Info					char"));
		strQry.Format(strQry + _T(");"));
		m_pDB->InsUpDel(strQry);
	}
}

void CLoginDlg::SqlInsertTable()
{
	if (m_pDB != NULL)
	{
		CString strQry;
		
		int i = 0;
		strQry.Format(_T("replace into codeInfo (ID, Class, Code, Info) values (%d, \"Rank\", 0,	\"민간인\") "), i++);
		m_pDB->InsUpDel(strQry);
		strQry.Format(_T("replace into codeInfo (ID, Class, Code, Info) values (%d, \"Rank\", 1,	\"군무원\") "), i++);
		m_pDB->InsUpDel(strQry);
		strQry.Format(_T("replace into codeInfo (ID, Class, Code, Info) values (%d, \"Rank\", 2,	\"이병\") "), i++);
		m_pDB->InsUpDel(strQry);
		strQry.Format(_T("replace into codeInfo (ID, Class, Code, Info) values (%d, \"Rank\", 3,	\"일병\") "), i++);
		m_pDB->InsUpDel(strQry);
		strQry.Format(_T("replace into codeInfo (ID, Class, Code, Info) values (%d, \"Rank\", 4,	\"상병\") "), i++);
		m_pDB->InsUpDel(strQry);
		strQry.Format(_T("replace into codeInfo (ID, Class, Code, Info) values (%d, \"Rank\", 5,	\"병장\") "), i++);
		m_pDB->InsUpDel(strQry);

		strQry.Format(_T("replace into codeInfo (ID, Class, Code, Info) values (%d, \"Power\", 0,	\"일반사용자\") "), i++);
		m_pDB->InsUpDel(strQry);
		strQry.Format(_T("replace into codeInfo (ID, Class, Code, Info) values (%d, \"Power\", 1,	\"운용자\") "), i++);
		m_pDB->InsUpDel(strQry);
		strQry.Format(_T("replace into codeInfo (ID, Class, Code, Info) values (%d, \"Power\", 2,	\"관리자\") "), i++);
		m_pDB->InsUpDel(strQry);
	}

}

void CLoginDlg::OnBnClickedbtlogin()
{
	// ID,Password,유효기간 확인 및 관리자 권한 저장 
	CMainDlg* pDlg = (CMainDlg*)m_pParent;
	eFailCode eFail = SqlLoginCheck();
	if (eFail == eFailCode::ePass)
	{
		pDlg->UserControlDlgCreate();
	}
	else if (eFail == eFailCode::eLoginPassDateFail)
	{
		pDlg->SetInfoMode(eInfoMode::eInfo_ChangePass);
		pDlg->UserInfoDlgCreate();
	}
	else
	{
		pDlg->SetLoginID(_T(""));
		pDlg->SetLoginPower(eLoginPower::eNone);
	}
}


void CLoginDlg::OnBnClickedbtsignup()
{
	CMainDlg* pDlg = (CMainDlg*)m_pParent;
	pDlg->SetInfoMode(eInfoMode::eInfo_SignUp);
	pDlg->UserInfoDlgCreate();
}

void CLoginDlg::OnBnClickedbtadminmange()
{	
	CMainDlg* pDlg = (CMainDlg*)m_pParent;
	eFailCode eFail = SqlLoginCheck();
	if (eFail == eFailCode::ePass && pDlg->GetLoginPower() == eLoginPower::eLP_Admin)
	{
		pDlg->UserManageDlgCreate();
	}
	else if(eFail == eFailCode::ePass)
	{
		AfxMessageBox(stStatusLoginText[(int)eStatusLogin::eNeedAdmin].text);
	}
}


eFailCode CLoginDlg::SqlLoginCheck()
{
	m_ctrLoginText.SetWindowTextW(_T(""));

	eFailCode eFail = eFailCode::ePass;
	CString strQry = _T("");
	CString strDbData = _T("");
	CString strEditID = _T("");
	CString strEditPass = _T("");
	m_edLoginID.GetWindowTextW(strEditID);
	strQry.Format(_T("SELECT ID, Password, D_Password, PowerCode  FROM LOGIN where ID = '%s'"), strEditID);

	CMainDlg* pDlg = (CMainDlg*)m_pParent;
	if (m_pDB->ExecuteQry(strQry) == TRUE)
	{
		int i = 0;
		int nSize = m_pDB->GetRowSize();
		if (nSize == 0)
		{
			m_ctrLoginText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::eNoID].text);
			eFail = eFailCode::eLoginIdFail;
		}
		else if(nSize == 1)
		{
			m_pDB->GetFieldNameData(i, "Password", strDbData);
				
			m_edLoginPassword.GetWindowTextW(strEditPass);
			if (strDbData == strEditPass)
			{
				// 접속 Login ID 저장
				pDlg->SetLoginID(strEditID);

				// Power Code 저장
				eLoginPower ePower = eLoginPower::eNone;

				int nLoginPower;
				m_pDB->GetFieldNameData(i, "PowerCode", nLoginPower);
				ePower = static_cast<eLoginPower>(nLoginPower);
				pDlg->SetLoginPower(ePower);

				if (strEditID != _T("admin"))
				{
					// 비밀번호 유효기간 체크 admin 제외
					COleDateTime tNow = COleDateTime::GetCurrentTime();
					COleDateTime tDbDate;
					COleDateTimeSpan tCalc;

					m_pDB->GetFieldNameData(i, "D_Password", strDbData);
					tDbDate.ParseDateTime(strDbData);
					tCalc = tNow - tDbDate;
					if (tCalc.GetDays() > 30)
					{
						AfxMessageBox(stStatusLoginText[(int)eStatusLogin::eNeedPassChange].text);
						eFail = eFailCode::eLoginPassDateFail;
					}
				}
			}
			else
			{
				m_ctrLoginText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::eWrongPassword].text);
				eFail = eFailCode::eLoginPasswordFail;
			}
		}
		else
		{
			m_ctrLoginText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::eTwoID].text);
			eFail = eFailCode::eLoginIdFail;
		}
	}
	return eFail;
}


void CLoginDlg::SetDbDestroy()
{
	if (m_pDB != NULL)
	{
		delete m_pDB;
		m_pDB = NULL;
	}
}

void CLoginDlg::OnBnClickedbtclose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetDbDestroy();
	CMainDlg* pDlg = (CMainDlg*)m_pParent;
	pDlg->LoginDlgDestroy(TRUE);
}


BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
