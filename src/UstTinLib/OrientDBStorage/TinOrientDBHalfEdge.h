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
	RID GetRIDPair()		{return ((m_pPair)?m_pPair->GetRID():m_RIDPair);};
	RID GetRIDCCW()		{return ((m_pCCW)?m_pCCW->GetRID():m_RIDCCW);};
	RID GetRIDCW()		{return ((m_pCW)?m_pCW->GetRID():m_RIDCW);};

	void SetRID(RID nRID)			{m_RID = nRID;};
	void SetRIDVertex(RID nRID)		{m_RIDVertex = nRID;};
	void SetRIDEndVertex(RID nRID)	{m_RIDEndVertex = nRID;};

	void SetRIDPair(RID nRID)		{m_RIDPair = nRID;m_pPair = NULL;};
	void SetRIDCCW(RID nRID)			{m_RIDCCW = nRID;m_pCCW = NULL;};
	void SetRIDCW(RID nRID)			{m_RIDCW = nRID;m_pCW = NULL;};

	void SetCode(RID code)			{m_Code = code;};
	RID GetCode(RID code)			{return m_Code;};

	bool IsMemory()					{return m_isOnlyMemory;};
	bool SetSyncDB()				{m_isOnlyMemory = false;};

	bool IsDeleted()					{return m_bDeleted;};
	bool SetDelete()					{m_bDeleted = true;};
private:

private:
	RID 	m_Code;
	RID		m_RID;
	RID		m_RIDVertex;
	RID		m_RIDEndVertex;
	RID		m_RIDPair;
	RID		m_RIDCCW;
	RID		m_RIDCW;
	bool	m_isOnlyMemory;
	bool	m_bDeleted;

	CTinOrientDBHalfEdge*	m_pPair;
	CTinOrientDBHalfEdge*	m_pCCW;
	CTinOrientDBHalfEdge*	m_pCW;
};

#endif //__TIN_ORIENT_HALFEDGE_H__
