#ifndef __TIN_DATAMANAGER_H__
#define __TIN_DATAMANAGER_H__

#include <vector>
#include <set>
#include "Common.h"

class CTinEdgeMaker
{
typedef std::set<CTinDelaunay*> DelaunayList;
enum LEFT_RIGHT{
	ONSEG,
	ONRIGHT,
	ONLEFT
};

enum IN_OUT_CIRCLE{
	ONCIRCLE,
	IN_SIDE,
	OUT_SIDE
};

public:
	CTinEdgeMaker();
	~CTinEdgeMaker();

	void MakeDelaunayEdge();
	void MakeDelaunayFace();

	void PrintVertexList();
	void PrintEdgeList();
	void PrintFaceList();

	void AttachTinStorage(ITinStorageManager* pStorage);


private:
	VertexPtr 		_GetVertex(int idx);
	EdgePtr 		_CreateEdge();
	int 			_GetCountOfVertexs();

	////////////////////////////////////////////////
	// Divide and Conquer 알고리즘부분
	////////////////////////////////////////////////
	void 			_DivideAndConquer(CTinDelaunay& delaunay);
	void 			_Del_Link(CTinDelaunay& result, CTinDelaunay& left, CTinDelaunay& right );
	void 			_Del_Init_Seg(CTinDelaunay& del);
	void 			_Del_Init_Tri(CTinDelaunay& del);
	LEFT_RIGHT  	_Classify_Point_Seg(VertexPtr& s, VertexPtr& e, VertexPtr& pt);
	LEFT_RIGHT  	_Del_Classify_Point(EdgePtr& d, VertexPtr& pt);
	IN_OUT_CIRCLE _In_Circle(VertexPtr& pt0, VertexPtr& pt1, VertexPtr& pt2, VertexPtr& p);
	EdgePtr 		_Del_Get_Lower_Supportant( CTinDelaunay& left, CTinDelaunay& right);
	EdgePtr 		_Del_Valid_Link(EdgePtr b);
	EdgePtr 		_Del_Valid_Left(EdgePtr b);
	EdgePtr 		_Del_Valid_Right(EdgePtr b);
	void 			_Del_Remove_Halfedge(EdgePtr d);
	void 			_Del_Remove_Single_Halfedge(EdgePtr d);
	void 			_Halfedge_Free(EdgePtr d);
	void 			_Build_Halfedge_Face(CTinDelaunay *del, ITinHalfEdge *d );

private:
	int				m_nDnCDepth;
	int				m_nMaxDepth;
	CTinDelaunay	m_delaunay;
	ITinStorageManager* m_pTinStorage;
};

#endif //__TIN_DATAMANAGER_H__
