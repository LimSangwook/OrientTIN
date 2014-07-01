#include <map>
#include <vector>
#include "../Common.h"

class CTinMemStorage : public ITinStorageManager
{
public:
	static CTinMemStorage* instance;
	static CTinMemStorage* GetInstance() {
		return CTinMemStorage::instance;
	}
public:
	CTinMemStorage();
	~CTinMemStorage();
	//////////////////////////////
	// ITinStorageManager 구현
	typedef std::map<int, VertexPtr> VertexList;
	typedef std::map<int, EdgePtr> HalfEdgeList;

	virtual VertexPtr 		GetVertex(int idx);
	virtual int 				GetCountOfVertexs();

	virtual EdgePtr		 	CreateEdge();
	virtual int				GetCountOfEdges();

	virtual bool 				DeleteHalfEdge(EdgePtr pEdge);
	virtual void				PrintEdgeList();
	virtual void				FlushCache(){};
	virtual void				Close(){};
	virtual void				SetNullFaceEdge(EdgePtr pNullFaceEdge){};
	//////////////////////////////
	ITinVertex* 	CreateVertex();
	EdgePtr		GetEdge(int idx);
	void 			SetRamdomVertexs(int DATA_NUM);

private:
	int				m_EdgeIdx;
	VertexList 	m_VertexList;
	HalfEdgeList 	m_HalfEdgeList;
};
