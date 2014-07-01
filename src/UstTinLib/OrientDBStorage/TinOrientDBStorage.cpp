#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "TinOrientDBStorage.h"
#include <time.h>

CTinOrientDBStorage* CTinOrientDBStorage::instance;

void orient_debug(const char *msg) {
	fprintf(stderr, "program: %s", msg);
}

void _PrintTime(String msg)
{
	time_t htime;
	tm* pTime;
	time(&htime);
	pTime = localtime(&htime);
	//std::cout << msg << " 시간 : " << asctime(pTime);
}

void CTinOrientDBStorage::SetNullFaceEdge(EdgePtr pNullFaceEdge)
{
	CTinOrientDBHalfEdge* pEdge = (CTinOrientDBHalfEdge*)(pNullFaceEdge.get());
	if (pEdge) {
		m_NullFaceEdge = pEdge->GetRID();
	}
}

CTinOrientDBStorage::~CTinOrientDBStorage()
{
	Close();
}

void CTinOrientDBStorage::FlushCache()
{
	// Edge Cache FLush
	if ((int)m_EdgeCache.size() >= m_MaXEdgeCache) {
		_FlushEdgeCache();
	}
	// Vertex Cache FLush
	if ((int)m_VertexCache.size() >= m_MaXVertexCache) {
		_FlushVertexCache();
	}
}

void CTinOrientDBStorage::Close()
{
	// 캐시된 넘들을 FlushAll 해준다.
	_FlushEdgeCache();
	_FlushVertexCache();
	_RemoveDeletedEdge();
}

bool CTinOrientDBStorage::_RemoveDeletedEdge()
{
	m_JNIOrientDB.RemoveDeletedEdge(m_NullFaceEdge);
	return true;
}
CTinOrientDBStorage::CTinOrientDBStorage()
{
	m_MaXVertexCache = 500000;
	m_MaXEdgeCache = 500000;

	m_nTotalRemoveEdgeCount=0;
	m_nTotalCreateEdgeCount=0;

	m_nCreatedMemoryEdge = 0;

	m_NowEdgeID = 0;
	m_MaxEdgeID = 0;

	CTinOrientDBStorage::instance = this;
}

bool CTinOrientDBStorage::InitDB(String url, String dbName, String id, String pw, String vertexClassName, String indexName, String edgeClassName)
{
 	if (!m_JNIOrientDB.InitDB(url, dbName, id, pw, vertexClassName, indexName, edgeClassName)) {
		return false;
	}

	return true;
}

bool CTinOrientDBStorage::_CreateBlankEdge(int num)
{
	String strRID = m_JNIOrientDB.CreateBlankEdge(num);
	int pos = strRID.find(":");
	m_EdgeClassID = strRID.substr(0, pos + 1);
	String nowID = strRID.substr(pos + 1 , strRID.length() - pos);
	m_NowEdgeID = std::atoi(nowID.c_str());
	m_MaxEdgeID = m_NowEdgeID + num - 1;
	return true;
}

void CTinOrientDBStorage::_FlushEdgeCache()
{
	if (m_EdgeCache.size() < 1) {
		return;
	}

	_PrintTime("\t_FlushEdgeCache 시작");

	if (m_nCreatedMemoryEdge > 0) {
		//_PrintTime("\t\t_CreateBlankEdge 시작");
		//std::cout << "\t\t m_nCreatedMemoryEdge : " << m_nCreatedMemoryEdge << " m_EdgeCache.size() : " << m_EdgeCache.size() << "\n";
		_CreateBlankEdge(m_nCreatedMemoryEdge);
		//_PrintTime("\t\t_CreateBlankEdge 종료");
	}
	/////////////////////////////////////////
	// 모든 메모리 Edge에 실제 RID를 부여한다.
	/////////////////////////////////////////
	std::map<RID,EdgePtr>::iterator iter =  m_EdgeCache.begin();
	for (;iter != m_EdgeCache.end() ; iter++) {
		CTinOrientDBHalfEdge* pEdge = (CTinOrientDBHalfEdge*)(iter->second.get());
		if (pEdge->IsMemory()){
			char str[20];
			sprintf(str,"%d",m_NowEdgeID);

			String newRID	= m_EdgeClassID + str;
			String oldRID = pEdge->GetRID();

			m_MemoryRIDList[newRID] = oldRID;
			if (m_NullFaceEdge == oldRID) {
				m_NullFaceEdge = newRID;
			}

			CTinOrientDBVertex* pVertex = (CTinOrientDBVertex*)(pEdge->GetVertex().get());
			if (pVertex->GetRIDHalfEdge() == oldRID) {
				pVertex->SetRIDHalfEdge(newRID);
			}
			pEdge->SetRID(newRID);
			pEdge->SetSyncDB();
			m_NowEdgeID++;
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// 모든 Edge의 Pair, CCW, CW 에 대하여 실제 RID를 부여하고 DB Update 한다.
	/////////////////////////////////////////////////////////////////////////////
//	_PrintTime("\t\t_FlushEdgeCache Pair, CCW, CW RID 부여 몇 Update 시작");
	iter =  m_EdgeCache.begin();
	int nUpdateCnt = 0;
	String strEdgeDatas;
	std::vector<RID> vRIDList;
	for (;iter != m_EdgeCache.end() ; iter++) {
		CTinOrientDBHalfEdge* pEdge = (CTinOrientDBHalfEdge*)(iter->second.get());
		if(pEdge->IsDirty() == false) {
			continue;
		}
		pEdge->SetRIDPair(pEdge->GetRIDPair());
		pEdge->SetRIDCCW(pEdge->GetRIDCCW());
		pEdge->SetRIDCW(pEdge->GetRIDCW());
		if (pEdge->GetRIDEndVertex() == "none") {
			CTinOrientDBVertex* pPairV = (CTinOrientDBVertex*)pEdge->GetPairEdge()->GetVertex().get();
			pEdge->SetRIDEndVertex(pPairV->GetRID());
		}

		strEdgeDatas += pEdge->GetRID() + ";";
		strEdgeDatas += pEdge->GetRIDVertex() + ";";
		strEdgeDatas += pEdge->GetRIDEndVertex() + ";";
		strEdgeDatas += pEdge->GetRIDPair() + ";";
		strEdgeDatas += pEdge->GetRIDCCW() + ";";
		strEdgeDatas += pEdge->GetRIDCW() + ";";
		strEdgeDatas += "/";
		nUpdateCnt++;
		if (nUpdateCnt % 1000 == 0 ) {
			m_JNIOrientDB.UpdateHalfEdge(strEdgeDatas);
		}
		if (pEdge->IsDeleted()) {
			vRIDList.push_back(pEdge->GetRID());
		}
	}
	m_JNIOrientDB.UpdateHalfEdge(strEdgeDatas);
	std::vector<RID>::iterator RIDListIter = vRIDList.begin();
	String strDelRID = "";
	for (int idx = 1 ;RIDListIter != vRIDList.end() ; RIDListIter++, idx++) {
		strDelRID += *RIDListIter + ";";
		if (idx % 1000 == 0) {
			m_JNIOrientDB.DeleteEdge(strDelRID);
		}
	}
	m_JNIOrientDB.DeleteEdge(strDelRID);
	//_PrintTime("\t\t_FlushEdgeCache Pair, CCW, CW RID 부여 몇 Update 종료");

	//////////////////////////////////////////////
	// 참조가 없는 EdgePtr을 Cache에서 제거한다.
	//////////////////////////////////////////////
	iter =  m_EdgeCache.begin();
	for (;iter != m_EdgeCache.end() ; iter++) {
		if (iter->second.use_count() < 2) {
			CTinOrientDBHalfEdge* pEdge = (CTinOrientDBHalfEdge*)(iter->second.get());
			std::map<RID,RID>::iterator oldRIDIter = m_MemoryRIDList.find(pEdge->GetRID());
			if (oldRIDIter != m_MemoryRIDList.end()) {
				int k = m_EdgeCache.erase(oldRIDIter->second);
				m_MemoryRIDList.erase(oldRIDIter);
				if (k != 1) {
					 m_EdgeCache.erase(iter);
				}
			} else {
				m_EdgeCache.erase(iter);
			}
		}
	}

	// 관련 변수 초기화
	m_nCreatedMemoryEdge = 0;

	//_PrintTime("\t_FlushEdgeCache 종료");
}

void CTinOrientDBStorage::_FlushVertexCache()
{
	if (m_VertexCache.size() < 1) {
		return;
	}
	std::map<RID,VertexPtr>::iterator iter =  m_VertexCache.begin();
	for (;iter != m_VertexCache.end() ; iter++) {
		if (iter->second.use_count() < 2) {
			m_JNIOrientDB.UpdateVertex((CTinOrientDBVertex*)(iter->second).get());
			m_VertexCache.erase(iter);
		}
	}
}

bool CTinOrientDBStorage::SetCleanNRamdomVertexs(int DataNum)
{
	return m_JNIOrientDB.SetCleanNRamdomVertexs(DataNum);
}

VertexPtr CTinOrientDBStorage::_GetStringToVertex(String& str)
{
	String vertexRID = _GetProperty(str, "rid");
	String X = _GetProperty(str, "x");
	String Y = _GetProperty(str, "y");
	String HalfEdgeRID = _GetProperty(str, "halfedge");

	if (HalfEdgeRID == "null")
		HalfEdgeRID = "";

	CTinOrientDBVertex* pV = new CTinOrientDBVertex(vertexRID, atof(X.c_str()), atof(Y.c_str()), HalfEdgeRID);

	return VertexPtr(pV);
}

VertexPtr CTinOrientDBStorage::GetVertex(int idx)
{
	if ((int)m_VertexCache.size() >= m_MaXVertexCache) {
		_FlushVertexCache();
	}

	String strV = m_JNIOrientDB.GetVertex(idx);

	VertexPtr vPtr = _GetStringToVertex(strV);

	// Fill Cache
	m_VertexCache[((CTinOrientDBVertex*)(vPtr.get()))->GetRID()] = vPtr;
	return vPtr;
}

String CTinOrientDBStorage::_GetProperty(String json, String propertyName)
{
	String token = "," + propertyName + ":";
	int findPropertyName = json.find(token) ;
	if ((findPropertyName == -1)) {
		token = "@" + propertyName + ":";
		findPropertyName = json.find(token);
		if ((findPropertyName == -1)) {
			return "";
		}
	}

	int startPostion = findPropertyName + token.length();
	int endPostion = -1;
	if (startPostion >= (int)json.length()) {
		endPostion = json.length() - 1;
	} else if ((endPostion = json.substr(startPostion + 1, json.length() - startPostion).find(',')) == -1) {
		endPostion = json.length() - 1;
	} else {
		endPostion++;
	}

	String retStr = json.substr(startPostion, endPostion);
	if (retStr =="null")	{
		retStr == "0";
	}
	return retStr;
}

VertexPtr CTinOrientDBStorage::GetVertex(RID vertexRID)
{
	if ((int)m_VertexCache.size() >= m_MaXVertexCache) {
		_FlushVertexCache();
	}

	////////////////////////////
	// Find Cache
	std::map<RID,VertexPtr>::iterator iter = m_VertexCache.find(vertexRID);

	if (iter != m_VertexCache.end()) {
		return iter->second;
	}

	////////////////////////////
	// Cache not found
	String strV = m_JNIOrientDB.GetVertex(vertexRID);;

	VertexPtr vPtr = _GetStringToVertex(strV);
	m_VertexCache[vertexRID] = vPtr;

	return vPtr;
}

EdgePtr CTinOrientDBStorage::GetHalfEdge(RID RealEdgeRID)
{
	RID EdgeRID = RealEdgeRID;
	std::map<RID,RID>::iterator oldRIDIter = m_MemoryRIDList.find(RealEdgeRID);
	if (oldRIDIter != m_MemoryRIDList.end()) {
		EdgeRID = oldRIDIter->second;
	}

	std::map<RID,EdgePtr>::iterator iter = m_EdgeCache.find(EdgeRID);

	if (iter != m_EdgeCache.end()) {
		return iter->second;
	}

	String strE = m_JNIOrientDB.GetHalfEdge(RealEdgeRID);

	String strEdgeRID = _GetProperty(strE, "rid");
	String strIn = _GetProperty(strE, "in");
	String strOut = _GetProperty(strE, "out");
	String strPair = _GetProperty(strE, "pair");
	String strCCW = _GetProperty(strE, "ccw");
	String strCW = _GetProperty(strE, "cw");

	CTinOrientDBHalfEdge* pV = new CTinOrientDBHalfEdge(strEdgeRID, strOut, strIn, strPair, strCCW, strCW);

	EdgePtr edgePtr = EdgePtr(pV);
	m_EdgeCache[strEdgeRID] = edgePtr;

	return edgePtr;
}

int CTinOrientDBStorage::GetCountOfVertexs()
{
	return m_JNIOrientDB.GetCountOfVertexs();
}

EdgePtr CTinOrientDBStorage::CreateEdge()
{
	char str[100];
	::sprintf(str,"%d", m_nTotalCreateEdgeCount);

	RID strEdgeRID = str;

	CTinOrientDBHalfEdge* pHalfEdge = new CTinOrientDBHalfEdge();
	pHalfEdge->SetRID(strEdgeRID);
	EdgePtr edgePtr = EdgePtr(pHalfEdge);
	m_EdgeCache[strEdgeRID] = edgePtr;
	m_nTotalCreateEdgeCount++;
	m_nCreatedMemoryEdge++;

	return edgePtr;
}

int	CTinOrientDBStorage::GetCountOfEdges()
{
	return m_JNIOrientDB.GetCountOfEdges();
}

bool CTinOrientDBStorage::DeleteHalfEdge(EdgePtr pEdge)
{
	CTinOrientDBHalfEdge* pDBEdge = ((CTinOrientDBHalfEdge*)(pEdge.get()));

	if (pDBEdge->IsMemory()) {
	//	m_nCreatedMemoryEdge--;
	}

	pDBEdge->SetDelete();
	m_nTotalRemoveEdgeCount++;
	return true;
}

void CTinOrientDBStorage::PrintEdgeList()
{
	std::cout << "### m_CreateEdgeCount " << m_nTotalCreateEdgeCount << "\n";
	std::cout << "### m_RemoveEdgeCount " << m_nTotalRemoveEdgeCount << "\n";
}
