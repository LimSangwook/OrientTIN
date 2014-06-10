#include "TinOrientDBVertex.h"
#include "TinOrientDBStorage.h"
#include <assert.h>

CTinOrientDBVertex::CTinOrientDBVertex(RID strRID, double X, double Y, RID strRIDHarfEdge):
m_X(X), m_Y(Y)
{
	m_RID = strRID;
	m_RIDHalfEdge = strRIDHarfEdge;
}

CTinOrientDBVertex::~CTinOrientDBVertex()
{
}

double CTinOrientDBVertex::GetX()
{
//	ReLoad();
	return m_X;
}

double CTinOrientDBVertex::GetY()
{
//	ReLoad();
	return m_Y;
}

EdgePtr CTinOrientDBVertex::GetHalfEdge()
{
	return CTinOrientDBStorage::GetInstance()->GetHalfEdge(m_RIDHalfEdge);
}

void CTinOrientDBVertex::SetX(double x)
{
	m_X = x;
}

void CTinOrientDBVertex::SetY(double y)
{
	m_Y = y;
}

bool CTinOrientDBVertex::equal(boost::shared_ptr<ITinVertex> pOther)
{
	CTinOrientDBVertex* pDBVertex = dynamic_cast<CTinOrientDBVertex*>(pOther.get());
	if (!pDBVertex)
		return false;
	if (this->m_RID == pDBVertex->m_RID)
		return true;
	return false;
}

void CTinOrientDBVertex::SetHalfEdge(EdgePtr pEdge)
{
	CTinOrientDBHalfEdge* pDBEdge = dynamic_cast<CTinOrientDBHalfEdge*>(pEdge.get());
	if (!pDBEdge) {
		m_RIDHalfEdge = -1;
		return;
	}
	m_RIDHalfEdge = pDBEdge->GetRID();
}

