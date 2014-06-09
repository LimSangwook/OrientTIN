#ifndef __TIN_DELAUNAY_H__
#define __TIN_DELAUNAY_H__
#include <vector>
#include <boost/shared_ptr.hpp>


class ITinHalfEdge;
class CTinFace;

typedef ITinHalfEdge* EdgePtr;
class CTinDelaunay
{

public:
	typedef std::vector<CTinFace*> FaceList;
	CTinDelaunay();
	~CTinDelaunay();

	EdgePtr& GetRightMostEdge();
	EdgePtr& GetLeftMostEdge();

	void SetRightMostEdge(EdgePtr pEdge);
	void SetLeftMostEdge(EdgePtr pEdge);

	int GetStartPointIdx();
	void SetStartPointIdx(int idx);

	int GetEndPointIdx();
	void SetEndPointIdx(int idx);

	FaceList& GetFaces();
	void SetFaces(FaceList& pFaces);

	int GetNumberOfFaces();
	void SetNumberOfFaces(int num);


private:

	EdgePtr		m_pRightMost_Edge;
	EdgePtr		m_pLeftMost_Edge;

	int					m_nNumFaces;
	FaceList			m_FaceList;

	int					m_StartPointIdx;
	int					m_EndPointIdx;

};

#endif //__TIN_DELAUNAY_H__
