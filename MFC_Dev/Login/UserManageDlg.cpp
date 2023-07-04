// UserManageDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include "UserManageDlg.h"


// CUserManageDlg 대화 상자

IMPLEMENT_DYNAMIC(CUserManageDlg, CDialogEx)

CUserManageDlg::CUserManageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USER_MANAGE_DIALOG, pParent)
{
	m_pDB = NULL;
	m_pParent = pParent;

	
}

CUserManageDlg::~CUserManageDlg()
{
}

void CUserManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_cbUM_APPROVAL, m_cbApprovalStatus);
	DDX_Control(pDX, IDC_edUM_ID,		m_edID);
	DDX_Control(pDX, IDC_edUM_RANK,		m_edRank);
	DDX_Control(pDX, IDC_edUM_POWER,	m_edPower);
	DDX_Control(pDX, IDC_listUM_INFO,	m_listInfo);
	DDX_Control(pDX, IDC_btUM_APPROVAL, m_btApproval);
	DDX_Control(pDX, IDC_btUM_DELETE,	m_btDelete);
	DDX_Control(pDX, IDC_btUM_CLOSE,	m_btClose);
}


BEGIN_MESSAGE_MAP(CUserManageDlg, CDialogEx)
	ON_BN_CLICKED(IDC_btUM_CLOSE, &CUserManageDlg::OnBnClickedbtumClose)
	ON_BN_CLICKED(IDC_btUM_SEARCH, &CUserManageDlg::OnBnClickedbtumSearch)
	ON_BN_CLICKED(IDC_btUM_APPROVAL, &CUserManageDlg::OnBnClickedbtumApproval)
	ON_NOTIFY(NM_DBLCLK, IDC_listUM_INFO, &CUserManageDlg::OnNMDblclklistumInfo)
	ON_BN_CLICKED(IDC_btUM_DELETE, &CUserManageDlg::OnBnClickedbtumDelete)
END_MESSAGE_MAP()


// CUserManageDlg 메시지 처리기


BOOL CUserManageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDbInit();
	SetControlInit();
	GetSearchData(_T(""));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CUserManageDlg::SetDbInit()
{
	if (m_pDB != NULL)
	{
		delete m_pDB;
		m_pDB = NULL;
	}

	m_pDB = new CLNSSQLiteDB(_T("D:\\Work\\MFC\\MFC_Dev\\output_x86\\DB\\TEST.DB"));
	m_pDB->InitDB();
}

void CUserManageDlg::SetDbDestroy()
{
	if (m_pDB != NULL)
	{
		delete m_pDB;
		m_pDB = NULL;
	}
}

void CUserManageDlg::GetSearchData(CString strData)
{
	CString strQry;
	m_listInfo.DeleteAllItems();

	if (strData == _T(""))
	{
		strQry.Format(_T("SELECT lg.*, c1.Info as rank, c2.Info as power from LOGIN lg				\
							left join codeInfo c1 on c1.Class = 'Rank' AND lg.RankCode = c1.Code	\
							left join codeInfo c2 on c2.Class = 'Power' AND lg.PowerCode = c2.Code	\
							where lg.ID != 'admin'; "));
	}
	else
	{
		std::vector<CString>vData = m_util.GetExSubString(strData, ',');
		if (vData.size() == 0)
		{
			return;
		}
		else
		{
			//0 : 승인 미승인 
			CString strWhere;
			if (_wtoi(vData[0]) != 0)
			{
				int nValue = _wtoi(vData[0]);
				nValue = nValue - 1;
				vData[0] = std::to_wstring(nValue).c_str();

				strWhere.Format(_T("lg.Approval = ") + vData[0]);
			}
			else
			{
				strWhere.Format(_T("(lg.[Approval] = 0 or lg.[Approval] = 1) "));
			}
			//1 : ID
			if (vData[1] != _T("-"))
			{
				strWhere.Format(_T("%s AND lg.ID like '%s%%' "),strWhere, vData[1]);
			}
			//2 : 계급
			if (vData[2] != _T("-"))
			{
				strWhere.Format(_T("%s AND rank like '%s%%' "), strWhere, vData[2]);
			}
			//3 : 권한
			if (vData[3] != _T("-"))
			{
				strWhere.Format(_T("%s AND power like '%s%%' "), strWhere, vData[3]);
			}

			strQry.Format(_T("SELECT lg.*, c1.Info as rank, c2.Info as power from LOGIN lg				\
							left join codeInfo c1 on c1.Class = 'Rank' AND lg.RankCode = c1.Code		\
							left join codeInfo c2 on c2.Class = 'Power' AND lg.PowerCode = c2.Code		\
							where lg.ID != 'admin' and %s;"), strWhere);
		}
	}

	CString strID				= _T("");
	CString strEmail			= _T("");
	CString strPhone			= _T("");
	CString	strRankCode			= _T("");
	CString str_DayReporting	= _T("");
	CString str_DayApproval		= _T("");
	CString	strPowerCode		= _T("");
	CString strMiliNo			= _T("");
	BOOL	bApproval			= FALSE;

	if (m_pDB->ExecuteQry(strQry) == TRUE)
	{
		int nSize = m_pDB->GetRowSize();

		for (int i = 0; i < nSize; i++)
		{
			int nSubItem = 0;

			m_pDB->GetFieldNameData(i, "Approval", bApproval);
			m_pDB->GetFieldNameData(i, "ID", strID);
			m_pDB->GetFieldNameData(i, "Email", strEmail);
			m_pDB->GetFieldNameData(i, "Phone", strPhone);
			m_pDB->GetFieldNameData(i, "rank", strRankCode);
			m_pDB->GetFieldNameData(i, "MiliNo", strMiliNo);
			m_pDB->GetFieldNameData(i, "D_Reporting", str_DayReporting);
			m_pDB->GetFieldNameData(i, "D_Approval", str_DayApproval);
			m_pDB->GetFieldNameData(i, "power", strPowerCode);

			m_listInfo.InsertItem(i, _T(""));
			m_listInfo.SetCheck(i, bApproval);
			m_listInfo.SetItemText(i, nSubItem++, strID);
			m_listInfo.SetItemText(i, nSubItem++, strEmail);
			m_listInfo.SetItemText(i, nSubItem++, strPhone);
			m_listInfo.SetItemText(i, nSubItem++, strRankCode);
			m_listInfo.SetItemText(i, nSubItem++, strMiliNo);
			m_listInfo.SetItemText(i, nSubItem++, str_DayReporting);
			m_listInfo.SetItemText(i, nSubItem++, str_DayApproval);
			m_listInfo.SetItemText(i, nSubItem++, strPowerCode);
		}
	}
}


void CUserManageDlg::SetControlInit()
{
	m_cbApprovalStatus.SetCurSel(0);

	// List Control 스타일 변경: 체크박스 사용
	DWORD dwStyle = m_listInfo.GetStyle();

	// 컬럼 인덱스 정의
	enum {
		COLUMN_CHECK,
		COLUMN_ID,
		COLUMN_EMAIL,
		COLUMN_PHONE,
		COLUMN_RANK,
		COLUMN_MILI_NUM,
		COLUMN_D_REPORTING,
		COLUMN_D_APPROVAL,
		COLUMN_POWER,
	};
	m_listInfo.SetExtendedStyle(dwStyle | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);

	//Initialize list
	for (int i = COLUMN_CHECK; i <= COLUMN_POWER; i++)
	{
		m_listInfo.DeleteColumn(0);
	}

	m_listInfo.InsertColumn(COLUMN_ID, _T("ID"), LVCFMT_LEFT, 100);
	m_listInfo.InsertColumn(COLUMN_EMAIL, _T("E-mail"), LVCFMT_LEFT, 100);
	m_listInfo.InsertColumn(COLUMN_PHONE, _T("Phone"), LVCFMT_LEFT, 100);
	m_listInfo.InsertColumn(COLUMN_RANK, _T("계급"), LVCFMT_LEFT, 100);
	m_listInfo.InsertColumn(COLUMN_MILI_NUM, _T("군번"), LVCFMT_LEFT, 100);
	m_listInfo.InsertColumn(COLUMN_D_REPORTING, _T("신청일"), LVCFMT_LEFT, 100);
	m_listInfo.InsertColumn(COLUMN_D_APPROVAL, _T("승인일"), LVCFMT_LEFT, 100);
	m_listInfo.InsertColumn(COLUMN_POWER, _T("권한"), LVCFMT_LEFT, 100);
}

void CUserManageDlg::OnBnClickedbtumClose()
{
	SetDbDestroy();
	
	CMainDlg* pDlg = (CMainDlg*)m_pParent;

	pDlg->UserManageDlgDestroy();
	pDlg->SystemDestroy();
}


void CUserManageDlg::OnBnClickedbtumSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strData;
	CString strID;
	CString strRank;
	CString strPower;
	m_edID.GetWindowTextW(strID);
	if (strID == _T(""))
	{
		strID.Format(_T("-"));
	}
	m_edRank.GetWindowTextW(strRank);
	if (strRank == _T(""))
	{
		strRank.Format(_T("-"));
	}
	m_edPower.GetWindowTextW(strPower);
	if (strPower == _T(""))
	{
		strPower.Format(_T("-"));
	}

	strData.Format(_T("%d,%s,%s,%s"), m_cbApprovalStatus.GetCurSel(), strID, strRank, strPower);
	GetSearchData(strData);
}


void CUserManageDlg::OnNMDblclklistumInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strID = _T("");
	if (pNMItemActivate->iItem != -1)
	{
		strID = m_listInfo.GetItemText(pNMItemActivate->iItem, 0);

		CMainDlg* pDlg = (CMainDlg*)m_pParent;

		pDlg->SetLoginID(strID);
		pDlg->SetInfoMode(eInfoMode::eInfo_Modify);

		pDlg->UserInfoDlgCreate();
	}
	*pResult = 0;
}


void CUserManageDlg::OnBnClickedbtumApproval()
{
	BOOL bApproval = FALSE;
	CString strID;
	std::vector<CString>vData;
	for (int i = 0; i < m_listInfo.GetItemCount(); i++)
	{
		bApproval = m_listInfo.GetCheck(i);
		if (bApproval == TRUE)
		{
			strID = m_listInfo.GetItemText(i, 0);
			vData.push_back(strID);
		}
	}

	if (m_pDB != NULL)
	{
		for(auto it = vData.begin(); it != vData.end(); it++)
		//for (int i = 0; i < (int)vData.size(); i++)
		{
			CString strQry;
			strQry.Format(_T("update LOGIN											\
								set Approval	= 1,								\
								D_Approval	=  DATETIME('NOW', 'LOCALTIME')			\
								where id = '%s';"),
								//vData[i]);
								*it);
			m_pDB->InsUpDel(strQry);
		}
		AfxMessageBox(stStatusLoginText[(int)eStatusLogin::eCompApproval].text);
		GetSearchData(_T(""));
	}

}


void CUserManageDlg::OnBnClickedbtumDelete()
{
	BOOL bApproval = FALSE;
	CString strID;

	if (m_pDB != NULL)
	{
		//클릭한 index 행 삭제
		int nIndex = m_listInfo.GetSelectionMark();
		strID = m_listInfo.GetItemText(nIndex, 0);

		if (strID != "admin")
		{
			CString strQry;
			strQry.Format(_T("delete from LOGIN	\
							where id = '%s';"),
							strID);
			m_pDB->InsUpDel(strQry);
			AfxMessageBox(stStatusLoginText[(int)eStatusLogin::eCompDelete].text);
			GetSearchData(_T(""));
		}
	}
}
