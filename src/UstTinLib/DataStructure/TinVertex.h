#ifndef __TINVERTEX_H__
#define __TINVERTEX_H__

#include "../Common.h"
#include "../ITinVector.h"

class CTinHalfEdge;

class CTinVertex : public ITinVertex
{
public :
	CTinVertex();
	CTinVertex(double x, double y);
	virtual ~CTinVertex();

	// X좌표
	virtual double GetX();
	virtual void SetX(double x);

	// Y좌표
	virtual double GetY();
	virtual void SetY(double y);

	// 대표 HalfEdge
	virtual ITinHalfEdge* GetHalfEdge();
	virtual void SetHalfEdge(ITinHalfEdge* pHalfEdge);
	int idx;
private:
	RID m_RID;
	double m_X;
	double m_Y;
	ITinHalfEdge* m_pHalfEdge;
};
#endif //__TINVERTEX_H__
