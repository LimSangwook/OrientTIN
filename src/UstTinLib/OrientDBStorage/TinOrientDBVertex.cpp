#include "TinOrientDBVertex.h"
#include "TinOrientDBStorage.h"
#include <assert.h>

CTinOrientDBVertex::CTinOrientDBVertex(RID strRID) :
m_X(0), m_Y(0)
{
	m_RID = strRID;
}

CTinOrientDBVertex::~CTinOrientDBVertex()
{
}

void CTinOrientDBVertex::_Update()
{
	CTinOrientDBStorage::GetInstance()->UpdateVertex(this);
}

double CTinOrientDBVertex::GetX()
{
	return m_X;
}

double CTinOrientDBVertex::GetY()
{
	return m_Y;
}

ITinHalfEdge* CTinOrientDBVertex::GetHalfEdge()
{
	ITinHalfEdge* edge = CTinOrientDBStorage::GetInstance()->GetHalfEdge(m_RIDHalfEdge);
	return edge;
}

void CTinOrientDBVertex::SetX(double x)
{
	m_X = x;
	_Update();
}

void CTinOrientDBVertex::SetY(double y)
{
	m_Y = y;
	_Update();
}

bool CTinOrientDBVertex::equal(ITinVertex* pOther)
{
	CTinOrientDBVertex* pDBVertex = dynamic_cast<CTinOrientDBVertex*>(pOther);
	if (!pDBVertex)
		return false;
	if (this->m_RID == pDBVertex->m_RID)
		return true;
	return false;
}

bool CTinOrientDBVertex::IsValid()
{
	if (this->m_RID.length() < 3) {
		return false;
	}
	return true;
}

void CTinOrientDBVertex::SetHalfEdge(ITinHalfEdge* pEdge)
{
	CTinOrientDBHalfEdge* pDBEdge = dynamic_cast<CTinOrientDBHalfEdge*>(pEdge);
	if (!pDBEdge) {
		m_RIDHalfEdge = -1;
		return;
	}
	m_RIDHalfEdge = pDBEdge->GetRID();
	_Update();
}
