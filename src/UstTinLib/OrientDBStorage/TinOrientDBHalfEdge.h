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


	RID GetRID()			{return m_RID;};
	RID GetRIDVertex()	{return m_RIDVertex;};
	RID GetRIDPair()		{return m_RIDPair;};
	RID GetRIDCCW()		{return m_RIDCCW;};
	RID GetRIDCW()		{return m_RIDCW;};


private:
	RID		m_RID;
	RID		m_RIDVertex;
	RID		m_RIDPair;
	RID		m_RIDCCW;
	RID		m_RIDCW;
	bool	m_Modify;
};

#endif //__TIN_HALFEDGE_H__
