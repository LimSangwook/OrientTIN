#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "TinOrientDBStorage.h"
#include <time.h>

void orient_debug(const char *msg) {
	fprintf(stderr, "program: %s", msg);
}

void _PrintTime(String msg)
{
	time_t htime;
	tm* pTime;
	time(&htime);
	pTime = localtime(&htime);
	std::cout << msg << " 시간 : " << asctime(pTime);

}

CTinOrientDBStorage* CTinOrientDBStorage::instance;

CTinOrientDBStorage::~CTinOrientDBStorage()
{
	// 캐시된 넘들을 FlushAll 해준다.
	_FlushEdgeCache();
	_FlushVertexCache();
}

void CTinOrientDBStorage::FlushCache()
{
	// Edge Cache FLush
	if (m_EdgeCache.size() >= m_MaXEdgeCache) {
		_FlushEdgeCache();
	}

	// Vertex Cache FLush
	if (m_VertexCache.size() >= m_MaXVertexCache) {
		_FlushVertexCache();
	}
}

void CTinOrientDBStorage::Close()
{
	_FlushEdgeCache();
	_FlushVertexCache();

}

CTinOrientDBStorage::CTinOrientDBStorage()
{
	m_MaXVertexCache = 500000;
	m_MaXEdgeCache = 500000;

	m_nTotalRemoveEdgeCount=0;
	m_nTotalCreateEdgeCount=0;

	m_nCreatedMemoryEdge = 0;

	m_NowEdgeID = 0;
	m_MaxEdgeID = 0;
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
	m_JNIFuncCreateBlankEdge = NULL;
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
	m_JNIEnv->DeleteLocalRef(a);
	m_JNIEnv->DeleteLocalRef(b);
	m_JNIEnv->DeleteLocalRef(c);
	m_JNIEnv->DeleteLocalRef(d);
	m_JNIEnv->DeleteLocalRef(e);
	m_JNIEnv->DeleteLocalRef(f);
	m_JNIEnv->DeleteLocalRef(g);

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
	m_JNIFuncCreateBlankEdge		= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "CreateBlankEdge", "(I)Ljava/lang/String;");


	return m_JNIFuncInitDB && 				m_JNIFuncCreateEdge &&			m_JNIFuncGetVertex &&
			m_JNIFuncGetVertexFromIdx && 	m_JNIFuncGetCountOfVertexs &&	m_JNIFuncGetCountOfEdges &&
			m_JNIFuncUpdateEdge && 			m_JNIFuncUpdateVertex && 		m_JNIFuncDeleteEdge &&
			m_JNIFuncSetRandomVertex &&		m_JNIFuncInit	&&					m_JNIFuncGetEdge &&
			m_JNIFuncCreateBlankEdge;
}

bool CTinOrientDBStorage::_CreateBlankEdge(int num)
{
	jstring jStrEdgeRID =  (jstring)m_JNIEnv->CallObjectMethod(m_JNIOrientLibObject, m_JNIFuncCreateBlankEdge, num);
	jboolean bInIsCopy;
	const char* strCln = m_JNIEnv->GetStringUTFChars(jStrEdgeRID, &bInIsCopy);
	String strRID = strCln;
	m_JNIEnv->ReleaseStringUTFChars(jStrEdgeRID, strCln);
	int pos = strRID.find(":");
	m_EdgeClassID = strRID.substr(0, pos + 1);
	String nowID = strRID.substr(pos + 1 , strRID.length() - pos);
	m_NowEdgeID = std::atoi(nowID.c_str());
	m_MaxEdgeID = m_NowEdgeID + num - 1;
	return true;
}

void CTinOrientDBStorage::_DeleteEdge(RID edgeRID)
{
	jstring jStrRID = m_JNIEnv->NewStringUTF(edgeRID.c_str());
	m_JNIEnv->CallBooleanMethod(m_JNIOrientLibObject, m_JNIFuncDeleteEdge, jStrRID);
	m_JNIEnv->DeleteLocalRef(jStrRID);
}

void CTinOrientDBStorage::_FlushEdgeCache()
{
	if (m_EdgeCache.size() < 1) {
		return;
	}

	_PrintTime("\t_FlushEdgeCache 시작");

	if (m_nCreatedMemoryEdge > 0) {
		_PrintTime("\t\t_CreateBlankEdge 시작");
		std::cout << "\t\t m_nCreatedMemoryEdge : " << m_nCreatedMemoryEdge << " m_EdgeCache.size() : " << m_EdgeCache.size() << "\n";
		_CreateBlankEdge(m_nCreatedMemoryEdge);
		_PrintTime("\t\t_CreateBlankEdge 종료");
	}
	/////////////////////////////////////////
	// 모든 메모리 Edge에 실제 RID를 부여한다.
	/////////////////////////////////////////
	std::map<RID,EdgePtr>::iterator iter =  m_EdgeCache.begin();
	for (;iter != m_EdgeCache.end() ; iter++) {
		CTinOrientDBHalfEdge* pEdge = (CTinOrientDBHalfEdge*)(iter->second.get());
		if (pEdge->IsMemory()){
			char str[20];
			sprintf(str,"%d",m_NowEdgeID);

			String newRID	= m_EdgeClassID + str;
			String oldRID = pEdge->GetRID();

			m_MemoryRIDList[newRID] = oldRID;

			CTinOrientDBVertex* pVertex = (CTinOrientDBVertex*)(pEdge->GetVertex().get());
			if (pVertex->GetRIDHalfEdge() == oldRID) {
				pVertex->SetRIDHalfEdge(newRID);
			}
			pEdge->SetRID(newRID);
			pEdge->SetSyncDB();
			m_NowEdgeID++;
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	// 모든 Edge의 Pair, CCW, CW 에 대하여 실제 RID를 부여하고 DB Update 한다.
	/////////////////////////////////////////////////////////////////////////////
	_PrintTime("\t\t_FlushEdgeCache Pair, CCW, CW RID 부여 몇 Update 시작");
	iter =  m_EdgeCache.begin();
	int nUpdateCnt = 0;
	for (;iter != m_EdgeCache.end() ; iter++) {
		CTinOrientDBHalfEdge* pEdge = (CTinOrientDBHalfEdge*)(iter->second.get());
		if(pEdge->IsDirty() == false) {
			continue;
		}
		pEdge->SetRIDPair(pEdge->GetRIDPair());
		pEdge->SetRIDCCW(pEdge->GetRIDCCW());
		pEdge->SetRIDCW(pEdge->GetRIDCW());
		_UpdateHalfEdge(pEdge);
		if (pEdge->IsDeleted()) {
			_DeleteEdge(pEdge->GetRID());
		}
		nUpdateCnt++;
	}
	std::cout << "\t\t UpdateCnt : " << nUpdateCnt << "\n";
	_PrintTime("\t\t_FlushEdgeCache Pair, CCW, CW RID 부여 몇 Update 종료");

	//////////////////////////////////////////////
	// 참조가 없는 EdgePtr을 Cache에서 제거한다.
	//////////////////////////////////////////////
	iter =  m_EdgeCache.begin();
	for (;iter != m_EdgeCache.end() ; iter++) {
		if (iter->second.use_count() < 2) {
			CTinOrientDBHalfEdge* pEdge = (CTinOrientDBHalfEdge*)(iter->second.get());
			std::map<RID,RID>::iterator oldRIDIter = m_MemoryRIDList.find(pEdge->GetRID());
			if (oldRIDIter != m_MemoryRIDList.end()) {
				int k = m_EdgeCache.erase(oldRIDIter->second);
				m_MemoryRIDList.erase(oldRIDIter);
				if (k != 1) {
					 m_EdgeCache.erase(iter);
				}
			} else {
				m_EdgeCache.erase(iter);
			}
		}
	}

	// 관련 변수 초기화
	m_nCreatedMemoryEdge = 0;

	_PrintTime("\t_FlushEdgeCache 종료");
}

void CTinOrientDBStorage::_FlushVertexCache()
{
	if (m_VertexCache.size() < 1) {
		return;
	}
	//std::cout << "_FlushVertexCache - Sta : m_VertexCache : " << m_VertexCache.size() << "\n";
	std::map<RID,VertexPtr>::iterator iter =  m_VertexCache.begin();
	for (;iter != m_VertexCache.end() ; iter++) {
		if (iter->second.use_count() < 2) {
			_UpdateVertex((CTinOrientDBVertex*)(iter->second).get());
			m_VertexCache.erase(iter);
		}
	}
//	std::cout << "_FlushVertexCache - End : m_VertexCache : " << m_VertexCache.size() << "\n";
}

void CTinOrientDBStorage::_UpdateVertex(CTinOrientDBVertex* pVertex)
{
	jstring jStrRID = m_JNIEnv->NewStringUTF(pVertex->GetRID().c_str());
	jstring jStrHalfEdge = m_JNIEnv->NewStringUTF(pVertex->GetRIDHalfEdge().c_str());
	m_JNIEnv->CallBooleanMethod(m_JNIOrientLibObject, m_JNIFuncUpdateVertex, jStrRID, pVertex->GetX(), pVertex->GetY(), jStrHalfEdge);
	m_JNIEnv->DeleteLocalRef(jStrRID);
	m_JNIEnv->DeleteLocalRef(jStrHalfEdge);
}

void CTinOrientDBStorage::_UpdateHalfEdge(CTinOrientDBHalfEdge* pEdge)
{
	jstring jStrRID = m_JNIEnv->NewStringUTF(pEdge->GetRID().c_str());
	jstring jStrOut = m_JNIEnv->NewStringUTF(pEdge->GetRIDVertex().c_str());
	jstring jStrIn = m_JNIEnv->NewStringUTF(pEdge->GetRIDEndVertex().c_str());
	jstring jStrPair = m_JNIEnv->NewStringUTF(pEdge->GetRIDPair().c_str());
	jstring jStrCCW = m_JNIEnv->NewStringUTF(pEdge->GetRIDCCW().c_str());
	jstring jStrCW = m_JNIEnv->NewStringUTF(pEdge->GetRIDCW().c_str());

	m_JNIEnv->CallBooleanMethod(m_JNIOrientLibObject, m_JNIFuncUpdateEdge, jStrRID, jStrOut, jStrIn, jStrPair, jStrCCW, jStrCW);
	m_JNIEnv->DeleteLocalRef(jStrRID);
	m_JNIEnv->DeleteLocalRef(jStrOut);
	m_JNIEnv->DeleteLocalRef(jStrIn);
	m_JNIEnv->DeleteLocalRef(jStrPair);
	m_JNIEnv->DeleteLocalRef(jStrCCW);
	m_JNIEnv->DeleteLocalRef(jStrCW);
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
	if ((int)m_VertexCache.size() >= m_MaXVertexCache) {
		_FlushVertexCache();
	}

	char buf[100];
	sprintf(buf, "%d", idx);
	String strIndex = buf;
	std::map<RID,VertexPtr>::iterator iter = m_VertexCache.find(strIndex);

	if (iter != m_VertexCache.end()) {
		return iter->second;
	}
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
	if (startPostion >= (int)json.length()) {
		endPostion = json.length() - 1;
	} else if ((endPostion = json.substr(startPostion + 1, json.length() - startPostion).find(',')) == -1) {
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
	if ((int)m_VertexCache.size() >= m_MaXVertexCache) {
		_FlushVertexCache();
	}

	int start = vertexRID.find(":") + 1;
	String strIndex = vertexRID.substr(start, vertexRID.length() - start );
	std::map<RID,VertexPtr>::iterator iter = m_VertexCache.find(strIndex);

	if (iter != m_VertexCache.end()) {
		return iter->second;
	}

	jstring a = m_JNIEnv->NewStringUTF(vertexRID.c_str());
	jstring jStrVertex =  (jstring)m_JNIEnv->CallObjectMethod(m_JNIOrientLibObject, m_JNIFuncGetVertex, a);
	jboolean bInIsCopy;
	const char* strCln = m_JNIEnv->GetStringUTFChars(jStrVertex, &bInIsCopy);
	String strV = strCln;
	m_JNIEnv->ReleaseStringUTFChars(jStrVertex, strCln);
	VertexPtr vPtr = _GetStringToVertex(strV);
	m_VertexCache[strIndex] = vPtr;

	m_JNIEnv->DeleteLocalRef(a);
	m_JNIEnv->DeleteLocalRef(jStrVertex);
	return vPtr;
}

EdgePtr CTinOrientDBStorage::GetHalfEdge(RID RealEdgeRID)
{
	RID EdgeRID = RealEdgeRID;
	std::map<RID,RID>::iterator oldRIDIter = m_MemoryRIDList.find(RealEdgeRID);
	if (oldRIDIter != m_MemoryRIDList.end()) {
		EdgeRID = oldRIDIter->second;
	}

	std::map<RID,EdgePtr>::iterator iter = m_EdgeCache.find(EdgeRID);

	if (iter != m_EdgeCache.end()) {
		return iter->second;
	}

	jstring a = m_JNIEnv->NewStringUTF(RealEdgeRID.c_str());
	jstring jStrVertex =  (jstring)m_JNIEnv->CallObjectMethod(m_JNIOrientLibObject, m_JNIFuncGetEdge, a);
	jboolean bInIsCopy;
	const char* strCln = m_JNIEnv->GetStringUTFChars(jStrVertex, &bInIsCopy);
	String strE = strCln;
	m_JNIEnv->ReleaseStringUTFChars(jStrVertex, strCln);

	m_JNIEnv->DeleteLocalRef(jStrVertex);
	m_JNIEnv->DeleteLocalRef(a);

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
	char str[100];
	::sprintf(str,"%d", m_nTotalCreateEdgeCount);

	RID strEdgeRID = str;

	CTinOrientDBHalfEdge* pHalfEdge = new CTinOrientDBHalfEdge();
	pHalfEdge->SetRID(strEdgeRID);
	EdgePtr edgePtr = EdgePtr(pHalfEdge);
	m_EdgeCache[strEdgeRID] = edgePtr;
	m_nTotalCreateEdgeCount++;
	m_nCreatedMemoryEdge++;

	return edgePtr;
}

int	CTinOrientDBStorage::GetCountOfEdges()
{
	int ret = m_JNIEnv->CallIntMethod(m_JNIOrientLibObject, m_JNIFuncGetCountOfEdges, NULL);

	return ret;
}

bool CTinOrientDBStorage::DeleteHalfEdge(EdgePtr pEdge)
{
	CTinOrientDBHalfEdge* pDBEdge = ((CTinOrientDBHalfEdge*)(pEdge.get()));

	if (pDBEdge->IsMemory()) {
	//	m_nCreatedMemoryEdge--;
	}

	pDBEdge->SetDelete();
	m_nTotalRemoveEdgeCount++;
	return true;
}

void CTinOrientDBStorage::PrintEdgeList()
{
	std::cout << "### m_CreateEdgeCount " << m_nTotalCreateEdgeCount << "\n";
	std::cout << "### m_RemoveEdgeCount " << m_nTotalRemoveEdgeCount << "\n";
}
