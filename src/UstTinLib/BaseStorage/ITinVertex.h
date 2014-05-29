#ifndef __ITIN_VERTEX_H__
#define __ITIN_VERTEX_H__

class ITinHalfEdge;

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
	virtual ITinHalfEdge* GetHalfEdge() = 0;
	virtual void SetHalfEdge(ITinHalfEdge* pHalfEdge) = 0;

	virtual bool equal(ITinVertex* pOther) = 0;
	virtual bool IsValid() = 0;
};

#endif //__ITIN_VERTEX_H__
