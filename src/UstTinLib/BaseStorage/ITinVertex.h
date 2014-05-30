#ifndef __ITIN_VERTEX_H__
#define __ITIN_VERTEX_H__

#include<boost/shared_ptr.hpp>

class ITinHalfEdge;
typedef boost::shared_ptr<ITinHalfEdge> EdgePtr;
class ITinVertex
{
public:
	virtual ~ITinVertex(){};
	virtual double GetX() = 0;
	virtual void SetX(double x) = 0;

	// Y좌표
	virtual double GetY() = 0;
	virtual void SetY(double y) = 0;

	// 대표 HalfEdge
	virtual EdgePtr GetHalfEdge() = 0;
	virtual void SetHalfEdge(EdgePtr pHalfEdge) = 0;

	virtual bool equal(boost::shared_ptr<ITinVertex> pOther) = 0;
};

#endif //__ITIN_VERTEX_H__
