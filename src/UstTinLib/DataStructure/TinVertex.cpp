#include "TinVertex.h"
#include "TinHalfEdge.h"

CTinVertex::CTinVertex() :
idx(-1), m_X(0), m_Y(0), m_pHalfEdge(0)
{
}

CTinVertex::CTinVertex(double x, double y) :
idx(-1), m_X(x), m_Y(y), m_pHalfEdge(0)
{
}

CTinVertex::~CTinVertex()
{
}

double CTinVertex::GetX()
{
	return m_X;
}

double CTinVertex::GetY()
{
	return m_Y;
}

CTinHalfEdge* CTinVertex::GetHalfEdge()
{
	return m_pHalfEdge;
}

void CTinVertex::SetX(double x)
{
	m_X = x;
}

void CTinVertex::SetY(double y)
{
	m_Y = y;
}

void CTinVertex::SetHalfEdge(CTinHalfEdge* pHalfEdge)
{
	m_pHalfEdge = pHalfEdge;
}
