#include "TinOrientDBVertex.h"
#include "TinOrientDBStorage.h"
#include <assert.h>

CTinOrientDBVertex::CTinOrientDBVertex(RID strRID) :
m_X(0), m_Y(0), m_Modify(false)
{
	m_RID = strRID;
}

CTinOrientDBVertex::~CTinOrientDBVertex()
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
	m_Modify = true;
	m_X = x;
}

void CTinOrientDBVertex::SetY(double y)
{
	m_Modify = true;
	m_Y = y;
}

void CTinOrientDBVertex::SetHalfEdge(ITinHalfEdge* pEdge)
{
	m_Modify = true;
	CTinOrientDBHalfEdge* pDBEdge = dynamic_cast<CTinOrientDBHalfEdge*>(pEdge);
	assert(pDBEdge);
	m_RIDHalfEdge = pDBEdge->GetRID();
	m_Modify = true;
}
