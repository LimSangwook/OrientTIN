#ifndef __TIN_HALFEDGE_H__
#define __TIN_HALFEDGE_H__

#include "../Common.h"
#include "../BaseStorage/ITinHalfEdge.h"

class CTinOrientDBHalfEdge : public ITinHalfEdge
{
public:
	CTinOrientDBHalfEdge(RID strRID);
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


	RID GetRID()			{;return m_RID;};
	RID GetRIDVertex()	{_CheckRIDS();return m_RIDVertex;};
	RID GetRIDEndVertex(){_CheckRIDS();return m_RIDEndVertex;};
	RID GetRIDPair()		{_CheckRIDS();return m_RIDPair;};
	RID GetRIDCCW()		{_CheckRIDS();return m_RIDCCW;};
	RID GetRIDCW()		{_CheckRIDS();return m_RIDCW;};

	void SetRIDVertex(RID nRID)		{m_RIDVertex = nRID;_Update();};
	void SetRIDEndVertex(RID nRID)	{m_RIDEndVertex = nRID;_Update();};
	void SetRIDPair(RID nRID)		{m_RIDPair = nRID;_Update();};
	void SetRIDCCW(RID nRID)			{m_RIDCCW = nRID;_Update();};
	void SetRIDCW(RID nRID)			{m_RIDCW = nRID;_Update();};

	void Copy(EdgePtr pOther);

	void ReLoad();

private:
	void _CheckRIDS();
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
