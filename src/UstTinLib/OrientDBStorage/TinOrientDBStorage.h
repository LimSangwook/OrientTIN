#ifndef __UST_TIN_MAKER_H__
#define __UST_TIN_MAKER_H__

#include <liborient/liborient.h>
#include "../Common.h"
#include "../BaseStorage/ITinStorageManager.h"
#include "TinOrientDBVertex.h"
#include "TinOrientDBHalfEdge.h"
#include "/usr/lib/jvm/java-7-openjdk-amd64/include/jni.h"

class CTinOrientDBHalfEdge;
class CTinOrientDBStorage : ITinStorageManager
{
public:
	static CTinOrientDBStorage* instance;
	static CTinOrientDBStorage* GetInstance() {
		return instance;
	}

	//TinMaker에서 사용될 Error Code를 지정한다.
	enum ErrCode{
		RET_OK,						// ERROR 없음
		ERR_DB_INIT,
		ERR_ETC						// 기타 ERROR
	};
public:
	/////////////////////////////////////////
	// ITinStorageManager Interface 구현
	virtual VertexPtr 		GetVertex(int idx);
	virtual int 				GetCountOfVertexs();

	virtual EdgePtr 			CreateEdge();
	virtual int				GetCountOfEdges();

	virtual VertexPtr 		GetVertex(RID nRID);
	virtual bool 				DeleteHalfEdge(EdgePtr pEdge);
	virtual void				PrintEdgeList();

public:
	CTinOrientDBStorage();
	~CTinOrientDBStorage();
	ErrCode InitDB(String url, String dbName, String id, String pw, String vertexClassName, String edgeClassName);

	EdgePtr GetHalfEdge(RID EdgeRID);
	void UpdateVertex(CTinOrientDBVertex* pVertex);
	void UpdateHalfEdge(CTinOrientDBHalfEdge* pEdge);

	void ReLoadVertex(CTinOrientDBVertex* pVertex);
	void ReLoadHalfEdge(CTinOrientDBHalfEdge* pEdge);


	bool SetCleanNRamdomVertexs(int DataNum);

private:
	String _GetProperty(String json, String propertyName);

	//////////////////////////
	// JNI 관련 함수
	bool _InitJNI();
	bool _GetJNIMethodID();
	JNIEnv* _Create_VM(JavaVM ** jvm);
	VertexPtr _GetStringToVertex(String& str);

private:
	////////////////////////////
	// JNI 호출 관련
	////////////////////////////
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

	jmethodID 	m_JNIFuncGetCountOfVertexs;
	jmethodID 	m_JNIFuncGetCountOfEdges;
	jmethodID 	m_JNIFuncUpdateEdge;
	jmethodID 	m_JNIFuncUpdateVertex;
	jmethodID 	m_JNIFuncDeleteEdge;
	jmethodID 	m_JNIFuncSetRandomVertex;
};

#endif //__UST_TIN_MAKER_H__
