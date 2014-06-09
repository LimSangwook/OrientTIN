#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "TinOrientDBStorage.h"


void orient_debug(const char *msg) {
	fprintf(stderr, "program: %s", msg);
}
CTinOrientDBStorage* CTinOrientDBStorage::instance = NULL;

CTinOrientDBStorage::~CTinOrientDBStorage()
{
	// 캐시된 넘들을 FlushAll 해준다.
	m_bPrintLog = false;
	_FlushEdgeCache(true);
	_FlushVertexCache();
}

CTinOrientDBStorage::CTinOrientDBStorage()
{
	m_bPrintLog = false;
	m_MaXVertexCache = 10010;
	m_MaXEdgeCache = 100000;
	m_RemoveEdgeCount=0;
	m_CreateEdgeCount=0;
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
	m_JNIFuncGetEdge = NULL;
	m_JNIFuncSetRandomVertex = NULL;
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

JNIEnv* CTinOrientDBStorage::_Create_VM(JavaVM ** jvm)
{
    JNIEnv *env;
    JavaVMInitArgs vm_args;
    JavaVMOption options[1];
    options[0].optionString = (char*)"-Djava.class.path=/home/iswook/workspace2/OrientLib/bin:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-graphdb-1.7-rc2.jar:/usr/lib/jvm/java-6-oracle/jre/lib/charsets.jar:/usr/lib/jvm/java-6-oracle/jre/lib/jce.jar:/usr/lib/jvm/java-6-oracle/jre/lib/jsse.jar:/usr/lib/jvm/java-6-oracle/jre/lib/resources.jar:/usr/lib/jvm/java-6-oracle/jre/lib/rt.jar:/usr/lib/jvm/java-6-oracle/jre/lib/ext/dnsns.jar:/usr/lib/jvm/java-6-oracle/jre/lib/ext/localedata.jar:/usr/lib/jvm/java-6-oracle/jre/lib/ext/sunjce_provider.jar:/usr/lib/jvm/java-6-oracle/jre/lib/ext/sunpkcs11.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/blueprints-core-2.5.0-20140320.105052-37.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/gremlin-groovy-2.5.0-20140125.153413-8.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/gremlin-java-2.5.0-20140125.153323-8.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/mail-1.4.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/colt-1.2.0.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/concurrent-1.3.4.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/concurrentlinkedhashmap-lru-1.4.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orient-commons-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-client-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-core-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-distributed-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-enterprise-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-nativeos-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-object-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-server-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-tools-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/activation-1.1.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/antlr-2.7.7.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/asm-3.2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/asm-analysis-3.2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/asm-commons-3.2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/asm-tree-3.2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/asm-util-3.2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/commons-beanutils-1.7.0.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/commons-beanutils-core-1.8.0.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/commons-collections-3.2.1.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/commons-configuration-1.6.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/commons-digester-1.8.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/commons-lang-2.4.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/commons-logging-1.0.4.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/groovy-1.8.9.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/hazelcast-3.1.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/hibernate-jpa-2.0-api-1.0.0.Final.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jackson-annotations-2.2.2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jackson-core-2.2.2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jackson-databind-2.2.2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jackson-datatype-json-org-2.2.3.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jansi-1.5.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/javassist-3.16.1-GA.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jettison-1.3.3.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jline-0.9.94.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jna-4.0.0.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jna-platform-4.0.0.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/json-20090211.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/pipes-2.5.0-20140125.162807-6.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/snappy-0.3.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/stax-api-1.0.1.jar";
    vm_args.version = JNI_VERSION_1_6; //JDK version. This indicates version 1.6
    vm_args.nOptions = 1;
    vm_args.options = options;
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
	m_JNIFuncCreateEdge 			= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "CreateEdge", "()Ljava/lang/String;");
	m_JNIFuncGetVertex 			= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "GetVertex", "(Ljava/lang/String;)Ljava/lang/String;");
	m_JNIFuncGetVertexFromIdx 	= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "GetVertexFromIndex", "(I)Ljava/lang/String;");
	m_JNIFuncGetCountOfVertexs 	= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "GetCountOfVertexs", "()I");
	m_JNIFuncGetCountOfEdges 	= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "GetCountOfEdges", "()I");
	m_JNIFuncUpdateEdge 			= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "UpdateEdge", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z");
	m_JNIFuncUpdateVertex 		= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "UpdateVertex", "(Ljava/lang/String;DDLjava/lang/String;)Z");
	m_JNIFuncDeleteEdge 			= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "DeleteHalfEdge", "(Ljava/lang/String;)Z");
	m_JNIFuncSetRandomVertex		= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "SetRandomVertex", "(I)Z");
	m_JNIFuncGetEdge				= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "GetEdge", "(Ljava/lang/String;)Ljava/lang/String;");

	return m_JNIFuncInitDB && 				m_JNIFuncCreateEdge &&			m_JNIFuncGetVertex &&
			m_JNIFuncGetVertexFromIdx && 	m_JNIFuncGetCountOfVertexs &&	m_JNIFuncGetCountOfEdges &&
			m_JNIFuncUpdateEdge && 			m_JNIFuncUpdateVertex && 		m_JNIFuncDeleteEdge &&
			m_JNIFuncSetRandomVertex &&		m_JNIFuncInit	&&					m_JNIFuncGetEdge;
}

void CTinOrientDBStorage::_FlushEdgeCache(bool AllFlush)
{
	std::map<RID,EdgePtr>::iterator iter =  m_EdgeCache.begin();
	for (;iter != m_EdgeCache.end() ; iter++) {
//		int nUseCount = iter->second.use_count();
//		if (nUseCount < 2 || AllFlush) {
//			if ("#11:11" == ((CTinOrientDBHalfEdge*)(iter->second.get()))->GetRID()) {
//				std::cout<<"dsafasdfasdf \n";
//			}
			_UpdateHalfEdge((CTinOrientDBHalfEdge*)(iter->second));
			m_EdgeCache.erase(iter);
//		}
	}
	//m_EdgeCache.clear();
}

void CTinOrientDBStorage::_FlushVertexCache()
{
	std::map<RID,VertexPtr>::iterator iter =  m_VertexCache.begin();
	for (;iter != m_VertexCache.end() ; iter++) {
		_UpdateVertex((CTinOrientDBVertex*)(iter->second).get());
	}
	m_VertexCache.clear();
}

void CTinOrientDBStorage::_UpdateVertex(CTinOrientDBVertex* pVertex)
{
	if (m_bPrintLog) std::cout << "_UpdateVertex DB I/O \n";
	jstring jStrRID = m_JNIEnv->NewStringUTF(pVertex->GetRID().c_str());
	jstring jStrHalfEdge = m_JNIEnv->NewStringUTF(pVertex->GetRIDHalfEdge().c_str());
	m_JNIEnv->CallBooleanMethod(m_JNIOrientLibObject, m_JNIFuncUpdateVertex, jStrRID, pVertex->GetX(), pVertex->GetY(), jStrHalfEdge);
}

void CTinOrientDBStorage::UpdateVertex(CTinOrientDBVertex* pVertex)
{
	String vertexRID = pVertex->GetRID();
	int start = vertexRID.find(":") + 1;
	String strIndex = vertexRID.substr(start, vertexRID.length() - start );
	std::map<RID,VertexPtr>::iterator iter = m_VertexCache.find(strIndex);

	if (iter != m_VertexCache.end()) {
		VertexPtr vPtr = iter->second;
		if (vPtr.get() != pVertex) {
			((CTinOrientDBVertex*)(vPtr.get()))->SetRIDHalfEdge(pVertex->GetRIDHalfEdge());
		}
		return;
	}
	_UpdateVertex(pVertex);
}
void CTinOrientDBStorage::_UpdateHalfEdge(CTinOrientDBHalfEdge* pEdge)
{
	if (m_bPrintLog)  std::cout << "_UpdateHalfEdge DB I/O \n";
	jstring jStrRID = m_JNIEnv->NewStringUTF(pEdge->GetRID().c_str());
	jstring jStrOut = m_JNIEnv->NewStringUTF(pEdge->GetRIDVertex().c_str());
	jstring jStrIn = m_JNIEnv->NewStringUTF(pEdge->GetRIDEndVertex().c_str());
	jstring jStrPair = m_JNIEnv->NewStringUTF(pEdge->GetRIDPair().c_str());
	jstring jStrCCW = m_JNIEnv->NewStringUTF(pEdge->GetRIDCCW().c_str());
	jstring jStrCW = m_JNIEnv->NewStringUTF(pEdge->GetRIDCW().c_str());
	m_JNIEnv->CallBooleanMethod(m_JNIOrientLibObject, m_JNIFuncUpdateEdge, jStrRID, jStrOut, jStrIn, jStrPair, jStrCCW, jStrCW);
}

void CTinOrientDBStorage::UpdateHalfEdge(CTinOrientDBHalfEdge* pEdge)
{
	std::map<RID,EdgePtr>::iterator iter = m_EdgeCache.find(pEdge->GetRID());
	if (iter != m_EdgeCache.end()) {
		if (pEdge == iter->second) {
			// 캐시와 동일하기 때문에 나중에 일괄 업데이트 한다.
			return;
		}
		((CTinOrientDBHalfEdge*)(iter->second))->Copy(EdgePtr(pEdge));
		return;
	}
	_UpdateHalfEdge(pEdge);
}

bool CTinOrientDBStorage::SetCleanNRamdomVertexs(int DataNum)
{
	jboolean ret = m_JNIEnv->CallIntMethod(m_JNIOrientLibObject, m_JNIFuncSetRandomVertex, DataNum);

	if (!ret){
		return false;
	}

	return true;
}

VertexPtr CTinOrientDBStorage::_GetStringToVertex(String& str)
{
	String vertexRID = _GetProperty(str, "rid");
	String X = _GetProperty(str, "x");
	String Y = _GetProperty(str, "y");
	String HalfEdgeRID = _GetProperty(str, "halfedge");

	if (HalfEdgeRID == "null")
		HalfEdgeRID = "";

	CTinOrientDBVertex* pV = new CTinOrientDBVertex(vertexRID, atof(X.c_str()), atof(Y.c_str()), HalfEdgeRID);

	return VertexPtr(pV);
}
VertexPtr CTinOrientDBStorage::GetVertex(int idx)
{
	if (m_VertexCache.size() >= m_MaXVertexCache) {
		_FlushVertexCache();
	}

	char buf[100];
	sprintf(buf, "%d", idx);
	String strIndex = buf;
	std::map<RID,VertexPtr>::iterator iter = m_VertexCache.find(strIndex);

	if (iter != m_VertexCache.end()) {
		return iter->second;
	}
	if (m_bPrintLog) std::cout << "GetVertex(int idx) DB I/O \n";
	jstring jStrVertex =  (jstring)m_JNIEnv->CallObjectMethod(m_JNIOrientLibObject, m_JNIFuncGetVertexFromIdx, idx);
	jboolean bInIsCopy;
	const char* strCln = m_JNIEnv->GetStringUTFChars(jStrVertex, &bInIsCopy);
	String strV = strCln;
	m_JNIEnv->ReleaseStringUTFChars(jStrVertex, strCln);

	VertexPtr vPtr = _GetStringToVertex(strV);
	m_VertexCache[strIndex] = vPtr;

	return vPtr;
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

	String retStr = json.substr(startPostion, endPostion);
	if (retStr =="null")	{
		retStr == "0";
	}
	return retStr;
}
VertexPtr CTinOrientDBStorage::GetVertex(RID vertexRID)
{
	if (m_VertexCache.size() >= m_MaXVertexCache) {
		_FlushVertexCache();
	}

	int start = vertexRID.find(":") + 1;
	String strIndex = vertexRID.substr(start, vertexRID.length() - start );
	std::map<RID,VertexPtr>::iterator iter = m_VertexCache.find(strIndex);

	if (iter != m_VertexCache.end()) {
		return iter->second;
	}

	if (m_bPrintLog) std::cout << "GetVertex(RID vertexRID) DB I/O \n";
	jstring a = m_JNIEnv->NewStringUTF(vertexRID.c_str());
	jstring jStrVertex =  (jstring)m_JNIEnv->CallObjectMethod(m_JNIOrientLibObject, m_JNIFuncGetVertex, a);
	jboolean bInIsCopy;
	const char* strCln = m_JNIEnv->GetStringUTFChars(jStrVertex, &bInIsCopy);
	String strV = strCln;
	m_JNIEnv->ReleaseStringUTFChars(jStrVertex, strCln);
	VertexPtr vPtr = _GetStringToVertex(strV);
	m_VertexCache[strIndex] = vPtr;

	return vPtr;
}

void CTinOrientDBStorage::ReLoadVertex(CTinOrientDBVertex* pVertex)
{
	VertexPtr preVertex = GetVertex(pVertex->GetRID());
	pVertex->Copy((CTinOrientDBVertex*)(preVertex.get()));
}

void CTinOrientDBStorage::ReLoadHalfEdge(CTinOrientDBHalfEdge* pEdge)
{
	EdgePtr pReEdge = GetHalfEdge(pEdge->GetRID());
	if (pReEdge != pEdge) {
		pEdge->Copy(pReEdge);
	}
}

EdgePtr CTinOrientDBStorage::GetHalfEdge(RID EdgeRID)
{
	std::map<RID,EdgePtr>::iterator iter = m_EdgeCache.find(EdgeRID);

	if (iter != m_EdgeCache.end()) {
		return iter->second;
	}

	int nEdgeCacheCnt = m_EdgeCache.size();
	if (nEdgeCacheCnt >= m_MaXEdgeCache) {
		_FlushEdgeCache();
	}

	if (m_bPrintLog) std::cout << "GetHalfEdge DB I/O \n";

	jstring a = m_JNIEnv->NewStringUTF(EdgeRID.c_str());
	jstring jStrVertex =  (jstring)m_JNIEnv->CallObjectMethod(m_JNIOrientLibObject, m_JNIFuncGetEdge, a);
	jboolean bInIsCopy;
	const char* strCln = m_JNIEnv->GetStringUTFChars(jStrVertex, &bInIsCopy);
	String strE = strCln;
	m_JNIEnv->ReleaseStringUTFChars(jStrVertex, strCln);

	String strEdgeRID = _GetProperty(strE, "rid");
	String strIn = _GetProperty(strE, "in");
	String strOut = _GetProperty(strE, "out");
	String strPair = _GetProperty(strE, "pair");
	String strCCW = _GetProperty(strE, "ccw");
	String strCW = _GetProperty(strE, "cw");

	CTinOrientDBHalfEdge* pV = new CTinOrientDBHalfEdge(strEdgeRID, strOut, strIn, strPair, strCCW, strCW);

	EdgePtr edgePtr = EdgePtr(pV);
	m_EdgeCache[strEdgeRID] = edgePtr;

	return edgePtr;
}

int CTinOrientDBStorage::GetCountOfVertexs()
{
	int ret = m_JNIEnv->CallIntMethod(m_JNIOrientLibObject, m_JNIFuncGetCountOfVertexs, NULL);

	return ret;
}

EdgePtr CTinOrientDBStorage::CreateEdge()
{
	int nEdgeCacheCnt = m_EdgeCache.size();
	if (nEdgeCacheCnt >= m_MaXEdgeCache) {
		_FlushEdgeCache();
	}
	jstring jStrEdgetRID = (jstring)m_JNIEnv->CallObjectMethod(m_JNIOrientLibObject, m_JNIFuncCreateEdge, NULL);
	jboolean bInIsCopy;
	const char* strEdgeRID = m_JNIEnv->GetStringUTFChars(jStrEdgetRID, &bInIsCopy);
	String str = strEdgeRID;

	CTinOrientDBHalfEdge* pHalfEdge = new CTinOrientDBHalfEdge(str);
	EdgePtr edgePtr = EdgePtr(pHalfEdge);

	m_EdgeCache[strEdgeRID] = edgePtr;
	m_CreateEdgeCount++;
	return edgePtr;
}

int	CTinOrientDBStorage::GetCountOfEdges()
{
	int ret = m_JNIEnv->CallIntMethod(m_JNIOrientLibObject, m_JNIFuncGetCountOfEdges, NULL);

	return ret;
}

bool CTinOrientDBStorage::DeleteHalfEdge(EdgePtr pEdge)
{
	String RIDEdge = ((CTinOrientDBHalfEdge*)(pEdge))->GetRID();
	jstring jStrRIDEdge = (jstring)m_JNIEnv->NewStringUTF(RIDEdge.c_str());
	((CTinOrientDBHalfEdge*)(pEdge))->SetRIDVertex("Deleted");
	bool blog = m_bPrintLog;
	m_bPrintLog = false;
	_UpdateHalfEdge((CTinOrientDBHalfEdge*)(pEdge));
	m_bPrintLog = blog;

	//int ret = m_JNIEnv->CallBooleanMethod(m_JNIOrientLibObject, m_JNIFuncDeleteEdge, jStrRIDEdge);
	m_RemoveEdgeCount++;
	std::map<RID,EdgePtr>::iterator iter = m_EdgeCache.find(RIDEdge);
	if (iter != m_EdgeCache.end()){
		m_EdgeCache.erase(RIDEdge);
	}
	return true;
}

void CTinOrientDBStorage::PrintEdgeList()
{
	std::cout << "### m_CreateEdgeCount " << m_CreateEdgeCount << "\n";
	std::cout << "### m_RemoveEdgeCount " << m_RemoveEdgeCount << "\n";
}
