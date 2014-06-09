#include "TinOrientDBHalfEdge.h"
#include "TinOrientDBStorage.h"
#include "../BaseStorage/ITinVertex.h"
#include "TinOrientDBVertex.h"
#include <assert.h>

CTinOrientDBStorage* GetDB() {
	return CTinOrientDBStorage::GetInstance();
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
}

CTinOrientDBHalfEdge::~CTinOrientDBHalfEdge()
{
}

void CTinOrientDBHalfEdge::_Update()
{
//	CTinOrientDBStorage::GetInstance()->UpdateHalfEdge(this);
}

VertexPtr CTinOrientDBHalfEdge::GetVertex()
{
	return	VertexPtr(GetDB()->GetVertex(m_RIDVertex));
}

EdgePtr CTinOrientDBHalfEdge::GetPairEdge()
{
	return GetDB()->GetHalfEdge(m_RIDPair);
}

EdgePtr CTinOrientDBHalfEdge::GetCCWEdge()
{
	return GetDB()->GetHalfEdge(m_RIDCCW);
}

EdgePtr CTinOrientDBHalfEdge::GetCWEdge()
{
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

void CTinOrientDBHalfEdge::_CheckRIDS()
{
	if (m_RIDVertex.length() < 3)		m_RIDVertex = "-1";
	if (m_RIDEndVertex.length() < 3) 	m_RIDEndVertex = "-1";
	if (m_RIDPair.length() < 3) 	m_RIDPair = "-1";
	if (m_RIDCCW.length() < 3) 	m_RIDCCW = "-1";
	if (m_RIDCW.length() < 3) 	m_RIDCW = "-1";
}

void CTinOrientDBHalfEdge::SetPairEdge(EdgePtr pEdge)
{
	CTinOrientDBHalfEdge* pDBEdge = dynamic_cast<CTinOrientDBHalfEdge*>(pEdge);
	assert(pDBEdge);
	m_RIDPair = pDBEdge->GetRID();
	m_RIDEndVertex = pDBEdge->GetRIDVertex();
	pDBEdge->SetRIDEndVertex(m_RIDVertex);
}

void CTinOrientDBHalfEdge::SetCCWEdge(EdgePtr pEdge)
{
	CTinOrientDBHalfEdge* pDBEdge = dynamic_cast<CTinOrientDBHalfEdge*>(pEdge);
	assert(pDBEdge);

	m_RIDCCW = pDBEdge->GetRID();
	if (m_RIDCW == m_RID) {
		m_RIDCW = m_RIDCCW;
	}
}

void CTinOrientDBHalfEdge::SetCWEdge(EdgePtr pEdge)
{
	CTinOrientDBHalfEdge* pDBEdge = dynamic_cast<CTinOrientDBHalfEdge*>(pEdge);
	assert(pDBEdge);

	m_RIDCW = pDBEdge->GetRID();
	if (m_RIDCCW == m_RID) {
		m_RIDCCW = m_RIDCW;
	}
}

void CTinOrientDBHalfEdge::Copy(EdgePtr pEdge)
{
	CTinOrientDBHalfEdge* pOther = dynamic_cast<CTinOrientDBHalfEdge*>(pEdge);
	assert(pOther);
	m_RID = pOther->m_RID;
	m_RIDVertex = pOther->m_RIDVertex;
	m_RIDEndVertex = pOther->m_RIDEndVertex;
	m_RIDPair = pOther->m_RIDPair;
	m_RIDCCW = pOther->m_RIDCCW;
	m_RIDCW = pOther->m_RIDCW;
}

void CTinOrientDBHalfEdge::ReLoad()
{
}
