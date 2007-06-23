//
// ODBCPostgreSQLTest.h
//
// $Id: //poco/Main/Data/ODBC/testsuite/src/ODBCPostgreSQLTest.h#4 $
//
// Definition of the ODBCPostgreSQLTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef ODBCPostgreSQLTest_INCLUDED
#define ODBCPostgreSQLTest_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/SharedPtr.h"
#include "CppUnit/TestCase.h"
#include "SQLExecutor.h"


class ODBCPostgreSQLTest: public CppUnit::TestCase
	/// PostgreSQL ODBC test class
	/// Tested:
	/// 
	/// Driver		  |	DB                | OS                                        | Driver Manager     |Notes
	/// --------------+-------------------+-------------------------------------------+--------------------+--------------------------
	///	07.03.02.60	  | PostgreSQL 7.4.6  | MS Windows XP Professional x64 v.2003/SP1 | 3.526.3959.0       | BLOB fails (missing 'lo')
	/// 08.01.02.00   | PostgreSQL 8.1.5-1| MS Windows XP Professional x64 v.2003/SP1 | 3.526.3959.0       |
	/// 1:08.01.0200-2| PostgreSQL 8.1.5-1| Ubuntu 7.04 (2.6.20-15-generic #2 SMP)    | unixODBC 2.2.11.-13|
	/// 
{
public:
	ODBCPostgreSQLTest(const std::string& name);
	~ODBCPostgreSQLTest();

	void testBareboneODBC();

	void testSimpleAccess();
	void testComplexType();

	void testSimpleAccessVector();
	void testComplexTypeVector();
	void testInsertVector();
	void testInsertEmptyVector();

	void testSimpleAccessList();
	void testComplexTypeList();
	void testInsertList();
	void testInsertEmptyList();

	void testSimpleAccessDeque();
	void testComplexTypeDeque();
	void testInsertDeque();
	void testInsertEmptyDeque();

	void testInsertSingleBulk();
	void testInsertSingleBulkVec();

	void testLimit();
	void testLimitOnce();
	void testLimitPrepare();
	void testLimitZero();
	void testPrepare();

	void testSetSimple();
	void testSetComplex();
	void testSetComplexUnique();
	void testMultiSetSimple();
	void testMultiSetComplex();
	void testMapComplex();
	void testMapComplexUnique();
	void testMultiMapComplex();
	void testSelectIntoSingle();
	void testSelectIntoSingleStep();
	void testSelectIntoSingleFail();
	void testLowerLimitOk();
	void testLowerLimitFail();
	void testCombinedLimits();
	void testCombinedIllegalLimits();
	void testRange();
	void testIllegalRange();
	void testSingleSelect();
	void testEmptyDB();

	void testBLOB();
	void testBLOBStmt();

	void testDateTime();

	void testFloat();
	void testDouble();

	void testTuple();
	void testTupleVector();

	void testInternalExtraction();
	void testInternalStorageType();

	void testStoredFunction();
	void testNull();
	void testRowIterator();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	void dropObject(const std::string& type, const std::string& name);
	void recreatePersonTable();
	void recreatePersonBLOBTable();
	void recreatePersonDateTimeTable();
	void recreateStringsTable();
	void recreateIntsTable();
	void recreateFloatsTable();
	void recreateTuplesTable();
	void recreateVectorsTable();
	void recreateNullsTable(const std::string& notNull="");

	static bool init(const std::string& driver, const std::string& dsn);
	static bool canConnect(const std::string& driver, const std::string& dsn);
	
	void configurePLPgSQL();
		/// Configures PL/pgSQL in the database. A reasonable defaults
		/// for the interpreter location on WIN32 and POSIX platforms are 
		/// supplied (see installDir member variable). 
		/// If these do not work, user must determine the proper location,
		/// modify the function and recompile.
		/// Alternative is direct database configuration for PL/pgSQL usage.

	static Poco::Data::ODBC::Utility::DriverMap _drivers;
	static std::string _dbConnString;
	static Poco::SharedPtr<Poco::Data::Session> _pSession;
	static Poco::SharedPtr<SQLExecutor> _pExecutor;
	static const bool bindValues[8];
	static const std::string libDir;
		/// Varible determining the location of the library directory
		/// on the database installation system.
		/// Used to enable PLpgSQL language programmaticaly when
		/// it is not enabled.
};


#endif // ODBCPostgreSQLTest_INCLUDED
