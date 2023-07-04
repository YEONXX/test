////////////////////////////////////////////////////////////////////////////////
// CppSQLite3 - A C++ wrapper around the SQLite3 embedded database library.
//
// Copyright (c) 2004..2007 Rob Groves. All Rights Reserved. rob.groves@btinternet.com
// 
// Permission to use, copy, modify, and distribute this software and its
// documentation for any purpose, without fee, and without a written
// agreement, is hereby granted, provided that the above copyright notice, 
// this paragraph and the following two paragraphs appear in all copies, 
// modifications, and distributions.
//
// IN NO EVENT SHALL THE AUTHOR BE LIABLE TO ANY PARTY FOR DIRECT,
// INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
// PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION,
// EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// THE AUTHOR SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF
// ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". THE AUTHOR HAS NO OBLIGATION
// TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
//
// V3.0		03/08/2004	-Initial Version for sqlite3
//
// V3.1		16/09/2004	-Implemented getXXXXField using sqlite3 functions
//						-Added CppSQLiteDB3::tableExists()
//
// V3.2		01/07/2005	-Fixed execScalar to handle a NULL result
//			12/07/2007	-Added int64 functions to CppSQLite3Query
//						-Throw exception from CppSQLite3DB::close() if error
//						-Trap above exception in CppSQLite3DB::~CppSQLite3DB()
//						-Fix to CppSQLite3DB::compile() as provided by Dave Rollins.
//						-sqlite3_prepare replaced with sqlite3_prepare_v2
//						-Added Name based parameter binding to CppSQLite3Statement.
////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "CppSQLite3.h"
#include <cstdlib>


// Named constant for passing to CppSQLite3Exception when passing it a string
// that cannot be deleted.
static const bool DONT_DELETE_MSG=false;

////////////////////////////////////////////////////////////////////////////////
// Prototypes for SQLite functions not included in SQLite DLL, but copied below
// from SQLite encode.c
////////////////////////////////////////////////////////////////////////////////
UINT sqlite3_encode_binary(const unsigned char *in, UINT n, unsigned char *out);
UINT sqlite3_decode_binary(const unsigned char *in, unsigned char *out);

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

CppSQLite3Exception::CppSQLite3Exception(const int nErrCode,
									char* szErrMess,
									bool bDeleteMsg/*=true*/) :
									mnErrCode(nErrCode)
{
	mpszErrMess = sqlite3_mprintf("%s[%d]: %s",
								errorCodeAsString(nErrCode),
								nErrCode,
								szErrMess ? szErrMess : "");

	if (bDeleteMsg && szErrMess)
	{
		sqlite3_free(szErrMess);
	}
}

									
CppSQLite3Exception::CppSQLite3Exception(const CppSQLite3Exception&  e) :
									mnErrCode(e.mnErrCode)
{
	mpszErrMess = 0;
	if (e.mpszErrMess)
	{
		mpszErrMess = sqlite3_mprintf("%s", e.mpszErrMess);
	}
}

/*@QAC_xxx_151001_[스타일-10][N] [스타일-8][N] 하나의 함수는 하나의 exit point를 가져야 한다. */
//const char* CppSQLite3Exception::errorCodeAsString(int nErrCode)
//{
//	switch (nErrCode)
//	{
//	case SQLITE_OK          : return "SQLITE_OK";
//	case SQLITE_ERROR       : return "SQLITE_ERROR";
//	case SQLITE_INTERNAL    : return "SQLITE_INTERNAL";
//	case SQLITE_PERM        : return "SQLITE_PERM";
//	case SQLITE_ABORT       : return "SQLITE_ABORT";
//	case SQLITE_BUSY        : return "SQLITE_BUSY";
//	case SQLITE_LOCKED      : return "SQLITE_LOCKED";
//	case SQLITE_NOMEM       : return "SQLITE_NOMEM";
//	case SQLITE_READONLY    : return "SQLITE_READONLY";
//	case SQLITE_INTERRUPT   : return "SQLITE_INTERRUPT";
//	case SQLITE_IOERR       : return "SQLITE_IOERR";
//	case SQLITE_CORRUPT     : return "SQLITE_CORRUPT";
//	case SQLITE_NOTFOUND    : return "SQLITE_NOTFOUND";
//	case SQLITE_FULL        : return "SQLITE_FULL";
//	case SQLITE_CANTOPEN    : return "SQLITE_CANTOPEN";
//	case SQLITE_PROTOCOL    : return "SQLITE_PROTOCOL";
//	case SQLITE_EMPTY       : return "SQLITE_EMPTY";
//	case SQLITE_SCHEMA      : return "SQLITE_SCHEMA";
//	case SQLITE_TOOBIG      : return "SQLITE_TOOBIG";
//	case SQLITE_CONSTRAINT  : return "SQLITE_CONSTRAINT";
//	case SQLITE_MISMATCH    : return "SQLITE_MISMATCH";
//	case SQLITE_MISUSE      : return "SQLITE_MISUSE";
//	case SQLITE_NOLFS       : return "SQLITE_NOLFS";
//	case SQLITE_AUTH        : return "SQLITE_AUTH";
//	case SQLITE_FORMAT      : return "SQLITE_FORMAT";
//	case SQLITE_RANGE       : return "SQLITE_RANGE";
//	case SQLITE_ROW         : return "SQLITE_ROW";
//	case SQLITE_DONE        : return "SQLITE_DONE";
//	case CPPSQLITE_ERROR    : return "CPPSQLITE_ERROR";
//	default: return "UNKNOWN_ERROR";
//	}
//}

const char* CppSQLite3Exception::errorCodeAsString(int nErrCode)
{

	char* l_pCh = NULL ; 

	switch (nErrCode)
	{
		case SQLITE_OK          : l_pCh = "SQLITE_OK";
		case SQLITE_ERROR       : l_pCh = "SQLITE_ERROR";
		case SQLITE_INTERNAL    : l_pCh = "SQLITE_INTERNAL";
		case SQLITE_PERM        : l_pCh = "SQLITE_PERM";
		case SQLITE_ABORT       : l_pCh = "SQLITE_ABORT";
		case SQLITE_BUSY        : l_pCh = "SQLITE_BUSY";
		case SQLITE_LOCKED      : l_pCh = "SQLITE_LOCKED";
		case SQLITE_NOMEM       : l_pCh = "SQLITE_NOMEM";
		case SQLITE_READONLY    : l_pCh = "SQLITE_READONLY";
		case SQLITE_INTERRUPT   : l_pCh = "SQLITE_INTERRUPT";
		case SQLITE_IOERR       : l_pCh = "SQLITE_IOERR";
		case SQLITE_CORRUPT     : l_pCh = "SQLITE_CORRUPT";
		case SQLITE_NOTFOUND    : l_pCh = "SQLITE_NOTFOUND";
		case SQLITE_FULL        : l_pCh = "SQLITE_FULL";
		case SQLITE_CANTOPEN    : l_pCh = "SQLITE_CANTOPEN";
		case SQLITE_PROTOCOL    : l_pCh = "SQLITE_PROTOCOL";
		case SQLITE_EMPTY       : l_pCh = "SQLITE_EMPTY";
		case SQLITE_SCHEMA      : l_pCh = "SQLITE_SCHEMA";
		case SQLITE_TOOBIG      : l_pCh = "SQLITE_TOOBIG";
		case SQLITE_CONSTRAINT  : l_pCh = "SQLITE_CONSTRAINT";
		case SQLITE_MISMATCH    : l_pCh = "SQLITE_MISMATCH";
		case SQLITE_MISUSE      : l_pCh = "SQLITE_MISUSE";
		case SQLITE_NOLFS       : l_pCh = "SQLITE_NOLFS";
		case SQLITE_AUTH        : l_pCh = "SQLITE_AUTH";
		case SQLITE_FORMAT      : l_pCh = "SQLITE_FORMAT";
		case SQLITE_RANGE       : l_pCh = "SQLITE_RANGE";
		case SQLITE_ROW         : l_pCh = "SQLITE_ROW";
		case SQLITE_DONE        : l_pCh = "SQLITE_DONE";
		case CPPSQLITE_ERROR    : l_pCh = "CPPSQLITE_ERROR";
		default: l_pCh = "UNKNOWN_ERROR";
	}
	return l_pCh ; 
}

CppSQLite3Exception::~CppSQLite3Exception()
{
	if (mpszErrMess)
	{
		sqlite3_free(mpszErrMess);
		mpszErrMess = 0;
	}
}


////////////////////////////////////////////////////////////////////////////////

CppSQLite3Buffer::CppSQLite3Buffer()
{
	mpBuf = NULL;
}


CppSQLite3Buffer::~CppSQLite3Buffer()
{
	clear();
}


void CppSQLite3Buffer::clear()
{
	if (mpBuf)
	{
		sqlite3_free(mpBuf);
		mpBuf = NULL;
	}

}

const char* CppSQLite3Buffer::format(const char* szFormat, ...)
{
	clear();
	va_list va;
	va_start(va, szFormat);
	//[변환-7][N] 포인터 Cast의 결과로 인한 이전 포인터의 CONST 특성의 상실을 유의해야 한다
	//mpBuf = sqlite3_vmprintf(szFormat, va);
	mpBuf = sqlite3_vmprintf(reinterpret_cast<const char*>(szFormat), va);
	va_end(va);
	return mpBuf;
}

////////////////////////////////////////////////////////////////////////////////

CppSQLite3Binary::CppSQLite3Binary() :						
						mnBinaryLen(0),
						mnBufferLen(0),
						mnEncodedLen(0),
						mbEncoded(false)
{
	mpBuf = NULL;
}


CppSQLite3Binary::~CppSQLite3Binary()
{
	clear();
}


void CppSQLite3Binary::setBinary(const unsigned char* pBuf, int nLen)
{
	if (pBuf != NULL && nLen > 0)
	{
		mpBuf = allocBuffer(nLen);
		if(mpBuf != NULL)
			memcpy(mpBuf, pBuf, nLen);
	}
}

/*@QAC_xxx_151001_[스타일-10][N] [스타일-8][N] 하나의 함수는 하나의 exit point를 가져야 한다. */
//void CppSQLite3Binary::setEncoded(const unsigned char* pBuf)
//{
//	if(pBuf == NULL)		// NULL Binary exception
//		return;
//	clear();
//
//	mnEncodedLen = strlen((const char*)pBuf);
//	mnBufferLen = mnEncodedLen + 1; // Allow for NULL terminator
//
//	mpBuf = (unsigned char*)malloc(mnBufferLen);	
//	if (!mpBuf)
//	{
//		throw CppSQLite3Exception(CPPSQLITE_ERROR,
//			"Cannot allocate memory",
//			DONT_DELETE_MSG);
//	}
//	memset(mpBuf, 0x00, mnBufferLen);
//	memcpy(mpBuf, pBuf, mnBufferLen);
//	mbEncoded = true;
//}
void CppSQLite3Binary::setEncoded(const unsigned char* pBuf)
{
	if(pBuf != NULL)		// NULL Binary exception
	{
		clear();

		mnEncodedLen = strlen((const char*)pBuf);
		mnBufferLen = mnEncodedLen + 1; // Allow for NULL terminator

		mpBuf = (unsigned char*)malloc(mnBufferLen);	
		if (!mpBuf)
		{
			throw CppSQLite3Exception(CPPSQLITE_ERROR,
				"Cannot allocate memory",
				DONT_DELETE_MSG);
		}
		memset(mpBuf, 0x00, mnBufferLen);
		memcpy(mpBuf, pBuf, mnBufferLen);
		mbEncoded = true;
	}
	
}


const unsigned char* CppSQLite3Binary::getEncoded()
{
	if (!mbEncoded )
	{
		if (  mnBinaryLen <= UINT_MAX ) // Integer Overflow of Allocation Size
		{
			if (mpBuf != NULL)
			{
				unsigned char* ptmp = (unsigned char*)malloc(mnBinaryLen);
				if (ptmp != NULL)
				{
					memcpy(ptmp, mpBuf, mnBinaryLen);
					mnEncodedLen = sqlite3_encode_binary(ptmp, mnBinaryLen, mpBuf);
					mbEncoded = true;
				}
				
				if (ptmp != NULL)
				{
					free(ptmp);
					ptmp = NULL;
				}
			}			
		}
	}

	return mpBuf;
}


const unsigned char* CppSQLite3Binary::getBinary()
{
	if (mbEncoded)
	{
		// in/out buffers can be the same

		mnBinaryLen = sqlite3_decode_binary(mpBuf, mpBuf) ;

		if (mnBinaryLen == 0)
		{
			throw CppSQLite3Exception(CPPSQLITE_ERROR,
									"Cannot decode binary",
									DONT_DELETE_MSG);
		}

		mbEncoded = false;
	}

	return mpBuf;
}


UINT CppSQLite3Binary::getBinaryLength()
{
	getBinary();
	return mnBinaryLen;
}


unsigned char* CppSQLite3Binary::allocBuffer(UINT nLen)
{
	clear();

	// Allow extra space for encoded binary as per comments in
	// SQLite encode.c See bottom of this file for implementation
	// of SQLite functions use 3 instead of 2 just to be sure ;-)
	mnBinaryLen = nLen;
	mnBufferLen = 3 + (257*nLen)/254;

	mpBuf = (unsigned char*)malloc(mnBufferLen);

	if (!mpBuf)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								"Cannot allocate memory",
								DONT_DELETE_MSG);
	}
	memset(mpBuf, 0x00, mnBufferLen);
	mbEncoded = false;

	return mpBuf;
}


void CppSQLite3Binary::clear()
{
	if (mpBuf != NULL)
	{
		mnBinaryLen = 0;
		mnBufferLen = 0;
		free(mpBuf); //free(mpBuf);
		mpBuf = NULL;
	}
}


////////////////////////////////////////////////////////////////////////////////

CppSQLite3Query::CppSQLite3Query()
{
	mpVM = 0;
	mbEof = true;
	mnCols = 0;
	mbOwnVM = false;
}


CppSQLite3Query::CppSQLite3Query(const CppSQLite3Query& rQuery)
{
	mpVM = rQuery.mpVM;
	// Only one object can own the VM
	const_cast<CppSQLite3Query&>(rQuery).mpVM = 0;
	mbEof = rQuery.mbEof;
	mnCols = rQuery.mnCols;
	mbOwnVM = rQuery.mbOwnVM;
}


CppSQLite3Query::CppSQLite3Query(sqlite3* pDB,
							sqlite3_stmt* pVM,
							bool bEof,
							bool bOwnVM/*=true*/)
{
	mpDB = pDB;
	mpVM = pVM;
	mbEof = bEof;
	mnCols = sqlite3_column_count(mpVM);
	mbOwnVM = bOwnVM;
}


CppSQLite3Query::~CppSQLite3Query()
{
	try
	{
		finalize();
	}
	catch (...)
	{
	}
}


CppSQLite3Query& CppSQLite3Query::operator=(const CppSQLite3Query& rQuery)
{
	try
	{
		finalize();
	}
	catch (...)
	{
	}
	mpVM = rQuery.mpVM;
	// Only one object can own the VM
	const_cast<CppSQLite3Query&>(rQuery).mpVM = 0;
	mbEof = rQuery.mbEof;
	mnCols = rQuery.mnCols;
	mbOwnVM = rQuery.mbOwnVM;
	return *this;
}


int CppSQLite3Query::numFields()
{
	checkVM();
	return mnCols;
}


const char* CppSQLite3Query::fieldValue(int nField)
{
	checkVM();

	if (nField < 0 || nField > mnCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								"Invalid field index requested",
								DONT_DELETE_MSG);
	}

	return (const char*)sqlite3_column_text(mpVM, nField);
}


const char* CppSQLite3Query::fieldValue(const char* szField)
{
	int nField = fieldIndex(szField);
	return (const char*)sqlite3_column_text(mpVM, nField);
}


/*@QAC_xxx_151001_[스타일-10][N] [스타일-8][N] 하나의 함수는 하나의 exit point를 가져야 한다. */
//int CppSQLite3Query::getIntField(int nField, int nNullValue/*=0*/)
//{
//
//	if (fieldDataType(nField) == SQLITE_NULL)
//	{
//		return nNullValue;
//	}
//	else
//	{
//		return sqlite3_column_int(mpVM, nField);
//	}
//}

int CppSQLite3Query::getIntField(int nField, int nNullValue)
{
	return (fieldDataType(nField) == SQLITE_NULL) ? nNullValue :  sqlite3_column_int(mpVM, nField) ;
}


int CppSQLite3Query::getIntField(const char* szField, int nNullValue)
{
	int nField = fieldIndex(szField);
	return getIntField(nField, nNullValue);
}


/*@QAC_xxx_151001_[스타일-10][N] [스타일-8][N] 하나의 함수는 하나의 exit point를 가져야 한다. */
//sqlite_int64 CppSQLite3Query::getInt64Field(int nField, sqlite_int64 nNullValue/*=0*/)
//{
//	if (fieldDataType(nField) == SQLITE_NULL)
//	{
//		return nNullValue;
//	}
//	else
//	{
//		return sqlite3_column_int64(mpVM, nField);
//	}
//}

sqlite_int64 CppSQLite3Query::getInt64Field(int nField, sqlite_int64 nNullValue/*=0*/)
{
	return (fieldDataType(nField) == SQLITE_NULL) ? nNullValue :  sqlite3_column_int64(mpVM, nField) ;
}


sqlite_int64 CppSQLite3Query::getInt64Field(const char* szField, sqlite_int64 nNullValue/*=0*/)
{
	int nField = fieldIndex(szField);
	return getInt64Field(nField, nNullValue);
}


/*@QAC_xxx_151001_[스타일-10][N] [스타일-8][N] 하나의 함수는 하나의 exit point를 가져야 한다. */
//double CppSQLite3Query::getFloatField(int nField, double fNullValue/*=0.0*/)
//{
//	if (fieldDataType(nField) == SQLITE_NULL)
//	{
//		return fNullValue;
//	}
//	else
//	{
//		return sqlite3_column_double(mpVM, nField);
//	}
//}
double CppSQLite3Query::getFloatField(int nField, double fNullValue/*=0.0*/)
{
	return  (fieldDataType(nField) == SQLITE_NULL) ? fNullValue : sqlite3_column_double(mpVM, nField) ;
}


double CppSQLite3Query::getFloatField(const char* szField, double fNullValue/*=0.0*/)
{
	int nField = fieldIndex(szField);
	return getFloatField(nField, fNullValue);
}


/*@QAC_xxx_151001_[스타일-10][N] [스타일-8][N] 하나의 함수는 하나의 exit point를 가져야 한다. */
//const char* CppSQLite3Query::getStringField(int nField, const char* szNullValue/*=""*/)
//{
//	if (fieldDataType(nField) == SQLITE_NULL)
//	{
//		return szNullValue;
//	}
//	else
//	{
//		return (const char*)sqlite3_column_text(mpVM, nField);
//	}
//}
const char* CppSQLite3Query::getStringField(int nField, const char* szNullValue/*=""*/)
{
	return  (fieldDataType(nField) == SQLITE_NULL) ? szNullValue :  (const char*)sqlite3_column_text(mpVM, nField) ;
}


const char* CppSQLite3Query::getStringField(const char* szField, const char* szNullValue/*=""*/)
{
	int nField = fieldIndex(szField);
	return getStringField(nField, szNullValue);
}


const unsigned char* CppSQLite3Query::getBlobField(int nField, int& nLen)
{
	checkVM();

	if (nField < 0 || nField > mnCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								"Invalid field index requested",
								DONT_DELETE_MSG);
	}

	nLen = sqlite3_column_bytes(mpVM, nField);
	return (const unsigned char*)sqlite3_column_blob(mpVM, nField);
}


const unsigned char* CppSQLite3Query::getBlobField(const char* szField, int& nLen)
{
	int nField = fieldIndex(szField);
	return getBlobField(nField, nLen);
}


bool CppSQLite3Query::fieldIsNull(int nField)
{
	return (fieldDataType(nField) == SQLITE_NULL);
}


bool CppSQLite3Query::fieldIsNull(const char* szField)
{
	int nField = fieldIndex(szField);
	return (fieldDataType(nField) == SQLITE_NULL);
}


int CppSQLite3Query::fieldIndex(const char* szField)
{
	checkVM();

	if (szField)
	{
		for (int nField = 0; nField < mnCols; nField++)
		{
			const char* szTemp = sqlite3_column_name(mpVM, nField);

			if (strcmp(szField, szTemp) == 0)
			{
				return nField;
			}
		}
	}

	throw CppSQLite3Exception(CPPSQLITE_ERROR,
							"Invalid field name requested",
							DONT_DELETE_MSG);
}


const char* CppSQLite3Query::fieldName(int nCol)
{
	checkVM();

	if (nCol < 0 || nCol > mnCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								"Invalid field index requested",
								DONT_DELETE_MSG);
	}

	return sqlite3_column_name(mpVM, nCol);
}


const char* CppSQLite3Query::fieldDeclType(int nCol)
{
	checkVM();

	if (nCol < 0 || nCol > mnCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								"Invalid field index requested",
								DONT_DELETE_MSG);
	}

	return sqlite3_column_decltype(mpVM, nCol);
}


int CppSQLite3Query::fieldDataType(int nCol)
{
	checkVM();

	if (nCol < 0 || nCol > mnCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								"Invalid field index requested",
								DONT_DELETE_MSG);
	}

	return sqlite3_column_type(mpVM, nCol);
}


bool CppSQLite3Query::eof()
{
	checkVM();
	return mbEof;
}


void CppSQLite3Query::nextRow()
{
	checkVM();

	int nRet = sqlite3_step(mpVM);

	if (nRet == SQLITE_DONE)
	{
		// no rows
		mbEof = true;
	}
	else if (nRet == SQLITE_ROW)
	{
		// more rows, nothing to do
	}
	else
	{
		nRet = sqlite3_finalize(mpVM);
		mpVM = 0;
		const char* szError = sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet,
								_strdup(szError),
								DONT_DELETE_MSG);
	}
}


void CppSQLite3Query::finalize()
{
	if (mpVM && mbOwnVM)
	{
		int nRet = sqlite3_finalize(mpVM);
		mpVM = 0;
		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(mpDB);
			throw CppSQLite3Exception(nRet, _strdup(szError), DONT_DELETE_MSG);
		}
	}
}


void CppSQLite3Query::checkVM()
{
	if (mpVM == 0)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								"Null Virtual Machine pointer",
								DONT_DELETE_MSG);
	}
}


////////////////////////////////////////////////////////////////////////////////

CppSQLite3Table::CppSQLite3Table()
{
	mpaszResults = 0;
	mnRows = 0;
	mnCols = 0;
	mnCurrentRow = 0;
}


CppSQLite3Table::CppSQLite3Table(const CppSQLite3Table& rTable)
{
	mpaszResults = rTable.mpaszResults;
	// Only one object can own the results
	const_cast<CppSQLite3Table&>(rTable).mpaszResults = 0;
	mnRows = rTable.mnRows;
	mnCols = rTable.mnCols;
	mnCurrentRow = rTable.mnCurrentRow;
}


CppSQLite3Table::CppSQLite3Table(char** paszResults, int nRows, int nCols)
{
	mpaszResults = paszResults;
	mnRows = nRows;
	mnCols = nCols;
	mnCurrentRow = 0;
}


CppSQLite3Table::~CppSQLite3Table()
{
	try
	{
		finalize();
	}
	catch (...)
	{
	}
}


CppSQLite3Table& CppSQLite3Table::operator=(const CppSQLite3Table& rTable)
{
	try
	{
		finalize();
	}
	catch (...)
	{
	}
	mpaszResults = rTable.mpaszResults;
	// Only one object can own the results
	const_cast<CppSQLite3Table&>(rTable).mpaszResults = 0;
	mnRows = rTable.mnRows;
	mnCols = rTable.mnCols;
	mnCurrentRow = rTable.mnCurrentRow;
	return *this;
}


void CppSQLite3Table::finalize()
{
	if (mpaszResults)
	{
		sqlite3_free_table(mpaszResults);
		mpaszResults = 0;
	}
}


int CppSQLite3Table::numFields()
{
	checkResults();
	return mnCols;
}


int CppSQLite3Table::numRows()
{
	checkResults();
	return mnRows;
}


const char* CppSQLite3Table::fieldValue(int nField)
{
	checkResults();

	if (nField < 0 || nField > mnCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								"Invalid field index requested",
								DONT_DELETE_MSG);
	}

	int nIndex = (mnCurrentRow*mnCols) + mnCols + nField;
	return mpaszResults[nIndex];
}


const char* CppSQLite3Table::fieldValue(const char* szField)
{
	checkResults();

	if (szField)
	{
		for (int nField = 0; nField < mnCols; nField++)
		{
			if (strcmp(szField, mpaszResults[nField]) == 0)
			{
				int nIndex = (mnCurrentRow*mnCols) + mnCols + nField;
				return mpaszResults[nIndex];
			}
		}
	}

	throw CppSQLite3Exception(CPPSQLITE_ERROR,
							"Invalid field name requested",
							DONT_DELETE_MSG);
}

int CppSQLite3Table::getIntField(int nField, int nNullValue/*=0*/)
{
	return fieldIsNull(nField) ? nNullValue :  atoi(fieldValue(nField)) ;  
}

double CppSQLite3Table::getFloatField(int nField, double fNullValue/*=0.0*/)
{
	return fieldIsNull(nField) ? fNullValue :   atof(fieldValue(nField)) ;
}

double CppSQLite3Table::getFloatField(const char* szField, double fNullValue/*=0.0*/)
{	
	return fieldIsNull(szField) ? fNullValue :  atof(fieldValue(szField)) ;
}

const char* CppSQLite3Table::getStringField(int nField, const char* szNullValue/*=""*/)
{
	return fieldIsNull(nField) ? szNullValue :  fieldValue(nField) ;
}

const char* CppSQLite3Table::getStringField(const char* szField, const char* szNullValue/*=""*/)
{
	return fieldIsNull(szField) ? szNullValue :  fieldValue(szField) ;  
}

int CppSQLite3Table::getIntField(const char* szField, int nNullValue/*=0*/)
{
	return fieldIsNull(szField) ? nNullValue :  atoi(fieldValue(szField)) ;  
}

bool CppSQLite3Table::fieldIsNull(int nField)
{
	checkResults();
	return (fieldValue(nField) == 0);
}

bool CppSQLite3Table::fieldIsNull(const char* szField)
{
	checkResults();
	return (fieldValue(szField) == 0);
}

const char* CppSQLite3Table::fieldName(int nCol)
{
	checkResults();

	if (nCol < 0 || nCol > mnCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								"Invalid field index requested",
								DONT_DELETE_MSG);
	}

	return mpaszResults[nCol];
}

void CppSQLite3Table::setRow(int nRow)
{
	checkResults();

	if (nRow < 0 || nRow > mnRows-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								"Invalid row index requested",
								DONT_DELETE_MSG);
	}

	mnCurrentRow = nRow;
}

void CppSQLite3Table::checkResults()
{
	if (mpaszResults == 0)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								"Null Results pointer",
								DONT_DELETE_MSG);
	}
}

CppSQLite3Statement::CppSQLite3Statement()
{
	mpDB = 0;
	mpVM = 0;
}

CppSQLite3Statement::CppSQLite3Statement(const CppSQLite3Statement& rStatement)
{
	mpDB = rStatement.mpDB;
	mpVM = rStatement.mpVM;
	// Only one object can own VM
	const_cast<CppSQLite3Statement&>(rStatement).mpVM = 0;
}

CppSQLite3Statement::CppSQLite3Statement(sqlite3* pDB, sqlite3_stmt* pVM)
{
	mpDB = pDB;
	mpVM = pVM;
}

CppSQLite3Statement::~CppSQLite3Statement()
{
	try
	{
		finalize();
	}
	catch (...)
	{
	}
}

CppSQLite3Statement& CppSQLite3Statement::operator=(const CppSQLite3Statement& rStatement)
{
	mpDB = rStatement.mpDB;
	mpVM = rStatement.mpVM;
	// Only one object can own VM
	const_cast<CppSQLite3Statement&>(rStatement).mpVM = 0;
	return *this;
}

int CppSQLite3Statement::execDML()
{
	checkDB();
	checkVM();

	const char* szError=0;

	int nRet = sqlite3_step(mpVM);

	if (nRet == SQLITE_DONE)
	{
		int nRowsChanged = sqlite3_changes(mpDB);

		nRet = sqlite3_reset(mpVM);

		if (nRet != SQLITE_OK)
		{
			szError = sqlite3_errmsg(mpDB);
			throw CppSQLite3Exception(nRet, _strdup(szError), DONT_DELETE_MSG);
		}

		return nRowsChanged;
	}
	else
	{
		nRet = sqlite3_reset(mpVM);
		szError = sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet, _strdup(szError), DONT_DELETE_MSG);
	}
}

CppSQLite3Query CppSQLite3Statement::execQuery()
{
	checkDB();
	checkVM();

	int nRet = sqlite3_step(mpVM);

	if (nRet == SQLITE_DONE)
	{
		// no rows
		return CppSQLite3Query(mpDB, mpVM, true/*eof*/, false);
	}
	else if (nRet == SQLITE_ROW)
	{
		// at least 1 row
		return CppSQLite3Query(mpDB, mpVM, false/*eof*/, false);
	}
	else
	{
		nRet = sqlite3_reset(mpVM);
		const char* szError = sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet, _strdup(szError), DONT_DELETE_MSG);
	}
}

void CppSQLite3Statement::SQL_bind(int nParam, const char* szValue)
{
	checkVM();
	int nRes = sqlite3_bind_text(mpVM, nParam, szValue, -1, SQLITE_TRANSIENT);

	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,
								"Error binding string param",
								DONT_DELETE_MSG);
	}
}


void CppSQLite3Statement::SQL_bind(int nParam, const int nValue)
{
	checkVM();
	int nRes = sqlite3_bind_int(mpVM, nParam, nValue);

	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,
								"Error binding int param",
								DONT_DELETE_MSG);
	}
}


void CppSQLite3Statement::SQL_bind(int nParam, const double dValue)
{
	checkVM();
	int nRes = sqlite3_bind_double(mpVM, nParam, dValue);

	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,
								"Error binding double param",
								DONT_DELETE_MSG);
	}
}

void CppSQLite3Statement::SQL_bind(int nParam, const unsigned char* blobValue, int nLen)
{
	checkVM();
	int nRes = sqlite3_bind_blob(mpVM, nParam,
								(const void*)blobValue, nLen, SQLITE_TRANSIENT);

	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,
								"Error binding blob param",
								DONT_DELETE_MSG);
	}
}

	
void CppSQLite3Statement::bindNull(int nParam)
{
	checkVM();
	int nRes = sqlite3_bind_null(mpVM, nParam);

	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,
								"Error binding NULL param",
								DONT_DELETE_MSG);
	}
}


int CppSQLite3Statement::bindParameterIndex(const char* szParam)
{
	checkVM();

	int nParam = sqlite3_bind_parameter_index(mpVM, szParam);

	sqlite3_bind_parameter_count(mpVM);
	sqlite3_bind_parameter_name(mpVM, 1);
	sqlite3_bind_parameter_name(mpVM, 2);
	
	if (!nParam)
	{
		char buf[128];
		sprintf_s(buf, sizeof(buf), "Parameter '%s' is not valid for this statement", szParam);
		throw CppSQLite3Exception(CPPSQLITE_ERROR, buf, DONT_DELETE_MSG);
	}

	return nParam;
}


void CppSQLite3Statement::SQL_bind(const char* szParam, const char* szValue)
{
	int nParam = bindParameterIndex(szParam);
	SQL_bind(nParam, szValue);
}


void CppSQLite3Statement::SQL_bind(const char* szParam, const int nValue)
{
	int nParam = bindParameterIndex(szParam);
	SQL_bind(nParam, nValue);
}

void CppSQLite3Statement::SQL_bind(const char* szParam, const double dwValue)
{
	int nParam = bindParameterIndex(szParam);
	SQL_bind(nParam, dwValue);
}

void CppSQLite3Statement::SQL_bind(const char* szParam, const unsigned char* blobValue, int nLen)
{
	int nParam = bindParameterIndex(szParam);
	SQL_bind(nParam, blobValue, nLen);
}


void CppSQLite3Statement::bindNull(const char* szParam)
{
	int nParam = bindParameterIndex(szParam);
	bindNull(nParam);
}


void CppSQLite3Statement::reset()
{
	if (mpVM)
	{
		int nRet = sqlite3_reset(mpVM);

		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(mpDB);
			throw CppSQLite3Exception(nRet, _strdup(szError), DONT_DELETE_MSG);
		}
	}
}


void CppSQLite3Statement::finalize()
{
	if (mpVM)
	{
		int nRet = sqlite3_finalize(mpVM);
		mpVM = 0;

		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(mpDB);
			throw CppSQLite3Exception(nRet, _strdup(szError), DONT_DELETE_MSG);
		}
	}
}


void CppSQLite3Statement::checkDB()
{
	if (mpDB == 0)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								"Database not open",
								DONT_DELETE_MSG);
	}
}


void CppSQLite3Statement::checkVM()
{
	if (mpVM == 0)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								"Null Virtual Machine pointer",
								DONT_DELETE_MSG);
	}
}

CppSQLite3DB::CppSQLite3DB()
{
	mpDB = 0;
	mnBusyTimeoutMs = 60000; // 60 seconds
}


CppSQLite3DB::CppSQLite3DB(const CppSQLite3DB& db)
{
	mpDB = db.mpDB;
	mnBusyTimeoutMs = 60000; // 60 seconds
}


CppSQLite3DB::~CppSQLite3DB()
{
	try
	{
		close();
	}
	catch (...)
	{
	}
}


CppSQLite3DB& CppSQLite3DB::operator=(const CppSQLite3DB& db)
{
	mpDB = db.mpDB;
	mnBusyTimeoutMs = 60000; // 60 seconds
	return *this;
}


void CppSQLite3DB::open(const char* szFile)
{
	int nRet = sqlite3_enable_shared_cache(1);

	sqlite3_open(szFile, &mpDB);

	if (nRet != SQLITE_OK)
	{
		const char* szError = sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet, _strdup(szError), DONT_DELETE_MSG);
	}

	setBusyTimeout(mnBusyTimeoutMs);
}


void CppSQLite3DB::close()
{
	if (mpDB)
	{
		if (sqlite3_close(mpDB) == SQLITE_OK)
		{
			mpDB = 0;
		}
		else
		{
			throw CppSQLite3Exception(CPPSQLITE_ERROR,
									"Unable to close database",
									DONT_DELETE_MSG);
		}
	}
}


CppSQLite3Statement CppSQLite3DB::compileStatement(const char* szSQL)
{
	checkDB();

	sqlite3_stmt* pVM = compile(szSQL);
	return CppSQLite3Statement(mpDB, pVM);
}


bool CppSQLite3DB::tableExists(const char* szTable)
{
	char szSQL[256];
	sprintf_s(szSQL, sizeof(szSQL),
			"select count(*) from sqlite_master where type='table' and name='%s'",
			szTable);
	int nRet = execScalar(szSQL);
	return (nRet > 0);
}


int CppSQLite3DB::execDML(const char* szSQL)
{
	checkDB();

	char* szError=0;

	int nRet = sqlite3_exec(mpDB, szSQL, 0, 0, &szError);

	if (nRet == SQLITE_OK)
	{
		return sqlite3_changes(mpDB);
	}
	else
	{
		throw CppSQLite3Exception(nRet, szError);
	}
}


CppSQLite3Query CppSQLite3DB::execQuery(const char* szSQL)
{
	checkDB();

	sqlite3_stmt* pVM = compile(szSQL);

	int nRet = sqlite3_step(pVM);

	if (nRet == SQLITE_DONE)
	{
		// no rows
		return CppSQLite3Query(mpDB, pVM, true/*eof*/);
	}
	else if (nRet == SQLITE_ROW)
	{
		// at least 1 row
		return CppSQLite3Query(mpDB, pVM, false/*eof*/);
	}
	else
	{
		nRet = sqlite3_finalize(pVM);
		const char* szError= sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet, _strdup(szError), DONT_DELETE_MSG);
	}
}


int CppSQLite3DB::execScalar(const char* szSQL, int nNullValue/*=0*/)
{
	CppSQLite3Query q = execQuery(szSQL);

	if (q.eof() || q.numFields() < 1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								"Invalid scalar query",
								DONT_DELETE_MSG);
	}

	return q.getIntField(0, nNullValue);
}


CppSQLite3Table CppSQLite3DB::getTable(const char* szSQL)
{
	checkDB();

	char* szError=0;
	char** paszResults=0;
	int nRet;
	int nRows(0);
	int nCols(0);

	nRet = sqlite3_get_table(mpDB, szSQL, &paszResults, &nRows, &nCols, &szError);

	if (nRet == SQLITE_OK)
	{
		return CppSQLite3Table(paszResults, nRows, nCols);
	}
	else
	{
		throw CppSQLite3Exception(nRet, szError);
	}
}


sqlite_int64 CppSQLite3DB::lastRowId()
{
	return sqlite3_last_insert_rowid(mpDB);
}


void CppSQLite3DB::setBusyTimeout(int nMillisecs)
{
	mnBusyTimeoutMs = nMillisecs;
	sqlite3_busy_timeout(mpDB, mnBusyTimeoutMs);
}


void CppSQLite3DB::checkDB()
{
	if (!mpDB)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								"Database not open",
								DONT_DELETE_MSG);
	}
}


sqlite3_stmt* CppSQLite3DB::compile(const char* szSQL)
{
	checkDB();

	const char* szTail=0;
	sqlite3_stmt* pVM;

	int nRet = sqlite3_prepare_v2(mpDB, szSQL, -1, &pVM, &szTail);

	if (nRet != SQLITE_OK)
	{
		const char* szError = sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet,
								_strdup(szError),
								DONT_DELETE_MSG);
	}

	return pVM;
}

bool CppSQLite3DB::IsAutoCommitOn()
{
	checkDB();
	return sqlite3_get_autocommit(mpDB) ? true : false;
}

UINT sqlite3_encode_binary(const unsigned char *in, UINT n, unsigned char *out){
	int l_nRet = 0 ; 
	UINT j, e, m;
	int i;
	int cnt[256];
	
	if( n<=0 )
	{
		out[0] = 'x';
		out[1] = 0;
		j = 1;
	}
	else
	{
		memset(cnt, 0, sizeof(cnt));
		for(i=(n-1); i>=0; i--){
			cnt[in[i]]++; 
		}
		m = n;

		for(UINT k=1; k<256; k++)
		{
			UINT sum;
			if( k=='\'' ) continue;
			sum = cnt[k] + cnt[(k+1u)&0xff] + cnt[(k+'\'')&0xff];
			if( sum<m ){
				m = sum;
				e = k;
				if( m==0 ) break;
			}
		}		
		
		out[0] = e;		
		j = 1;

		for(i=0; i<n; i++){
			int c = (in[i] - e)&0xff;
			if( c==0 ){
				out[j++] = 1;
				out[j++] = 1;
			}else if( c==1 ){
				out[j++] = 1;
				out[j++] = 2;
			}else if( c=='\'' ){
				out[j++] = 1;
				out[j++] = 3;
			}else
			{				
				out[j++] = c;
			}
		}
		out[j] = 0;
		
	}
	
	return j;
}

UINT sqlite3_decode_binary(const unsigned char *in, unsigned char *out){

	UINT i, c, e;
	e = *(in++);
	i = 0;
	while( (c = *(in++))!=0 )
	{
		if( c==1 )
		{
			c = *(in++);
			if( c==1 )
			{
				c = 0;
			}
			else if( c==2 )
			{
				c = 1;
			}
			else if( c==3 )
			{
				c = '\'';
			}
			else
			{
				i = 0;
				break;
			}
		}
		out[i++] = (c + e)&0xff;
	}
	
	return i;
}