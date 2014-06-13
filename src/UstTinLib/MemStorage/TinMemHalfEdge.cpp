#include "TinMemHalfEdge.h"
#include "TinMemStorage.h"

CTinMemHalfEdge::CTinMemHalfEdge(int idx) :
m_pFace(0), m_VertexIdx(-1), m_Idx(idx)
{
}

CTinMemHalfEdge::~CTinMemHalfEdge()
{
}
CTinMemStorage* CTinMemStorage::instance;
VertexPtr CTinMemHalfEdge::GetVertex()
{
	return CTinMemStorage::GetInstance()->GetVertex(m_VertexIdx);
	//return VertexPtr();
}

EdgePtr CTinMemHalfEdge::GetPairEdge()
{
	return CTinMemStorage::GetInstance()->GetEdge(m_PairIdx);
}

EdgePtr CTinMemHalfEdge::GetCCWEdge()
{
	return CTinMemStorage::GetInstance()->GetEdge(m_CCWIdx);
}

EdgePtr CTinMemHalfEdge::GetCWEdge()
{
	return CTinMemStorage::GetInstance()->GetEdge(m_CWIdx);
}

void CTinMemHalfEdge::SetVertex(VertexPtr pVertex)
{
	m_VertexIdx = ((CTinMemVertex*)pVertex.get())->idx;
}

void CTinMemHalfEdge::SetPairEdge(EdgePtr pEdge)
{
	m_PairIdx = ((CTinMemHalfEdge*)(pEdge.get()))->GetIdx();
}

void CTinMemHalfEdge::SetCCWEdge(EdgePtr pEdge)
{
	m_CCWIdx = ((CTinMemHalfEdge*)(pEdge.get()))->GetIdx();
}

void CTinMemHalfEdge::SetCWEdge(EdgePtr pEdge)
{
	m_CWIdx  = ((CTinMemHalfEdge*)(pEdge.get()))->GetIdx();
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


