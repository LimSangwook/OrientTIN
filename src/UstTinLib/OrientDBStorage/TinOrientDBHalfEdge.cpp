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

	m_isOnlyMemory = true;
	m_bDeleted = false;
	m_pPair = NULL;
	m_pCCW = NULL;
	m_pCW = NULL;
	m_bDirty = true;
}

CTinOrientDBHalfEdge::CTinOrientDBHalfEdge(RID strRID, RID strRIDVertex, RID strRIDEndVertex, RID strRIDPair, RID strRIDCCW, RID strRIDCw)
: CTinOrientDBHalfEdge()
{
	m_RID = strRID;
	m_RIDVertex = strRIDVertex;
	m_RIDEndVertex = strRIDEndVertex;
	m_RIDPair = strRIDPair;
	m_RIDCCW = strRIDCCW;
	m_RIDCW = strRIDCw;

	m_bDirty = false;

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
	if(m_pPair)
		return GetDB()->GetHalfEdge(m_pPair->GetRID());
	return GetDB()->GetHalfEdge(m_RIDPair);
}

EdgePtr CTinOrientDBHalfEdge::GetCCWEdge()
{
	if(m_pCCW)
		return GetDB()->GetHalfEdge(m_pCCW->GetRID());
	return GetDB()->GetHalfEdge(m_RIDCCW);
}

EdgePtr CTinOrientDBHalfEdge::GetCWEdge()
{
	if (m_pCW)
		return GetDB()->GetHalfEdge(m_pCW->GetRID());
	return GetDB()->GetHalfEdge(m_RIDCW);
}

bool CTinOrientDBHalfEdge::equal(EdgePtr pOther)
{
	CTinOrientDBHalfEdge* pDBEdage = dynamic_cast<CTinOrientDBHalfEdge*>(pOther.get());
	if (pDBEdage && pDBEdage->m_RID == this->m_RID)
		return true;
	return false;
}

void CTinOrientDBHalfEdge::SetVertex(VertexPtr pVertex)
{
	CTinOrientDBVertex* pDBVertex = dynamic_cast<CTinOrientDBVertex*>(pVertex.get());
	assert(pDBVertex);

	if (pDBVertex->GetRID() == m_RIDVertex)
		return;

	m_RIDVertex = pDBVertex->GetRID();
	m_bDirty = true;
}

void CTinOrientDBHalfEdge::SetPairEdge(EdgePtr pEdge)
{
	if (pEdge.get() == m_pPair)
		return;

	m_pPair = (CTinOrientDBHalfEdge*)pEdge.get();

	if (!m_pPair->IsMemory()) {
		SetRIDPair(m_pPair->GetRID());
	}

	if (m_pPair->GetRID() != m_RIDPair)
		m_bDirty = true;
}

void CTinOrientDBHalfEdge::SetCCWEdge(EdgePtr pEdge)
{
	if (pEdge.get() == m_pCCW)
		return;


	m_pCCW = (CTinOrientDBHalfEdge*)pEdge.get();

	if (m_pCCW->GetRID() != m_RIDCCW)
		m_bDirty = true;
}

void CTinOrientDBHalfEdge::SetCWEdge(EdgePtr pEdge)
{
	if (pEdge.get() == m_pCW)
			return;

	m_pCW = (CTinOrientDBHalfEdge*)pEdge.get();

	if (m_pCW->GetRID() != m_RIDCW)
		m_bDirty = true;
}
