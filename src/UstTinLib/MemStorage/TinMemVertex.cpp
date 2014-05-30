#include "TinMemVertex.h"
#include "TinMemHalfEdge.h"

CTinMemVertex::CTinMemVertex() :
idx(-1), m_X(0), m_Y(0)
{
}

CTinMemVertex::CTinMemVertex(double x, double y) :
idx(-1), m_X(x), m_Y(y)
{
}

CTinMemVertex::~CTinMemVertex()
{
}

double CTinMemVertex::GetX()
{
	return m_X;
}

double CTinMemVertex::GetY()
{
	return m_Y;
}

EdgePtr CTinMemVertex::GetHalfEdge()
{
	return m_pHalfEdge;
}

void CTinMemVertex::SetX(double x)
{
	m_X = x;
}

void CTinMemVertex::SetY(double y)
{
	m_Y = y;
}

bool CTinMemVertex::equal(ITinVertex* pOther)
{
	if (!pOther)
		return false;
	if (((CTinMemVertex*)(pOther))->idx == idx)
		return true;
	return false;
}


void CTinMemVertex::SetHalfEdge(EdgePtr pHalfEdge)
{
	m_pHalfEdge = pHalfEdge;
}
