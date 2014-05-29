#ifndef __TIN_HALFEDGE_H__
#define __TIN_HALFEDGE_H__

#include "../Common.h"
#include "../BaseStorage/ITinHalfEdge.h"

class CTinOrientDBHalfEdge : public ITinHalfEdge
{
public:
	CTinOrientDBHalfEdge(RID strRID);
	virtual ~CTinOrientDBHalfEdge();

	virtual ITinVertex* GetVertex();
	virtual void SetVertex(ITinVertex* pVertex);

	virtual ITinHalfEdge* GetPairEdge();
	virtual void SetPairEdge(ITinHalfEdge* pEdge);

	virtual ITinHalfEdge* GetCCWEdge();
	virtual void SetCCWEdge(ITinHalfEdge* pEdge);

	virtual ITinHalfEdge* GetCWEdge();
	virtual void SetCWEdge(ITinHalfEdge* pEdge);

	virtual bool equal(ITinHalfEdge* pOther);

	RID GetRID()			{return m_RID;};
	RID GetRIDVertex()	{return m_RIDVertex;};
	RID GetRIDEndVertex(){return m_RIDEndVertex;};
	RID GetRIDPair()		{return m_RIDPair;};
	RID GetRIDCCW()		{return m_RIDCCW;};
	RID GetRIDCW()		{return m_RIDCW;};

	void SetRIDVertex(RID nRID)		{m_RIDVertex = nRID;_Update();};
	void SetRIDEndVertex(RID nRID)	{m_RIDEndVertex = nRID;_Update();};
	void SetRIDPair(RID nRID)		{m_RIDPair = nRID;_Update();};
	void SetRIDCCW(RID nRID)			{m_RIDCCW = nRID;_Update();};
	void SetRIDCW(RID nRID)			{m_RIDCW = nRID;_Update();};


private:
	void _Update();

private:
	RID		m_RID;
	RID		m_RIDVertex;
	RID		m_RIDEndVertex;
	RID		m_RIDPair;
	RID		m_RIDCCW;
	RID		m_RIDCW;
};

#endif //__TIN_HALFEDGE_H__
