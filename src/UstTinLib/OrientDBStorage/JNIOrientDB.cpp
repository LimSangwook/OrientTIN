#include "JNIOrientDB.h"
#include "TinOrientDBVertex.h"

CJNIOrientDB::CJNIOrientDB()
{
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
	m_JNIFuncRemoveDeletedEdge = NULL;
	m_JNIFuncCreateEdges = NULL;
	m_JNIFuncGetNextEdgeRID = NULL;
}

bool CJNIOrientDB::InitDB(
		String url, String dbName, String id, String pw, String vertexClassName, String indexName, String edgeClassName)
{
	if (!_InitJNI()) {
		return false;
	}
	String port = "2424";

	jstring a = m_JNIEnv->NewStringUTF(url.c_str());
	jstring b = m_JNIEnv->NewStringUTF(dbName.c_str());
	jstring c = m_JNIEnv->NewStringUTF(port.c_str());
	jstring d = m_JNIEnv->NewStringUTF(id.c_str());
	jstring e = m_JNIEnv->NewStringUTF(pw.c_str());
	jstring f = m_JNIEnv->NewStringUTF(vertexClassName.c_str());
	jstring g = m_JNIEnv->NewStringUTF(indexName.c_str());
	jstring h = m_JNIEnv->NewStringUTF(edgeClassName.c_str());

	jboolean ret = m_JNIEnv->CallBooleanMethod(m_JNIOrientLibObject, m_JNIFuncInitDB, a, b, c, d, e, f, g, h);
	m_JNIEnv->DeleteLocalRef(a);
	m_JNIEnv->DeleteLocalRef(b);
	m_JNIEnv->DeleteLocalRef(c);
	m_JNIEnv->DeleteLocalRef(d);
	m_JNIEnv->DeleteLocalRef(e);
	m_JNIEnv->DeleteLocalRef(f);
	m_JNIEnv->DeleteLocalRef(g);
	m_JNIEnv->DeleteLocalRef(h);

	if (!ret){
		return false;
	}

	return true;
}

bool CJNIOrientDB::_InitJNI()
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

JNIEnv* CJNIOrientDB::_Create_VM(JavaVM ** jvm)
{
    JNIEnv *env;
    JavaVMInitArgs vm_args;
    JavaVMOption options[1];
    options[0].optionString = (char*)"-Djava.class.path=./:/home/iswook/workspace2/OrientLib/bin:/usr/lib/jvm/java-6-oracle/jre/lib/charsets.jar:/usr/lib/jvm/java-6-oracle/jre/lib/jce.jar:/usr/lib/jvm/java-6-oracle/jre/lib/jsse.jar:/usr/lib/jvm/java-6-oracle/jre/lib/resources.jar:/usr/lib/jvm/java-6-oracle/jre/lib/rt.jar:/usr/lib/jvm/java-6-oracle/jre/lib/ext/dnsns.jar:/usr/lib/jvm/java-6-oracle/jre/lib/ext/localedata.jar:/usr/lib/jvm/java-6-oracle/jre/lib/ext/sunjce_provider.jar:/usr/lib/jvm/java-6-oracle/jre/lib/ext/sunpkcs11.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/activation-1.1.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/antlr-2.7.7.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/asm-3.2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/asm-analysis-3.2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/asm-commons-3.2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/asm-tree-3.2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/asm-util-3.2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/blueprints-core-2.5.0-20140320.105052-37.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/colt-1.2.0.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/commons-beanutils-1.7.0.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/commons-beanutils-core-1.8.0.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/commons-collections-3.2.1.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/commons-configuration-1.6.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/commons-digester-1.8.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/commons-lang-2.4.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/commons-logging-1.0.4.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/concurrent-1.3.4.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/concurrentlinkedhashmap-lru-1.4.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/gremlin-groovy-2.5.0-20140125.153413-8.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/gremlin-java-2.5.0-20140125.153323-8.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/groovy-1.8.9.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/hazelcast-3.1.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/hibernate-jpa-2.0-api-1.0.0.Final.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jackson-annotations-2.2.2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jackson-core-2.2.2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jackson-databind-2.2.2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jackson-datatype-json-org-2.2.3.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jansi-1.5.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/javassist-3.16.1-GA.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jettison-1.3.3.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jline-0.9.94.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jna-4.0.0.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/jna-platform-4.0.0.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/json-20090211.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/mail-1.4.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orient-commons-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-client-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-core-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-distributed-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-enterprise-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-graphdb-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-nativeos-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-object-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-server-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/orientdb-tools-1.7-rc2.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/pipes-2.5.0-20140125.162807-6.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/snappy-0.3.jar:/home/iswook/server/orientdb-community-1.7-rc2/lib/stax-api-1.0.1.jar ";
    vm_args.version = JNI_VERSION_1_6; //JDK version. This indicates version 1.6
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = 0;

    int ret = JNI_CreateJavaVM(jvm, (void**)&env, &vm_args);
    if(ret < 0)
    	printf("\nUnable to Launch JVM\n");
	return env;
}

bool CJNIOrientDB::_GetJNIMethodID()
{
	m_JNIFuncInit 				= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "<init>", "()V");
	m_JNIFuncInitDB 				= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "InitDB", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z");
	m_JNIFuncCreateEdge 			= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "CreateEdge", "()Ljava/lang/String;");
	m_JNIFuncGetVertex 			= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "GetVertex", "(Ljava/lang/String;)Ljava/lang/String;");
	m_JNIFuncGetVertexFromIdx 	= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "GetVertexFromIndex", "(I)Ljava/lang/String;");
	m_JNIFuncGetCountOfVertexs 	= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "GetCountOfVertexs", "()I");
	m_JNIFuncGetCountOfEdges 	= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "GetCountOfEdges", "()I");
	m_JNIFuncUpdateEdge 			= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "UpdateEdge", "(Ljava/lang/String;)Z");
	m_JNIFuncUpdateVertex 		= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "UpdateVertex", "(Ljava/lang/String;DDLjava/lang/String;)Z");
	m_JNIFuncDeleteEdge 			= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "DeleteHalfEdge", "(Ljava/lang/String;)Z");
	m_JNIFuncSetRandomVertex		= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "SetRandomVertex", "(I)Z");
	m_JNIFuncGetEdge				= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "GetEdge", "(Ljava/lang/String;)Ljava/lang/String;");
	m_JNIFuncCreateBlankEdge		= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "CreateBlankEdge", "(I)Ljava/lang/String;");
	m_JNIFuncRemoveDeletedEdge	= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "RemoveDeletedEdge", "()Z");
	m_JNIFuncGetNextEdgeRID		= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "GetNextEdgeRID", "()Ljava/lang/String;");
	m_JNIFuncCreateEdges			= m_JNIEnv->GetMethodID(m_JNIOrientLibClass, "CreateEdges", "(Ljava/lang/String;)Z");

	return m_JNIFuncInitDB && 				m_JNIFuncCreateEdge &&			m_JNIFuncGetVertex &&
			m_JNIFuncGetVertexFromIdx && 	m_JNIFuncGetCountOfVertexs &&	m_JNIFuncGetCountOfEdges &&
			m_JNIFuncUpdateEdge && 			m_JNIFuncUpdateVertex && 		m_JNIFuncDeleteEdge &&
			m_JNIFuncSetRandomVertex &&		m_JNIFuncInit	&&					m_JNIFuncGetEdge &&
			m_JNIFuncCreateBlankEdge &&		m_JNIFuncRemoveDeletedEdge&&	m_JNIFuncGetNextEdgeRID&&
			m_JNIFuncCreateEdges;
}

String CJNIOrientDB::GetNextEdgeRID()
{
	jstring jStrEdgeRID =  (jstring)m_JNIEnv->CallObjectMethod(m_JNIOrientLibObject, m_JNIFuncGetNextEdgeRID);
	jboolean bInIsCopy;
	const char* strCln = m_JNIEnv->GetStringUTFChars(jStrEdgeRID, &bInIsCopy);
	String strRID = strCln;
	m_JNIEnv->ReleaseStringUTFChars(jStrEdgeRID, strCln);
	return strRID;
}

bool CJNIOrientDB::CreateEdges(String& strEdgeDatas)
{
	if (strEdgeDatas.length() == 0) {
		return true;
	}

	jstring jStrEdgeDatas = m_JNIEnv->NewStringUTF(strEdgeDatas.c_str());

	m_JNIEnv->CallBooleanMethod(m_JNIOrientLibObject, m_JNIFuncCreateEdges, jStrEdgeDatas);
	m_JNIEnv->DeleteLocalRef(jStrEdgeDatas);
	strEdgeDatas = "";
	return true;
}

String CJNIOrientDB::CreateBlankEdge(int num)
{
	jstring jStrEdgeRID =  (jstring)m_JNIEnv->CallObjectMethod(m_JNIOrientLibObject, m_JNIFuncCreateBlankEdge, num);
	jboolean bInIsCopy;
	const char* strCln = m_JNIEnv->GetStringUTFChars(jStrEdgeRID, &bInIsCopy);
	String strRID = strCln;
	m_JNIEnv->ReleaseStringUTFChars(jStrEdgeRID, strCln);
	return strRID;
}

bool CJNIOrientDB::RemoveDeletedEdge()
{
	m_JNIEnv->CallBooleanMethod(m_JNIOrientLibObject, m_JNIFuncRemoveDeletedEdge);
	return true;
}

void CJNIOrientDB::DeleteEdge(RID& edgeRID)
{
	if (edgeRID.length() == 0) {
		return;
	}
	jstring jStrRID = m_JNIEnv->NewStringUTF(edgeRID.c_str());
	m_JNIEnv->CallBooleanMethod(m_JNIOrientLibObject, m_JNIFuncDeleteEdge, jStrRID);
	m_JNIEnv->DeleteLocalRef(jStrRID);
	edgeRID ="";
}

void CJNIOrientDB::UpdateVertex(CTinOrientDBVertex* pVertex)
{
	jstring jStrRID = m_JNIEnv->NewStringUTF(pVertex->GetRID().c_str());
	jstring jStrHalfEdge = m_JNIEnv->NewStringUTF(pVertex->GetRIDHalfEdge().c_str());
	m_JNIEnv->CallBooleanMethod(m_JNIOrientLibObject, m_JNIFuncUpdateVertex, jStrRID, pVertex->GetX(), pVertex->GetY(), jStrHalfEdge);
	m_JNIEnv->DeleteLocalRef(jStrRID);
	m_JNIEnv->DeleteLocalRef(jStrHalfEdge);
}

void CJNIOrientDB::UpdateHalfEdge(String& strEdgeDatas)
{
	if (strEdgeDatas.length() == 0) {
		return;
	}

	jstring jStrEdgeDatas = m_JNIEnv->NewStringUTF(strEdgeDatas.c_str());

	m_JNIEnv->CallBooleanMethod(m_JNIOrientLibObject, m_JNIFuncUpdateEdge, jStrEdgeDatas);
	m_JNIEnv->DeleteLocalRef(jStrEdgeDatas);
	strEdgeDatas = "";
}

bool CJNIOrientDB::SetCleanNRamdomVertexs(int DataNum)
{
	jboolean ret = m_JNIEnv->CallIntMethod(m_JNIOrientLibObject, m_JNIFuncSetRandomVertex, DataNum);

	if (!ret){
		return false;
	}

	return true;
}

int CJNIOrientDB::GetCountOfVertexs()
{
	return  m_JNIEnv->CallIntMethod(m_JNIOrientLibObject, m_JNIFuncGetCountOfVertexs, NULL);
}

int	CJNIOrientDB::GetCountOfEdges()
{
	return m_JNIEnv->CallIntMethod(m_JNIOrientLibObject, m_JNIFuncGetCountOfEdges, NULL);
}

String CJNIOrientDB::GetHalfEdge(RID RealEdgeRID)
{
	jstring a = m_JNIEnv->NewStringUTF(RealEdgeRID.c_str());
	jstring jStrVertex =  (jstring)m_JNIEnv->CallObjectMethod(m_JNIOrientLibObject, m_JNIFuncGetEdge, a);
	jboolean bInIsCopy;
	const char* strCln = m_JNIEnv->GetStringUTFChars(jStrVertex, &bInIsCopy);
	String strE = strCln;
	m_JNIEnv->ReleaseStringUTFChars(jStrVertex, strCln);
	m_JNIEnv->DeleteLocalRef(jStrVertex);
	m_JNIEnv->DeleteLocalRef(a);
	return strE;
}

String CJNIOrientDB::GetVertex(RID vertexRID)
{
	jstring a = m_JNIEnv->NewStringUTF(vertexRID.c_str());
	jstring jStrVertex =  (jstring)m_JNIEnv->CallObjectMethod(m_JNIOrientLibObject, m_JNIFuncGetVertex, a);
	jboolean bInIsCopy;
	const char* strCln = m_JNIEnv->GetStringUTFChars(jStrVertex, &bInIsCopy);
	String strV = strCln;
	m_JNIEnv->ReleaseStringUTFChars(jStrVertex, strCln);
	m_JNIEnv->DeleteLocalRef(a);
	m_JNIEnv->DeleteLocalRef(jStrVertex);
	return strV;
}

String CJNIOrientDB::GetVertex(int idx)
{
	jstring jStrVertex =  (jstring)m_JNIEnv->CallObjectMethod(m_JNIOrientLibObject, m_JNIFuncGetVertexFromIdx, idx);
	jboolean bInIsCopy;
	const char* strCln = m_JNIEnv->GetStringUTFChars(jStrVertex, &bInIsCopy);
	String strV = strCln;
	m_JNIEnv->ReleaseStringUTFChars(jStrVertex, strCln);
	return strV;
}
