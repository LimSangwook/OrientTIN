#include <math.h>
#include <memory>
#include <assert.h>
#include "UstTinMaker.h"
#include <iostream>
CTinDataManager::CTinDataManager()
{
}
CTinDataManager::~CTinDataManager()
{
}
CTinVertex* CTinDataManager::GetVertex(int idx)
{
 return m_VertexList.at(idx);
}

int CTinDataManager::GetCountOfVertexs()
{
 return m_VertexList.size();
}

CTinVertex* CTinDataManager::CreateVertex()
{
 CTinVertex* pVertex = new CTinVertex;
 pVertex->idx = m_VertexList.size();
 m_VertexList.push_back(pVertex);
 return pVertex;
}
CTinHalfEdge* CTinDataManager::CreateEdge()
{
 CTinHalfEdge* pEdge = new CTinHalfEdge;
 m_HalfEdgeList.insert(pEdge);
 return pEdge;
}
void PutVertexPoint(CTinVertex* pVertex)
{
 std::cout << "(" << pVertex->GetX() << " , " <<pVertex->GetY() << ")";
}
void CTinDataManager::PrintVertexList()
{
 std::cout << "VertexList size : " << m_VertexList.size() << "\n";
 VertexList::iterator iter = m_VertexList.begin();
// for(int i = 0; iter != m_VertexList.end() ; iter++, i++) {
//  std::cout << "Vertex ("<<i<<") : ";
//  PutVertexPoint(*iter);
//  std:: cout << "\n";
// }
}
void CTinDataManager::PrintEdgeList()
{
 std::cout << "EdgeList size : " << m_HalfEdgeList.size() << "\n";
 HalfEdgeList::iterator iter = m_HalfEdgeList.begin();
// for(int i = 0; iter != m_HalfEdgeList.end() ; iter++, i++) {
//  std::cout << "Edge (" << i << ") : ";
//  PutVertexPoint((*iter)->GetVertex());
//  std::cout << " -> ";
//  PutVertexPoint((*iter)->GetPairEdge()->GetVertex());
//  std::cout << "\n";
// }
}
void CTinDataManager::PrintFaceList()
{
 std::cout << "FaceList size : " << m_delaunay.GetFaces().size() - 1 << "\n";
}
void CTinDataManager::SetRamdomVertexs(int DATA_NUM)
{
 int sq =(int) sqrt((double)DATA_NUM);
 int idx = 0;
 for (int i = 0 ; i < sq * 10 + 1 ; i += 10) {
  for(int j = 0 ; j < sq * 10 + 1 ; j += 10){
   double  x = (double)i;
   double y = (double)j;
   CTinVertex* pVertex = CreateVertex();
   pVertex->SetX(x);
   pVertex->SetY(y);
   idx++;
   if (idx == DATA_NUM) break;
  }
  if (idx == DATA_NUM) break;
 }
}
void CTinDataManager::MakeDelaunayEdge()
{
 m_delaunay.SetStartPointIdx(0);
 m_delaunay.SetEndPointIdx(GetCountOfVertexs() - 1);
 _DivideAndConquer(m_delaunay);
}
void CTinDataManager::MakeDelaunayFace()
{
 int  i;
 CTinHalfEdge *curr;
 m_delaunay.SetNumberOfFaces(0);
 m_delaunay.GetFaces().clear();
 /* build external face first */
 _Build_Halfedge_Face(&m_delaunay, m_delaunay.GetRightMostEdge()->GetPairEdge());
 for( i = m_delaunay.GetStartPointIdx(); i <= m_delaunay.GetEndPointIdx(); i++ )
 {
  curr = m_VertexList.at(i)->GetHalfEdge();
  do {
   _Build_Halfedge_Face(& m_delaunay, curr );
   curr = curr->GetCCWEdge();
  } while( curr != m_VertexList.at(i)->GetHalfEdge() );
 }
}
void CTinDataManager::_Build_Halfedge_Face( CTinDelaunay *del, CTinHalfEdge *d )
{
 CTinHalfEdge *curr;
 /* test if the halfedge has already a pointing face */
 if( d->GetFace() != NULL )
  return;
 CTinFace* f = new CTinFace();
 del->GetFaces().push_back(f);
 curr = d;
 f->SetHalfEdge(d);
 f->SetNumberOfVertexs(0);
 do {
  curr->SetFace(f);
  f->SetNumberOfVertexs(f->GetNumberOfVertexs() + 1);
  curr = curr->GetPairEdge()->GetCWEdge();
 } while( curr != d );
 del->SetNumberOfFaces(del->GetNumberOfFaces() + 1);
}
void CTinDataManager::_DivideAndConquer(CTinDelaunay& delaunay)
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
void CTinDataManager::_Del_Link( CTinDelaunay& result, CTinDelaunay& left, CTinDelaunay& right )
{
 CTinVertex  *u, *v, *ml, *mr;
 CTinHalfEdge  *b;
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
CTinHalfEdge* CTinDataManager::_Del_Valid_Link( CTinHalfEdge *b )
{
 CTinVertex *g, *g_p, *d, *d_p;
 CTinHalfEdge *gd, *dd, *new_gd, *new_dd;
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
   new_gd = CreateEdge();
   new_dd = CreateEdge();
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
 new_gd = CreateEdge();
 new_dd = CreateEdge();
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
CTinHalfEdge* CTinDataManager::_Del_Valid_Right( CTinHalfEdge *b )
{
 CTinVertex  *g, *d, *u, *v;
 CTinHalfEdge  *c, *dd, *du;
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
CTinHalfEdge* CTinDataManager::_Del_Valid_Left( CTinHalfEdge* b )
{
 CTinVertex  *g, *d, *u, *v;
 CTinHalfEdge  *c, *du, *dg;
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
void CTinDataManager::_Del_Remove_Halfedge( CTinHalfEdge *d )
{
 CTinHalfEdge *alpha;
 alpha = d->GetPairEdge();
 _Del_Remove_Single_Halfedge(d);
 _Del_Remove_Single_Halfedge(alpha);
}
void CTinDataManager::_Del_Remove_Single_Halfedge( CTinHalfEdge *d )
{
 CTinHalfEdge *sigma, *amgis, *alpha;
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
void CTinDataManager::_Halfedge_Free( CTinHalfEdge* d )
{
 assert( d != NULL );
 delete(d);
}
#define CCW(A,B,C) ((B.GetX()-A.GetX())*(C.GetY()-A.GetY())-(B.GetY()-A.GetY())*(C.GetX()-A.GetX()))
CTinDataManager::IN_OUT_CIRCLE CTinDataManager::_In_Circle( CTinVertex *pt0, CTinVertex *pt1, CTinVertex *pt2, CTinVertex *p )
{
 CTinVertex A = *pt0;
 CTinVertex B = *pt1;
 CTinVertex C = *pt2;
 CTinVertex D = *p;
 double ccw = CCW(A, B, C);
 double adx = A.GetX() - D.GetX();
 double ady = A.GetY() - D.GetY();
 double bdx = B.GetX() - D.GetX();
 double bdy = B.GetY() - D.GetY();
 double cdx = C.GetX() - D.GetX();
 double cdy = C.GetY() - D.GetY();
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
CTinHalfEdge* CTinDataManager::_Del_Get_Lower_Supportant( CTinDelaunay& left, CTinDelaunay& right )
{
 CTinVertex *pl, *pr;
 CTinHalfEdge *right_d, *left_d, *new_ld, *new_rd;
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
 new_ld = CreateEdge();
 new_rd = CreateEdge();
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
CTinDataManager::LEFT_RIGHT CTinDataManager::_Del_Classify_Point( CTinHalfEdge *d, CTinVertex *pt )
{
 CTinVertex  *s, *e;
 s  = d->GetVertex();
 e  = d->GetPairEdge()->GetVertex();
 return _Classify_Point_Seg(s, e, pt);
}
void CTinDataManager::_Del_Init_Seg(CTinDelaunay& del)
{
 CTinHalfEdge *d0, *d1;
 CTinVertex  *pt0, *pt1;
 int start =  del.GetStartPointIdx();
 /* setup pt0 and pt1 */
 pt0   = GetVertex(start);
 pt1   = GetVertex(start + 1);
 /* allocate the halfedges and setup them */
 d0 = CreateEdge();
 d1 = CreateEdge();
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
void CTinDataManager::_Del_Init_Tri( CTinDelaunay& del)
{
 int start = del.GetStartPointIdx();
 CTinHalfEdge *d0, *d1, *d2, *d3, *d4, *d5;
 CTinVertex  *pt0, *pt1, *pt2;
 /* setup the points */
 pt0     = GetVertex(start);
 pt1     = GetVertex(start + 1);
 pt2     = GetVertex(start + 2);
 LEFT_RIGHT valLeftRight = _Classify_Point_Seg(pt0, pt2, pt1);
 ////////////////////////////////////
 // allocate the 6 halfedges
 ////////////////////////////////////
 d0 = CreateEdge();
 d1 = CreateEdge();
 d2 = CreateEdge();
 d3 = CreateEdge();
 // 원안에 있지 않으면 D4 D5까지 쓴다.
 if (valLeftRight != ONSEG) {
  d4 = CreateEdge();
  d5 = CreateEdge();
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
CTinDataManager::LEFT_RIGHT CTinDataManager::_Classify_Point_Seg( CTinVertex *s, CTinVertex *e, CTinVertex *pt )
{
 CTinVertex  se, spt;
 double  res;
 se.SetX(e->GetX() - s->GetX());
 se.SetY(e->GetY() - s->GetY());
 spt.SetX(pt->GetX() - s->GetX());
 spt.SetY(pt->GetY() - s->GetY());
 res = (( se.GetX() * spt.GetY() ) - ( se.GetY() * spt.GetX() ));
 if( res < 0.0f )
  return ONRIGHT;
 else if( res > 0.0f )
  return ONLEFT;
 return ONSEG;
}
