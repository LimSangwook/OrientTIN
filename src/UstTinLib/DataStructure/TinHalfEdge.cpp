#include "TinHalfEdge.h"

CTinHalfEdge::CTinHalfEdge() :
m_pVertex(0), m_pPair(0), m_pCCW(0), m_pCW(0), m_pFace(0)
{

}

CTinHalfEdge::~CTinHalfEdge()
{
}

ITinVertex* CTinHalfEdge::GetVertex()
{
	return m_pVertex;
}

ITinHalfEdge* CTinHalfEdge::GetPairEdge()
{
	return m_pPair;
}

ITinHalfEdge* CTinHalfEdge::GetCCWEdge()
{
	return m_pCCW;
}

ITinHalfEdge* CTinHalfEdge::GetCWEdge()
{
	return m_pCW;
}

void CTinHalfEdge::SetVertex(ITinVertex* pVertex)
{
	m_pVertex = pVertex;
}

void CTinHalfEdge::SetPairEdge(ITinHalfEdge* pEdge)
{
	m_pPair = pEdge;
}

void CTinHalfEdge::SetCCWEdge(ITinHalfEdge* pEdge)
{
	m_pCCW = pEdge;
}

void CTinHalfEdge::SetCWEdge(ITinHalfEdge* pEdge)
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

