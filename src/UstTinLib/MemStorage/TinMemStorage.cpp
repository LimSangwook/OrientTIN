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

ITinVertex* CTinMemStorage::GetVertex(int idx)
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
	m_VertexList.push_back(pVertex);
	return pVertex;
}

void CTinMemStorage::SetRamdomVertexs(int DATA_NUM)
{
	int sq =(int) sqrt((double)DATA_NUM);
	int idx = 0;
	for (int i = 0 ; i < sq * 10 + 1 ; i += 10) {
		for(int j = 0 ; j < sq * 10 + 1 ; j += 10){
			double  x = (double)i;
			double y = (double)j;
			ITinVertex* pVertex = CreateVertex();
			pVertex->SetX(x);
			pVertex->SetY(y);
			idx++;
			if (idx == DATA_NUM) break;
		}
		if (idx == DATA_NUM) break;
	}
}
