#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "TinOrientDBStorage.h"
#include "TinOrientDBVertex.h"

void orient_debug(const char *msg) {
	fprintf(stderr, "program: %s", msg);
}
CTinOrientDBStorage* CTinOrientDBStorage::instance = NULL;
CTinOrientDBStorage::CTinOrientDBStorage() :
		m_DBPort("2424"), m_OrientDB(NULL), m_OrientCon(NULL)
{
	CTinOrientDBStorage::instance = this;
}

CTinOrientDBStorage::~CTinOrientDBStorage()
{
	if (m_OrientDB && m_OrientCon) {
		struct timeval tv;
		tv.tv_sec = 15;
		tv.tv_usec = 0;
		o_bin_dbclose(m_OrientDB, m_OrientCon, &tv, 0);
		o_close(m_OrientDB, m_OrientCon);
		m_OrientCon = NULL;
	}
	if (m_OrientDB) {
		o_free(m_OrientDB);
		m_OrientDB = NULL;
	}
}

void CTinOrientDBStorage::UpdateVertex(CTinOrientDBVertex* pVertex)
{

}

void CTinOrientDBStorage::UpdateHalfEdge(ITinHalfEdge* pEdge)
{

}

CTinOrientDBStorage::ErrCode CTinOrientDBStorage::InitDB(
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

	_CreateEdgeClass();

	_CreateBlankClass();

	return RET_OK;
}

bool CTinOrientDBStorage::_ConnectDBServer()
{
	int rc;
	char str[O_ERR_MAXLEN];
	struct timeval tv;

	m_OrientDB = o_new();

	o_debug_setlevel(m_OrientDB, ORIENT_SILENT);
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

bool CTinOrientDBStorage::_CheckDBName()
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

bool CTinOrientDBStorage::_CheckVertexClass()
{
	ODOC_OBJECT *odoc;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, "SELECT FROM V", 20, "*:-1");
	if (!odoc) {
		return false;
	}

	// free the returned document when you no longer need it
	ODOC_FREE_DOCUMENT(odoc);

	return true;
}

bool CTinOrientDBStorage::_CreateEdgeClass()
{
	ODOC_OBJECT *odoc;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	String Query = "drop class "+ m_EdgeClassName;
	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");
	ODOC_FREE_DOCUMENT(odoc);

	Query = "create class " + m_EdgeClassName + " extends E";
	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");

	if (!odoc) {
		return false;
	}

	ODOC_FREE_DOCUMENT(odoc);
	return true;
}

bool CTinOrientDBStorage::_CreateBlankClass()
{
	ODOC_OBJECT *odoc;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	String Query = "drop class USTBlankClass";
	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");
	ODOC_FREE_DOCUMENT(odoc);

	Query = "create class USTBlankClass extends V";
	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");
	ODOC_FREE_DOCUMENT(odoc);

	Query = "create vertex UstBlankClass set name = \'blank\'";
	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, NULL);
	if (!odoc) {
		return false;
	}
	ODOC_FREE_DOCUMENT(odoc);

	Query = "SELECT @rid FROM UstBlankClass";
	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, Query.c_str(), 20, "*:-1");
	if (!odoc) {
		return -1;
	}

	String strRecrodCount = odoc_getraw(odoc, NULL);
	m_BlankRID = strRecrodCount.substr(strRecrodCount.find(':') + 1, strRecrodCount.length() - (strRecrodCount.find(':') + 1));

	return true;
}
bool CTinOrientDBStorage::SetCleanNRamdomVertexs(int DataNum)
{
	ODOC_OBJECT *odoc;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	/////////////////
	// DROP CLASS
	String Query = "drop class " + m_VertexClassName;
	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");
	if (!odoc) {
		return false;
	}
	ODOC_FREE_DOCUMENT(odoc);

	/////////////////
	// Create Class
	Query = "create class " + m_VertexClassName + " extends V";
	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");
	if (!odoc) {
		return false;
	}
	ODOC_FREE_DOCUMENT(odoc);

	/////////////////////
	// SetRandomVertex
	int sq =(int) sqrt((double)DataNum);
	int idx = 0;
	for (int i = 0 ; i < sq * 10 + 1 ; i += 10) {
		for(int j = 0 ; j < sq * 10 + 1 ; j += 10){
			idx++;
			char buf[100];
			::sprintf(buf, "%d", i);
			String xPos = buf;
			::sprintf(buf, "%d", j);
			String yPos = buf;

			String Query = "create vertex " + m_VertexClassName + " set x = " + xPos + ", y = " + yPos;
			odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, NULL);
			if (!odoc) {
				return false;
			}
			ODOC_FREE_DOCUMENT(odoc);
			if (idx == DataNum) break;
		}
		if (idx == DataNum) break;
	}
	return true;
}

ITinVertex* CTinOrientDBStorage::GetVertex(int idx)
{
	return NULL;
}

ITinVertex* CTinOrientDBStorage::GetVertex(RID vertexRID)
{
	return NULL;
}

ITinHalfEdge* CTinOrientDBStorage::GetHalfEdge(RID EdgeRID)
{
	return NULL;
}

int CTinOrientDBStorage::GetCountOfVertexs()
{
	ODOC_OBJECT *odoc;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	String Query = "SELECT Count(*) FROM " + m_VertexClassName;
	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, Query.c_str(), 20, "*:-1");
	if (!odoc) {
		return -1;
	}

	String strRecrodCount = odoc_getraw(odoc, NULL);
	strRecrodCount = strRecrodCount.substr(strRecrodCount.find(':') + 1, strRecrodCount.length() - 1 - (strRecrodCount.find(':') + 1));

	int recordCount = ::atoi(strRecrodCount.c_str());

	// free the returned document when you no longer need it
	ODOC_FREE_DOCUMENT(odoc);

	return recordCount;
}

ITinHalfEdge* CTinOrientDBStorage::CreateEdge()
{
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	int edges = GetCountOfEdges();

	char buffer[100];
	sprintf(buffer,"%d", edges);
	String numberRecord = buffer;

	String Query = "create edge " + m_EdgeClassName + " from " + m_BlankRID + " to " + m_BlankRID + " set ccw = " + numberRecord;
	ODOC_OBJECT* odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, NULL);

	Query = "SELECT @rid FROM " + m_EdgeClassName + " where ccw = " + numberRecord;
	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, Query.c_str(), 20, "*:-1");
	if (!odoc) {
		return NULL;
	}

	String strRID= odoc_getraw(odoc, NULL);
	strRID = strRID.substr(strRID.find(':') + 1, strRID.length() - (strRID.find(':') + 1));
	CTinOrientDBVertex* pVertex = new CTinOrientDBVertex(strRID);
	ODOC_FREE_DOCUMENT(odoc);
	return pVertex;
}

int	CTinOrientDBStorage::GetCountOfEdges()
{
	ODOC_OBJECT *odoc;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	String Query = "SELECT Count(*) FROM " + m_EdgeClassName;
	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, Query.c_str(), 20, "*:-1");
	if (!odoc) {
		return -1;
	}

	String strRecrodCount = odoc_getraw(odoc, NULL);
	strRecrodCount = strRecrodCount.substr(strRecrodCount.find(':') + 1, strRecrodCount.length() - 1 - (strRecrodCount.find(':') + 1));

	int recordCount = ::atoi(strRecrodCount.c_str());

	// free the returned document when you no longer need it
	ODOC_FREE_DOCUMENT(odoc);

	return recordCount;
}
