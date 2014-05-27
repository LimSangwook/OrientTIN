#include "TinDelaunay.h"

CTinDelaunay::CTinDelaunay():
 m_pRightMost_Edge(0), m_pLeftMost_Edge(0), m_nNumFaces(0), m_StartPointIdx(-1), m_EndPointIdx(-1)
{
}

CTinDelaunay::~CTinDelaunay()
{
}

void CTinDelaunay::SetRightMostEdge(CTinHalfEdge* pEdge)
{
	m_pRightMost_Edge = pEdge;
}

void CTinDelaunay::SetLeftMostEdge(CTinHalfEdge* pEdge)
{
	m_pLeftMost_Edge = pEdge;
}

int CTinDelaunay::GetStartPointIdx()
{
	return m_StartPointIdx;
}

int CTinDelaunay::GetEndPointIdx()
{
	return m_EndPointIdx;
}

void CTinDelaunay::SetStartPointIdx(int idx)
{
	m_StartPointIdx = idx;
}

void CTinDelaunay::SetEndPointIdx(int idx)
{
	m_EndPointIdx = idx;
}

CTinHalfEdge* CTinDelaunay::GetRightMostEdge()
{
	return m_pRightMost_Edge;
}

CTinHalfEdge* CTinDelaunay::GetLeftMostEdge()
{
	return m_pLeftMost_Edge;
}

CTinDelaunay::FaceList& CTinDelaunay::GetFaces()
{
	return m_FaceList;
}

void CTinDelaunay::SetFaces(FaceList& pFaces)
{
	m_FaceList = pFaces;
}

int CTinDelaunay::GetNumberOfFaces()
{
	return m_nNumFaces;
}

void CTinDelaunay::SetNumberOfFaces(int num)
{
	m_nNumFaces = num;
}
