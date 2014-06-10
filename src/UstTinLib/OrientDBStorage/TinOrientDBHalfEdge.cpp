#include "TinOrientDBHalfEdge.h"
#include "TinOrientDBStorage.h"
#include "../BaseStorage/ITinVertex.h"
#include "TinOrientDBVertex.h"
#include <assert.h>

CTinOrientDBStorage* GetDB() {
	return CTinOrientDBStorage::GetInstance();
}

CTinOrientDBHalfEdge::CTinOrientDBHalfEdge()
{
	m_RID = "none";
	m_RIDVertex = "none";
	m_RIDEndVertex = "none";
	m_RIDPair = "none";
	m_RIDCCW = "none";
	m_RIDCW = "none";
	m_pPair = NULL;
	m_pCCW = NULL;
	m_pCW = NULL;
	m_isOnlyMemory = true;

}

CTinOrientDBHalfEdge::CTinOrientDBHalfEdge(RID strRID, RID strRIDVertex, RID strRIDEndVertex, RID strRIDPair, RID strRIDCCW, RID strRIDCw)
{
	m_RID = strRID;
	m_RIDVertex = strRIDVertex;
	m_RIDEndVertex = strRIDEndVertex;
	m_RIDPair = strRIDPair;
	m_RIDCCW = strRIDCCW;
	m_RIDCW = strRIDCw;

	m_pPair = NULL;
	m_pCCW = NULL;
	m_pCW = NULL;
	m_isOnlyMemory = false;
}

CTinOrientDBHalfEdge::~CTinOrientDBHalfEdge()
{
}


VertexPtr CTinOrientDBHalfEdge::GetVertex()
{
	return	VertexPtr(GetDB()->GetVertex(m_RIDVertex));
}

EdgePtr CTinOrientDBHalfEdge::GetPairEdge()
{
	if (m_pPair)
		return m_pPair;

	return GetDB()->GetHalfEdge(m_RIDPair);
}

EdgePtr CTinOrientDBHalfEdge::GetCCWEdge()
{
	if (m_pCCW)
		return m_pCCW;

	return GetDB()->GetHalfEdge(m_RIDCCW);
}

EdgePtr CTinOrientDBHalfEdge::GetCWEdge()
{
	if (m_pCW)
		return m_pCW;

	return GetDB()->GetHalfEdge(m_RIDCW);
}

bool CTinOrientDBHalfEdge::equal(EdgePtr pOther)
{
	CTinOrientDBHalfEdge* pDBEdage = dynamic_cast<CTinOrientDBHalfEdge*>(pOther);
	if (pDBEdage && pDBEdage->m_RID == this->m_RID)
		return true;
	return false;
}

void CTinOrientDBHalfEdge::SetVertex(VertexPtr pVertex)
{
	CTinOrientDBVertex* pDBVertex = dynamic_cast<CTinOrientDBVertex*>(pVertex.get());
	assert(pDBVertex);
	m_RIDVertex = pDBVertex->GetRID();
}

void CTinOrientDBHalfEdge::SetPairEdge(EdgePtr pEdge)
{
	m_pPair = pEdge;
}

void CTinOrientDBHalfEdge::SetCCWEdge(EdgePtr pEdge)
{
	m_pCCW = pEdge;
}

void CTinOrientDBHalfEdge::SetCWEdge(EdgePtr pEdge)
{
	m_pCW = pEdge;
}
