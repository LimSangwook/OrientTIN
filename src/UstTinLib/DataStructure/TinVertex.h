#ifndef __TINVERTEX_H__
#define __TINVERTEX_H__

#include "../Common.h"

class CTinHalfEdge;

class CTinVertex
{
public :
	CTinVertex();
	CTinVertex(double x, double y);
	~CTinVertex();

	// X좌표
	double GetX();
	void SetX(double x);

	// Y좌표
	double GetY();
	void SetY(double y);

	// 대표 HalfEdge
	CTinHalfEdge* GetHalfEdge();
	void SetHalfEdge(CTinHalfEdge* pHalfEdge);
	int idx;
private:
	RID m_RID;
	double m_X;
	double m_Y;
	CTinHalfEdge* m_pHalfEdge;
};
#endif //__TINVERTEX_H__
