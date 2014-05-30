#include "TinMemStorage.h"
#include "../BaseStorage/ITinHalfEdge.h"
#include "TinMemHalfEdge.h"
#include <math.h>


ITinHalfEdge* CTinMemStorage::CreateEdge()
{
	 CTinMemHalfEdge* pEdge = new CTinMemHalfEdge();
	 m_HalfEdgeList.insert(pEdge);
	 return pEdge;
}

int CTinMemStorage::GetCountOfVertexs()
{
	return m_VertexList.size();
}

VertexPtr CTinMemStorage::GetVertex(int idx)
{
	return m_VertexList.at(idx);
}

int	CTinMemStorage::GetCountOfEdges()
{
	return m_HalfEdgeList.size();
}

ITinVertex* CTinMemStorage::CreateVertex()
{
	CTinMemVertex* pVertex = new CTinMemVertex;
	m_VertexList.push_back(VertexPtr(pVertex));
	return pVertex;
}

bool CTinMemStorage::DeleteHalfEdge(ITinHalfEdge* pEdge)
{
	m_HalfEdgeList.erase(pEdge);
	delete pEdge;
	return true;
}

void CTinMemStorage::PrintEdgeList()
{
//	HalfEdgeList::iterator iter = m_HalfEdgeList.begin();
//	std::cout << "\n #### Edge List #### ";
//	for (int idx = 0; iter != m_HalfEdgeList.end() ; iter ++, idx++) {
//		CTinMemVertex* pSVertex = (CTinMemVertex*)((*iter)->GetVertex());
//		CTinMemVertex* pEVertex = (CTinMemVertex*)((*iter)->GetPairEdge()->GetVertex());
//
//		std::cout << idx << " : " << pSVertex->idx << " -> " << pEVertex->idx;
////		std::cout << " ccw : " << ((CTinMemVertex*)((*iter)->GetCCWEdge()->GetVertex()))->idx;
////		std::cout << " cw : " << ((CTinMemVertex*)((*iter)->GetCWEdge()->GetVertex()))->idx;
//		std::cout << "\n";
//	}
}

void CTinMemStorage::SetRamdomVertexs(int DATA_NUM)
{
	int sq =(int) sqrt((double)DATA_NUM);
	int idx = 0;
	for (int i = 0 ; i < sq * 10 + 1 ; i += 10) {
		for(int j = 0 ; j < sq * 10 + 1 ; j += 10){
			double  x = (double)i;
			double y = (double)j;
			CTinMemVertex* pVertex = (CTinMemVertex*) CreateVertex();
			pVertex->SetX(x);
			pVertex->SetY(y);
			pVertex->idx = idx;
			idx++;
			if (idx == DATA_NUM) break;
		}
		if (idx == DATA_NUM) break;
	}
}
