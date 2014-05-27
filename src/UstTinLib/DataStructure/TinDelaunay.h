#ifndef __TIN_DELAUNAY_H__
#define __TIN_DELAUNAY_H__
#include <vector>

class CTinHalfEdge;
class CTinFace;
class CTinDelaunay
{

public:
	typedef std::vector<CTinFace*> FaceList;
	CTinDelaunay();
	~CTinDelaunay();

	CTinHalfEdge* GetRightMostEdge();
	CTinHalfEdge* GetLeftMostEdge();

	void SetRightMostEdge(CTinHalfEdge* pEdge);
	void SetLeftMostEdge(CTinHalfEdge* pEdge);

	int GetStartPointIdx();
	void SetStartPointIdx(int idx);

	int GetEndPointIdx();
	void SetEndPointIdx(int idx);

	FaceList& GetFaces();
	void SetFaces(FaceList& pFaces);

	int GetNumberOfFaces();
	void SetNumberOfFaces(int num);


private:

	CTinHalfEdge*		m_pRightMost_Edge;
	CTinHalfEdge*		m_pLeftMost_Edge;

	int					m_nNumFaces;
	FaceList			m_FaceList;

	int					m_StartPointIdx;
	int					m_EndPointIdx;

};

#endif //__TIN_DELAUNAY_H__
