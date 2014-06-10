#ifndef __COMMON_H__
#define __COMMON_H__

#include <iostream>
#include <boost/shared_ptr.hpp>

#include "BaseStorage/ITinHalfEdge.h"
typedef boost::shared_ptr<ITinHalfEdge> EdgePtr;

#include "BaseStorage/ITinVertex.h"
typedef boost::shared_ptr<ITinVertex> VertexPtr;

#include "BaseStorage/ITinStorageManager.h"
#include "BaseStorage/TinDelaunay.h"

typedef std::string String;
typedef std::string RID;


#endif //__COMMON_H__
