#ifndef __TIN_HALFEDGE_H__
#define __TIN_HALFEDGE_H__

#include "TinMemVertex.h"
#include "TinFace.h"
#include "../Common.h"

class CTinMemHalfEdge : public ITinHalfEdge
{
public:
	CTinMemHalfEdge(int idx);
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
	int GetIdx() {return m_Idx;};
private:
	int				m_Idx;
	int				m_VertexIdx;
	int				m_PairIdx;
	int 			m_CCWIdx;
	int				m_CWIdx;
	CTinFace*			m_pFace;
};

#endif //__TIN_HALFEDGE_H__
