#ifndef __UST_TIN_MAKER_H__
#define __UST_TIN_MAKER_H__

#include <liborient/liborient.h>
#include "../Common.h"
#include "../BaseStorage/ITinStorageManager.h"

class CTinOrientDBStorage
{
	//TinMaker에서 사용될 Error Code를 지정한다.
	enum ErrCode{
		RET_OK,						// ERROR 없음
		ERR_DB_URL,					// DB URL로 DB를 찾을수 없음
		ERR_ID_PW,						// DB ID/PW 로 접속할 수 없음.
		ERR_DB_NAME,					// DB Name 오류
		ERR_VERTEX_CLASS_NOT_FOUND,	// DB 내 VertexClass를 찾을 수 없음.
		ERR_ETC						// 기타 ERROR
	};
public:
	CTinOrientDBStorage();
	~CTinOrientDBStorage();
	ErrCode InitDB(String url, String dbName, String id, String pw, String vertexClassName, String edgeClassName);

private:
	bool _ConnectDBServer();
	bool _CheckDBName();
	bool _CheckVertexClass();

private:
	////////////////////////////
	// DB 기본정보
	////////////////////////////
	String m_Url;					// DB Server URL
	String	m_DBPort;				// DB Server Port
	String m_Id;					// DB Server 접속 ID
	String m_Pw;					// DB Server 접속 PW
	String m_DbName;				// DB Name
	String m_VertexClassName;	// HalfEdge로 구성될 Vertex들
	String m_EdgeClassName;		// 최종 산출물인 HalfEdge Class의 Name

	////////////////////////////
	// Orient DB 접속관련 변수
	////////////////////////////
	orientdb* m_OrientDB;
	orientdb_con* m_OrientCon;
};

#endif //__UST_TIN_MAKER_H__
