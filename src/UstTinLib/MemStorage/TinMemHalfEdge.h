#ifndef __TIN_HALFEDGE_H__
#define __TIN_HALFEDGE_H__

#include "TinMemVertex.h"
#include "TinFace.h"
#include "../Common.h"
#include "../BaseStorage/ITinHalfEdge.h"

class CTinMemHalfEdge : public ITinHalfEdge
{
public:
	CTinMemHalfEdge();
	virtual ~CTinMemHalfEdge();

	virtual VertexPtr GetVertex();
	virtual void SetVertex(VertexPtr pVertex);

	virtual ITinHalfEdge* GetPairEdge();
	virtual void SetPairEdge(ITinHalfEdge* pEdge);

	virtual ITinHalfEdge* GetCCWEdge();
	virtual void SetCCWEdge(ITinHalfEdge* pEdge);

	virtual ITinHalfEdge* GetCWEdge();
	virtual void SetCWEdge(ITinHalfEdge* pEdge);

	CTinFace* GetFace();
	void SetFace(CTinFace* pFace);

	virtual bool equal(ITinHalfEdge* pOther);
private:
	VertexPtr		m_pVertex;
	ITinHalfEdge*		m_pPair;
	ITinHalfEdge*		m_pCCW;
	ITinHalfEdge*		m_pCW;
	CTinFace*			m_pFace;
};

#endif //__TIN_HALFEDGE_H__
