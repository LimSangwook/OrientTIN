#include <math.h>
#include "../Common.h"
#include "TinMemStorage.h"
#include "TinMemHalfEdge.h"
#include "../BaseStorage/TinDelaunay.h"

CTinMemStorage::CTinMemStorage()
{
	instance = this;

	std::cout<<"sizeof(CTinDelaunay)" << sizeof(CTinDelaunay) <<"\n";
	std::cout<<"sizeof(CTinMemVertex)" << sizeof(CTinMemVertex) <<"\n";
	std::cout<<"sizeof(VertexPtr)" << sizeof(VertexPtr) <<"\n";
	std::cout<<"sizeof(CTinMemHalfEdge)" << sizeof(CTinMemHalfEdge) <<"\n";
	std::cout<<"sizeof(EdgePtr)" << sizeof(EdgePtr) <<"\n";
	m_EdgeIdx = 0;
}

CTinMemStorage::~CTinMemStorage()
{
	std::cout<<"m_EdgeIdx :" << m_EdgeIdx <<"\n";

//	VertexList::iterator iter = m_VertexList.begin();
//	for (int i = 0 ;iter != m_VertexList.end() ; iter++, i++) {
//		std::cout<<i << " : " << iter->use_count() <<"\n";
//	}
//
//	HalfEdgeList::iterator eIter = m_HalfEdgeList.begin();
//	for (int i = 0 ;eIter != m_HalfEdgeList.end() ; eIter++, i++) {
//		std::cout<<i << " : " << eIter->second.use_count() <<"\n";
//	}
}

EdgePtr CTinMemStorage::CreateEdge()
{

	EdgePtr edgePtr = EdgePtr(new CTinMemHalfEdge(m_EdgeIdx));
	m_HalfEdgeList[m_EdgeIdx] = (edgePtr);
	m_EdgeIdx++;
	return edgePtr;
}

EdgePtr CTinMemStorage::GetEdge(int idx)
{
	HalfEdgeList::iterator iter = m_HalfEdgeList.find(idx);
	if (iter != m_HalfEdgeList.end()) {
		return iter->second;
	}
	return EdgePtr();
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

	m_VertexList[m_VertexList.size()]= VertexPtr(pVertex);
	return pVertex;
}

bool CTinMemStorage::DeleteHalfEdge(EdgePtr pEdge)
{
	HalfEdgeList::iterator iter = m_HalfEdgeList.find(((CTinMemHalfEdge*)(pEdge.get()))->GetIdx());
	if (iter != m_HalfEdgeList.end()) {
		m_HalfEdgeList.erase(iter);
	}
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
