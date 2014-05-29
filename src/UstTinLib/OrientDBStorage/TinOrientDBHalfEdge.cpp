#include "TinOrientDBHalfEdge.h"
#include "TinOrientDBStorage.h"
#include "../BaseStorage/ITinVertex.h"
#include "TinOrientDBVertex.h"
#include <assert.h>

CTinOrientDBStorage* GetDB() {
	return CTinOrientDBStorage::GetInstance();
}

CTinOrientDBHalfEdge::CTinOrientDBHalfEdge(RID strRID)
{
	m_RID = strRID;
	m_RIDVertex = "none";
	m_RIDPair = "none";
	m_RIDCCW = "none";
	m_RIDCW = "none";
}

CTinOrientDBHalfEdge::~CTinOrientDBHalfEdge()
{
}

void CTinOrientDBHalfEdge::_Update()
{
	CTinOrientDBStorage::GetInstance()->UpdateHalfEdge(this);
}

ITinVertex* CTinOrientDBHalfEdge::GetVertex()
{
	ReLoad();
	return	GetDB()->GetVertex(m_RIDVertex);
}

ITinHalfEdge* CTinOrientDBHalfEdge::GetPairEdge()
{
	ReLoad();
	return GetDB()->GetHalfEdge(m_RIDPair);
}

ITinHalfEdge* CTinOrientDBHalfEdge::GetCCWEdge()
{
	ReLoad();
	return GetDB()->GetHalfEdge(m_RIDCCW);
}

ITinHalfEdge* CTinOrientDBHalfEdge::GetCWEdge()
{
	ReLoad();
	return GetDB()->GetHalfEdge(m_RIDCW);
}

bool CTinOrientDBHalfEdge::equal(ITinHalfEdge* pOther)
{
	ReLoad();
	CTinOrientDBHalfEdge* pDBEdage = dynamic_cast<CTinOrientDBHalfEdge*>(pOther);
	if (pDBEdage && pDBEdage->m_RID == this->m_RID)
		return true;
	return false;
}

void CTinOrientDBHalfEdge::SetVertex(ITinVertex* pVertex)
{
	ReLoad();
	CTinOrientDBVertex* pDBVertex = dynamic_cast<CTinOrientDBVertex*>(pVertex);
	assert(pDBVertex);
	m_RIDVertex = pDBVertex->GetRID();

//	CTinOrientDBHalfEdge* pPair = (CTinOrientDBHalfEdge*)GetDB()->GetHalfEdge(m_RIDPair);
//	if (pPair != NULL){
//		pPair->SetRIDEndVertex(m_RIDVertex);
//		//pPair->_Update();
//	}
	_Update();
}
void CTinOrientDBHalfEdge::_CheckRIDS()
{
	if (m_RIDVertex.length() < 3)		m_RIDVertex = "-1";
	if (m_RIDEndVertex.length() < 3) 	m_RIDEndVertex = "-1";
	if (m_RIDPair.length() < 3) 	m_RIDPair = "-1";
	if (m_RIDCCW.length() < 3) 	m_RIDCCW = "-1";
	if (m_RIDCW.length() < 3) 	m_RIDCW = "-1";
}
void CTinOrientDBHalfEdge::SetPairEdge(ITinHalfEdge* pEdge)
{
	ReLoad();
	if (!pEdge){
		m_RIDPair = GetDB()->GetBlankRID();
		return;
	}
	CTinOrientDBHalfEdge* pDBEdge = dynamic_cast<CTinOrientDBHalfEdge*>(pEdge);
	assert(pDBEdge);
	m_RIDPair = pDBEdge->GetRID();
	m_RIDEndVertex = pDBEdge->GetRIDVertex();
	((CTinOrientDBHalfEdge*) (pEdge))->SetRIDEndVertex(m_RIDVertex);
	_Update();
}

void CTinOrientDBHalfEdge::SetCCWEdge(ITinHalfEdge* pEdge)
{
	ReLoad();
	CTinOrientDBHalfEdge* pDBEdge = dynamic_cast<CTinOrientDBHalfEdge*>(pEdge);
	assert(pDBEdge);

	m_RIDCCW = pDBEdge->GetRID();
	if (m_RIDCW == m_RID) {
		m_RIDCW = m_RIDCCW;
	}
	_Update();
}

void CTinOrientDBHalfEdge::SetCWEdge(ITinHalfEdge* pEdge)
{
	ReLoad();
	CTinOrientDBHalfEdge* pDBEdge = dynamic_cast<CTinOrientDBHalfEdge*>(pEdge);
	assert(pDBEdge);

	m_RIDCW = pDBEdge->GetRID();
	if (m_RIDCCW == m_RID) {
		m_RIDCCW = m_RIDCW;
	}


	_Update();
}

void CTinOrientDBHalfEdge::Copy(CTinOrientDBHalfEdge* pOther)
{
	m_RID = pOther->m_RID;
	m_RIDVertex = pOther->m_RIDVertex;
	m_RIDEndVertex = pOther->m_RIDEndVertex;
	m_RIDPair = pOther->m_RIDPair;
	m_RIDCCW = pOther->m_RIDCCW;
	m_RIDCW = pOther->m_RIDCW;
}

void CTinOrientDBHalfEdge::ReLoad()
{
	GetDB()->ReLoadHalfEdge(this);
}
