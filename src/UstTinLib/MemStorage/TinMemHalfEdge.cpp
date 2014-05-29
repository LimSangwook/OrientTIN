#include "TinMemHalfEdge.h"

CTinMemHalfEdge::CTinMemHalfEdge() :
m_pVertex(0), m_pPair(0), m_pCCW(0), m_pCW(0), m_pFace(0)
{

}

CTinMemHalfEdge::~CTinMemHalfEdge()
{
}

ITinVertex* CTinMemHalfEdge::GetVertex()
{
	return m_pVertex;
}

ITinHalfEdge* CTinMemHalfEdge::GetPairEdge()
{
	return m_pPair;
}

ITinHalfEdge* CTinMemHalfEdge::GetCCWEdge()
{
	return m_pCCW;
}

ITinHalfEdge* CTinMemHalfEdge::GetCWEdge()
{
	return m_pCW;
}

void CTinMemHalfEdge::SetVertex(ITinVertex* pVertex)
{
	m_pVertex = pVertex;
}

void CTinMemHalfEdge::SetPairEdge(ITinHalfEdge* pEdge)
{
	m_pPair = pEdge;
}

void CTinMemHalfEdge::SetCCWEdge(ITinHalfEdge* pEdge)
{
	m_pCCW = pEdge;
}

void CTinMemHalfEdge::SetCWEdge(ITinHalfEdge* pEdge)
{
	m_pCW = pEdge;
}
bool CTinMemHalfEdge::equal(ITinHalfEdge* pOther)
{
	CTinMemHalfEdge* pMemEdge = dynamic_cast<CTinMemHalfEdge*>(pOther);
	if(pMemEdge) {
		if (this == pOther)
			return true;
	}
	return false;
}

CTinFace* CTinMemHalfEdge::GetFace()
{
	return m_pFace;
}
void CTinMemHalfEdge::SetFace(CTinFace* pFace)
{
	m_pFace = pFace;
}


