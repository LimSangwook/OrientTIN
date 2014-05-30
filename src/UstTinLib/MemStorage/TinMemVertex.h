#ifndef __TINVERTEX_H__
#define __TINVERTEX_H__

#include "../Common.h"
#include "../BaseStorage/ITinVertex.h"

class CTinHalfEdge;

class CTinMemVertex : public ITinVertex
{
public :
	CTinMemVertex();
	CTinMemVertex(double x, double y);
	virtual ~CTinMemVertex();

	// X좌표
	virtual double GetX();
	virtual void SetX(double x);

	// Y좌표
	virtual double GetY();
	virtual void SetY(double y);

	// 대표 HalfEdge
	virtual EdgePtr GetHalfEdge();
	virtual void SetHalfEdge(EdgePtr pHalfEdge);

	virtual bool equal(ITinVertex* pOther);
	int idx;

private:
	double m_X;
	double m_Y;
	EdgePtr m_pHalfEdge;
};
#endif //__TINVERTEX_H__
