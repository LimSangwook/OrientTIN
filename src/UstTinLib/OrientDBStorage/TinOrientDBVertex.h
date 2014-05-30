#ifndef __TIN_ORIENTDB_VERTEX_H__
#define __TIN_ORIENTDB_VERTEX_H__

#include "../Common.h"
#include "../BaseStorage/ITinVertex.h"

class CTinHalfEdge;
class CTinOrientDBStorage;

class CTinOrientDBVertex : public ITinVertex
{
public :
	CTinOrientDBVertex(RID strRID, double X, double Y, RID strRIDHarfEdge);
	virtual ~CTinOrientDBVertex();

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

	RID GetRIDHalfEdge() {return m_RIDHalfEdge;};
	void SetRIDHalfEdge(RID RIDHalfEdge) {m_RIDHalfEdge = RIDHalfEdge;_Update();};
	RID GetRID(){return m_RID;};
	void Copy(CTinOrientDBVertex* pOther);
	void ReLoad();
private:
	void _Update();

private:
	RID m_RID;
	double m_X;
	double m_Y;
	RID m_RIDHalfEdge;
};
#endif //__TIN_ORIENTDB_VERTEX_H__
