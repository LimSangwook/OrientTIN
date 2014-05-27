#ifndef __TIN_DATAMANAGER_H__
#define __TIN_DATAMANAGER_H__

#include <vector>
#include <set>
#include "DataStructure/TinVertex.h"
#include "DataStructure/TinHalfEdge.h"
#include "DataStructure/TinDelaunay.h"

class CTinDataManager
{
typedef std::vector<ITinVertex*> VertexList;
typedef std::set<ITinHalfEdge*> HalfEdgeList;
typedef std::set<CTinDelaunay*> DelaunayList;


public:
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

	CTinDataManager();
	~CTinDataManager();

	ITinVertex* GetVertex(int idx);

	ITinVertex* CreateVertex();
	ITinHalfEdge* CreateEdge();
	//CTinDelaunay* CreateDelaunay();

	int GetCountOfVertexs();
	void SetRamdomVertexs(int DATA_NUM);

	void MakeDelaunayEdge();
	void MakeDelaunayFace();

	void PrintVertexList();
	void PrintEdgeList();
	void PrintFaceList();

private:
	// D&C Tin 알고리즘부분
	void _DivideAndConquer(CTinDelaunay& delaunay);
	void _Del_Link( CTinDelaunay& result, CTinDelaunay& left, CTinDelaunay& right );
	void _Del_Init_Seg( CTinDelaunay& del);
	void _Del_Init_Tri( CTinDelaunay& del);
	LEFT_RIGHT  _Classify_Point_Seg( ITinVertex *s, ITinVertex *e, ITinVertex *pt );
	LEFT_RIGHT  _Del_Classify_Point( ITinHalfEdge *d, ITinVertex *pt );
	IN_OUT_CIRCLE _In_Circle( ITinVertex *pt0, ITinVertex *pt1, ITinVertex *pt2, ITinVertex *p );
	ITinHalfEdge* _Del_Get_Lower_Supportant( CTinDelaunay& left, CTinDelaunay& right );
	ITinHalfEdge* _Del_Valid_Link( ITinHalfEdge *b );
	ITinHalfEdge* _Del_Valid_Left( ITinHalfEdge* b );
	ITinHalfEdge* _Del_Valid_Right( ITinHalfEdge *b );
	void _Del_Remove_Halfedge( ITinHalfEdge *d );
	void _Del_Remove_Single_Halfedge( ITinHalfEdge *d );
	void _Halfedge_Free( ITinHalfEdge* d );
	void _Build_Halfedge_Face( CTinDelaunay *del, ITinHalfEdge *d );

private:
	VertexList 	m_VertexList;
	HalfEdgeList 	m_HalfEdgeList;
	CTinDelaunay	m_delaunay;
};

#endif //__TIN_DATAMANAGER_H__
