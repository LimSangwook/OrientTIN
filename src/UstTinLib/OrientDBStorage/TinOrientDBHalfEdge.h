#ifndef __TIN_ORIENT_HALFEDGE_H__
#define __TIN_ORIENT_HALFEDGE_H__

#include "../Common.h"
#include "../BaseStorage/ITinHalfEdge.h"

class CTinOrientDBHalfEdge : public ITinHalfEdge
{
public:
	CTinOrientDBHalfEdge();
	CTinOrientDBHalfEdge(RID strRID, RID strRIDVertex , RID strRIDEndVertex = "none", RID strRIDPair = "none", RID strRIDCCW = "none", RID strRIDCw = "none");
	~CTinOrientDBHalfEdge();

public:
	/////////////////////////////////////////
	// ITinHalfEdge Interface 구현
	/////////////////////////////////////////
	virtual VertexPtr 	GetVertex();
	virtual EdgePtr 		GetPairEdge();
	virtual EdgePtr 		GetCCWEdge();
	virtual EdgePtr 		GetCWEdge();

	virtual void 			SetVertex(VertexPtr pVertex);
	virtual void 			SetPairEdge(EdgePtr pEdge);
	virtual void 			SetCCWEdge(EdgePtr pEdge);
	virtual void 			SetCWEdge(EdgePtr pEdge);

	virtual bool equal(EdgePtr pOther);

public:
	RID GetRID()			{return m_RID;};
	RID GetRIDVertex()	{return m_RIDVertex;};
	RID GetRIDEndVertex(){return m_RIDEndVertex;};
	RID GetRIDPair()		{return ((m_pPair)?m_pPair->GetRID():m_RIDPair);};
	RID GetRIDCCW()		{return ((m_pCCW)?m_pCCW->GetRID():m_RIDCCW);};
	RID GetRIDCW()		{return ((m_pCW)?m_pCW->GetRID():m_RIDCW);};

	void SetRID(RID nRID)			{if (m_RID 			== nRID) return; m_bDirty = true; m_RID 			= nRID;};
	void SetRIDVertex(RID nRID)		{if (m_RIDVertex 		== nRID) return; m_bDirty = true; m_RIDVertex 	= nRID;};
	void SetRIDEndVertex(RID nRID)	{if (m_RIDEndVertex 	== nRID) return; m_bDirty = true; m_RIDEndVertex = nRID;};
	void SetRIDPair(RID nRID)		{if (m_RIDPair 		== nRID) return; m_bDirty = true; m_RIDPair 		= nRID;m_pPair 	= NULL;};
	void SetRIDCCW(RID nRID)			{if (m_RIDCCW 		== nRID) return; m_bDirty = true; m_RIDCCW 		= nRID;m_pCCW 	= NULL;};
	void SetRIDCW(RID nRID)			{if (m_RIDCW 			== nRID) return; m_bDirty = true; m_RIDCW 		= nRID;m_pCW 		= NULL;};

	bool IsMemory()					{return m_isOnlyMemory;};
	void SetSyncDB()					{m_isOnlyMemory = false;};

	bool IsDeleted()					{return m_bDeleted;};
	void SetDelete()					{m_bDeleted = true;m_bDirty = true;};

	bool IsDirty()					{return m_bDirty;};
	bool ReSetDirty()					{m_bDirty = false;};
private:

private:
	RID		m_RID;
	RID		m_RIDVertex;
	RID		m_RIDEndVertex;
	RID		m_RIDPair;
	RID		m_RIDCCW;
	RID		m_RIDCW;
	bool	m_isOnlyMemory;
	bool	m_bDeleted;
	bool	m_bDirty;

	CTinOrientDBHalfEdge*	m_pPair;
	CTinOrientDBHalfEdge*	m_pCCW;
	CTinOrientDBHalfEdge*	m_pCW;
};

#endif //__TIN_ORIENT_HALFEDGE_H__
