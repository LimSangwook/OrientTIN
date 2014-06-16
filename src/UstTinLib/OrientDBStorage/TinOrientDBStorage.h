#ifndef __UST_TIN_MAKER_H__
#define __UST_TIN_MAKER_H__

#include <liborient/liborient.h>
#include <map>
#include "../Common.h"
#include "TinOrientDBVertex.h"
#include "TinOrientDBHalfEdge.h"
#include "JNIOrientDB.h"

class CTinOrientDBHalfEdge;
class CTinOrientDBStorage : ITinStorageManager
{
public:
	/////////////////////////////////////////
	// Singleton 구현
	/////////////////////////////////////////
	static CTinOrientDBStorage* instance;
	static CTinOrientDBStorage* GetInstance() {
		return instance;
	}

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

	bool	 		InitDB(String url, String dbName, String id, String pw, String vertexClassName, String edgeClassName);
	bool 			SetCleanNRamdomVertexs(int DataNum);
	bool			_CreateBlankEdge(int num);

private:
	void 			_UpdateHalfEdge(String& strEdgeDatas);
	void 			_UpdateVertex(CTinOrientDBVertex* pVertex);
	void 			_FlushEdgeCache();
	void 			_FlushVertexCache();
	String 		_GetProperty(String json, String propertyName);
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
	// JNI OrientDB API(JAVA) CALL
	///////////////////////////
	CJNIOrientDB				m_JNIOrientDB;

	///////////////////////////
	// Etc.
	///////////////////////////
	int							m_NowEdgeID;
	int							m_MaxEdgeID;
	String						m_EdgeClassID;
};

#endif //__UST_TIN_MAKER_H__
