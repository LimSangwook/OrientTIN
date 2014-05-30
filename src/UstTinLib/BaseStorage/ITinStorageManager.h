#ifndef __ITIN_STORAGEMANAGER_H__
#define __ITIN_STORAGEMANAGER_H__
#include<boost/shared_ptr.hpp>

class ITinHalfEdge;
class ITinVertex;
typedef boost::shared_ptr<ITinVertex> VertexPtr;

class ITinStorageManager
{
public :
	virtual ~ITinStorageManager(){};
	virtual VertexPtr 		GetVertex(int idx) = 0;
	virtual int 				GetCountOfVertexs() = 0;

	virtual ITinHalfEdge* 	CreateEdge() = 0;
	virtual int				GetCountOfEdges() = 0;

	virtual bool 				DeleteHalfEdge(ITinHalfEdge* pEdge) = 0;
	virtual void				PrintEdgeList() = 0;
};

#endif //__ITIN_STORAGEMANAGER_H__
