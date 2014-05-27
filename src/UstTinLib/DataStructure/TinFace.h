#ifndef __TIN_FACE_H__
#define __TIN_FACE_H__

class CTinHalfEdge;

class CTinFace
{
public:
	CTinFace();
	~CTinFace();

	CTinHalfEdge* GetHalfEdge();
	void SetHalfEdge(CTinHalfEdge* pEdge);

	int GetNumberOfVertexs();
	void SetNumberOfVertexs(int num);
private:
	CTinHalfEdge*	m_HalfEdge;			/* a pointing half edge */
	int				m_nNumVertexs;		/* number of vertices on this face */
};

#endif
