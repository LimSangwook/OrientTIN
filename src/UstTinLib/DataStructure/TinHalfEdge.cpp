#include "TinHalfEdge.h"

CTinHalfEdge::CTinHalfEdge() :
m_pVertex(0), m_pPair(0), m_pCCW(0), m_pCW(0), m_pFace(0)
{

}

CTinHalfEdge::~CTinHalfEdge()
{
}

CTinVertex* CTinHalfEdge::GetVertex()
{
	return m_pVertex;
}

CTinHalfEdge* CTinHalfEdge::GetPairEdge()
{
	return m_pPair;
}

CTinHalfEdge* CTinHalfEdge::GetCCWEdge()
{
	return m_pCCW;
}

CTinHalfEdge* CTinHalfEdge::GetCWEdge()
{
	return m_pCW;
}

void CTinHalfEdge::SetVertex(CTinVertex* pVertex)
{
	m_pVertex = pVertex;
}

void CTinHalfEdge::SetPairEdge(CTinHalfEdge* pEdge)
{
	m_pPair = pEdge;
}

void CTinHalfEdge::SetCCWEdge(CTinHalfEdge* pEdge)
{
	m_pCCW = pEdge;
}

void CTinHalfEdge::SetCWEdge(CTinHalfEdge* pEdge)
{
	m_pCW = pEdge;
}

CTinFace* CTinHalfEdge::GetFace()
{
	return m_pFace;
}
void CTinHalfEdge::SetFace(CTinFace* pFace)
{
	m_pFace = pFace;
}

