#ifndef __TIN_ORIENT_HALFEDGE_H__
#define __TIN_ORIENT_HALFEDGE_H__

#include "../Common.h"
#include "../BaseStorage/ITinHalfEdge.h"

class CTinOrientDBHalfEdge : public ITinHalfEdge
{
public:
	CTinOrientDBHalfEdge();
	CTinOrientDBHalfEdge(RID strRID, RID strRIDVertex , RID strRIDEndVertex = "none", RID strRIDPair = "none", RID strRIDCCW = "none", RID strRIDCw = "none");
	virtual ~CTinOrientDBHalfEdge();

	virtual VertexPtr GetVertex();
	virtual void SetVertex(VertexPtr pVertex);

	virtual EdgePtr GetPairEdge();
	virtual void SetPairEdge(EdgePtr pEdge);

	virtual EdgePtr GetCCWEdge();
	virtual void SetCCWEdge(EdgePtr pEdge);

	virtual EdgePtr GetCWEdge();
	virtual void SetCWEdge(EdgePtr pEdge);

	virtual bool equal(EdgePtr pOther);



	RID GetRID()			{return m_RID;};
	RID GetRIDVertex()	{return m_RIDVertex;};
	RID GetRIDEndVertex(){return m_RIDEndVertex;};
	RID GetRIDPair()		{return m_RIDPair;};
	RID GetRIDCCW()		{return m_RIDCCW;};
	RID GetRIDCW()		{return m_RIDCW;};

	void SetRID(RID nRID)			{m_RID = nRID;};
	void SetRIDVertex(RID nRID)		{m_RIDVertex = nRID;};
	void SetRIDEndVertex(RID nRID)	{m_RIDEndVertex = nRID;};

	void SetRIDPair(RID nRID)		{m_RIDPair = nRID;};
	void SetRIDCCW(RID nRID)			{m_RIDCCW = nRID;};
	void SetRIDCW(RID nRID)			{m_RIDCW = nRID;};

	bool IsMemory()					{return m_isOnlyMemory;};
private:

private:
	RID		m_RID;
	RID		m_RIDVertex;
	RID		m_RIDEndVertex;
	RID		m_RIDPair;
	RID		m_RIDCCW;
	RID		m_RIDCW;
	bool	m_isOnlyMemory;

	ITinHalfEdge*	m_pPair;
	ITinHalfEdge*	m_pCCW;
	ITinHalfEdge*	m_pCW;
};

#endif //__TIN_ORIENT_HALFEDGE_H__
