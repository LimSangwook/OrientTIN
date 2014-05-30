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
	ODOC_OBJECT *odoc;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	String Query = "update "+ m_VertexClassName + " ";
	Query += "set halfedge = " + pVertex->GetRIDHalfEdge() + " ";
	Query += " where @rid = " + pVertex->GetRID();

	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");
	ODOC_FREE_DOCUMENT(odoc);
}

void CTinOrientDBStorage::UpdateHalfEdge(CTinOrientDBHalfEdge* pEdge)
{
	ODOC_OBJECT *odoc;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	String Query = "update "+ m_EdgeClassName + " ";
	Query += "set out = " + pEdge->GetRIDVertex() + " ";
	Query += ", in = " + pEdge->GetRIDEndVertex() + " ";
	Query += ", pair = " + pEdge->GetRIDPair() + " ";
	Query += ", ccw = " + pEdge->GetRIDCCW() + " ";
	Query += ", cw = " + pEdge->GetRIDCW() + " ";
	Query += " where @rid = " + pEdge->GetRID();

	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");
	ODOC_FREE_DOCUMENT(odoc);
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
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	char buffer[100];
	sprintf(buffer,"%d",idx);
	String strSkip = buffer;

	String Query = "SELECT *, @rid FROM " + m_VertexClassName + " skip " + strSkip + " limit 1";
	ODOC_OBJECT* odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, Query.c_str(), 20, "*:-1");
	if (!odoc) {
		return NULL;
	}

	String strRow= odoc_getraw(odoc, NULL);
	String strX = _GetProperty(strRow,"x");
	String strY = _GetProperty(strRow,"y");
	String strRIDHalfEdge = _GetProperty(strRow,"halfedge");
	String strRID = _GetProperty(strRow,"rid");

	CTinOrientDBVertex* pVertex = new CTinOrientDBVertex(strRID, ::atof(strX.c_str()), ::atof(strY.c_str()), strRIDHalfEdge);

	ODOC_FREE_DOCUMENT(odoc);
	return pVertex;
}

String CTinOrientDBStorage::_GetProperty(String json, String propertyName)
{

	//',' + propertyName + ":" 일수 있고
	//'@' + propertyName + ":" 일수 있다

	String token = "," + propertyName + ":";
	int findPropertyName = json.find(token) ;
	if ((findPropertyName == -1)) {
		token = "@" + propertyName + ":";
		findPropertyName = json.find(token);
		if ((findPropertyName == -1)) {
			return "";
		}
	}

	int startPostion = findPropertyName + token.length();
	int endPostion = -1;
	if ((endPostion = json.substr(startPostion + 1, json.length() - startPostion).find(',')) == -1) {
		endPostion = json.length() - 1;
	} else {
		endPostion++;
	}
	return json.substr(startPostion, endPostion);
}
ITinVertex* CTinOrientDBStorage::GetVertex(RID vertexRID)
{
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	if (vertexRID.find('#') == -1){
		return NULL;
	}

	String Query = "SELECT FROM " + vertexRID;
	ODOC_OBJECT* odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, Query.c_str(), 20, "*:-1");
	if (!odoc) {
		return NULL;
	}

	String strRow= odoc_getraw(odoc, NULL);
	String strX = _GetProperty(strRow,"x");
	String strY = _GetProperty(strRow,"y");
	String strRIDHalfEdge = _GetProperty(strRow,"halfedge");

	CTinOrientDBVertex* pVertex = new CTinOrientDBVertex(vertexRID, ::atof(strX.c_str()), ::atof(strY.c_str()), strRIDHalfEdge);

	ODOC_FREE_DOCUMENT(odoc);
	return pVertex;
}

void CTinOrientDBStorage::ReLoadVertex(CTinOrientDBVertex* pVertex)
{
	CTinOrientDBVertex* pReVertex = (CTinOrientDBVertex*)GetVertex(pVertex->GetRID());
	pVertex->Copy(pReVertex);
	delete pReVertex;
}

void CTinOrientDBStorage::ReLoadHalfEdge(CTinOrientDBHalfEdge* pEdge)
{
	CTinOrientDBHalfEdge* pReEdge = (CTinOrientDBHalfEdge*)GetHalfEdge(pEdge->GetRID());
	pEdge->Copy(pReEdge);
	delete pReEdge;
}


ITinHalfEdge* CTinOrientDBStorage::GetHalfEdge(RID EdgeRID)
{
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	if (EdgeRID.find('#') == -1){
		return NULL;
	}

	String Query = "SELECT FROM " + EdgeRID;
	ODOC_OBJECT* odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, Query.c_str(), 20, "*:-1");
	if (!odoc) {
		return NULL;
	}

	String strRow = odoc_getraw(odoc, NULL);
	String	strRIDVertex = _GetProperty(strRow,"out");
	String	strRIDEndVertex = _GetProperty(strRow,"in");
	String	strRIDPair = _GetProperty(strRow,"pair");
	String	strRIDCCW = _GetProperty(strRow,"ccw");
	String	strRIDCW = _GetProperty(strRow,"cw");

	CTinOrientDBHalfEdge* pHalfEdge = new CTinOrientDBHalfEdge(EdgeRID);
	pHalfEdge->SetRIDVertex(strRIDVertex);
	pHalfEdge->SetRIDEndVertex(strRIDEndVertex);
	pHalfEdge->SetRIDPair(strRIDPair);
	pHalfEdge->SetRIDCCW(strRIDCCW);
	pHalfEdge->SetRIDCW(strRIDCW);

	ODOC_FREE_DOCUMENT(odoc);
	return pHalfEdge;
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

	String Query = "create edge " + m_EdgeClassName + " from " + m_BlankRID + " to " + m_BlankRID + " set pair = -1, cw = -1, ccw = " + numberRecord;
	ODOC_OBJECT* odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, NULL);

	Query = "SELECT @rid FROM " + m_EdgeClassName + " where ccw = " + numberRecord;
	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, Query.c_str(), 20, "*:-1");
	if (!odoc) {
		return NULL;
	}

	String strRID= odoc_getraw(odoc, NULL);
	strRID = strRID.substr(strRID.find(':') + 1, strRID.length() - (strRID.find(':') + 1));

	CTinOrientDBHalfEdge* pHalfEdge = new CTinOrientDBHalfEdge(strRID);
	ODOC_FREE_DOCUMENT(odoc);
	return pHalfEdge;
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

bool CTinOrientDBStorage::DeleteHalfEdge(ITinHalfEdge* pEdge)
{
	ODOC_OBJECT *odoc;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	RID strRID = ((CTinOrientDBHalfEdge*)(pEdge))->GetRID();
	String Query = "delete edge " + strRID;
	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");
	if (!odoc) {
		return false;
	}
	ODOC_FREE_DOCUMENT(odoc);
	return true;
}

void CTinOrientDBStorage::PrintEdgeList()
{
	ODOC_OBJECT *odoc;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	String Query = "SELECT  FROM " + m_EdgeClassName + " limit 100";
	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, Query.c_str(), 20, "*:-1");
	if (!odoc) {
		return ;
	}

	for (int i=0; i<odoc_getnumrecords(odoc); i++) {
		fprintf(stdout, "Record number %i: %s\n", i, odoc_fetchrawrecord(odoc, NULL, i));
	}
}
