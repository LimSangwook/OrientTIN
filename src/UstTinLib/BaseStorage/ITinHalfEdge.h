#ifndef __ITIN_HALF_EDGE_H__
#define __ITIN_HALF_EDGE_H__
#include<boost/shared_ptr.hpp>

class ITinVertex;
typedef boost::shared_ptr<ITinVertex> VertexPtr;


class ITinHalfEdge
{

public:
	virtual ~ITinHalfEdge(){};
	virtual VertexPtr GetVertex() = 0;
	virtual void SetVertex(VertexPtr pVertex) = 0;

	virtual ITinHalfEdge* GetPairEdge() = 0;
	virtual void SetPairEdge(ITinHalfEdge* pEdge) = 0;

	virtual ITinHalfEdge* GetCCWEdge() = 0;
	virtual void SetCCWEdge(ITinHalfEdge* pEdge) = 0;

	virtual ITinHalfEdge* GetCWEdge() = 0;
	virtual void SetCWEdge(ITinHalfEdge* pEdge) = 0;
	virtual bool equal(ITinHalfEdge* pOther) = 0;

};
typedef ITinHalfEdge* EdgePtr;
#endif //__ITIN_HALF_EDGE_H__
