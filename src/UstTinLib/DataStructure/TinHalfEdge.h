#ifndef __TIN_HALFEDGE_H__
#define __TIN_HALFEDGE_H__

#include "TinVertex.h"
#include "TinFace.h"
#include "../Common.h"

class CTinHalfEdge
{
public:
	CTinHalfEdge();
	~CTinHalfEdge();

	CTinVertex* GetVertex();
	void SetVertex(CTinVertex* pVertex);

	CTinHalfEdge* GetPairEdge();
	void SetPairEdge(CTinHalfEdge* pEdge);

	CTinHalfEdge* GetCCWEdge();
	void SetCCWEdge(CTinHalfEdge* pEdge);

	CTinHalfEdge* GetCWEdge();
	void SetCWEdge(CTinHalfEdge* pEdge);

	CTinFace* GetFace();
	void SetFace(CTinFace* pFace);
private:
	RID m_RID;
	CTinVertex*		m_pVertex;
	CTinHalfEdge*		m_pPair;
	CTinHalfEdge*		m_pCCW;
	CTinHalfEdge*		m_pCW;
	CTinFace*			m_pFace;
};

#endif //__TIN_HALFEDGE_H__
