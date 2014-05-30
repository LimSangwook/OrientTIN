#include <set>
#include <vector>
#include "../BaseStorage/ITinStorageManager.h"

class ITinHalfEdge;
class ITinVertex;

class CTinMemStorage : public ITinStorageManager
{
public:
	//////////////////////////////
	// ITinStorageManager 구현
	typedef std::vector<VertexPtr> VertexList;
	typedef std::set<ITinHalfEdge*> HalfEdgeList;

	virtual VertexPtr 		GetVertex(int idx);
	virtual int 				GetCountOfVertexs();

	virtual ITinHalfEdge* 	CreateEdge();
	virtual int				GetCountOfEdges();

	virtual bool 				DeleteHalfEdge(ITinHalfEdge* pEdge);
	virtual void				PrintEdgeList();

	//////////////////////////////
	ITinVertex* 	CreateVertex();
	void 			SetRamdomVertexs(int DATA_NUM);

private:
	VertexList 	m_VertexList;
	HalfEdgeList 	m_HalfEdgeList;
};
