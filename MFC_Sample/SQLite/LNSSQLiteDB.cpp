#include "pch.h"
#include "afxmt.h"
#include "LNSSQLiteDB.h"

//CCriticalSection	g_csDBSync;
CLNSSQLiteDB::CLNSSQLiteDB()
{
	m_bInitFlg = FALSE;
	m_bInitTable = FALSE;
	m_hDBMuex = NULL;
	m_strPath = NULL;
	m_strDatabaseFilePath = _T("");

}

CLNSSQLiteDB::CLNSSQLiteDB(CString arg_strDatabaseFilePath /* = _T("") */)
{
	m_bInitFlg = FALSE;
	m_bInitTable = FALSE;
	m_hDBMuex = NULL;
	m_strPath = NULL;
	m_strDatabaseFilePath = arg_strDatabaseFilePath;
}

BOOL CLNSSQLiteDB::SetDBPath()
{
	BOOL bRet = TRUE;
	if (m_strDatabaseFilePath.GetLength())
	{
		try
		{
			int sLen = WideCharToMultiByte(CP_ACP, 0, m_strDatabaseFilePath, -1, NULL, 0, NULL, NULL);
			m_strPath = new (std::nothrow)char[(UINT)sLen];			

			if (m_strPath != NULL)
			{
				memset(m_strPath, 0x00, sLen);
				WideCharToMultiByte(CP_ACP, 0, m_strDatabaseFilePath, -1, m_strPath, sLen, NULL, NULL);
			}
			else
			{
				bRet = FALSE;
			}
		}
		catch (CException* e)
		{
			bRet = FALSE;

			TCHAR szCause[255];
			CString strError = _T("");
			e->GetErrorMessage(szCause, 255);

			strError = _T("Error -> ");
			strError += szCause;

			TRACE(strError);

			e->Delete();
		}
	}
	else
	{
		bRet = FALSE;
	}

	return bRet;
}

CLNSSQLiteDB::~CLNSSQLiteDB()
{
	if(m_bInitFlg)
		m_cDB.close();
	if(m_bInitTable)
		m_cTable.finalize();
	if(m_hDBMuex)
	{
		WaitForMultipleObjects(1, &m_hDBMuex, TRUE, INFINITE);
		CloseHandle(m_hDBMuex);
		m_hDBMuex = NULL;
	}

	DeleteDBPath();
}

void CLNSSQLiteDB::DeleteDBPath()
{
	if (m_strPath != NULL)
	{
		delete[] m_strPath;
		m_strPath = NULL;
	}
}

void CLNSSQLiteDB::InitDB(HANDLE arg_CreateMutex /* = NULL */)
{
	try
	{
		if (arg_CreateMutex == NULL)
		{
			m_hDBMuex = CreateMutex(NULL, FALSE, NULL);
		}			
		else
		{
			m_hDBMuex = arg_CreateMutex;
		}

		SetDBPath();
	}
	catch (CException *e)
	{
		TCHAR szCause[255];
		CString strError = _T("");
		e->GetErrorMessage(szCause, 255);
		
		strError = _T("Error -> ");
		strError += szCause;
		
		TRACE(strError);
		
		e->Delete();
	}
}

BOOL CLNSSQLiteDB::InsUpDel(CString arg_strQuery)
{
	BOOL l_bTransFlg = TRUE;
	if(m_hDBMuex == NULL)
	{
		 l_bTransFlg = FALSE;
	}
	else
	{
		if(WaitForSingleObject (m_hDBMuex,INFINITE) == WAIT_FAILED)
		{
			l_bTransFlg = FALSE;
		}
		else
		{
			if(!m_bInitFlg)
			{
				m_cDB.open(m_strPath); 
				m_cDB.execDML("PRAGMA cache_size = 16000; PRAGMA temp_store = 2; PRAGMA synchronous = OFF; PRAGMA journal_mode = MEMORY; PRAGMA redad_uncommited;");
				m_bInitFlg = TRUE;
			}


			BOOL l_bTransaction = FALSE;
			try
			{
				char* l_strSQL;
				int sLen = WideCharToMultiByte(CP_ACP,0,arg_strQuery,-1,NULL,0,NULL,NULL);
				l_strSQL = new char[(UINT)sLen+1];
				WideCharToMultiByte(CP_ACP, 0, arg_strQuery, -1, l_strSQL, sLen,NULL,NULL);

				m_cDB.execDML("BEGIN TRANSACTION;");
				l_bTransaction = TRUE;
				m_cDB.execDML(l_strSQL);
				delete [] l_strSQL;
				m_cDB.execDML("END TRANSACTION;");				
			}
			catch (...)
			{
				l_bTransFlg = FALSE;
				if(l_bTransaction)
					m_cDB.execDML("rollback transaction;");
			}	

			m_cDB.close();
			m_bInitFlg = FALSE;	

			if( ReleaseMutex(m_hDBMuex) == FALSE)
			{
				TRACE(_T("ReleaseMutex --> FALSE\r\n"));
			}
		}
	}

	return l_bTransFlg;
}


void CLNSSQLiteDB::CloseDB()
{	
	try
	{
		m_cDB.close();
		m_bInitFlg = FALSE;
	}	
	catch (CException* e)
	{
		TCHAR szCause[255];
		CString strError = _T("");
		e->GetErrorMessage(szCause, 255);
		
		strError = _T("Error -> ");
		strError += szCause;
		
		TRACE(strError);
		
		e->Delete();
	}
}

BOOL CLNSSQLiteDB::InsUpDel(char* arg_strQuery)
{
	BOOL l_bTransFlg = TRUE;
	if(m_hDBMuex == NULL)
	{
		l_bTransFlg = FALSE;
	}
	else
	{
		
		if( WaitForSingleObject (m_hDBMuex,INFINITE) == WAIT_FAILED)
		{
			l_bTransFlg = FALSE;
		}
		else
		{

			if(!m_bInitFlg)
			{
				m_cDB.open(m_strPath); 
				m_cDB.execDML("PRAGMA cache_size = 16000; PRAGMA temp_store = 2; PRAGMA synchronous = OFF; PRAGMA journal_mode = MEMORY; PRAGMA redad_uncommited;");
				m_bInitFlg = TRUE;
			}


			BOOL l_bTransaction = FALSE;
			try
			{
				m_cDB.execDML("BEGIN TRANSACTION;");
				l_bTransaction = TRUE;
				m_cDB.execDML(arg_strQuery);
				m_cDB.execDML("END TRANSACTION;");				

			}
			catch (...)
			{
				l_bTransFlg = FALSE;
				if(l_bTransaction)
					m_cDB.execDML("rollback transaction;");
			}	

			m_cDB.close();
			m_bInitFlg = FALSE;
		
			if( ReleaseMutex(m_hDBMuex) == FALSE)
			{
				TRACE(_T("ReleaseMutex --> FALSE\r\n"));
			}
		}
   }
	
	return l_bTransFlg;
}

BOOL CLNSSQLiteDB::ExecuteQryData(CString arg_strTableName, CString arg_strFindFieldName, CString arg_strCondition)
{

	BOOL l_bTransFlg = TRUE;	
	if(m_hDBMuex == NULL)
	{
		l_bTransFlg = FALSE;	
	}
	else
	{
		if(  WaitForSingleObject (m_hDBMuex,INFINITE) == WAIT_FAILED)
		{
			l_bTransFlg = FALSE;	
		}
		else
		{

			CString strSQL = _T("");

			strSQL.Format(_T("SELECT %s FROM %s WHERE %s"), arg_strFindFieldName, arg_strTableName, arg_strCondition);

			if(!m_bInitFlg)
			{
				m_cDB.open(m_strPath); 
				m_cDB.execDML("PRAGMA cache_size = 16000; PRAGMA temp_store = 2; PRAGMA synchronous = OFF; PRAGMA journal_mode = MEMORY; PRAGMA redad_uncommited;");
				m_bInitFlg = TRUE;
			}

			try
			{
				CppSQLite3Table l_cTable;
				m_cTable = l_cTable;

				char* l_strSQL = NULL;
				int sLen = WideCharToMultiByte(CP_ACP,0,strSQL,-1,NULL,0,NULL,NULL);
				l_strSQL = new char[(UINT)sLen+1];
				WideCharToMultiByte(CP_ACP, 0, strSQL, -1, l_strSQL, sLen,NULL,NULL);

				m_cTable = m_cDB.getTable(l_strSQL);
				m_bInitTable = TRUE;
				delete [] l_strSQL;
				
			}
			catch (...)
			{
				l_bTransFlg = FALSE;
				//////////////////////////////////printf("===Error ExecuteQry(%s)===\r\n", e.errorMessage());
			}

			m_cDB.close();
			m_bInitFlg = FALSE;

			if( ReleaseMutex(m_hDBMuex) == FALSE)
			{
				TRACE(_T("ReleaseMutex --> FALSE\r\n"));
			}
		}

	}	
	return l_bTransFlg;

}

BOOL CLNSSQLiteDB::ExecuteQry(CString arg_strQuery)
{
	BOOL l_bTransFlg = TRUE;

	if(m_hDBMuex == NULL )
	{
		l_bTransFlg = FALSE;
	}
	else
	{	
		if(  WaitForSingleObject (m_hDBMuex,INFINITE) == WAIT_FAILED)
		{
			l_bTransFlg = FALSE;
		}
		else
		{
			if(!m_bInitFlg)
			{
				m_cDB.open(m_strPath); 
				m_cDB.execDML("PRAGMA cache_size = 16000; PRAGMA temp_store = 2; PRAGMA synchronous = OFF; PRAGMA journal_mode = MEMORY; PRAGMA redad_uncommited;");
				m_bInitFlg = TRUE;
			}


			try
			{
				CppSQLite3Table l_cTable;
				m_cTable = l_cTable;

				char* l_strSQL = NULL;
				int sLen = WideCharToMultiByte(CP_ACP,0,arg_strQuery,-1,NULL,0,NULL,NULL);
				l_strSQL = new char[(UINT)sLen+1];
				WideCharToMultiByte(CP_ACP, 0, arg_strQuery, -1, l_strSQL, sLen,NULL,NULL);

				m_cTable = m_cDB.getTable(l_strSQL);
				m_bInitTable = TRUE;
				delete [] l_strSQL;			
			}
			catch (...)
			{
				l_bTransFlg = FALSE;
				//////////////////////////////////printf("===Error ExecuteQry(%s,%s)===\r\n", arg_strQuery, e.errorMessage());
			}


			m_cDB.close();
			m_bInitFlg = FALSE;			
			if( ReleaseMutex(m_hDBMuex) == FALSE)
			{
				TRACE(_T("ReleaseMutex --> FALSE\r\n"));
			}
		}
	}
	
	return l_bTransFlg;
}

BOOL CLNSSQLiteDB::ExecuteQry(char* arg_strQuery)
{
	BOOL l_bTransFlg = TRUE;
	//////////////////////////////////printf(" === ExecuteQry(char* arg_strQuery) =====\n");
	if(m_hDBMuex == NULL)
	{
		l_bTransFlg = FALSE;
	}
	else
	{
		if(  WaitForSingleObject (m_hDBMuex,INFINITE) == WAIT_FAILED)
		{
			l_bTransFlg = FALSE;
		}
		else
		{

			if(!m_bInitFlg)
			{
				m_cDB.open(m_strPath); 
				m_cDB.execDML("PRAGMA cache_size = 16000; PRAGMA temp_store = 2; PRAGMA synchronous = OFF; PRAGMA journal_mode = MEMORY; PRAGMA redad_uncommited;");
				m_bInitFlg = TRUE;
			}


			try
			{
				CppSQLite3Table l_cTable;
				m_cTable = l_cTable;
				m_cTable = m_cDB.getTable(arg_strQuery);
				m_bInitTable = TRUE;
				
			}
			catch (...)
			{
				m_bInitTable = FALSE;
				l_bTransFlg = FALSE;
				//////////////////////////////////printf("===Error ExecuteQry(%s,%s)===\r\n", arg_strQuery, e.errorMessage());
			}

			m_cDB.close();
			m_bInitFlg = FALSE;
			if(ReleaseMutex(m_hDBMuex) == FALSE)
			{
				TRACE(_T("ReleaseMutex --> FALSE\r\n"));
			}
		}
	}
	
	return l_bTransFlg;
}

BOOL CLNSSQLiteDB::GetData(long arg_nRow, long arg_nCol, CString &arg_strData)
{
	BOOL l_bRet = FALSE ; 

	if(m_bInitTable)
	{
		if( arg_nRow < m_cTable.numRows() && arg_nCol < m_cTable.numFields() )
		{
			m_cTable.setRow(arg_nRow);

			if (!m_cTable.fieldIsNull(arg_nCol))
			{
				TCHAR *str1 = NULL;  //char To TCHAR

				//str1의 길이 값을 동적할당 하기위해 len 선언

				UINT len = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_cTable.fieldValue(arg_nCol),-1,NULL,NULL);

				if ( sizeof(TCHAR) * len  <= UINT_MAX)
				{
					str1 = new TCHAR[len];
					memset(str1,0x00, len*sizeof(TCHAR) ) ;
					//char To TCHAR 변환
					MultiByteToWideChar(CP_ACP,0,m_cTable.fieldValue(arg_nCol),-1,str1,len);
					arg_strData.Format(_T("%s"),str1);
					delete[] str1;
					str1 = NULL;
					l_bRet = TRUE;
				}
			}
		}
	}

	if (l_bRet == FALSE)
	{
		arg_strData.Empty();
	}
	return l_bRet ; 
}

BOOL CLNSSQLiteDB::GetData(long arg_nRow, long arg_nCol,int &arg_nData)
{
	BOOL l_bRet = FALSE ; 
	if(m_bInitTable)
	{
		if( arg_nRow < m_cTable.numRows() && arg_nCol < m_cTable.numFields() )
		{
			m_cTable.setRow(arg_nRow);

			if (!m_cTable.fieldIsNull(arg_nCol))
			{
				CString l_strTemp = _T("");
				TCHAR *str1 = NULL;  //char To TCHAR

				//str1의 길이 값을 동적할당 하기위해 len 선언

				UINT len = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_cTable.fieldValue(arg_nCol),-1,NULL,NULL);
				
				if ( sizeof(TCHAR) * len  <= INT_MAX)  /*@CodeSonar_xxx_151005_Integer Overflow of Allocation Size  */
				{
					str1 = new TCHAR[len];
					memset(str1,0x00, len*sizeof(TCHAR)) ;
					//char To TCHAR 변환
					MultiByteToWideChar(CP_ACP,0,m_cTable.fieldValue(arg_nCol),-1,str1,len);

					l_strTemp.Format(_T("%s"),str1);
					arg_nData = _tstoi(l_strTemp);
					l_bRet = TRUE;
					delete[] str1;
					str1 = NULL;
				}
			}
		}
	}
	if (l_bRet == FALSE)
	{
		arg_nData = 0;
	}
	return l_bRet ; 
}

BOOL CLNSSQLiteDB::GetData(long arg_nRow, long arg_nCol,UINT &arg_nData)
{
	BOOL l_bRet = FALSE ; 

	if(m_bInitTable)
	{
		if( arg_nRow < m_cTable.numRows() && arg_nCol < m_cTable.numFields() )
		{
			m_cTable.setRow(arg_nRow);

			if (!m_cTable.fieldIsNull(arg_nCol))
			{
				CString l_strTemp = _T("");
				TCHAR *str1 = NULL;  //char To TCHAR

				//str1의 길이 값을 동적할당 하기위해 len 선언

				UINT len = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_cTable.fieldValue(arg_nCol),-1,NULL,NULL);
				
				if ( sizeof(TCHAR) * len  <= INT_MAX)  /*@CodeSonar_xxx_151005_Integer Overflow of Allocation Size  */
				{
					str1 = new TCHAR[len];
					memset(str1,0x00, len*sizeof(TCHAR)) ;
					//char To TCHAR 변환
					MultiByteToWideChar(CP_ACP,0,m_cTable.fieldValue(arg_nCol),-1,str1,len);

					l_strTemp.Format(_T("%s"),str1);
					arg_nData = _tstoi(l_strTemp);			
					delete[] str1;
					str1 = NULL;
					l_bRet = TRUE;
				}
			}
		}
	}

	if (l_bRet == FALSE)
	{
		arg_nData = 0;
	}
	return l_bRet ; 
}


BOOL CLNSSQLiteDB::GetData(long arg_nRow, long arg_nCol,long &arg_nData)
{
	BOOL l_bRet = FALSE ; 

	if(m_bInitTable)
	{
		if( arg_nRow < m_cTable.numRows() && arg_nCol < m_cTable.numFields() )
		{
			m_cTable.setRow(arg_nRow);

			if (!m_cTable.fieldIsNull(arg_nCol))
			{
				CString l_strTemp = _T("");
				TCHAR *str1 = NULL;  //char To TCHAR

				//str1의 길이 값을 동적할당 하기위해 len 선언

				UINT len = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_cTable.fieldValue(arg_nCol),-1,NULL,NULL);

				if ( sizeof(TCHAR) * len  <= INT_MAX)  /*@CodeSonar_xxx_151005_Integer Overflow of Allocation Size  */
				{
					str1 = new TCHAR[len];
					memset(str1,0x00, len*sizeof(TCHAR)) ;
					//char To TCHAR 변환
					MultiByteToWideChar(CP_ACP,0,m_cTable.fieldValue(arg_nCol),-1,str1,len);
					l_strTemp.Format(_T("%s"),str1);
					arg_nData = _tstol(l_strTemp);
					delete[] str1;
					str1 = NULL;
					l_bRet = TRUE;
				}
			}
		}
	}
	if (l_bRet == FALSE)
	{
		arg_nData = 0;
	}
	return l_bRet ; 
}
BOOL CLNSSQLiteDB::GetFieldNameData(long arg_nRow, char* arg_strFieldName,  CString &arg_strData)
{
	BOOL l_bRet = FALSE ; 
	if(m_bInitTable)
	{
		if( arg_nRow < m_cTable.numRows() && arg_strFieldName != NULL )
		{
			m_cTable.setRow(arg_nRow);

			if (!m_cTable.fieldIsNull(arg_strFieldName))
			{
				TCHAR *str1 = NULL;  //char To TCHAR

				//str1의 길이 값을 동적할당 하기위해 len 선언
				UINT len = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_cTable.fieldValue(arg_strFieldName),-1,NULL,NULL);

				if ( sizeof(TCHAR) * len  <= INT_MAX)  /*@CodeSonar_xxx_151005_Integer Overflow of Allocation Size  */
				{
					str1 = new TCHAR[len];
					memset(str1,0x00, len*sizeof(TCHAR)) ;
					//char To TCHAR 변환
					MultiByteToWideChar(CP_ACP,0,m_cTable.fieldValue(arg_strFieldName),-1,str1,len);
					arg_strData.Format(_T("%s"),str1);
					delete[] str1;
					str1 = NULL;
					l_bRet = TRUE;
				}
			}
		}
	}
	if(l_bRet == FALSE)
	{
		arg_strData.Format(_T(""));
	}
	return l_bRet;
}

BOOL CLNSSQLiteDB::GetFieldNameData(long arg_nRow, char* arg_strFieldName,  char* arg_strData ,  int arg_nstrDataSize )
{
	BOOL l_bRet = FALSE ; 
	if(m_bInitTable)
	{
		if( arg_nRow < m_cTable.numRows() && arg_strFieldName != NULL )
		{
			m_cTable.setRow(arg_nRow);

			if (!m_cTable.fieldIsNull(arg_strFieldName))
			{
				TCHAR *str1 = NULL;  //char To TCHAR
				//str1의 길이 값을 동적할당 하기위해 len 선언
				UINT len = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_cTable.fieldValue(arg_strFieldName),-1,NULL,NULL);
				
				if ( sizeof(TCHAR) * len  <= INT_MAX)  /*@CodeSonar_xxx_151005_Integer Overflow of Allocation Size  */
				{
					str1 = new TCHAR[len]; //Integer Overflow of Allocation Size					
					memset(str1,0x00, len*sizeof(TCHAR)) ;
					//char To TCHAR 변환
					MultiByteToWideChar(CP_ACP,0,m_cTable.fieldValue(arg_strFieldName),-1,str1,len);

					if (sizeof(TCHAR) * len <=  (UINT)arg_nstrDataSize)
					{
						WideCharToMultiByte(CP_ACP, 0, str1, len, arg_strData, arg_nstrDataSize , NULL, NULL);
						l_bRet = TRUE;
					}
					
					delete[] str1;
					str1 = NULL;
				}
			}
		}
	}

	return l_bRet ; 
}

BOOL CLNSSQLiteDB::GetFieldNameData(long arg_nRow, char* arg_strFieldName, long &arg_nData)
{
	BOOL l_bRet = FALSE ; 
	if(m_bInitTable)
	{
		if( arg_nRow < m_cTable.numRows() && arg_strFieldName != NULL )
		{
			m_cTable.setRow(arg_nRow);

			if (!m_cTable.fieldIsNull(arg_strFieldName))
			{
				CString l_strTemp = _T("");
				TCHAR *str1 = NULL;  //char To TCHAR

				//str1의 길이 값을 동적할당 하기위해 len 선언

				UINT len = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_cTable.fieldValue(arg_strFieldName),-1,NULL,NULL);

				if ( sizeof(TCHAR) * len  <= INT_MAX)  /*@CodeSonar_xxx_151005_Integer Overflow of Allocation Size  */
				{
					str1 = new TCHAR[len];
					memset(str1,0x00, len*sizeof(TCHAR)) ;
					//char To TCHAR 변환
					MultiByteToWideChar(CP_ACP,0,m_cTable.fieldValue(arg_strFieldName),-1,str1,len);
					l_strTemp.Format(_T("%s"),str1);
					arg_nData = _tstoi(l_strTemp);
					delete[] str1;
					str1 = NULL;
					l_bRet = TRUE;
				}
			}
		}
	}
	return l_bRet ; 
}
BOOL CLNSSQLiteDB::GetFieldNameData(long arg_nRow, char* arg_strFieldName,  UINT &arg_nData)
{
	BOOL l_bRet = FALSE ;

	if(m_bInitTable)
	{
		if( arg_nRow < m_cTable.numRows() && arg_strFieldName != NULL )
		{
			m_cTable.setRow(arg_nRow);

			if (!m_cTable.fieldIsNull(arg_strFieldName))
			{
				CString l_strTemp = _T("");
				TCHAR *str1 = NULL;  //char To TCHAR
				UINT len = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_cTable.fieldValue(arg_strFieldName), -1, NULL, NULL);

				if ( sizeof(TCHAR) * len  <= UINT_MAX)  /*@CodeSonar_xxx_151005_Integer Overflow of Allocation Size  */
				{
					str1 = new TCHAR[len];
					//char To TCHAR 변환
					memset(str1,0x00, len*sizeof(TCHAR)) ;
					char l_str[50] = {0,};
					MultiByteToWideChar(CP_ACP,0,m_cTable.fieldValue(arg_strFieldName),-1,str1,len);
					l_strTemp.Format(_T("%s"),str1);
					arg_nData = _ttoi(l_strTemp);
					l_bRet = TRUE;
					delete[] str1;
					str1 = NULL;
				}
			}
		}
	}

	return l_bRet ; 
}

BOOL CLNSSQLiteDB::GetFieldNameData(long arg_nRow, char* arg_strFieldName,  int &arg_nData)
{
	BOOL l_bRet = FALSE ; 

	if(&arg_nData != NULL)
	{
		if(m_bInitTable)
		{
			if( arg_nRow < m_cTable.numRows() && arg_strFieldName != NULL )
			{
				m_cTable.setRow(arg_nRow);

				if (!m_cTable.fieldIsNull(arg_strFieldName))
				{
					CString l_strTemp = _T("");
					TCHAR *str1 = NULL;  //char To TCHAR

					//str1의 길이 값을 동적할당 하기위해 len 선언

					UINT len = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_cTable.fieldValue(arg_strFieldName),-1,NULL,NULL);
					
					if ( sizeof(TCHAR) * len  <= INT_MAX)  /*@CodeSonar_xxx_151005_Integer Overflow of Allocation Size  */
					{
						str1 = new TCHAR[len];
						memset(str1,0x00, len*sizeof(TCHAR)) ;
						//char To TCHAR 변환
						char l_str[50] = {0,};
						MultiByteToWideChar(CP_ACP,0,m_cTable.fieldValue(arg_strFieldName),-1,str1,len);
						l_strTemp.Format(_T("%s"),str1);
						arg_nData = _ttoi(l_strTemp);
						delete[] str1;
						str1 = NULL;
						l_bRet = TRUE;
					}
				}
			}
		}
	}
	return l_bRet ; 
}

BOOL CLNSSQLiteDB::GetFieldNameData(long arg_nRow, char* arg_strFieldName,  double &arg_nData)
{
	BOOL l_bRet = FALSE ; 
	if(m_bInitTable)
	{
		if( arg_nRow < m_cTable.numRows() && arg_strFieldName != NULL )
		{
			m_cTable.setRow(arg_nRow);

			if (!m_cTable.fieldIsNull(arg_strFieldName))
			{
				CString l_strTemp = _T("");
				TCHAR *str1 = NULL;  //char To TCHAR

				//str1의 길이 값을 동적할당 하기위해 len 선언

				UINT len = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_cTable.fieldValue(arg_strFieldName),-1,NULL,NULL);

				if ( sizeof(TCHAR) * len  <= INT_MAX)  /*@CodeSonar_xxx_151005_Integer Overflow of Allocation Size  */
				{
					str1 = new TCHAR[len];
					memset(str1,0x00, len*sizeof(TCHAR)) ;  /*@CodeSonar_xxx_151007_Uninitialized Variable  */
					//char To TCHAR 변환
					char l_str[50] = {0,};
					MultiByteToWideChar(CP_ACP,0,m_cTable.fieldValue(arg_strFieldName),-1,str1,len);
					l_strTemp.Format(_T("%s"),str1);
					arg_nData = _ttof(l_strTemp);
					delete[] str1;
					str1 = NULL;
					l_bRet = TRUE ;
				}
			}
		}
	}

	return l_bRet ; 
}

BOOL CLNSSQLiteDB::SetBinaryData_UpdateNum(const char* arg_strTable, const char* arg_strDataField, const char* arg_strBlobField, const int arg_nDataValue, const UCHAR* arg_pBuf, UINT arg_nLen)
{
	BOOL l_bTransFlg = TRUE;

	if ( m_hDBMuex == NULL || arg_nLen == 0 )
	{
		l_bTransFlg = FALSE;
	}
	else
	{
		if( WaitForSingleObject (m_hDBMuex,INFINITE) == WAIT_FAILED)
		{
			l_bTransFlg = FALSE;			
		}
		else
		{
			if(!m_bInitFlg)
			{
				m_cDB.open(m_strPath); 
				m_cDB.execDML("PRAGMA cache_size = 16000; PRAGMA temp_store = 2; PRAGMA synchronous = OFF; PRAGMA journal_mode = MEMORY; PRAGMA redad_uncommited;");
				m_bInitFlg = TRUE;
			}

			CString l_strSql;
			CppSQLite3Binary	l_cBlob;	


			BOOL l_bTransaction = FALSE;

			try
			{
				l_cBlob.setBinary(arg_pBuf, arg_nLen);
				m_cbufSQL.format("update %s set %s = '%q' where %s = %d;", arg_strTable, arg_strBlobField, l_cBlob.getEncoded(), arg_strDataField, arg_nDataValue);		
				m_cDB.execDML("BEGIN TRANSACTION;");
				l_bTransaction = TRUE;
				m_cDB.execDML(m_cbufSQL);
				m_cDB.execDML("END TRANSACTION;");
				//l_bTransFlg = TRUE;
				l_cBlob.clear();
				m_cbufSQL.clear();
				
			}
			catch (...)
			{	
				if(l_bTransaction)
				{
					m_cDB.execDML("rollback transaction;");
					l_bTransFlg = FALSE;
				}
			}

			/*@CodeSonar_xxx_151007_Redundant Condition  m_bInitFlg always evaluates to true.  */
			//if(m_bInitFlg)
			{
				m_cDB.close();
				m_bInitFlg = FALSE;
			}	
		}	

		if( ReleaseMutex(m_hDBMuex) == FALSE)
		{
			TRACE(_T("ReleaseMutex --> FALSE\r\n"));
		}
	}

	return l_bTransFlg;
}


BOOL CLNSSQLiteDB::SetBinaryData_UpdateNum(const char* arg_strTable, const char* arg_strDataField, const char* arg_strBlobField, const int arg_nDataValue, const UCHAR* arg_pBuf, UINT arg_nLen, int arg_nBlob_Status)
{

	BOOL l_bTransFlg = TRUE;

	if ( m_hDBMuex == NULL || arg_nLen == 0 )
	{
		l_bTransFlg = FALSE;
	}
	else
	{	
		if(  WaitForSingleObject (m_hDBMuex,INFINITE) == WAIT_FAILED)
		{
			l_bTransFlg = FALSE;
		}
		else
		{

			if(!m_bInitFlg)
			{
				m_cDB.open(m_strPath); 
				m_cDB.execDML("PRAGMA cache_size = 16000; PRAGMA temp_store = 2; PRAGMA synchronous = OFF; PRAGMA journal_mode = MEMORY; PRAGMA redad_uncommited;");
				m_bInitFlg = TRUE;
			}

			CString l_strSql;
			CppSQLite3Binary	l_cBlob;	


			BOOL l_bTransaction = FALSE;

			try
			{
				l_cBlob.setBinary(arg_pBuf, arg_nLen);
				m_cbufSQL.format("update %s set %s = '%q', BMODULE_STATUS = %d where %s = %d;", arg_strTable, arg_strBlobField, l_cBlob.getEncoded(), arg_nBlob_Status, arg_strDataField, arg_nDataValue);		
				m_cDB.execDML("BEGIN TRANSACTION;");
				l_bTransaction = TRUE;
				m_cDB.execDML(m_cbufSQL);
				m_cDB.execDML("END TRANSACTION;");
				l_cBlob.clear();
				m_cbufSQL.clear();				
			}
			catch (...)
			{	
				if(l_bTransaction)
				  m_cDB.execDML("rollback transaction;");
				l_bTransFlg = FALSE;
			}

			/*@CodeSonar_xxx_151007_Redundant Condition  m_bInitFlg always evaluates to true.  */
			//if(m_bInitFlg)
			{
				m_cDB.close();
				m_bInitFlg = FALSE;
			}
		}
		if(  ReleaseMutex(m_hDBMuex) == FALSE)
		{
			TRACE(_T("ReleaseMutex --> FALSE\r\n"));
		}
	}
	return l_bTransFlg;
}


BOOL CLNSSQLiteDB::SetBinaryData_Update(const char* arg_strTable, const char* arg_strDataField, const char* arg_strBlobField, const char * arg_strDataValue, const UCHAR* arg_pBuf, UINT arg_nLen)
{
	
	BOOL l_bTransFlg = TRUE;
	
	if ( m_hDBMuex == NULL || arg_nLen == 0 )
	{
		l_bTransFlg = FALSE;
	}
	else
	{	
		if( WaitForSingleObject (m_hDBMuex,INFINITE) == WAIT_FAILED)
		{
			l_bTransFlg = FALSE;
		}
		else
		{
			if(!m_bInitFlg)
			{
				m_cDB.open(m_strPath); 
				m_cDB.execDML("PRAGMA cache_size = 16000; PRAGMA temp_store = 2; PRAGMA synchronous = OFF; PRAGMA journal_mode = MEMORY; PRAGMA redad_uncommited;");
				m_bInitFlg = TRUE;
			}

			try
			{
				CString l_strSql;
				CppSQLite3Binary	l_cBlob;
				l_cBlob.setBinary(arg_pBuf, arg_nLen);

				//	m_cbufSQL.format("insert into bindata values ('test1', %Q);",l_cBlob.getEncoded());
				m_cbufSQL.format("update %s ('%s', '%s') values ('%s', %Q);", arg_strTable, arg_strDataField, arg_strBlobField, arg_strDataValue, l_cBlob.getEncoded());
				//	m_cbufSQL.format("insert into %s (%s, %s)values (%s, %Q);", arg_strTable, arg_strDataField, arg_strBlobField, arg_strDataValue, l_cBlob.getEncoded());
				m_cDB.execDML(m_cbufSQL);			
				l_cBlob.clear();
				m_cbufSQL.clear();
			}
			catch (...)
			{
				l_bTransFlg = FALSE;				
			}
			
			{
				m_cDB.close();
				m_bInitFlg = FALSE;
			}
		}		
		
		if( ReleaseMutex(m_hDBMuex) == FALSE)
		{
			TRACE(_T("ReleaseMutex --> FALSE\r\n"));
		}
	}	
	
	return l_bTransFlg;
}

BOOL CLNSSQLiteDB::SetBinaryData_UpdateBlob(const char* arg_strTable, const char* arg_strDataField, const char* arg_strBlobField, const char * arg_strDataValue, const UCHAR* arg_pBuf, UINT arg_nLen)
{

	BOOL l_bTransFlg = TRUE;

	if ( m_hDBMuex == NULL || arg_nLen == 0 )
	{
		l_bTransFlg = FALSE;
	}
	else
	{
	
		if(  WaitForSingleObject (m_hDBMuex,INFINITE) == WAIT_FAILED)
		{
			l_bTransFlg = FALSE;
		}
		else
		{
			if(!m_bInitFlg)
			{
				m_cDB.open(m_strPath); 
				m_cDB.execDML("PRAGMA cache_size = 50000; PRAGMA temp_store = 2; PRAGMA synchronous = OFF; PRAGMA journal_mode = MEMORY; PRAGMA redad_uncommited;");
				m_bInitFlg = TRUE;
			}

			try
			{
				CString l_strSql;
				CppSQLite3Binary	l_cBlob;
				l_cBlob.setBinary(arg_pBuf, arg_nLen);

				m_cbufSQL.format("update %s set %s = '%q' where %s = '%s';", arg_strTable, arg_strBlobField, l_cBlob.getEncoded(), arg_strDataField, arg_strDataValue);		
				m_cDB.execDML(m_cbufSQL);				
				l_cBlob.clear();
				m_cbufSQL.clear();
			}
			catch (...)
			{
				l_bTransFlg = FALSE;
			}

			{
				m_cDB.close();
				m_bInitFlg = FALSE;
			}
		}		
		if(ReleaseMutex(m_hDBMuex) == FALSE)
		{
			TRACE(_T("ReleaseMutex --> FALSE\r\n"));
		}
	}


	return l_bTransFlg;
}


BOOL CLNSSQLiteDB::SetBinaryData_InsertNum(const char* arg_strTable, const char* arg_strDataField, const char* arg_strBlobField, int arg_nDataValue, const UCHAR* arg_pBuf, UINT arg_nLen)
{
	BOOL l_bTransFlg = TRUE;

	if ( m_hDBMuex == NULL || arg_nLen == 0 )
	{
		l_bTransFlg = FALSE;
	}
	else
	{
	
		if(WaitForSingleObject (m_hDBMuex,INFINITE) == WAIT_FAILED)
		{
			l_bTransFlg = FALSE;
		}
		else
		{
			if(!m_bInitFlg)
			{
				m_cDB.open(m_strPath); 
				m_cDB.execDML("PRAGMA cache_size = 16000; PRAGMA temp_store = 2; PRAGMA synchronous = OFF; PRAGMA journal_mode = MEMORY; PRAGMA redad_uncommited;");
				m_bInitFlg = TRUE;
			}

			try
			{
				CString l_strSql;
				CppSQLite3Binary	l_cBlob;
				l_cBlob.setBinary(arg_pBuf, arg_nLen);

				m_cbufSQL.format("insert into %s ('%s', '%s') values (%d, %Q);", arg_strTable, arg_strDataField, arg_strBlobField, arg_nDataValue, l_cBlob.getEncoded());
				m_cDB.execDML(m_cbufSQL);
				l_bTransFlg = TRUE;
				l_cBlob.clear();
				m_cbufSQL.clear();
			}
			catch (...)
			{
				l_bTransFlg = FALSE;				
			}
			
			{
				m_cDB.close();
				m_bInitFlg = FALSE;
			}
		}
		if(ReleaseMutex(m_hDBMuex) == FALSE)
		{
			TRACE(_T("ReleaseMutex --> FALSE\r\n"));
		}
	}

	return l_bTransFlg;
}

BOOL CLNSSQLiteDB::SetBinaryData_Insert(const char* arg_strTable, const char* arg_strDataField, const char* arg_strBlobField, const char * arg_strDataValue, const UCHAR* arg_pBuf, UINT arg_nLen)
{
	BOOL l_bTransFlg = TRUE;
	
	if ( m_hDBMuex == NULL || arg_nLen == 0 )
	{
		l_bTransFlg = FALSE;
	}
	else
	{	
		if(  WaitForSingleObject (m_hDBMuex,INFINITE) == WAIT_FAILED)
		{
			l_bTransFlg = FALSE;
		}
		else
		{
			if(!m_bInitFlg)
			{
				m_cDB.open(m_strPath); 
				m_cDB.execDML("PRAGMA cache_size = 16000; PRAGMA temp_store = 2; PRAGMA synchronous = OFF; PRAGMA journal_mode = MEMORY; PRAGMA redad_uncommited;");
				m_bInitFlg = TRUE;
			}

			try
			{
				CString l_strSql;
				CppSQLite3Binary	l_cBlob;
				l_cBlob.setBinary(arg_pBuf, arg_nLen);

				m_cbufSQL.format("insert into %s ('%s', '%s') values ('%s', %Q);", arg_strTable, arg_strDataField, arg_strBlobField, arg_strDataValue, l_cBlob.getEncoded());
				m_cDB.execDML(m_cbufSQL);
				l_bTransFlg = TRUE;
				l_cBlob.clear();
				m_cbufSQL.clear();
			}
			catch (...)
			{
				l_bTransFlg = FALSE; 
			}
			
			{
				m_cDB.close();
				m_bInitFlg = FALSE;
			}	
		}		
		if( ReleaseMutex(m_hDBMuex) == FALSE)
		{
			TRACE(_T("ReleaseMutex --> FALSE\r\n"));
		}
	}
	return l_bTransFlg;
}

UCHAR* CLNSSQLiteDB::GetBinaryData(const char* arg_strTable, const char* arg_strBlobField, const char* arg_strValueField, const char* arg_strValue, UINT& arg_nReturnSize)
{
	UCHAR*				l_pBlobData = NULL;	
	if ( m_hDBMuex != NULL )
	{	
		if( WaitForSingleObject (m_hDBMuex,INFINITE) == WAIT_FAILED)
		{
			TRACE(_T("ReleaseMutex --> FALSE\r\n"));
		}
		
		if(!m_bInitFlg)
		{
			m_cDB.open(m_strPath); 
			m_cDB.execDML("PRAGMA cache_size = 16000; PRAGMA temp_store = 2; PRAGMA synchronous = OFF; PRAGMA journal_mode = MEMORY; PRAGMA redad_uncommited;");
			m_bInitFlg = TRUE;
		}
		try
		{
			char				l_strQuery[512] = {'\n',};
			const  UCHAR*		l_pTemp = NULL;
			CppSQLite3Binary	l_cBlob;
			CppSQLite3Query		l_cQuery;

			sprintf_s(l_strQuery, sizeof(l_strQuery), "select %s from %s where %s = '%s'", arg_strBlobField, arg_strTable, arg_strValueField, arg_strValue);
			l_cQuery = m_cDB.execQuery(l_strQuery);

			if (!l_cQuery.eof())
			{
				int l_nBlobSize = 0;
				/*@QAC_xxx_151001_[변환-7][N] 포인터 Cast의 결과로 인한 이전 포인터의 CONST 특성의 상실을 유의해야 한다.. */
				//l_cBlob.setEncoded((unsigned char*)l_cQuery.fieldValue(arg_strBlobField));
				l_cBlob.setEncoded(reinterpret_cast<const unsigned char*>(l_cQuery.fieldValue(arg_strBlobField)));	

				l_nBlobSize = l_cBlob.getBinaryLength();
				if(l_nBlobSize > 0) {
					l_pBlobData = (UCHAR*)calloc((UINT)l_nBlobSize,sizeof(UCHAR));

					l_pTemp = l_cBlob.getBinary();
					if (l_pBlobData != NULL  )
						memcpy(l_pBlobData, l_pTemp, (UINT)l_nBlobSize);
					l_cBlob.clear();
				}
				arg_nReturnSize = l_nBlobSize;
			}
			l_cQuery.finalize();
		}
		catch (CException *e)
		{
			TCHAR szCause[255];
			CString strError = _T("");
			e->GetErrorMessage(szCause, 255);
			
			strError = _T("Error -> ");
			strError += szCause;
			
			TRACE(strError);
			
			e->Delete();
		}

		{
			m_cDB.close();
			m_bInitFlg = FALSE;
		}
	
		if( ReleaseMutex(m_hDBMuex) == FALSE)
		{
			TRACE(_T("ReleaseMutex --> FALSE\r\n"));
		}
	}	
	
	return l_pBlobData;
}

UCHAR* CLNSSQLiteDB::GetBinaryData(const char* arg_strTable, const char* arg_strBlobField, const char* arg_strValueField, const char* arg_strValue, UINT& arg_nReturnSize, int& arg_nBlob_Status)
{
	UCHAR*				l_pBlobData = NULL;
	

	if ( m_hDBMuex != NULL  )
	{
		if( WaitForSingleObject (m_hDBMuex,INFINITE) == WAIT_FAILED)
		{
			TRACE(_T("ReleaseMutex --> FALSE\r\n"));
		}
		
		arg_nBlob_Status = FALSE;
		if(!m_bInitFlg)
		{
			m_cDB.open(m_strPath); 
			m_cDB.execDML("PRAGMA cache_size = 16000; PRAGMA temp_store = 2; PRAGMA synchronous = OFF; PRAGMA journal_mode = MEMORY; PRAGMA redad_uncommited;");
			m_bInitFlg = TRUE;
		}
		try
		{
			char				l_strQuery[512] = {'\n',};
			const  UCHAR*		l_pTemp = NULL;
			CppSQLite3Binary	l_cBlob;
			CppSQLite3Query		l_cQuery;

			sprintf_s(l_strQuery, sizeof(l_strQuery), "select %s from %s where %s = '%s'", arg_strBlobField, arg_strTable, arg_strValueField, arg_strValue);
			l_cQuery = m_cDB.execQuery(l_strQuery);

			if (!l_cQuery.eof())
			{
				int l_nBlobSize = 0;			
				/*@QAC_xxx_151001_[변환-7][N] 포인터 Cast의 결과로 인한 이전 포인터의 CONST 특성의 상실을 유의해야 한다.. */
				//l_cBlob.setEncoded((unsigned char*)l_cQuery.fieldValue(arg_strBlobField));
				l_cBlob.setEncoded(reinterpret_cast<const unsigned char*>(l_cQuery.fieldValue(arg_strBlobField)));	

				l_nBlobSize = l_cBlob.getBinaryLength();
				if(l_nBlobSize > 0) {
					l_pBlobData = (UCHAR*)calloc((UINT)l_nBlobSize,sizeof(UCHAR));

					l_pTemp = l_cBlob.getBinary();
					if (l_pBlobData != NULL)
						memcpy(l_pBlobData, l_pTemp, (UINT)l_nBlobSize);
					l_cBlob.clear();
				}
				arg_nReturnSize = l_nBlobSize;
			}

			l_cQuery.finalize();
		}
		catch (CException* e)
		{
			TCHAR szCause[255];
			CString strError = _T("");
			e->GetErrorMessage(szCause, 255);
			
			strError = _T("Error -> ");
			strError += szCause;
			
			TRACE(strError);
			
			e->Delete();
		}
		
		{
			m_cDB.close();
			m_bInitFlg = FALSE;
		}
		
		if( ReleaseMutex(m_hDBMuex) == FALSE)
		{
			TRACE(_T("ReleaseMutex --> FALSE\r\n"));
		}
	}
	
	return l_pBlobData;
}

UCHAR* CLNSSQLiteDB::GetBinaryDataNum(const char* arg_strTable, const char* arg_strBlobField, const char* arg_strValueField, const int arg_nValue, UINT &arg_nReturnSize)
{
    UCHAR*				l_pBlobData = NULL;
	if ( m_hDBMuex != NULL  )
	{	
		if( WaitForSingleObject (m_hDBMuex,INFINITE) == WAIT_FAILED)
		{
			TRACE(_T("ReleaseMutex --> FALSE\r\n"));
		}
		if(!m_bInitFlg)
		{
			m_cDB.open(m_strPath); 
			m_cDB.execDML("PRAGMA cache_size = 16000; PRAGMA temp_store = 2; PRAGMA synchronous = OFF; PRAGMA journal_mode = MEMORY; PRAGMA redad_uncommited;");
			m_bInitFlg = TRUE;
		}
		try
		{
			char				l_strQuery[512] = {'\n',};
			const  UCHAR*		l_pTemp = NULL;
			CppSQLite3Binary	l_cBlob;
			CppSQLite3Query		l_cQuery;

			sprintf_s(l_strQuery, sizeof(l_strQuery), "select %s from %s where %s = %d", arg_strBlobField, arg_strTable, arg_strValueField, arg_nValue);
			l_cQuery = m_cDB.execQuery(l_strQuery);

			if (!l_cQuery.eof())
			{
				int l_nBlobSize = 0;		

				l_cBlob.setEncoded(reinterpret_cast<const unsigned char*>(l_cQuery.fieldValue(arg_strBlobField)));	

				l_nBlobSize = l_cBlob.getBinaryLength();
				if(l_nBlobSize > 0)
				{
					l_pBlobData = (UCHAR*)calloc((UINT)l_nBlobSize,sizeof(UCHAR));

					l_pTemp = l_cBlob.getBinary();
					
					if (l_pBlobData != NULL && l_pTemp != NULL)
						memcpy(l_pBlobData, l_pTemp, l_nBlobSize);
					l_cBlob.clear();
				}
				arg_nReturnSize = l_nBlobSize;
			}
			l_cQuery.finalize();
	
		}
		catch (CException *e)
		{	
			if (l_pBlobData != NULL)
			{
				free(l_pBlobData);
				l_pBlobData = NULL;
			}

			e->Delete();
		}	

		{
			m_cDB.close();
			m_bInitFlg = FALSE;
		}
		if(  ReleaseMutex(m_hDBMuex) == FALSE)
		{
			TRACE(_T("ReleaseMutex --> FALSE\r\n"));
		}
	}	
	return l_pBlobData;
}