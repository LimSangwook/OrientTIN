#ifndef __JNI_ORIENTDB_H__
#define __JNI_ORIENTDB_H__
#include "../Common.h"
#include "jni.h"

class CTinOrientDBVertex;
class CJNIOrientDB
{
public:
	CJNIOrientDB();
	bool 		InitDB(String url, String dbName, String id, String pw, String vertexClassName, String indexName, String edgeClassName);

	String 	CreateBlankEdge(int num);
	void 		DeleteEdge(RID& edgeRID);
	bool 		SetCleanNRamdomVertexs(int DataNum);

	void 		UpdateVertex(CTinOrientDBVertex* pVertex);
	void 		UpdateHalfEdge(String& strEdgeDatas);

	int  		GetCountOfVertexs();
	int	 		GetCountOfEdges();
	String 	GetHalfEdge(RID RealEdgeRID);
	String 	GetVertex(RID vertexRID);
	String 	GetVertex(int idx);
	bool 		RemoveDeletedEdge(String NullFaceEdge);
	String 	GetNextEdgeRID();
	bool		CreateEdges(String& strEdgeDatas);
private:
	bool 		_InitJNI();
	JNIEnv* 	_Create_VM(JavaVM ** jvm);
	bool 		_GetJNIMethodID();

private:
	JNIEnv* 	m_JNIEnv;
	JavaVM* 	m_JNIJvm;
	jclass  	m_JNIOrientLibClass;
	jobject	m_JNIOrientLibObject;
	// Function
	jmethodID 	m_JNIFuncInit;
	jmethodID 	m_JNIFuncInitDB;
	jmethodID 	m_JNIFuncCreateEdge;
	jmethodID 	m_JNIFuncGetVertex;
	jmethodID 	m_JNIFuncGetVertexFromIdx;
	jmethodID 	m_JNIFuncGetEdge;
	jmethodID 	m_JNIFuncCreateBlankEdge;

	jmethodID 	m_JNIFuncGetCountOfVertexs;
	jmethodID 	m_JNIFuncGetCountOfEdges;
	jmethodID 	m_JNIFuncUpdateEdge;
	jmethodID 	m_JNIFuncUpdateVertex;
	jmethodID 	m_JNIFuncDeleteEdge;
	jmethodID 	m_JNIFuncSetRandomVertex;
	jmethodID 	m_JNIFuncRemoveDeletedEdge;

	jmethodID 	m_JNIFuncGetNextEdgeRID;
	jmethodID 	m_JNIFuncCreateEdges;

};

#endif //__JNI_ORIENTDB_H__
