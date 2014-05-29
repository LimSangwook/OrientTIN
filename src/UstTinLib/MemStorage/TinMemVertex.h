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
	virtual ITinHalfEdge* GetHalfEdge();
	virtual void SetHalfEdge(ITinHalfEdge* pHalfEdge);

	virtual bool equal(ITinVertex* pOther);
	virtual bool IsValid(){ return true;};
	int idx;

private:
	double m_X;
	double m_Y;
	ITinHalfEdge* m_pHalfEdge;
};
#endif //__TINVERTEX_H__
