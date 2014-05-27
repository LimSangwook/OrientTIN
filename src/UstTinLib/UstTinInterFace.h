
class IHalfEdge;

class IVertex
{
public:
	virtual double GetX() = 0;
	virtual void SetX(double x) = 0;

	// Y좌표
	virtual double GetY() = 0;
	virtual void SetY(double y) = 0;

	// 대표 HalfEdge
	virtual IHalfEdge* GetHalfEdge() = 0;
	virtual void SetHalfEdge(IHalfEdge* pHalfEdge) = 0;
};

class IHalfEdge
{
	virtual IVertex* GetVertex() = 0;
	virtual void SetVertex(IVertex* pVertex) = 0;

	virtual IHalfEdge* GetPairEdge() = 0;
	virtual void SetPairEdge(IHalfEdge* pEdge) = 0;

	virtual IHalfEdge* GetCCWEdge() = 0;
	virtual void SetCCWEdge(IHalfEdge* pEdge) = 0;

	virtual IHalfEdge* GetCWEdge() = 0;
	virtual void SetCWEdge(IHalfEdge* pEdge) = 0;

};
