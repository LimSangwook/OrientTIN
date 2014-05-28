#ifndef __ITIN_HALF_EDGE_H__
#define __ITIN_HALF_EDGE_H__

class ITinVertex;

class ITinHalfEdge
{
public:
	virtual ~ITinHalfEdge(){};
	virtual ITinVertex* GetVertex() = 0;
	virtual void SetVertex(ITinVertex* pVertex) = 0;

	virtual ITinHalfEdge* GetPairEdge() = 0;
	virtual void SetPairEdge(ITinHalfEdge* pEdge) = 0;

	virtual ITinHalfEdge* GetCCWEdge() = 0;
	virtual void SetCCWEdge(ITinHalfEdge* pEdge) = 0;

	virtual ITinHalfEdge* GetCWEdge() = 0;
	virtual void SetCWEdge(ITinHalfEdge* pEdge) = 0;

};

#endif //__ITIN_HALF_EDGE_H__
