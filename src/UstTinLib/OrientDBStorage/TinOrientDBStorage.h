#ifndef __UST_TIN_MAKER_H__
#define __UST_TIN_MAKER_H__

#include <liborient/liborient.h>
#include "../Common.h"
#include "TinOrientDBVertex.h"
#include "TinOrientDBHalfEdge.h"
#include "/usr/lib/jvm/java-7-openjdk-amd64/include/jni.h"
#include <map>

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
	/////////////////////////////////////////
	virtual VertexPtr 	GetVertex(int idx);
	virtual int 			GetCountOfVertexs();

	virtual EdgePtr 		CreateEdge();
	virtual int			GetCountOfEdges();
	virtual bool 			DeleteHalfEdge(EdgePtr pEdge);
	virtual void			PrintEdgeList();
	virtual void			FlushCache();
	virtual void			Close();

public:
	CTinOrientDBStorage();
	~CTinOrientDBStorage();

	VertexPtr 		GetVertex(RID nRID);
	EdgePtr 		GetHalfEdge(RID EdgeRID);

	ErrCode 		InitDB(String url, String dbName, String id, String pw, String vertexClassName, String edgeClassName);
	bool 			SetCleanNRamdomVertexs(int DataNum);

	bool			_CreateBlankEdge(int num);
private:
	void 			_UpdateHalfEdge(CTinOrientDBHalfEdge* pEdge);
	void 			_UpdateVertex(CTinOrientDBVertex* pVertex);
	void 			_FlushEdgeCache();
	void 			_FlushVertexCache();
	String 		_GetProperty(String json, String propertyName);
	void 			_DeleteEdge(RID edgeRID);

	//////////////////////////
	// JNI 관련 함수
	//////////////////////////
	bool 			_InitJNI();
	bool 			_GetJNIMethodID();
	JNIEnv* 		_Create_VM(JavaVM ** jvm);
	VertexPtr 		_GetStringToVertex(String& str);

private:
	///////////////////////////
	// Cache 용
	///////////////////////////
	std::map<RID,EdgePtr> 	m_EdgeCache;
	std::map<RID,VertexPtr> 	m_VertexCache;
	std::map<RID,RID> 		m_MemoryRIDList;		// std::map<실제rid, 가상rid>
	int							m_MaXVertexCache;
	int							m_MaXEdgeCache;

	///////////////////////////
	// 통게용
	///////////////////////////
	int 						m_nTotalRemoveEdgeCount;
	int 						m_nTotalCreateEdgeCount;
	int							m_nCreatedMemoryEdge;

	///////////////////////////
	// Etc.
	///////////////////////////
	int							m_NowEdgeID;
	int							m_MaxEdgeID;
	String						m_EdgeClassID;

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
	jmethodID 	m_JNIFuncCreateBlankEdge;

	jmethodID 	m_JNIFuncGetCountOfVertexs;
	jmethodID 	m_JNIFuncGetCountOfEdges;
	jmethodID 	m_JNIFuncUpdateEdge;
	jmethodID 	m_JNIFuncUpdateVertex;
	jmethodID 	m_JNIFuncDeleteEdge;
	jmethodID 	m_JNIFuncSetRandomVertex;

};

#endif //__UST_TIN_MAKER_H__
