#include "TinMemHalfEdge.h"

CTinMemHalfEdge::CTinMemHalfEdge() :
m_pFace(0)
{
}

CTinMemHalfEdge::~CTinMemHalfEdge()
{
}

VertexPtr CTinMemHalfEdge::GetVertex()
{
	return m_pVertex;
}

EdgePtr CTinMemHalfEdge::GetPairEdge()
{
	return m_pPair;
}

EdgePtr CTinMemHalfEdge::GetCCWEdge()
{
	return m_pCCW;
}

EdgePtr CTinMemHalfEdge::GetCWEdge()
{
	return m_pCW;
}

void CTinMemHalfEdge::SetVertex(VertexPtr pVertex)
{
	m_pVertex = pVertex;
}

void CTinMemHalfEdge::SetPairEdge(EdgePtr pEdge)
{
	m_pPair = pEdge;
}

void CTinMemHalfEdge::SetCCWEdge(EdgePtr pEdge)
{
	m_pCCW = pEdge;
}

void CTinMemHalfEdge::SetCWEdge(EdgePtr pEdge)
{
	m_pCW = pEdge;
}
bool CTinMemHalfEdge::equal(EdgePtr pOther)
{
	if (this == pOther.get())
			return true;
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


