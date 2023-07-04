// UserInfoDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include "UserInfoDlg.h"


// CUserInfoDlg 대화 상자

IMPLEMENT_DYNAMIC(CUserInfoDlg, CDialogEx)

CUserInfoDlg::CUserInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INFO_DIALOG, pParent)
{
	m_pParent = pParent;
	m_pDB = NULL;
}

CUserInfoDlg::~CUserInfoDlg()
{
}

void CUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_edINFO_ID,			m_edInfoID);
	DDX_Control(pDX, IDC_edINFO_PASSWORD,	m_edInfoPassword);
	DDX_Control(pDX, IDC_edINFO_PASSCHECK,	m_edInfoPassCheck);
	DDX_Control(pDX, IDC_edINFO_EMAIL,		m_edInfoEmail);
	DDX_Control(pDX, IDC_cbINFO_PHONE1,		m_cbInfoPhone1);
	DDX_Control(pDX, IDC_edINFO_PHONE2,		m_edInfoPhone2);
	DDX_Control(pDX, IDC_edINFO_PHONE3,		m_edInfoPhone3);
	DDX_Control(pDX, IDC_edINFO_MILINUM2,	m_edInfoMiliNo2);
	DDX_Control(pDX, IDC_edINFO_MILINUM1,	m_edInfoMiliNo1);
	DDX_Control(pDX, IDC_stINFO_TEXT, m_ctrInfoText);

	DDX_Control(pDX, IDC_cbINFO_RANK, m_cbInfoRank);
	DDX_Control(pDX, IDC_cbINFO_POWER, m_cbInfoPower);
	DDX_Control(pDX, IDC_btINFO_MODIFY, m_btInfoModify);
	DDX_Control(pDX, IDC_btINFO_CLOSE, m_btInfoClose);
}


BEGIN_MESSAGE_MAP(CUserInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_btINFO_CLOSE, &CUserInfoDlg::OnBnClickedbtinfoClose)
	ON_BN_CLICKED(IDC_btINFO_SIGNUP, &CUserInfoDlg::OnBnClickedbtinfoSignup)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btINFO_MODIFY, &CUserInfoDlg::OnBnClickedbtinfoModify)
	ON_EN_CHANGE(IDC_edINFO_ID, &CUserInfoDlg::OnEnChangeedinfoId)
	ON_EN_CHANGE(IDC_edINFO_PASSWORD, &CUserInfoDlg::OnEnChangeedinfoPassword)
	ON_EN_CHANGE(IDC_edINFO_PASSCHECK, &CUserInfoDlg::OnEnChangeedinfoPasscheck)
END_MESSAGE_MAP()


// CUserInfoDlg 메시지 처리기
BOOL CUserInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetDbInit();
	SetControlInit();
	return TRUE; 
}


void CUserInfoDlg::SetDbInit()
{
	if (m_pDB != NULL)
	{
		delete m_pDB;
		m_pDB = NULL;
	}

	m_pDB = new CLNSSQLiteDB(_T("D:\\Work\\MFC\\MFC_Dev\\output_x86\\DB\\TEST.DB"));
	m_pDB->InitDB();
}

void CUserInfoDlg::SetControlInit()
{
	CMainDlg* pDlg = (CMainDlg*)m_pParent;
	eInfoMode eInfoMode = pDlg->GetInfoMode();

	if (m_pDB != NULL)
	{
		CString strQry = _T("");
		CString strInfo = _T("");
		strQry.Format(_T("SELECT * FROM codeInfo WHERE Class = \'Power\'"));
		if (m_pDB->ExecuteQry(strQry) == TRUE)
		{
			int nSize = m_pDB->GetRowSize();
			for (int i = 0; i < nSize; i++)
			{
				m_pDB->GetFieldNameData(i, "Info", strInfo);
				m_cbInfoPower.AddString(strInfo);
			}
		}

		strQry.Format(_T("SELECT * FROM codeInfo WHERE Class = \'Rank\'"));
		if (m_pDB->ExecuteQry(strQry) == TRUE)
		{
			int nSize = m_pDB->GetRowSize();
			for (int i = 0; i < nSize; i++)
			{
				m_pDB->GetFieldNameData(i, "Info", strInfo);
				m_cbInfoRank.AddString(strInfo);
			}
		}

		if (eInfoMode == eInfoMode::eInfo_SignUp)
		{
			m_cbInfoRank.SetWindowTextW(_T("선택"));
			m_cbInfoPhone1.SetWindowTextW(_T("선택"));
			m_cbInfoPower.SetWindowTextW(_T("선택"));
		}
		else if (eInfoMode == eInfoMode::eInfo_Modify || eInfoMode == eInfoMode::eInfo_ChangePass)
		{
			CString strQry = _T("");
			strQry.Format(_T("SELECT * FROM LOGIN WHERE ID = '%s'"), pDlg->GetLoginID());
			if (m_pDB->ExecuteQry(strQry) == TRUE)
			{
				int nSize = m_pDB->GetRowSize();
				if (nSize == 1)
				{
					int i = 0;
					CString strData = _T("");

					m_pDB->GetFieldNameData(i, "Email", strData);
					m_edInfoEmail.SetWindowTextW(strData);

					m_pDB->GetFieldNameData(i, "Phone", strData);
					std::vector<CString>vData = m_util.GetExSubString(strData, '-');
					
					if (vData.size() != 0)
					{
						const int nFirstPhoneEA = 6; //010, 011, 016 ...
						for (int i = 0; i < nFirstPhoneEA; i++)
						{
							m_cbInfoPhone1.GetLBText(i, strData);
							if (strData == vData[0])
							{
								m_cbInfoPhone1.SetCurSel(i);
								break;
							}
						}
						m_edInfoPhone2.SetWindowTextW(vData[1]);
						m_edInfoPhone3.SetWindowTextW(vData[2]);
					}

					m_pDB->GetFieldNameData(i, "MiliNo", strData);
					vData = m_util.GetExSubString(strData, '-');
					if (vData.size() != 0)
					{
						m_edInfoMiliNo1.SetWindowTextW(vData[0]);
						m_edInfoMiliNo2.SetWindowTextW(vData[1]);
					}

					int nData;
					m_pDB->GetFieldNameData(i, "RankCode", nData);
					m_cbInfoRank.SetCurSel(nData);

					m_pDB->GetFieldNameData(i, "PowerCode", nData);
					m_cbInfoPower.SetCurSel(nData);

				}
			}
		}
	}
}

void CUserInfoDlg::SetDbDestroy()
{
	if (m_pDB != NULL)
	{
		delete m_pDB;
		m_pDB = NULL;
	}
}

void CUserInfoDlg::OnBnClickedbtinfoClose()
{
	SetDbDestroy();

	CMainDlg* pDlg = (CMainDlg*)m_pParent;
	pDlg->UserInfoDlgDestroy();
}



void CUserInfoDlg::OnBnClickedbtinfoSignup()
{
	if (SetTypeCheck() == FALSE)
	{
		return;
	}
	CString strID = _T("");
	m_edInfoID.GetWindowTextW(strID);

	CString strPassword = _T("");
	m_edInfoPassword.GetWindowTextW(strPassword);

	CString strEmail = _T("");
	m_edInfoEmail.GetWindowTextW(strEmail);

	if (m_pDB != NULL)
	{
		CString strQry;
		strQry.Format(_T("Insert into LOGIN  \
								values (0, '%s', '%s', '%s', '%s', %d, '%s', DATETIME('NOW', 'LOCALTIME'), 0, DATETIME('NOW', 'LOCALTIME'), %d);"),
			strID, strPassword, strEmail, m_strInfoPhone, m_cbInfoRank.GetCurSel(), m_strInfoMiliNo, m_cbInfoPower.GetCurSel());
		m_pDB->InsUpDel(strQry);
	}

	AfxMessageBox(stStatusLoginText[(int)eStatusLogin::eCompSignUp].text);
}

BOOL CUserInfoDlg::SetTypeCheck()
{
	CMainDlg* pDlg = (CMainDlg*)m_pParent;
	CString strData;
	m_ctrInfoText.SetWindowTextW(_T(""));

	m_edInfoID.GetWindowTextW(strData);
	if (m_pDB != NULL)
	{
		CString strQry;
		strQry.Format(_T("select ID from LOGIN where id = '%s'"), strData);
		if (m_pDB->ExecuteQry(strQry) == TRUE)
		{
			int nSize = m_pDB->GetRowSize();
			if (nSize > 0)
			{
				m_ctrInfoText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::eSameID].text);
				return FALSE;
			}
		}
		else
		{
			m_ctrInfoText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::eDbDiconnect].text);
			return FALSE;
		}
	}
	if (m_typeCheck.CheckID(strData) == FALSE)
	{
		m_ctrInfoText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::eIdCondition].text);
		return FALSE;
	}

	CString str1, str2;
	m_edInfoPassword.GetWindowTextW(str1);
	m_edInfoPassCheck.GetWindowTextW(str2);
	if (pDlg->GetInfoMode() != eInfoMode::eInfo_Modify)
	{
		if (str1 == str2)
		{
			if (m_typeCheck.CheckPassword(str1) == FALSE)
			{
				m_ctrInfoText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::ePassCondition].text);
				return FALSE;
			}
		}
		else
		{
			m_ctrInfoText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::eNotSamePass].text);
			return FALSE;
		}
	}

	m_edInfoEmail.GetWindowTextW(strData);
	if (m_typeCheck.CheckEmail(strData) == FALSE)
	{
		m_ctrInfoText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::eEmailCondition].text);
		return FALSE;
	}
	
	if (m_cbInfoPhone1.GetCurSel() == -1)
	{
		m_ctrInfoText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::ePhoneSelect].text);
		return FALSE;
	}
	else
	{
		m_cbInfoPhone1.GetLBText(m_cbInfoPhone1.GetCurSel(), strData);
		CString strPhone2, strPhone3;
		m_edInfoPhone2.GetWindowTextW(strPhone2);
		m_edInfoPhone3.GetWindowTextW(strPhone3);
		m_strInfoPhone = _T("");
		m_strInfoPhone.Format(strData + _T("-%s-%s"), strPhone2, strPhone3);
	}
	if (m_typeCheck.CheckPhoneNumber(m_strInfoPhone) == FALSE)
	{
		m_ctrInfoText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::ePhoneCondition].text);
		return FALSE;
	}
	
	if (m_cbInfoRank.GetCurSel() == -1)
	{
		m_ctrInfoText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::eRankSelect].text);
		return FALSE;
	}

	if (m_cbInfoRank.GetCurSel() != 0 && m_cbInfoRank.GetCurSel() != 1)
	{
		m_edInfoMiliNo1.GetWindowTextW(str1);
		m_edInfoMiliNo2.GetWindowTextW(str2);
		m_strInfoMiliNo.Format(_T("%s-%s"), str1, str2);

		if (m_typeCheck.CheckMiliNo(m_strInfoMiliNo) == FALSE)
		{
			m_ctrInfoText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::eMiliCondition].text);
			return FALSE;
		}
	}
	else
	{
		m_strInfoMiliNo.Format(_T("00-00000000"));
	}

	if (m_cbInfoPower.GetCurSel() == -1)
	{
		m_ctrInfoText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::ePowerSelect].text);
		return FALSE;
	}

	return TRUE;
}

void CUserInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	CMainDlg* pDlg = (CMainDlg*)m_pParent;

	if (pDlg->GetInfoMode() == eInfoMode::eInfo_SignUp)
	{
		GetDlgItem(IDC_edINFO_ID)->EnableWindow(TRUE);
		GetDlgItem(IDC_edINFO_PASSWORD)->EnableWindow(TRUE);
		GetDlgItem(IDC_edINFO_PASSCHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_edINFO_EMAIL)->EnableWindow(TRUE);
		GetDlgItem(IDC_cbINFO_PHONE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_edINFO_PHONE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_edINFO_PHONE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_cbINFO_RANK)->EnableWindow(TRUE);
		GetDlgItem(IDC_edINFO_MILINUM1)->EnableWindow(TRUE);
		GetDlgItem(IDC_edINFO_MILINUM2)->EnableWindow(TRUE);
		GetDlgItem(IDC_cbINFO_POWER)->EnableWindow(TRUE);

		GetDlgItem(IDC_btINFO_SIGNUP)->ShowWindow(TRUE);
		GetDlgItem(IDC_btINFO_MODIFY)->ShowWindow(FALSE);
		GetDlgItem(IDC_btINFO_CLOSE)->ShowWindow(TRUE);
	}
	else if (pDlg->GetInfoMode() == eInfoMode::eInfo_Modify)
	{
		GetDlgItem(IDC_edINFO_ID)->EnableWindow(FALSE);
		GetDlgItem(IDC_edINFO_PASSWORD)->EnableWindow(FALSE);
		GetDlgItem(IDC_edINFO_PASSCHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_edINFO_EMAIL)->EnableWindow(TRUE);
		GetDlgItem(IDC_cbINFO_PHONE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_edINFO_PHONE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_edINFO_PHONE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_cbINFO_RANK)->EnableWindow(TRUE);
		GetDlgItem(IDC_edINFO_MILINUM1)->EnableWindow(TRUE);
		GetDlgItem(IDC_edINFO_MILINUM2)->EnableWindow(TRUE);
		GetDlgItem(IDC_cbINFO_POWER)->EnableWindow(TRUE);

		GetDlgItem(IDC_btINFO_SIGNUP)->ShowWindow(FALSE);
		GetDlgItem(IDC_btINFO_MODIFY)->ShowWindow(TRUE);
		GetDlgItem(IDC_btINFO_CLOSE)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_edINFO_ID)->EnableWindow(FALSE);
		GetDlgItem(IDC_edINFO_PASSWORD)->EnableWindow(TRUE);
		GetDlgItem(IDC_edINFO_PASSCHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_edINFO_EMAIL)->EnableWindow(TRUE);
		GetDlgItem(IDC_cbINFO_PHONE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_edINFO_PHONE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_edINFO_PHONE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_cbINFO_RANK)->EnableWindow(FALSE);
		GetDlgItem(IDC_edINFO_MILINUM1)->EnableWindow(TRUE);
		GetDlgItem(IDC_edINFO_MILINUM2)->EnableWindow(TRUE);
		GetDlgItem(IDC_cbINFO_POWER)->EnableWindow(FALSE);

		GetDlgItem(IDC_btINFO_SIGNUP)->ShowWindow(FALSE);
		GetDlgItem(IDC_btINFO_MODIFY)->ShowWindow(TRUE);
		GetDlgItem(IDC_btINFO_CLOSE)->ShowWindow(TRUE);
	}
}


void CUserInfoDlg::OnBnClickedbtinfoModify()
{
	CMainDlg* pDlg = (CMainDlg*)m_pParent;

	if (SetTypeCheck() == FALSE)
	{
		return;
	}

	if (m_pDB != NULL)
	{
		CString strPassword = _T("");
		m_edInfoPassword.GetWindowTextW(strPassword);

		CString strEmail = _T("");
		m_edInfoEmail.GetWindowTextW(strEmail);

		CString strQry;
		strQry.Format(_T("update LOGIN  \
								set Approval	= 0,									\
									Password	= '%s',									\
									Email		= '%s',									\
									Phone		= '%s',									\
									RankCode	=  %d,									\
									MiliNo		= '%s',									\
									D_Reporting = DATETIME('NOW', 'LOCALTIME'),			\
									D_Approval	= 0,									\
									D_Password	= DATETIME('NOW', 'LOCALTIME'),			\
									PowerCode	= %d									\
								where id = '%s';"),
			strPassword, strEmail, m_strInfoPhone, m_cbInfoRank.GetCurSel(), m_strInfoMiliNo,
			m_cbInfoPower.GetCurSel(), pDlg->GetLoginID());
		m_pDB->InsUpDel(strQry);
	}
}


void CUserInfoDlg::OnEnChangeedinfoId()
{
	CString strData;
	m_ctrInfoText.SetWindowTextW(_T(""));

	m_edInfoID.GetWindowTextW(strData);
	if (m_pDB != NULL)
	{
		CString strQry;
		strQry.Format(_T("select ID from LOGIN where id = '%s'"), strData);
		if (m_pDB->ExecuteQry(strQry) == TRUE)
		{
			int nSize = m_pDB->GetRowSize();
			if (nSize > 0)
			{
				m_ctrInfoText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::eSameID].text);
			}
			else
			{
				m_ctrInfoText.SetWindowTextW(_T(""));
			}
		}
		else
		{
			m_ctrInfoText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::eDbDiconnect].text);
		}
	}
	if (m_typeCheck.CheckID(strData) == FALSE)
	{
		m_ctrInfoText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::eIdCondition].text);
	}
	else
	{
		m_ctrInfoText.SetWindowTextW(_T(""));
	}
}


void CUserInfoDlg::OnEnChangeedinfoPassword()
{
	CString str1, str2;
	m_edInfoPassword.GetWindowTextW(str1);
	if (m_typeCheck.CheckPassword(str1) == FALSE)
	{
		m_ctrInfoText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::ePassCondition].text);
	}
	else
	{
		m_ctrInfoText.SetWindowTextW(_T(""));
	}
}


void CUserInfoDlg::OnEnChangeedinfoPasscheck()
{
	CString str1, str2;
	m_edInfoPassword.GetWindowTextW(str1);
	m_edInfoPassCheck.GetWindowTextW(str2);

	if (str1 == str2)
	{
		m_ctrInfoText.SetWindowTextW(_T(""));
	}
	else
	{
		m_ctrInfoText.SetWindowTextW(stStatusLoginText[(int)eStatusLogin::eNotSamePass].text);
	}
}
