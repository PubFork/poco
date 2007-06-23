//
// ODBCSQLServerTest.cpp
//
// $Id: //poco/Main/Data/ODBC/testsuite/src/ODBCSQLServerTest.cpp#5 $
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


#include "ODBCSQLServerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Tuple.h"
#include "Poco/DateTime.h"
#include "Poco/Exception.h"
#include "Poco/Data/Common.h"
#include "Poco/Data/BLOB.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/Data/ODBC/Connector.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/Diagnostics.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Data/ODBC/ODBCStatementImpl.h"
#include <sqltypes.h>
#include <iostream>


using namespace Poco::Data;
using Poco::Data::ODBC::Utility;
using Poco::Data::ODBC::ConnectionException;
using Poco::Data::ODBC::StatementException;
using Poco::Data::ODBC::StatementDiagnostics;
using Poco::format;
using Poco::Tuple;
using Poco::DateTime;
using Poco::NotFoundException;


const bool ODBCSQLServerTest::bindValues[8] = {true, true, true, false, false, true, false, false};
Poco::SharedPtr<Poco::Data::Session> ODBCSQLServerTest::_pSession = 0;
Poco::SharedPtr<SQLExecutor> ODBCSQLServerTest::_pExecutor = 0;
std::string ODBCSQLServerTest::_dbConnString;
Poco::Data::ODBC::Utility::DriverMap ODBCSQLServerTest::_drivers;


ODBCSQLServerTest::ODBCSQLServerTest(const std::string& name): 
	CppUnit::TestCase(name)
{

}


ODBCSQLServerTest::~ODBCSQLServerTest()
{
}


void ODBCSQLServerTest::testBareboneODBC()
{
	if (!_pSession) fail ("Test not available.");

	std::string tableCreateString = "CREATE TABLE Test "
		"(First VARCHAR(30),"
		"Second VARCHAR(30),"
		"Third VARBINARY(30),"
		"Fourth INTEGER,"
		"Fifth FLOAT,"
		"Sixth DATETIME)";

	_pExecutor->bareboneODBCTest(_dbConnString, tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_MANUAL);
	_pExecutor->bareboneODBCTest(_dbConnString, tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_BOUND);
	_pExecutor->bareboneODBCTest(_dbConnString, tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_MANUAL);
	_pExecutor->bareboneODBCTest(_dbConnString, tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_BOUND);
}


void ODBCSQLServerTest::testSimpleAccess()
{
	if (!_pSession) fail ("Test not available.");

	std::string tableName("Person");
	int count = 0;

	recreatePersonTable();

	try { *_pSession << "SELECT count(*) FROM sys.tables WHERE name = 'Person'", into(count), use(tableName), now;  }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("testSimpleAccess()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("testSimpleAccess()"); }

	assert (1 == count);

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->simpleAccess();
		i += 2;
	}
}


void ODBCSQLServerTest::testComplexType()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->complexType();
		i += 2;
	}
}


void ODBCSQLServerTest::testSimpleAccessVector()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->simpleAccessVector();
		i += 2;
	}
}


void ODBCSQLServerTest::testComplexTypeVector()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->complexTypeVector();
		i += 2;
	}	
}


void ODBCSQLServerTest::testInsertVector()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateStringsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->insertVector();
		i += 2;
	}	
}


void ODBCSQLServerTest::testInsertEmptyVector()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateStringsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->insertEmptyVector();
		i += 2;
	}	
}


void ODBCSQLServerTest::testSimpleAccessList()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->simpleAccessList();
		i += 2;
	}
}


void ODBCSQLServerTest::testComplexTypeList()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->complexTypeList();
		i += 2;
	}	
}


void ODBCSQLServerTest::testInsertList()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateStringsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->insertList();
		i += 2;
	}	
}


void ODBCSQLServerTest::testInsertEmptyList()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateStringsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->insertEmptyList();
		i += 2;
	}	
}


void ODBCSQLServerTest::testSimpleAccessDeque()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->simpleAccessDeque();
		i += 2;
	}
}


void ODBCSQLServerTest::testComplexTypeDeque()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->complexTypeDeque();
		i += 2;
	}	
}


void ODBCSQLServerTest::testInsertDeque()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateStringsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->insertDeque();
		i += 2;
	}	
}


void ODBCSQLServerTest::testInsertEmptyDeque()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateStringsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->insertEmptyDeque();
		i += 2;
	}	
}


void ODBCSQLServerTest::testInsertSingleBulk()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->insertSingleBulk();
		i += 2;
	}	
}


void ODBCSQLServerTest::testInsertSingleBulkVec()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->insertSingleBulkVec();
		i += 2;
	}	
}


void ODBCSQLServerTest::testLimit()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->limits();
		i += 2;
	}
}


void ODBCSQLServerTest::testLimitZero()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->limitZero();
		i += 2;
	}	
}


void ODBCSQLServerTest::testLimitOnce()
{
	if (!_pSession) fail ("Test not available.");

	recreateIntsTable();
	_pExecutor->limitOnce();
	
}


void ODBCSQLServerTest::testLimitPrepare()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->limitPrepare();
		i += 2;
	}
}



void ODBCSQLServerTest::testPrepare()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->prepare();
		i += 2;
	}
}


void ODBCSQLServerTest::testSetSimple()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->setSimple();
		i += 2;
	}
}


void ODBCSQLServerTest::testSetComplex()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->setComplex();
		i += 2;
	}
}


void ODBCSQLServerTest::testSetComplexUnique()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->setComplexUnique();
		i += 2;
	}
}

void ODBCSQLServerTest::testMultiSetSimple()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->multiSetSimple();
		i += 2;
	}
}


void ODBCSQLServerTest::testMultiSetComplex()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->multiSetComplex();
		i += 2;
	}	
}


void ODBCSQLServerTest::testMapComplex()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->mapComplex();
		i += 2;
	}
}


void ODBCSQLServerTest::testMapComplexUnique()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->mapComplexUnique();
		i += 2;
	}
}


void ODBCSQLServerTest::testMultiMapComplex()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->multiMapComplex();
		i += 2;
	}
}


void ODBCSQLServerTest::testSelectIntoSingle()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->selectIntoSingle();
		i += 2;
	}
}


void ODBCSQLServerTest::testSelectIntoSingleStep()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->selectIntoSingleStep();
		i += 2;
	}	
}


void ODBCSQLServerTest::testSelectIntoSingleFail()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->selectIntoSingleFail();
		i += 2;
	}	
}


void ODBCSQLServerTest::testLowerLimitOk()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->lowerLimitOk();
		i += 2;
	}	
}


void ODBCSQLServerTest::testSingleSelect()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->singleSelect();
		i += 2;
	}	
}


void ODBCSQLServerTest::testLowerLimitFail()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->lowerLimitFail();
		i += 2;
	}
}


void ODBCSQLServerTest::testCombinedLimits()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->combinedLimits();
		i += 2;
	}
}



void ODBCSQLServerTest::testRange()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->ranges();
		i += 2;
	}
}


void ODBCSQLServerTest::testCombinedIllegalLimits()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->combinedIllegalLimits();
		i += 2;
	}
}



void ODBCSQLServerTest::testIllegalRange()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->illegalRange();
		i += 2;
	}
}


void ODBCSQLServerTest::testEmptyDB()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->emptyDB();
		i += 2;
	}
}


void ODBCSQLServerTest::testBLOB()
{
	if (!_pSession) fail ("Test not available.");
	
	const std::size_t maxFldSize = 250000;
	_pSession->setProperty("maxFieldSize", Poco::Any(maxFldSize-1));
	recreatePersonBLOBTable();

	try
	{
		_pExecutor->blob(maxFldSize);
		fail ("must fail");
	}
	catch (DataException&) 
	{
		_pSession->setProperty("maxFieldSize", Poco::Any(maxFldSize));
	}

	for (int i = 0; i < 8;)
	{
		recreatePersonBLOBTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->blob(maxFldSize);
		i += 2;
	}

	recreatePersonBLOBTable();
	try
	{
		_pExecutor->blob(maxFldSize+1);
		fail ("must fail");
	}
	catch (DataException&) { }
}


void ODBCSQLServerTest::testBLOBStmt()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonBLOBTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->blobStmt();
		i += 2;
	}
}


void ODBCSQLServerTest::testDateTime()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonDateTimeTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->dateTime();
		i += 2;
	}
}


void ODBCSQLServerTest::testFloat()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateFloatsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->floats();
		i += 2;
	}
}


void ODBCSQLServerTest::testDouble()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateFloatsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->doubles();
		i += 2;
	}
}


void ODBCSQLServerTest::testTuple()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateTuplesTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->tuples();
		i += 2;
	}
}


void ODBCSQLServerTest::testTupleVector()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateTuplesTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->tupleVector();
		i += 2;
	}
}


void ODBCSQLServerTest::testInternalExtraction()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateVectorsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->internalExtraction();
		i += 2;
	}
}


void ODBCSQLServerTest::testInternalStorageType()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateVectorsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->internalStorageType();
		i += 2;
	}
}


void ODBCSQLServerTest::testNull()
{
	if (!_pSession) fail ("Test not available.");

	// test for NOT NULL violation exception
	for (int i = 0; i < 8;)
	{
		recreateNullsTable("NOT NULL");
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->notNulls("23000");
		i += 2;
	}

	// test for null insertion
	for (int i = 0; i < 8;)
	{
		recreateNullsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->nulls();
		i += 2;
	}
}


void ODBCSQLServerTest::testStoredProcedure()
{
	for (int k = 0; k < 8;)
	{
		_pSession->setFeature("autoBind", bindValues[k]);
		_pSession->setFeature("autoExtract", bindValues[k+1]);

		dropObject("PROCEDURE", "storedProcedure");

		*_pSession << "CREATE PROCEDURE storedProcedure(@outParam int OUTPUT) AS "
			"BEGIN "
			"SET @outParam = -1; "
			"END;"
		, now;
		
		int i = 0;
		*_pSession << "{call storedProcedure(?)}", out(i), now;
		assert(-1 == i);
		dropObject("PROCEDURE", "storedProcedure");

		*_pSession << "CREATE PROCEDURE storedProcedure(@inParam int, @outParam int OUTPUT) AS "
			"BEGIN "
			"SET @outParam = @inParam*@inParam; "
			"END;"
		, now;

		i = 2;
		int j = 0;
		*_pSession << "{call storedProcedure(?, ?)}", in(i), out(j), now;
		assert(4 == j);
		dropObject("PROCEDURE", "storedProcedure");

		*_pSession << "CREATE PROCEDURE storedProcedure(@ioParam int OUTPUT) AS "
			"BEGIN "
			"SET @ioParam = @ioParam*@ioParam; "
			"END;"
		, now;

		i = 2;
		*_pSession << "{call storedProcedure(?)}", io(i), now;
		assert(4 == i);
		dropObject("PROCEDURE", "storedProcedure");

		*_pSession << "CREATE PROCEDURE storedProcedure(@ioParam DATETIME OUTPUT) AS "
			"BEGIN "
			" SET @ioParam = @ioParam + 1; "
			"END;" , now;

		DateTime dt(1965, 6, 18, 5, 35, 1);
		*_pSession << "{call storedProcedure(?)}", io(dt), now;
		assert(19 == dt.day());
		dropObject("PROCEDURE", "storedProcedure");

		k += 2;
	}
/*TODO - currently fails with following error:

[Microsoft][ODBC SQL Server Driver][SQL Server]Invalid parameter 
2 (''):  Data type 0x23 is a deprecated large object, or LOB, but is marked as output parameter.  
Deprecated types are not supported as output parameters.  Use current large object types instead.

	_pSession->setFeature("autoBind", true);
	*_pSession << "CREATE PROCEDURE storedProcedure(@inParam VARCHAR(MAX), @outParam VARCHAR(MAX) OUTPUT) AS "
		"BEGIN "
		"SET @outParam = @inParam; "
		"END;"
	, now;

	std::string inParam = "123";
	std::string outParam;
	try{
	*_pSession << "{call storedProcedure(?, ?)}", in(inParam), out(outParam), now;
	}catch(StatementException& ex){std::cout << ex.toString();}
	assert(outParam == inParam);
	dropObject("PROCEDURE", "storedProcedure");
	*/
}


void ODBCSQLServerTest::testStoredFunction()
{
	for (int k = 0; k < 8;)
	{
		_pSession->setFeature("autoBind", bindValues[k]);
		_pSession->setFeature("autoExtract", bindValues[k+1]);

		dropObject("PROCEDURE", "storedFunction");
		*_pSession << "CREATE PROCEDURE storedFunction AS "
			"BEGIN "
			"DECLARE @retVal int;"
			"SET @retVal = -1;"
			"RETURN @retVal;"
			"END;"
		, now;

		int i = 0;
		*_pSession << "{? = call storedFunction}", out(i), now;
		assert(-1 == i);
		dropObject("PROCEDURE", "storedFunction");


		*_pSession << "CREATE PROCEDURE storedFunction(@inParam int) AS "
			"BEGIN "
			"RETURN @inParam*@inParam;"
			"END;"
		, now;

		i = 2;
		int result = 0;
		*_pSession << "{? = call storedFunction(?)}", out(result), in(i), now;
		assert(4 == result);
		dropObject("PROCEDURE", "storedFunction");


		*_pSession << "CREATE PROCEDURE storedFunction(@inParam int, @outParam int OUTPUT) AS "
			"BEGIN "
			"SET @outParam = @inParam*@inParam;"
			"RETURN @outParam;"
			"END"
		, now;

		i = 2;
		int j = 0;
		result = 0;
		*_pSession << "{? = call storedFunction(?, ?)}", out(result), in(i), out(j), now;
		assert(4 == j);
		assert(j == result);
		dropObject("PROCEDURE", "storedFunction");


		*_pSession << "CREATE PROCEDURE storedFunction(@param1 int OUTPUT,@param2 int OUTPUT) AS "
			"BEGIN "
			"DECLARE @temp int; "
			"SET @temp = @param1;"
			"SET @param1 = @param2;"
			"SET @param2 = @temp;"
			"RETURN @param1 + @param2; "
			"END"
		, now;

		i = 1;
		j = 2;
		result = 0;
		*_pSession << "{? = call storedFunction(?, ?)}", out(result), io(i), io(j), now;
		assert(1 == j);
		assert(2 == i);
		assert(3 == result); 

		Tuple<int, int> params(1, 2);
		assert(1 == params.get<0>());
		assert(2 == params.get<1>());
		result = 0;
		*_pSession << "{? = call storedFunction(?, ?)}", out(result), io(params), now;
		assert(1 == params.get<1>());
		assert(2 == params.get<0>());
		assert(3 == result); 

		dropObject("PROCEDURE", "storedFunction");

		k += 2;
	}
}


void ODBCSQLServerTest::testRowIterator()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateVectorsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->rowIterator();
		i += 2;
	}
}


void ODBCSQLServerTest::dropObject(const std::string& type, const std::string& name)
{
	try
	{
		*_pSession << format("DROP %s %s", type, name), now;
	}
	catch (StatementException& ex)
	{
		bool ignoreError = false;
		const StatementDiagnostics::FieldVec& flds = ex.diagnostics().fields();
		StatementDiagnostics::Iterator it = flds.begin();
		for (; it != flds.end(); ++it)
		{
			if (3701 == it->_nativeError)//(table does not exist)
			{
				ignoreError = true;
				break;
			}
		}

		if (!ignoreError) throw;
	}
}


void ODBCSQLServerTest::recreatePersonTable()
{
	dropObject("TABLE", "Person");
	try { *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Age INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonTable()"); }
}


void ODBCSQLServerTest::recreatePersonBLOBTable()
{
	dropObject("TABLE", "Person");
	try { *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Image VARBINARY(MAX))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonBLOBTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonBLOBTable()"); }
}


void ODBCSQLServerTest::recreatePersonDateTimeTable()
{
	dropObject("TABLE", "Person");
	try { *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Born DATETIME)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonDateTimeTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonDateTimeTable()"); }
}


void ODBCSQLServerTest::recreateIntsTable()
{
	dropObject("TABLE", "Strings");
	try { *_pSession << "CREATE TABLE Strings (str INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateIntsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateIntsTable()"); }
}


void ODBCSQLServerTest::recreateStringsTable()
{
	dropObject("TABLE", "Strings");
	try { *_pSession << "CREATE TABLE Strings (str VARCHAR(30))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateStringsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateStringsTable()"); }
}


void ODBCSQLServerTest::recreateFloatsTable()
{
	dropObject("TABLE", "Strings");
	try { *_pSession << "CREATE TABLE Strings (str FLOAT)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateFloatsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateFloatsTable()"); }
}


void ODBCSQLServerTest::recreateTuplesTable()
{
	dropObject("TABLE", "Tuples");
	try { *_pSession << "CREATE TABLE Tuples "
		"(int0 INTEGER, int1 INTEGER, int2 INTEGER, int3 INTEGER, int4 INTEGER, int5 INTEGER, int6 INTEGER, "
		"int7 INTEGER, int8 INTEGER, int9 INTEGER, int10 INTEGER, int11 INTEGER, int12 INTEGER, int13 INTEGER,"
		"int14 INTEGER, int15 INTEGER, int16 INTEGER, int17 INTEGER, int18 INTEGER, int19 INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateTuplesTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateTuplesTable()"); }
}


void ODBCSQLServerTest::recreateVectorTable()
{
	dropObject("TABLE", "Vector");
	try { *_pSession << "CREATE TABLE Vector (i0 INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateVectorTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateVectorTable()"); }
}


void ODBCSQLServerTest::recreateVectorsTable()
{
	dropObject("TABLE", "Vectors");
	try { *_pSession << "CREATE TABLE Vectors (int0 INTEGER, flt0 FLOAT, str0 VARCHAR(30))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateVectorsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateVectorsTable()"); }
}


void ODBCSQLServerTest::recreateNullsTable(const std::string& notNull)
{
	dropObject("TABLE", "NullTest");
	try { *_pSession << format("CREATE TABLE NullTest (i INTEGER %s, r FLOAT %s, v VARCHAR(30) %s)",
		notNull,
		notNull,
		notNull), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateNullsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateNullsTable()"); }
}


bool ODBCSQLServerTest::canConnect(const std::string& driver, const std::string& dsn)
{
	Utility::DriverMap::iterator itDrv = _drivers.begin();
	for (; itDrv != _drivers.end(); ++itDrv)
	{
		if (((itDrv->first).find(driver) != std::string::npos))
		{
			std::cout << "Driver found: " << itDrv->first 
				<< " (" << itDrv->second << ')' << std::endl;
			break;
		}
	}

	if (_drivers.end() == itDrv) 
	{
		std::cout << driver << " driver NOT found, tests not available." << std::endl;
		return false;
	}

	Utility::DSNMap dataSources;
	Utility::dataSources(dataSources);
	Utility::DSNMap::iterator itDSN = dataSources.begin();
	for (; itDSN != dataSources.end(); ++itDSN)
	{
		if (itDSN->first == dsn && itDSN->second == driver)
		{
			std::cout << "DSN found: " << itDSN->first 
				<< " (" << itDSN->second << ')' << std::endl;
			format(_dbConnString, "DSN=%s", dsn);
			return true;
		}
	}

	// DSN not found, try connect without it
	format(_dbConnString, "DRIVER=%s;"
		"UID=test;"
		"PWD=test;"
		"DATABASE=test;"
		"SERVER=localhost;"
		"PORT=1433;", driver);

	return true;
}


void ODBCSQLServerTest::setUp()
{
}


void ODBCSQLServerTest::tearDown()
{
	dropObject("TABLE", "Person");
	dropObject("TABLE", "Strings");
	dropObject("TABLE", "Tuples");
}


bool ODBCSQLServerTest::init(const std::string& driver, const std::string& dsn)
{
	Utility::drivers(_drivers);
	if (!canConnect(driver, dsn)) return false;
	
	ODBC::Connector::registerConnector();
	try
	{
		_pSession = new Session(ODBC::Connector::KEY, _dbConnString);
	}catch (ConnectionException& ex)
	{
		std::cout << ex.toString() << std::endl;
		return false;
	}

	if (_pSession && _pSession->isConnected()) 
		std::cout << "*** Connected to [" << driver << "] test database." << std::endl;
	
	_pExecutor = new SQLExecutor(driver + " SQL Executor", _pSession);

	return true;
}


CppUnit::Test* ODBCSQLServerTest::suite()
{
#ifdef POCO_OS_FAMILY_WINDOWS
	if (init("SQL Server", "PocoDataSQLServerTest"))
	//if (init("SQL Native Client", "PocoDataSQLServerTest"))
#else
	if (init("FreeTDS", "PocoDataSQLServerTest"))
#endif
	{
		CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ODBCSQLServerTest");

		CppUnit_addTest(pSuite, ODBCSQLServerTest, testBareboneODBC);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSimpleAccess);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testComplexType);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSimpleAccessVector);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testComplexTypeVector);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInsertVector);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInsertEmptyVector);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSimpleAccessList);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testComplexTypeList);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInsertList);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInsertEmptyList);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSimpleAccessDeque);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testComplexTypeDeque);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInsertDeque);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInsertEmptyDeque);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInsertSingleBulk);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInsertSingleBulkVec);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testLimit);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testLimitOnce);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testLimitPrepare);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testLimitZero);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testPrepare);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSetSimple);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSetComplex);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSetComplexUnique);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testMultiSetSimple);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testMultiSetComplex);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testMapComplex);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testMapComplexUnique);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testMultiMapComplex);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSelectIntoSingle);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSelectIntoSingleStep);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSelectIntoSingleFail);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testLowerLimitOk);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testLowerLimitFail);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testCombinedLimits);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testCombinedIllegalLimits);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testRange);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testIllegalRange);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testSingleSelect);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testEmptyDB);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testBLOB);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testBLOBStmt);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testDateTime);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testFloat);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testDouble);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testTuple);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testTupleVector);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testStoredProcedure);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testStoredFunction);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInternalExtraction);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testInternalStorageType);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testNull);
		CppUnit_addTest(pSuite, ODBCSQLServerTest, testRowIterator);

		return pSuite;
	}

	return 0;
}
