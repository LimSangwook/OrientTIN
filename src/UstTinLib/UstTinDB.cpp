#include <stdio.h>
#include <liborient/liborient.h>
#include "UstTinDB.h"
#include "UstTinMaker.h"



void orient_debug(const char *msg) {
	fprintf(stderr, "program: %s", msg);
}

CUstTinDB::CUstTinDB() :
		m_DBPort("2424"), m_OrientDB(NULL), m_OrientCon(NULL)
{
}

CUstTinDB::~CUstTinDB()
{
	if (m_OrientDB && m_OrientCon) {
		o_close(m_OrientDB, m_OrientCon);
		m_OrientCon = NULL;
	}
	if (m_OrientDB) {
		o_free(m_OrientDB);
		m_OrientDB = NULL;
	}
}

CUstTinDB::ErrCode CUstTinDB::InitFromOrientDB(
		String url, String dbName, String id, String pw, String vertexClassName, String edgeClassName)
{
	m_Url 					= url;
	m_DbName 				= dbName;
	m_Id 					= id;
	m_Pw 					= pw;
	m_VertexClassName 	= vertexClassName;
	m_EdgeClassName 		= edgeClassName;

	// Server 접속 확인
	if (!_ConnectDBServer()) {
		return ERR_DB_URL;
	}

	// Server에 DBName 의 DB가 전재하는지 확인.
	if (!_CheckDBName()) {
		return ERR_DB_NAME;
	}

	// DB에 VertexClass가 존재하는지 확
	if (!_CheckVertexClass()) {
		return ERR_VERTEX_CLASS_NOT_FOUND;
	}

	return RET_OK;
}

bool CUstTinDB::_ConnectDBServer()
{
	int rc;
	char str[O_ERR_MAXLEN];
	struct timeval tv;

	m_OrientDB = o_new();

	o_debug_setlevel(m_OrientDB, ORIENT_DEBUG);
	o_debug_sethook(m_OrientDB, (void*)&orient_debug);

	rc = o_prepare_connection(m_OrientDB, ORIENT_PROTO_BINARY, m_Url.c_str(), m_DBPort.c_str());
	if (rc != O_OK) {
		o_strerr(rc, str, O_ERR_MAXLEN);
		fprintf(stderr, "Error preparing connections (%i): %s\n", rc, str);
		o_free(m_OrientDB);
		m_OrientDB = NULL;
		return false;
	}

	// this user will be used to execute administrative commands to orientdb
	rc = o_prepare_user(m_OrientDB, ORIENT_ADMIN, m_Id.c_str(), m_Pw.c_str());
	if (rc != O_OK) {
		o_free(m_OrientDB);
		m_OrientDB = NULL;
		return false;
	}
	// this user will be used on database open to execute commands to the db
	rc = o_prepare_user(m_OrientDB, ORIENT_USER, m_Id.c_str(), m_Pw.c_str());
	if (rc != O_OK) {
		o_free(m_OrientDB);
		m_OrientDB = NULL;
		return false;
	}
	// set a connection timeout of 5 seconds
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	// try to create the connection with orientdb. NULL if a problem is encountered
	m_OrientCon = o_connect(m_OrientDB, &tv, 0);
	if (!m_OrientCon) {
		o_free(m_OrientDB);
		m_OrientDB = NULL;
		return false;
	}

	rc = o_bin_connect(m_OrientDB, m_OrientCon, &tv, 0);
	if (rc != O_OK) {
			o_close(m_OrientDB, m_OrientCon);
			m_OrientCon = NULL;
			o_free(m_OrientDB);
			m_OrientDB = NULL;
			return false;
		}
	return true;
}

bool CUstTinDB::_CheckDBName()
{
	int rc;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	rc = o_bin_dbexists(m_OrientDB, m_OrientCon, &tv, 0, m_DbName.c_str());
	if (rc != 1) {
		return false;
	}

	rc = o_bin_dbopen(m_OrientDB, m_OrientCon, &tv, 0, m_DbName.c_str());
	if (rc != O_OK) {
		o_close(m_OrientDB, m_OrientCon);
		m_OrientCon = NULL;
		o_free(m_OrientDB);
		m_OrientDB = NULL;
		return false;
	}

	return true;
}

bool CUstTinDB::_CheckVertexClass()
{
	ODOC_OBJECT *odoc;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, "SELECT FROM E", 20, "*:-1");
	if (!odoc) {
		return false;
	}

	fprintf(stdout, "COMMAND: got a document: %s\n", odoc_getraw(odoc, NULL));
	if (odoc_getnumrecords(odoc) == 0)	{
		// Vertex가 하나도 없음으로 Error
		ODOC_FREE_DOCUMENT(odoc);
		return false;
	}

	// free the returned document when you no longer need it
	ODOC_FREE_DOCUMENT(odoc);

	return true;
}

CUstTinDB::ErrCode CUstTinDB::MakeTin()
{
	CTinDataManager tinDataMaanger;

	ErrCode retVal = RET_OK;
	clock_t StartClock, LoadingClock, TinClock;
	StartClock = clock();

	tinDataMaanger.SetRamdomVertexs(1000000);
	LoadingClock = clock();

	tinDataMaanger.PrintVertexList();
	std::cout << "Load Points Time : " << (double)(LoadingClock - StartClock)/CLOCKS_PER_SEC << " sec" << std::endl;

	tinDataMaanger.MakeDelaunayEdge();
	TinClock = clock();

	tinDataMaanger.PrintEdgeList();
	std::cout << "Make Tin Time : " << (double)(TinClock - LoadingClock)/CLOCKS_PER_SEC << " sec" << std::endl;

	tinDataMaanger.MakeDelaunayFace();
	tinDataMaanger.PrintFaceList();

	return retVal;
}
