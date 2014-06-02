#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "TinOrientDBStorage.h"
#include "TinOrientDBVertex.h"
#include "TinOrientDBHalfEdge.h"


void orient_debug(const char *msg) {
	fprintf(stderr, "program: %s", msg);
}
CTinOrientDBStorage* CTinOrientDBStorage::instance = NULL;

CTinOrientDBStorage::CTinOrientDBStorage()
{
	CTinOrientDBStorage::instance = this;
	m_JNIEnv = NULL;
	m_JNIJvm = NULL;
	m_JNIOrientLibClass = NULL;
	m_JNIOrientLibObject = NULL;
	// Function.
	m_JNIFuncInit = NULL;
	m_JNIFuncInitDB = NULL;
	m_JNIFuncCreateEdge = NULL;
	m_JNIFuncGetVertex = NULL;
	m_JNIFuncGetVertexFromIdx = NULL;
	m_JNIFuncGetCountOfVertexs = NULL;
	m_JNIFuncGetCountOfEdges = NULL;
	m_JNIFuncUpdateEdge = NULL;
	m_JNIFuncUpdateVertex = NULL;
	m_JNIFuncDeleteEdge = NULL;
}

JNIEnv* CTinOrientDBStorage::_Create_VM(JavaVM ** jvm)
{
    JNIEnv *env;
    JavaVMInitArgs vm_args;
    JavaVMOption options;
    options.optionString = (char*)"-Djava.class.path=/home/iswook/workspace2/OrientLib/bin/"; //Path to the java source code
    vm_args.version = JNI_VERSION_1_6; //JDK version. This indicates version 1.6
    vm_args.nOptions = 1;
    vm_args.options = &options;
    vm_args.ignoreUnrecognized = 0;

    int ret = JNI_CreateJavaVM(jvm, (void**)&env, &vm_args);
    if(ret < 0)
    	printf("\nUnable to Launch JVM\n");
	return env;
}

bool CTinOrientDBStorage::_InitJNI()
{
	m_JNIEnv = _Create_VM(&m_JNIJvm);
	if (!m_JNIEnv) {
		return false;
	}

	m_JNIOrientLibClass = m_JNIEnv->FindClass("OrientLib");
	if (!m_JNIOrientLibClass) {
		return false;
	}

	if (!_GetJNIMethodID())
		return false;

	m_JNIOrientLibObject = m_JNIEnv->NewObject(m_JNIOrientLibClass, m_JNIFuncInit, NULL);

	return true;
}

bool CTinOrientDBStorage::_GetJNIMethodID()
{
	m_JNIFuncInit 				= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "<init>", "()V");
	m_JNIFuncInitDB 				= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "InitDB", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z");
	m_JNIFuncCreateEdge 			= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "<init>", "()V");
	m_JNIFuncGetVertex 			= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "<init>", "()V");
	m_JNIFuncGetVertexFromIdx 	= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "<init>", "()V");
	m_JNIFuncGetCountOfVertexs 	= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "GetCountOfVertexs", "()I");
	m_JNIFuncGetCountOfEdges 	= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "<init>", "()V");
	m_JNIFuncUpdateEdge 			= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "<init>", "()V");
	m_JNIFuncUpdateVertex 		= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "<init>", "()V");
	m_JNIFuncDeleteEdge 			= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "<init>", "()V");
	return m_JNIFuncInitDB && m_JNIFuncCreateEdge && m_JNIFuncGetVertex && m_JNIFuncGetVertexFromIdx && m_JNIFuncGetCountOfVertexs &&
			m_JNIFuncGetCountOfEdges && m_JNIFuncUpdateEdge && m_JNIFuncUpdateVertex && m_JNIFuncDeleteEdge;
}

CTinOrientDBStorage::~CTinOrientDBStorage()
{
//	if (m_OrientDB && m_OrientCon) {
//		struct timeval tv;
//		tv.tv_sec = 15;
//		tv.tv_usec = 0;
//		o_bin_dbclose(m_OrientDB, m_OrientCon, &tv, 0);
//		o_close(m_OrientDB, m_OrientCon);
//		m_OrientCon = NULL;
//	}
//	if (m_OrientDB) {
//		o_free(m_OrientDB);
//		m_OrientDB = NULL;
//	}
}

void CTinOrientDBStorage::UpdateVertex(CTinOrientDBVertex* pVertex)
{
//	ODOC_OBJECT *odoc;
//	struct timeval tv;
//	tv.tv_sec = 5;
//	tv.tv_usec = 0;
//
//	String Query = "update "+ m_VertexClassName + " ";
//	Query += "set halfedge = " + pVertex->GetRIDHalfEdge() + " ";
//	Query += " where @rid = " + pVertex->GetRID();
//
//	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");
//	ODOC_FREE_DOCUMENT(odoc);
}

void CTinOrientDBStorage::UpdateHalfEdge(CTinOrientDBHalfEdge* pEdge)
{
//	ODOC_OBJECT *odoc;
//	struct timeval tv;
//	tv.tv_sec = 5;
//	tv.tv_usec = 0;
//
//	String Query = "update "+ m_EdgeClassName + " ";
//	Query += "set out = " + pEdge->GetRIDVertex() + " ";
//	Query += ", in = " + pEdge->GetRIDEndVertex() + " ";
//	Query += ", pair = " + pEdge->GetRIDPair() + " ";
//	Query += ", ccw = " + pEdge->GetRIDCCW() + " ";
//	Query += ", cw = " + pEdge->GetRIDCW() + " ";
//	Query += " where @rid = " + pEdge->GetRID();
//
//	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");
//	ODOC_FREE_DOCUMENT(odoc);
}

CTinOrientDBStorage::ErrCode CTinOrientDBStorage::InitDB(
		String url, String dbName, String id, String pw, String vertexClassName, String edgeClassName)
{
	if (!_InitJNI()) {
		return ERR_DB_INIT;
	}
	String port = "2424";

	jstring a = m_JNIEnv->NewStringUTF(url.c_str());
	jstring b = m_JNIEnv->NewStringUTF(dbName.c_str());
	jstring c = m_JNIEnv->NewStringUTF(port.c_str());
	jstring d = m_JNIEnv->NewStringUTF(id.c_str());
	jstring e = m_JNIEnv->NewStringUTF(pw.c_str());
	jstring f = m_JNIEnv->NewStringUTF(vertexClassName.c_str());
	jstring g = m_JNIEnv->NewStringUTF(edgeClassName.c_str());

	jboolean ret = m_JNIEnv->CallBooleanMethod(m_JNIOrientLibObject, m_JNIFuncInitDB, a, b, c, d, e, f, g);

	if (!ret){
		return ERR_DB_INIT;
	}


	return RET_OK;
}

bool CTinOrientDBStorage::_ConnectDBServer()
{

//	int rc;
//	char str[O_ERR_MAXLEN];
//	struct timeval tv;
//
//
//
//
//	m_OrientDB = o_new();
//
//	o_debug_setlevel(m_OrientDB, ORIENT_SILENT);
//	o_debug_sethook(m_OrientDB, (void*)&orient_debug);
//
//	rc = o_prepare_connection(m_OrientDB, ORIENT_PROTO_BINARY, m_Url.c_str(), m_DBPort.c_str());
//	if (rc != O_OK) {
//		o_strerr(rc, str, O_ERR_MAXLEN);
//		fprintf(stderr, "Error preparing connections (%i): %s\n", rc, str);
//		o_free(m_OrientDB);
//		m_OrientDB = NULL;
//		return false;
//	}
//
//	// this user will be used to execute administrative commands to orientdb
//	rc = o_prepare_user(m_OrientDB, ORIENT_ADMIN, m_Id.c_str(), m_Pw.c_str());
//	if (rc != O_OK) {
//		o_free(m_OrientDB);
//		m_OrientDB = NULL;
//		return false;
//	}
//	// this user will be used on database open to execute commands to the db
//	rc = o_prepare_user(m_OrientDB, ORIENT_USER, m_Id.c_str(), m_Pw.c_str());
//	if (rc != O_OK) {
//		o_free(m_OrientDB);
//		m_OrientDB = NULL;
//		return false;
//	}
//	// set a connection timeout of 5 seconds
//	tv.tv_sec = 5;
//	tv.tv_usec = 0;
//	// try to create the connection with orientdb. NULL if a problem is encountered
//	m_OrientCon = o_connect(m_OrientDB, &tv, 0);
//	if (!m_OrientCon) {
//		o_free(m_OrientDB);
//		m_OrientDB = NULL;
//		return false;
//	}
//
//	rc = o_bin_connect(m_OrientDB, m_OrientCon, &tv, 0);
//	if (rc != O_OK) {
//			o_close(m_OrientDB, m_OrientCon);
//			m_OrientCon = NULL;
//			o_free(m_OrientDB);
//			m_OrientDB = NULL;
//			return false;
//		}
	return true;
}

//bool CTinOrientDBStorage::_CheckDBName()
//{
//	int rc;
//	struct timeval tv;
//	tv.tv_sec = 5;
//	tv.tv_usec = 0;
//	rc = o_bin_dbexists(m_OrientDB, m_OrientCon, &tv, 0, m_DbName.c_str());
//	if (rc != 1) {
//		return false;
//	}
//
//	rc = o_bin_dbopen(m_OrientDB, m_OrientCon, &tv, 0, m_DbName.c_str());
//	if (rc != O_OK) {
//		o_close(m_OrientDB, m_OrientCon);
//		m_OrientCon = NULL;
//		o_free(m_OrientDB);
//		m_OrientDB = NULL;
//		return false;
//	}
////
////	return true;
////}
//
//bool CTinOrientDBStorage::_CheckVertexClass()
//{
//	ODOC_OBJECT *odoc;
//	struct timeval tv;
//	tv.tv_sec = 5;
//	tv.tv_usec = 0;
//
//	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, "SELECT FROM V", 20, "*:-1");
//	if (!odoc) {
//		return false;
//	}
//
//	// free the returned document when you no longer need it
//	ODOC_FREE_DOCUMENT(odoc);
//
//	return true;
//}

bool CTinOrientDBStorage::_CreateEdgeClass()
{
//	ODOC_OBJECT *odoc;
//	struct timeval tv;
//	tv.tv_sec = 5;
//	tv.tv_usec = 0;
//	String Query = "drop class "+ m_EdgeClassName;
//	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");
//	ODOC_FREE_DOCUMENT(odoc);
//
//	Query = "create class " + m_EdgeClassName + " extends E";
//	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");
//
//	if (!odoc) {
//		return false;
//	}
//
//	ODOC_FREE_DOCUMENT(odoc);
	return true;
}

bool CTinOrientDBStorage::_CreateBlankClass()
{
//	ODOC_OBJECT *odoc;
//	struct timeval tv;
//	tv.tv_sec = 5;
//	tv.tv_usec = 0;
//
//	String Query = "drop class USTBlankClass";
//	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");
//	ODOC_FREE_DOCUMENT(odoc);
//
//	Query = "create class USTBlankClass extends V";
//	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");
//	ODOC_FREE_DOCUMENT(odoc);
//
//	Query = "create vertex UstBlankClass set name = \'blank\'";
//	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, NULL);
//	if (!odoc) {
//		return false;
//	}
//	ODOC_FREE_DOCUMENT(odoc);
//
//	Query = "SELECT @rid FROM UstBlankClass";
//	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, Query.c_str(), 20, "*:-1");
//	if (!odoc) {
//		return -1;
//	}
//
//	String strRecrodCount = odoc_getraw(odoc, NULL);
//	m_BlankRID = strRecrodCount.substr(strRecrodCount.find(':') + 1, strRecrodCount.length() - (strRecrodCount.find(':') + 1));

	return true;
}
bool CTinOrientDBStorage::SetCleanNRamdomVertexs(int DataNum)
{
//	ODOC_OBJECT *odoc;
//	struct timeval tv;
//	tv.tv_sec = 5;
//	tv.tv_usec = 0;
//
//	/////////////////
//	// DROP CLASS
//	String Query = "drop class " + m_VertexClassName;
//	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");
//	if (!odoc) {
//		return false;
//	}
//	ODOC_FREE_DOCUMENT(odoc);
//
//	/////////////////
//	// Create Class
//	Query = "create class " + m_VertexClassName + " extends V";
//	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");
//	if (!odoc) {
//		return false;
//	}
//	ODOC_FREE_DOCUMENT(odoc);
//
//	/////////////////////
//	// SetRandomVertex
//	int sq =(int) sqrt((double)DataNum);
//	int idx = 0;
//	for (int i = 0 ; i < sq * 10 + 1 ; i += 10) {
//		for(int j = 0 ; j < sq * 10 + 1 ; j += 10){
//			idx++;
//			char buf[100];
//			::sprintf(buf, "%d", i);
//			String xPos = buf;
//			::sprintf(buf, "%d", j);
//			String yPos = buf;
//
//			String Query = "create vertex " + m_VertexClassName + " set x = " + xPos + ", y = " + yPos;
//			odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, NULL);
//			if (!odoc) {
//				return false;
//			}
//			ODOC_FREE_DOCUMENT(odoc);
//			if (idx == DataNum) break;
//		}
//		if (idx == DataNum) break;
//	}
	return true;
}

VertexPtr CTinOrientDBStorage::GetVertex(int idx)
{
//	struct timeval tv;
//	tv.tv_sec = 5;
//	tv.tv_usec = 0;
//
//	char buffer[100];
//	sprintf(buffer,"%d",idx);
//	String strSkip = buffer;
//
//	String Query = "SELECT *, @rid FROM " + m_VertexClassName + " skip " + strSkip + " limit 1";
//	ODOC_OBJECT* odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, Query.c_str(), 20, "*:-1");
//	if (!odoc) {
//		return VertexPtr();
//	}
//
//	String strRow= odoc_getraw(odoc, NULL);
//	String strX = _GetProperty(strRow,"x");
//	String strY = _GetProperty(strRow,"y");
//	String strRIDHalfEdge = _GetProperty(strRow,"halfedge");
//	String strRID = _GetProperty(strRow,"rid");
//
//	CTinOrientDBVertex* pVertex = new CTinOrientDBVertex(strRID, ::atof(strX.c_str()), ::atof(strY.c_str()), strRIDHalfEdge);
//
//	ODOC_FREE_DOCUMENT(odoc);


	return VertexPtr((CTinOrientDBVertex*)(NULL));
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
//	struct timeval tv;
//	tv.tv_sec = 5;
//	tv.tv_usec = 0;
//
//	if (vertexRID.find('#') == -1){
//		return NULL;
//	}
//
//	String Query = "SELECT FROM " + vertexRID;
//	ODOC_OBJECT* odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, Query.c_str(), 20, "*:-1");
//	if (!odoc) {
//		return NULL;
//	}
//
//	String strRow= odoc_getraw(odoc, NULL);
//	String strX = _GetProperty(strRow,"x");
//	String strY = _GetProperty(strRow,"y");
//	String strRIDHalfEdge = _GetProperty(strRow,"halfedge");
//
//	CTinOrientDBVertex* pVertex = new CTinOrientDBVertex(vertexRID, ::atof(strX.c_str()), ::atof(strY.c_str()), strRIDHalfEdge);
//
//	ODOC_FREE_DOCUMENT(odoc);
//	return pVertex;
	return NULL;
}

void CTinOrientDBStorage::ReLoadVertex(CTinOrientDBVertex* pVertex)
{
	CTinOrientDBVertex* pReVertex = (CTinOrientDBVertex*)GetVertex(pVertex->GetRID());
	pVertex->Copy(pReVertex);
	delete pReVertex;
}

void CTinOrientDBStorage::ReLoadHalfEdge(CTinOrientDBHalfEdge* pEdge)
{
	EdgePtr pReEdge = GetHalfEdge(pEdge->GetRID());
	pEdge->Copy(pReEdge);
}


EdgePtr CTinOrientDBStorage::GetHalfEdge(RID EdgeRID)
{
//	struct timeval tv;
//	tv.tv_sec = 5;
//	tv.tv_usec = 0;
//
//	if (EdgeRID.find('#') == -1){
//		return EdgePtr((ITinHalfEdge*)NULL);
//	}
//
//	String Query = "SELECT FROM " + EdgeRID;
//	ODOC_OBJECT* odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, Query.c_str(), 20, "*:-1");
//	if (!odoc) {
//		return EdgePtr((ITinHalfEdge*)NULL);
//	}
//
//	String strRow = odoc_getraw(odoc, NULL);
//	String	strRIDVertex = _GetProperty(strRow,"out");
//	String	strRIDEndVertex = _GetProperty(strRow,"in");
//	String	strRIDPair = _GetProperty(strRow,"pair");
//	String	strRIDCCW = _GetProperty(strRow,"ccw");
//	String	strRIDCW = _GetProperty(strRow,"cw");
//
//	CTinOrientDBHalfEdge* pHalfEdge = new CTinOrientDBHalfEdge(EdgeRID);
//	pHalfEdge->SetRIDVertex(strRIDVertex);
//	pHalfEdge->SetRIDEndVertex(strRIDEndVertex);
//	pHalfEdge->SetRIDPair(strRIDPair);
//	pHalfEdge->SetRIDCCW(strRIDCCW);
//	pHalfEdge->SetRIDCW(strRIDCW);
//
//	ODOC_FREE_DOCUMENT(odoc);
//	return EdgePtr(pHalfEdge);
	return EdgePtr((ITinHalfEdge*)NULL);
}

int CTinOrientDBStorage::GetCountOfVertexs()
{
//	ODOC_OBJECT *odoc;
//	struct timeval tv;
//	tv.tv_sec = 5;
//	tv.tv_usec = 0;
//
//	String Query = "SELECT Count(*) FROM " + m_VertexClassName;
//	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, Query.c_str(), 20, "*:-1");
//	if (!odoc) {
//		return -1;
//	}
//
//	String strRecrodCount = odoc_getraw(odoc, NULL);
//	strRecrodCount = strRecrodCount.substr(strRecrodCount.find(':') + 1, strRecrodCount.length() - 1 - (strRecrodCount.find(':') + 1));
//
//	int recordCount = ::atoi(strRecrodCount.c_str());
//
//	// free the returned document when you no longer need it
//	ODOC_FREE_DOCUMENT(odoc);
//
//	return recordCount;

	return 0;
}

EdgePtr CTinOrientDBStorage::CreateEdge()
{
//	struct timeval tv;
//	tv.tv_sec = 5;
//	tv.tv_usec = 0;
//
//	int edges = GetCountOfEdges();
//
//	char buffer[100];
//	sprintf(buffer,"%d", edges);
//	String numberRecord = buffer;
//
//	String Query = "create edge " + m_EdgeClassName + " from " + m_BlankRID + " to " + m_BlankRID + " set pair = -1, cw = -1, ccw = " + numberRecord;
//	ODOC_OBJECT* odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, NULL);
//
//	Query = "SELECT @rid FROM " + m_EdgeClassName + " where ccw = " + numberRecord;
//	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, Query.c_str(), 20, "*:-1");
//	if (!odoc) {
//		return EdgePtr((ITinHalfEdge*)NULL);
//	}
//
//	String strRID= odoc_getraw(odoc, NULL);
//	strRID = strRID.substr(strRID.find(':') + 1, strRID.length() - (strRID.find(':') + 1));
//
//	CTinOrientDBHalfEdge* pHalfEdge = new CTinOrientDBHalfEdge(strRID);
//	ODOC_FREE_DOCUMENT(odoc);
//	return EdgePtr(pHalfEdge);

	return EdgePtr((ITinHalfEdge*)NULL);
}

int	CTinOrientDBStorage::GetCountOfEdges()
{
//	ODOC_OBJECT *odoc;
//	struct timeval tv;
//	tv.tv_sec = 5;
//	tv.tv_usec = 0;
//
//	String Query = "SELECT Count(*) FROM " + m_EdgeClassName;
//	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, Query.c_str(), 20, "*:-1");
//	if (!odoc) {
//		return -1;
//	}
//
//	String strRecrodCount = odoc_getraw(odoc, NULL);
//	strRecrodCount = strRecrodCount.substr(strRecrodCount.find(':') + 1, strRecrodCount.length() - 1 - (strRecrodCount.find(':') + 1));
//
//	int recordCount = ::atoi(strRecrodCount.c_str());
//
//	// free the returned document when you no longer need it
//	ODOC_FREE_DOCUMENT(odoc);
//
//	return recordCount;

	return 0;
}

bool CTinOrientDBStorage::DeleteHalfEdge(EdgePtr pEdge)
{
//	ODOC_OBJECT *odoc;
//	struct timeval tv;
//	tv.tv_sec = 5;
//	tv.tv_usec = 0;
//	RID strRID = ((CTinOrientDBHalfEdge*)(pEdge).get())->GetRID();
//	String Query = "delete edge " + strRID;
//	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYCMD, Query.c_str(), 20, "*:-1");
//	if (!odoc) {
//		return false;
//	}
//	ODOC_FREE_DOCUMENT(odoc);
	return true;
}

void CTinOrientDBStorage::PrintEdgeList()
{
//	ODOC_OBJECT *odoc;
//	struct timeval tv;
//	tv.tv_sec = 5;
//	tv.tv_usec = 0;
//
//	String Query = "SELECT  FROM " + m_EdgeClassName + " limit 100";
//	odoc = o_bin_command(m_OrientDB, m_OrientCon, &tv, 0, O_CMD_QUERYSYNC, Query.c_str(), 20, "*:-1");
//	if (!odoc) {
//		return ;
//	}
//
//	for (int i=0; i<odoc_getnumrecords(odoc); i++) {
//		fprintf(stdout, "Record number %i: %s\n", i, odoc_fetchrawrecord(odoc, NULL, i));
//	}
}
