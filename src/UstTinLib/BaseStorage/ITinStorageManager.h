#ifndef __ITIN_STORAGEMANAGER_H__
#define __ITIN_STORAGEMANAGER_H__

class ITinHalfEdge;
class ITinVertex;

class ITinStorageManager
{
public :
	virtual ~ITinStorageManager(){};
	virtual ITinVertex* 		GetVertex(int idx) = 0;
	virtual int 				GetCountOfVertexs() = 0;

	virtual ITinHalfEdge* 	CreateEdge() = 0;
	virtual int				GetCountOfEdges() = 0;
};

#endif //__ITIN_STORAGEMANAGER_H__
