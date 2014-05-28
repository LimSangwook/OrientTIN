#include <math.h>
#include <memory>
#include <assert.h>
#include "TinEdgeMaker.h"
#include <iostream>

CTinEdgeMaker::CTinEdgeMaker() :
m_pTinStorage(0)
{
}

CTinEdgeMaker::~CTinEdgeMaker()
{
}

void CTinEdgeMaker::AttachTinStorage(ITinStorageManager* pStorage)
{
	m_pTinStorage = pStorage;
}
ITinVertex* CTinEdgeMaker::_GetVertex(int idx)
{
 return m_pTinStorage->GetVertex(idx);
}

int CTinEdgeMaker::_GetCountOfVertexs()
{
 return m_pTinStorage->GetCountOfVertexs();
}

ITinHalfEdge* CTinEdgeMaker::_CreateEdge()
{
	assert(m_pTinStorage != NULL);
	return m_pTinStorage->CreateEdge();
}
void PutVertexPoint(ITinVertex* pVertex)
{
 std::cout << "(" << pVertex->GetX() << " , " <<pVertex->GetY() << ")";
}
void CTinEdgeMaker::PrintVertexList()
{
 std::cout << "VertexList size : " << m_pTinStorage->GetCountOfVertexs() << "\n";
}
void CTinEdgeMaker::PrintEdgeList()
{
 std::cout << "EdgeList size : " << m_pTinStorage->GetCountOfEdges() << "\n";

}
void CTinEdgeMaker::PrintFaceList()
{
 std::cout << "FaceList size : " << m_delaunay.GetFaces().size() - 1 << "\n";
}

void CTinEdgeMaker::MakeDelaunayEdge()
{
	m_delaunay.SetStartPointIdx(0);
	m_delaunay.SetEndPointIdx(_GetCountOfVertexs() - 1);
	_DivideAndConquer(m_delaunay);
}
void CTinEdgeMaker::MakeDelaunayFace()
{
// int  i;
// ITinHalfEdge *curr;
// m_delaunay.SetNumberOfFaces(0);
// m_delaunay.GetFaces().clear();
// /* build external face first */
// _Build_Halfedge_Face(&m_delaunay, m_delaunay.GetRightMostEdge()->GetPairEdge());
// for( i = m_delaunay.GetStartPointIdx(); i <= m_delaunay.GetEndPointIdx(); i++ )
// {
//  curr = m_VertexList.at(i)->GetHalfEdge();
//  do {
//   _Build_Halfedge_Face(& m_delaunay, curr );
//   curr = curr->GetCCWEdge();
//  } while( curr != m_VertexList.at(i)->GetHalfEdge() );
// }
}
//void CTinEdgeMaker::_Build_Halfedge_Face( CTinDelaunay *del, ITinHalfEdge *d )
//{
// ITinHalfEdge *curr;
// /* test if the halfedge has already a pointing face */
// if( d->GetFace() != NULL )
//  return;
// CTinFace* f = new CTinFace();
// del->GetFaces().push_back(f);
// curr = d;
// f->SetHalfEdge(d);
// f->SetNumberOfVertexs(0);
// do {
//  curr->SetFace(f);
//  f->SetNumberOfVertexs(f->GetNumberOfVertexs() + 1);
//  curr = curr->GetPairEdge()->GetCWEdge();
// } while( curr != d );
// del->SetNumberOfFaces(del->GetNumberOfFaces() + 1);
//}
void CTinEdgeMaker::_DivideAndConquer(CTinDelaunay& delaunay)
{
 CTinDelaunay left, right;
 int   i, n;
 int start = delaunay.GetStartPointIdx();
 int end =  delaunay.GetEndPointIdx();
 n = (end - start + 1);
 if( n > 3 )
 {
  i  = (n / 2) + (n & 1);
  left.SetStartPointIdx(start);
  left.SetEndPointIdx(start + i - 1 );
  right.SetStartPointIdx(start + i);
  right.SetEndPointIdx(end);
  _DivideAndConquer(left);
  _DivideAndConquer(right);
  _Del_Link(delaunay, left, right );
 } else
  if( n == 3 )
   _Del_Init_Tri(delaunay);
  else
   if( n == 2 )
    _Del_Init_Seg(delaunay);
}
void CTinEdgeMaker::_Del_Link( CTinDelaunay& result, CTinDelaunay& left, CTinDelaunay& right )
{
 ITinVertex  *u, *v, *ml, *mr;
 ITinHalfEdge  *b;
 /* save the most right point and the most left point */
 ml  = left.GetLeftMostEdge()->GetVertex();
 mr  = right.GetRightMostEdge()->GetVertex();
 b  = _Del_Get_Lower_Supportant(left, right);
 u  = b->GetCCWEdge()->GetPairEdge()->GetVertex();
 v  = b->GetPairEdge()->GetCWEdge()->GetPairEdge()->GetVertex();
 while( _Del_Classify_Point(b, u) == ONLEFT ||
   _Del_Classify_Point(b, v) == ONLEFT )
 {
  b = _Del_Valid_Link(b);
  u = b->GetCCWEdge()->GetPairEdge()->GetVertex();
  v = b->GetPairEdge()->GetCWEdge()->GetPairEdge()->GetVertex();
 }
 right.SetRightMostEdge(mr->GetHalfEdge());
 left.SetLeftMostEdge(ml->GetHalfEdge());
 /* TODO: this part is not needed, and can be optimized */
 while( _Del_Classify_Point( right.GetRightMostEdge(), right.GetRightMostEdge()->GetCWEdge()->GetPairEdge()->GetVertex() ) == ONRIGHT )
  right.SetRightMostEdge(right.GetRightMostEdge()->GetCWEdge());
 while( _Del_Classify_Point( left.GetLeftMostEdge(), left.GetLeftMostEdge()->GetCWEdge()->GetPairEdge()->GetVertex() ) == ONRIGHT )
  left.SetLeftMostEdge(left.GetLeftMostEdge()->GetCWEdge());
 result.SetLeftMostEdge(left.GetLeftMostEdge());
 result.SetRightMostEdge(right.GetRightMostEdge());
 result.SetStartPointIdx(left.GetStartPointIdx());
 result.SetEndPointIdx(right.GetEndPointIdx());
}
ITinHalfEdge* CTinEdgeMaker::_Del_Valid_Link( ITinHalfEdge *b )
{
 ITinVertex *g, *g_p, *d, *d_p;
 ITinHalfEdge *gd, *dd, *new_gd, *new_dd;
 int  a;
 g = b->GetVertex();
 gd = _Del_Valid_Left(b);
 g_p = gd->GetVertex();

 d = b->GetPairEdge()->GetVertex();
 dd = _Del_Valid_Right(b);
 d_p = dd->GetVertex();
 if( g != g_p && d != d_p )
 {
  a = _In_Circle(g, d, g_p, d_p);
  if( a != ONCIRCLE )
  {
   if( a == IN_SIDE )
   {
    g_p = g;
    gd = b;
   }
   else
   {
    d_p = d;
    dd = b->GetPairEdge();
   }
  } else {
   //* create the 2 halfedges */
   new_gd = _CreateEdge();
   new_dd = _CreateEdge();
   //* setup new_gd and new_dd */
   new_gd->SetVertex(gd->GetVertex());
   new_gd->SetPairEdge(new_dd);
   new_gd->SetCWEdge(gd);
   new_gd->SetCCWEdge(gd->GetCCWEdge());
   gd->GetCCWEdge()->SetCWEdge(new_gd);
   gd->SetCCWEdge(new_gd);
   new_dd->SetVertex(b->GetPairEdge()->GetVertex());
   new_dd->SetPairEdge(new_gd);
   new_dd->SetCWEdge(b->GetPairEdge()->GetCWEdge());
   b->GetPairEdge()->GetCWEdge()->SetCCWEdge(new_dd);
   new_dd->SetCCWEdge(b->GetPairEdge());
   b->GetPairEdge()->SetCWEdge(new_dd);
   return new_gd;
  }
 }
 /* create the 2 halfedges */
 new_gd = _CreateEdge();
 new_dd = _CreateEdge();
 /* setup new_gd and new_dd */
 new_gd->SetVertex(gd->GetVertex());
 new_gd->SetPairEdge(new_dd);
 new_gd->SetCWEdge(gd);
 new_gd->SetCCWEdge(gd->GetCCWEdge());
 gd->GetCCWEdge()->SetCWEdge(new_gd);
 gd->SetCCWEdge(new_gd);
 new_dd->SetVertex(dd->GetVertex());
 new_dd->SetPairEdge(new_gd);
 new_dd->SetCWEdge(dd->GetCWEdge());
 dd->GetCWEdge()->SetCCWEdge(new_dd);
 new_dd->SetCCWEdge(dd);
 dd->SetCWEdge(new_dd);
 return new_gd;
}
ITinHalfEdge* CTinEdgeMaker::_Del_Valid_Right( ITinHalfEdge *b )
{
 ITinVertex  *g, *d, *u, *v;
 ITinHalfEdge  *c, *dd, *du;
 b = b->GetPairEdge();
 d = b->GetVertex();
 dd = b;
 g = b->GetPairEdge()->GetVertex();
 b = b->GetCWEdge();
 u = b->GetPairEdge()->GetVertex();
 du = b->GetPairEdge();
 v = b->GetCWEdge()->GetPairEdge()->GetVertex();
 if( _Classify_Point_Seg(g, d, u) == ONLEFT )
 {
  while( v != g && _In_Circle(g, d, u, v) == IN_SIDE )
  {
   c = b->GetCWEdge();
   du = c->GetPairEdge();
   _Del_Remove_Halfedge(b);
   b = c;
   u = du->GetVertex();
   v = b->GetCWEdge()->GetPairEdge()->GetVertex();
  }
 } else
  du = dd;
 return du;
}
ITinHalfEdge* CTinEdgeMaker::_Del_Valid_Left( ITinHalfEdge* b )
{
 ITinVertex  *g, *d, *u, *v;
 ITinHalfEdge  *c, *du, *dg;
 g = b->GetVertex();    /* base halfedge point */
 dg = b;
 d = b->GetPairEdge()->GetVertex();   /* alpha(halfedge) point */
 b = b->GetCCWEdge();
 u = b->GetPairEdge()->GetVertex();   /* sigma(alpha(halfedge)) point */
 du = b->GetPairEdge();
 v = b->GetCCWEdge()->GetPairEdge()->GetVertex(); /* alpha(sigma(sigma(halfedge)) point */
 if( _Classify_Point_Seg(g, d, u) == ONLEFT )
 {
  /* 3 points aren't colinear */
  /* as long as the 4 points belong to the same circle, do the cleaning */
  while( v != d && _In_Circle(g, d, u, v) == IN_SIDE )
  {
   c = b->GetCCWEdge();
   du = b->GetCCWEdge()->GetPairEdge();
   _Del_Remove_Halfedge(b);
   b = c;
   u = du->GetVertex();
   v = b->GetCCWEdge()->GetPairEdge()->GetVertex();
  }
 } else /* treat the case where the 3 points are colinear */
  du  = dg;
 return du;
}
void CTinEdgeMaker::_Del_Remove_Halfedge( ITinHalfEdge *d )
{
 ITinHalfEdge *alpha;
 alpha = d->GetPairEdge();
 _Del_Remove_Single_Halfedge(d);
 _Del_Remove_Single_Halfedge(alpha);
}
void CTinEdgeMaker::_Del_Remove_Single_Halfedge( ITinHalfEdge *d )
{
 ITinHalfEdge *sigma, *amgis, *alpha;
 sigma = d->GetCCWEdge();
 amgis = d->GetCWEdge();
 alpha = d->GetPairEdge();
 assert(sigma != NULL);
 assert(amgis != NULL);
 sigma->SetCWEdge(amgis);
 amgis->SetCCWEdge(sigma);
 /* check to see if we have already removed alpha */
 if( alpha )
  alpha->SetPairEdge(NULL);
 /* check to see if the vertex points to this halfedge */
 if( d->GetVertex()->GetHalfEdge()== d )
  d->GetVertex()->SetHalfEdge(sigma);
 if (sigma == d) {
  d->GetVertex()->SetHalfEdge(NULL);
 }
 /* finally free the halfedge */
 _Halfedge_Free(d);
}
void CTinEdgeMaker::_Halfedge_Free( ITinHalfEdge* d )
{
 assert( d != NULL );
 delete(d);
}
#define CCW(A,B,C) ((B->GetX()-A->GetX())*(C->GetY()-A->GetY())-(B->GetY()-A->GetY())*(C->GetX()-A->GetX()))
CTinEdgeMaker::IN_OUT_CIRCLE CTinEdgeMaker::_In_Circle( ITinVertex *pt0, ITinVertex *pt1, ITinVertex *pt2, ITinVertex *p )
{
 ITinVertex* A = pt0;
 ITinVertex* B = pt1;
 ITinVertex* C = pt2;
 ITinVertex* D = p;
 double ccw = CCW(A, B, C);
 double adx = A->GetX() - D->GetX();
 double ady = A->GetY() - D->GetY();
 double bdx = B->GetX() - D->GetX();
 double bdy = B->GetY() - D->GetY();
 double cdx = C->GetX() - D->GetX();
 double cdy = C->GetY() - D->GetY();
 double bdxcdy = bdx * cdy;
 double cdxbdy = cdx * bdy;
 double alift = adx * adx + ady * ady;
 double cdxady = cdx * ady;
 double adxcdy = adx * cdy;
 double blift = bdx * bdx + bdy * bdy;
 double adxbdy = adx * bdy;
 double bdxady = bdx * ady;
 double clift = cdx * cdx + cdy * cdy;
 double det = alift * (bdxcdy - cdxbdy) + blift * (cdxady - adxcdy) + clift * (adxbdy - bdxady);
 if (det == 0.0)
  return ONCIRCLE;
 if (ccw > 0) {
  if (det > 0)
   return IN_SIDE;
  else
   return OUT_SIDE;
 }
 else {
  if (det > 0)
   return OUT_SIDE;
  else
   return IN_SIDE;
 }
}
ITinHalfEdge* CTinEdgeMaker::_Del_Get_Lower_Supportant( CTinDelaunay& left, CTinDelaunay& right )
{
 ITinVertex *pl, *pr;
 ITinHalfEdge *right_d, *left_d, *new_ld, *new_rd;
 int  sl, sr;
 left_d = left.GetRightMostEdge();
 right_d = right.GetLeftMostEdge();
 do {
  pl  = left_d->GetCWEdge()->GetPairEdge()->GetVertex();
  pr  = right_d->GetPairEdge()->GetVertex();
  if( (sl = _Classify_Point_Seg(left_d->GetVertex(), right_d->GetVertex(), pl)) == ONRIGHT )
  {
   left_d = left_d->GetCWEdge()->GetPairEdge();
  }
  if( (sr = _Classify_Point_Seg(left_d->GetVertex(), right_d->GetVertex(), pr)) == ONRIGHT )
  {
   right_d = right_d->GetPairEdge()->GetCCWEdge();
  }
 } while( sl == ONRIGHT || sr == ONRIGHT );
 /* create the 2 halfedges */
 new_ld = _CreateEdge();
 new_rd = _CreateEdge();
 /* setup new_gd and new_dd */
 new_ld->SetVertex(left_d->GetVertex());
 new_ld->SetPairEdge(new_rd);
 new_ld->SetCWEdge(left_d->GetCWEdge());
 left_d->GetCWEdge()->SetCCWEdge(new_ld);
 new_ld->SetCCWEdge(left_d);
 left_d->SetCWEdge(new_ld);
 new_rd->SetVertex(right_d->GetVertex());
 new_rd->SetPairEdge(new_ld);
 new_rd->SetCWEdge(right_d->GetCWEdge());
 right_d->GetCWEdge()->SetCCWEdge(new_rd);
 new_rd->SetCCWEdge(right_d);
 right_d->SetCWEdge(new_rd);
 return new_ld;
}
CTinEdgeMaker::LEFT_RIGHT CTinEdgeMaker::_Del_Classify_Point( ITinHalfEdge *d, ITinVertex *pt )
{
 ITinVertex  *s, *e;
 s  = d->GetVertex();
 e  = d->GetPairEdge()->GetVertex();
 return _Classify_Point_Seg(s, e, pt);
}
void CTinEdgeMaker::_Del_Init_Seg(CTinDelaunay& del)
{
 ITinHalfEdge *d0, *d1;
 ITinVertex  *pt0, *pt1;
 int start =  del.GetStartPointIdx();
 /* setup pt0 and pt1 */
 pt0   = _GetVertex(start);
 pt1   = _GetVertex(start + 1);
 /* allocate the halfedges and setup them */
 d0 = _CreateEdge();
 d1 = _CreateEdge();
 d0->SetVertex(pt0);
 d1->SetVertex(pt1);
 d0->SetCCWEdge(d0);
 d0->SetCWEdge(d0);
 d1->SetCCWEdge(d1);
 d1->SetCWEdge(d1);
 d0->SetPairEdge(d1);
 d1->SetPairEdge(d0);
 pt0->SetHalfEdge(d0);
 pt1->SetHalfEdge(d1);
 del.SetRightMostEdge(d1);
 del.SetLeftMostEdge(d0);
}
void CTinEdgeMaker::_Del_Init_Tri( CTinDelaunay& del)
{
 int start = del.GetStartPointIdx();
 ITinHalfEdge *d0, *d1, *d2, *d3, *d4, *d5;
 ITinVertex  *pt0, *pt1, *pt2;
 /* setup the points */
 pt0     = _GetVertex(start);
 pt1     = _GetVertex(start + 1);
 pt2     = _GetVertex(start + 2);
 LEFT_RIGHT valLeftRight = _Classify_Point_Seg(pt0, pt2, pt1);
 ////////////////////////////////////
 // allocate the 6 halfedges
 ////////////////////////////////////
 d0 = _CreateEdge();
 d1 = _CreateEdge();
 d2 = _CreateEdge();
 d3 = _CreateEdge();
 // 원안에 있지 않으면 D4 D5까지 쓴다.
 if (valLeftRight != ONSEG) {
  d4 = _CreateEdge();
  d5 = _CreateEdge();
 }
 if( valLeftRight == ONLEFT ) /* first case */
 {
  /* set halfedges points */
  d0->SetVertex(pt0);  d1->SetVertex(pt2);  d2->SetVertex(pt1);
  d3->SetVertex(pt2);  d4->SetVertex(pt1);  d5->SetVertex(pt0);
  /* set points halfedges */
  pt0->SetHalfEdge(d0); pt1->SetHalfEdge(d2); pt2->SetHalfEdge(d1);
  /* sigma and sigma -1 setup */
  d0->SetCCWEdge(d5);  d0->SetCWEdge(d5);
  d1->SetCCWEdge(d3);  d1->SetCWEdge(d3);
  d2->SetCCWEdge(d4);  d2->SetCWEdge(d4);
  d3->SetCCWEdge(d1);  d3->SetCWEdge(d1);
  d4->SetCCWEdge(d2);  d4->SetCWEdge(d2);
  d5->SetCCWEdge(d0);  d5->SetCWEdge(d0);
  /* set halfedges alpha */
  d0->SetPairEdge(d3);  d3->SetPairEdge(d0);
  d1->SetPairEdge(d4);  d4->SetPairEdge(d1);
  d2->SetPairEdge(d5);  d5->SetPairEdge(d2);
  del.SetRightMostEdge(d1);
  del.SetLeftMostEdge(d0);
 } else if( valLeftRight == ONRIGHT )/* 2nd case */
 {
  /* set halfedges points */
  d0->SetVertex(pt0);  d1->SetVertex(pt1);  d2->SetVertex(pt2);
  d3->SetVertex(pt1);  d4->SetVertex(pt2);  d5->SetVertex(pt0);
  /* set points halfedges */
  pt0->SetHalfEdge(d0);  pt1->SetHalfEdge(d1);  pt2->SetHalfEdge(d2);
  /* sigma and sigma -1 setup */
  d0->SetCCWEdge(d5);  d0->SetCWEdge(d5);
  d1->SetCCWEdge(d3);  d1->SetCWEdge(d3);
  d2->SetCCWEdge(d4);  d2->SetCWEdge(d4);
  d3->SetCCWEdge(d1);  d3->SetCWEdge(d1);
  d4->SetCCWEdge(d2);  d4->SetCWEdge(d2);
  d5->SetCCWEdge(d0);  d5->SetCWEdge(d0);
  /* set halfedges alpha */
  d0->SetPairEdge(d3);  d3->SetPairEdge(d0);
  d1->SetPairEdge(d4);  d4->SetPairEdge(d1);
  d2->SetPairEdge(d5);  d5->SetPairEdge(d2);
  del.SetRightMostEdge(d2);
  del.SetLeftMostEdge(d0);
 }else if( valLeftRight == ONSEG )
 {
  /* set halfedges points */
  d0->SetVertex(pt0);  d1->SetVertex(pt1);
  d2->SetVertex(pt1);  d3->SetVertex(pt2);
  /* set points halfedges */
  pt0->SetHalfEdge(d0);  pt1->SetHalfEdge(d2);  pt2->SetHalfEdge(d3);
  /* sigma and sigma -1 setup */
  d0->SetCCWEdge(d0);  d0->SetCWEdge(d0);
  d1->SetCCWEdge(d2);  d1->SetCWEdge(d2);
  d2->SetCCWEdge(d1);  d2->SetCWEdge(d1);
  d3->SetCCWEdge(d3);  d3->SetCWEdge(d3);
  /* set halfedges alpha */
  d0->SetPairEdge(d1);  d1->SetPairEdge(d0);
  d2->SetPairEdge(d3);  d3->SetPairEdge(d2);
  del.SetRightMostEdge(d3);
  del.SetLeftMostEdge(d0);
 }
}
CTinEdgeMaker::LEFT_RIGHT CTinEdgeMaker::_Classify_Point_Seg( ITinVertex *s, ITinVertex *e, ITinVertex *pt )
{
 double  res;
 double se_x, se_y, spt_x, spt_y;
 se_x = (e->GetX() - s->GetX());
 se_y = (e->GetY() - s->GetY());
 spt_x = (pt->GetX() - s->GetX());
 spt_y = (pt->GetY() - s->GetY());
 res = (( se_x * spt_y ) - ( se_y * spt_x ));
 if( res < 0.0f )
  return ONRIGHT;
 else if( res > 0.0f )
  return ONLEFT;
 return ONSEG;
}
