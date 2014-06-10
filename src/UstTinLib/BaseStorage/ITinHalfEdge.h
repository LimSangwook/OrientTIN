#ifndef __ITIN_HALF_EDGE_H__
#define __ITIN_HALF_EDGE_H__

class ITinVertex;
typedef boost::shared_ptr<ITinVertex> VertexPtr;

class ITinHalfEdge
{

public:
	virtual ~ITinHalfEdge(){};
	virtual VertexPtr GetVertex() = 0;
	virtual void SetVertex(VertexPtr pVertex) = 0;

	virtual boost::shared_ptr<ITinHalfEdge> GetPairEdge() = 0;
	virtual void SetPairEdge(boost::shared_ptr<ITinHalfEdge> pEdge) = 0;

	virtual boost::shared_ptr<ITinHalfEdge> GetCCWEdge() = 0;
	virtual void SetCCWEdge(boost::shared_ptr<ITinHalfEdge> pEdge) = 0;

	virtual boost::shared_ptr<ITinHalfEdge> GetCWEdge() = 0;
	virtual void SetCWEdge(boost::shared_ptr<ITinHalfEdge> pEdge) = 0;
	virtual bool equal(boost::shared_ptr<ITinHalfEdge> pOther) = 0;

};
#endif //__ITIN_HALF_EDGE_H__
