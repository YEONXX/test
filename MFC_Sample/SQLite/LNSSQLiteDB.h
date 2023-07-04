#pragma once

#include "CppSQLite3.h"

class CLNSSQLiteDB : public CppSQLite3DB
{
public:
	CLNSSQLiteDB();
	CLNSSQLiteDB(CString arg_strDatabaseFilePath = _T(""));
	~CLNSSQLiteDB();

	CString				m_strDatabaseFilePath;

	HANDLE				m_hDBMuex;
	char*				m_strPath;
	BOOL				m_bInitTable;
	BOOL				m_bInitFlg;	
	CppSQLite3Buffer	m_cbufSQL;
	CppSQLite3Table		m_cTable;

	CppSQLite3DB		m_cDB;
	CppSQLite3Statement m_cStatement;

	void InitDB(HANDLE arg_CreateMutex = NULL);
	void CloseDB();
	BOOL ColumnExists(sqlite3* a_db, const char* arg_strTable, const char* arg_strColumn);
	BOOL InsUpDel(CString arg_strQuery);
	BOOL InsUpDel(char* arg_strQuery);
	BOOL ExecuteQry(CString arg_strQuery);
	BOOL ExecuteQry(char* arg_strQuery);
	BOOL ExecuteQryData(CString arg_strTableName, CString arg_strFindFieldName, CString arg_strCondition);

	int	GetRowSize(){return m_cTable.numRows();}
	int	GetColSize(){return m_cTable.numFields();}
	BOOL GetData(long arg_nRow, long arg_nCol, CString &arg_strData);
	BOOL GetData(long arg_nRow, long arg_nCol, int &arg_nData);
	BOOL GetData(long arg_nRow, long arg_nCol, UINT &arg_nData);
	BOOL GetData(long arg_nRow, long arg_nCol, long &arg_nData);
	BOOL GetFieldNameData(long arg_nRow, char* arg_strFieldName,  CString &strData);
	BOOL GetFieldNameData(long arg_nRow, char* arg_strFieldName,  UINT &arg_nData);
	BOOL GetFieldNameData(long arg_nRow, char* arg_strFieldName,  int &arg_nData);
	BOOL GetFieldNameData(long arg_nRow, char* arg_strFieldName, long &arg_nData);
	BOOL GetFieldNameData(long arg_nRow, char* arg_strFieldName, double &arg_nData);
	BOOL GetFieldNameData(long arg_nRow, char* arg_strFieldName, char* arg_strData,  int arg_nstrDataSize);
	
	BOOL	SetBinaryData_UpdateNum(const char* arg_strTable, const char* arg_strDataField, const char* arg_strBlobField, const int arg_strDataValue, const UCHAR* arg_pBuf, UINT arg_nLen);
	BOOL	SetBinaryData_UpdateNum(const char* arg_strTable, const char* arg_strDataField, const char* arg_strBlobField, const int arg_strDataValue, const UCHAR* arg_pBuf, UINT arg_nLen, int arg_nBlob_Status);
	BOOL	SetBinaryData_Update(const char* arg_strTable, const char* arg_strDataField, const char* arg_strBlobField, const char * arg_strDataValue, const UCHAR* arg_pBuf, UINT arg_nLen);
	BOOL	SetBinaryData_UpdateBlob(const char* arg_strTable, const char* arg_strDataField, const char* arg_strBlobField, const char * arg_strDataValue, const UCHAR* arg_pBuf, UINT arg_nLen);
	BOOL	SetBinaryData_InsertNum(const char* arg_strTable, const char* arg_strDataField, const char* arg_strBlobField, int arg_nDataValue, const UCHAR* arg_pBuf, UINT arg_nLen);
	BOOL	SetBinaryData_Insert(const char* arg_strTable, const char* arg_strDataField, const char* arg_strBlobField, const char * arg_strDataValue, const UCHAR* arg_pBuf, UINT arg_nLen);
	UCHAR*	GetBinaryData(const char* arg_strTable, const char* arg_strBlobField, const char* arg_strValueField, const char* arg_strValue, UINT& arg_nReturnSize);	//함수내에서 메모리 할당을 하니 리턴받은 데이터는 필이 free 함수 사용하여 메모리 해제를 해줘야함.
	UCHAR*	GetBinaryData(const char* arg_strTable, const char* arg_strBlobField, const char* arg_strValueField, const char* arg_strValue, UINT& arg_nReturnSize, int& arg_nBlob_Status);	//함수내에서 메모리 할당을 하니 리턴받은 데이터는 필이 free 함수 사용하여 메모리 해제를 해줘야함.
	UCHAR*	GetBinaryDataNum(const char* arg_strTable, const char* arg_strBlobField, const char* arg_strValueField, const int arg_nValue, UINT& arg_nReturnSize);	//함수내에서 메모리 할당을 하니 리턴받은 데이터는 필이 free 함수 사용하여 메모리 해제를 해줘야함.

	BOOL	SetDBPath();
	void	DeleteDBPath();
};