#ifndef __ITIN_STORAGEMANAGER_H__
#define __ITIN_STORAGEMANAGER_H__

class ITinStorageManager
{
public :
	virtual ~ITinStorageManager(){};
	virtual VertexPtr 		GetVertex(int idx) = 0;
	virtual int 				GetCountOfVertexs() = 0;

	virtual EdgePtr 			CreateEdge() = 0;
	virtual int				GetCountOfEdges() = 0;

	virtual bool 				DeleteHalfEdge(EdgePtr pEdge) = 0;
	virtual void				PrintEdgeList() = 0;

	virtual void				FlushCache() = 0;
	virtual void				Close() = 0;

	virtual void				SetNullFaceEdge(EdgePtr pNullFaceEdge) = 0;
};

#endif //__ITIN_STORAGEMANAGER_H__
