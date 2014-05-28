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
	typedef std::set<ITinHalfEdge*> HalfEdgeList;
	typedef std::vector<ITinVertex*> VertexList;

	virtual ITinVertex* 		GetVertex(int idx);
	virtual int 				GetCountOfVertexs();

	virtual ITinHalfEdge* 	CreateEdge();
	virtual int				GetCountOfEdges();

	//////////////////////////////
	ITinVertex* 	CreateVertex();
	void 			SetRamdomVertexs(int DATA_NUM);

private:
	HalfEdgeList 	m_HalfEdgeList;
	VertexList 	m_VertexList;
};
