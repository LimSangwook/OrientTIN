#ifndef __TIN_DATAMANAGER_H__
#define __TIN_DATAMANAGER_H__

#include <vector>
#include <set>
#include "DataStructure/TinVertex.h"
#include "DataStructure/TinHalfEdge.h"
#include "DataStructure/TinDelaunay.h"

class CTinDataManager
{
typedef std::vector<CTinVertex*> VertexList;
typedef std::set<CTinHalfEdge*> HalfEdgeList;
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

	CTinVertex* GetVertex(int idx);

	CTinVertex* CreateVertex();
	CTinHalfEdge* CreateEdge();
	//CTinDelaunay* CreateDelaunay();

	int GetCountOfVertexs();
	void SetRamdomVertexs(int DATA_NUM);

	void MakeDelaunayEdge();
	void MakeDelaunayFace();

	void PrintVertexList();
	void PrintEdgeList();
	void PrintFaceList();

private:
	//알고리즘부분
	void _DivideAndConquer(CTinDelaunay& delaunay);
	void _Del_Link( CTinDelaunay& result, CTinDelaunay& left, CTinDelaunay& right );
	void _Del_Init_Seg( CTinDelaunay& del);
	void _Del_Init_Tri( CTinDelaunay& del);
	LEFT_RIGHT  _Classify_Point_Seg( CTinVertex *s, CTinVertex *e, CTinVertex *pt );
	LEFT_RIGHT  _Del_Classify_Point( CTinHalfEdge *d, CTinVertex *pt );
	IN_OUT_CIRCLE _In_Circle( CTinVertex *pt0, CTinVertex *pt1, CTinVertex *pt2, CTinVertex *p );
	CTinHalfEdge* _Del_Get_Lower_Supportant( CTinDelaunay& left, CTinDelaunay& right );
	CTinHalfEdge* _Del_Valid_Link( CTinHalfEdge *b );
	CTinHalfEdge* _Del_Valid_Left( CTinHalfEdge* b );
	CTinHalfEdge* _Del_Valid_Right( CTinHalfEdge *b );
	void _Del_Remove_Halfedge( CTinHalfEdge *d );
	void _Del_Remove_Single_Halfedge( CTinHalfEdge *d );
	void _Halfedge_Free( CTinHalfEdge* d );
	void _Build_Halfedge_Face( CTinDelaunay *del, CTinHalfEdge *d );

private:
	VertexList 	m_VertexList;
	HalfEdgeList 	m_HalfEdgeList;
	CTinDelaunay	m_delaunay;
};

#endif //__TIN_DATAMANAGER_H__
