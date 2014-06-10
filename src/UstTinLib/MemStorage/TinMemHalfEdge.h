#ifndef __TIN_HALFEDGE_H__
#define __TIN_HALFEDGE_H__

#include "TinMemVertex.h"
#include "TinFace.h"
#include "../Common.h"

class CTinMemHalfEdge : public ITinHalfEdge
{
public:
	CTinMemHalfEdge();
	virtual ~CTinMemHalfEdge();

	virtual VertexPtr GetVertex();
	virtual void SetVertex(VertexPtr pVertex);

	virtual EdgePtr GetPairEdge();
	virtual void SetPairEdge(EdgePtr pEdge);

	virtual EdgePtr GetCCWEdge();
	virtual void SetCCWEdge(EdgePtr pEdge);

	virtual EdgePtr GetCWEdge();
	virtual void SetCWEdge(EdgePtr pEdge);

	CTinFace* GetFace();
	void SetFace(CTinFace* pFace);

	virtual bool equal(EdgePtr pOther);
private:
	VertexPtr		m_pVertex;
	EdgePtr		m_pPair;
	EdgePtr 		m_pCCW;
	EdgePtr		m_pCW;
	CTinFace*			m_pFace;
};

#endif //__TIN_HALFEDGE_H__
