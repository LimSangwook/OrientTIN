#ifndef __ITIN_STORAGEMANAGER_H__
#define __ITIN_STORAGEMANAGER_H__
#include<boost/shared_ptr.hpp>

class ITinHalfEdge;
class ITinVertex;
typedef boost::shared_ptr<ITinVertex> VertexPtr;
typedef ITinHalfEdge* EdgePtr;

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
};

#endif //__ITIN_STORAGEMANAGER_H__
