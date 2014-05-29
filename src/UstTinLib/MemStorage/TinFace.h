#ifndef __TIN_FACE_H__
#define __TIN_FACE_H__

class ITinHalfEdge;

class CTinFace
{
public:
	CTinFace();
	~CTinFace();

	ITinHalfEdge* GetHalfEdge();
	void SetHalfEdge(ITinHalfEdge* pEdge);

	int GetNumberOfVertexs();
	void SetNumberOfVertexs(int num);
private:
	ITinHalfEdge*	m_HalfEdge;			/* a pointing half edge */
	int				m_nNumVertexs;		/* number of vertices on this face */
};

#endif
