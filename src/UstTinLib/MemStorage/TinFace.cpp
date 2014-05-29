#include "TinFace.h"
#include "TinMemHalfEdge.h"

CTinFace::CTinFace() :
m_HalfEdge(0), m_nNumVertexs(0)
{
}

CTinFace::~CTinFace()
{
}

ITinHalfEdge* CTinFace::GetHalfEdge()
{
	return m_HalfEdge;
}

void CTinFace::SetHalfEdge(ITinHalfEdge* pEdge)
{
	m_HalfEdge = pEdge;
}

int CTinFace::GetNumberOfVertexs()
{
	return m_nNumVertexs;
}

void CTinFace::SetNumberOfVertexs(int num)
{
	m_nNumVertexs = num;
}
