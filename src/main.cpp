#include "UstTinLib/Include.h"
#include <time.h>

int main(int argc, char* args[]) {
	if (argc != 8) {
		std::cout << "OrientTIN <IP> <DBName> <ID> <PW> <VertexClassName> <VertexIdxName> <EdgeClassName>\n";
		std::cout << "ex) OrientTIN 127.0.0.1 tin3 root root xyz xyUniq Eq\n";
//		return 0;
	}

	// Argument값 변수 초기화
	String strURL;
	String strDBName;
	String strID;
	String strPW;
	String strVClassName;
	String strVIndexName;
	String strEClassName;

	// 값이 안들어 없을때 Default 값
	if (argc!=8) {
		strURL 		= "127.0.0.1";
		strDBName 		= "tin3";
		strID 			= "root";
		strPW 			= "root";
		strVClassName = "xyz";
		strVIndexName = "xyUniq";
		strEClassName = "Eg";
	} else{
		strURL 		= args[1];
		strDBName 		= args[2];
		strID 			= args[3];
		strPW 			= args[4];
		strVClassName = args[5];
		strVIndexName = args[6];
		strEClassName = args[7];
	}


	///////////////////////////////////////
	// TinEdgeMaker 생성
	CTinEdgeMaker tinEdgeMaker;
	time_t htime;
	tm* pTime;
	clock_t StartClock, LoadingClock, TinClock;

	///////////////////////////////////////
	// Memory를 사용하여 Edge를 만들 경우
//	CTinMemStorage  tinMemStorage;
//	tinMemStorage.SetRamdomVertexs(10000);
//	StartClock = clock();
//	tinEdgeMaker.AttachTinStorage(&tinMemStorage);

	///////////////////////////////////////
	// OrientDB를 사용하여 edge를 만들 경우
	CTinOrientDBStorage tinDbStorage;
	tinDbStorage.InitDB(strURL, strDBName, strID, strPW, strVClassName, strVIndexName, strEClassName);


	StartClock = clock();
	//tinDbStorage.SetCleanNRamdomVertexs(1000);
	tinEdgeMaker.AttachTinStorage((ITinStorageManager*)&tinDbStorage);


	tinEdgeMaker.PrintVertexList();		// Vertex 갯수 출력
	LoadingClock = clock();
	std::cout << "Load Points Time : " << (double)(LoadingClock - StartClock)/CLOCKS_PER_SEC << " sec" << std::endl;

	time(&htime);
	pTime = localtime(&htime);
	std::cout << "MakeTin 시작 시간 : " << asctime(pTime);
	tinEdgeMaker.MakeDelaunayEdge();	// Edge 만들기
	time(&htime);
	pTime = localtime(&htime);
	std::cout << "MakeTin 종료 시간 : " << asctime(pTime);

	TinClock = clock();
	tinEdgeMaker.PrintEdgeList();		// Edge 갯수 출력
	std::cout << "Make Tin Time : " << (double)(TinClock - LoadingClock)/CLOCKS_PER_SEC << " sec" << std::endl;

	return 0;
}
