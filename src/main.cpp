#include "UstTinLib/Include.h"
#include <time.h>

int main(int argc, char* args[]) {
	if (argc != 7) {
		std::cout << "MakeTIN <DBURL> <DBName> <ID> <PW> <VertexClassName> <VertexIndexName> <EdgeClassName>\n";
		return 0;
	}

	// Argument값 변수 초기화
	String strURL 		= args[0];
	String strDBName 		= args[1];
	String strID 			= args[2];
	String strPW 			= args[3];
	String strVClassName = args[4];
	String strVIndexName = args[5];
	String strEClassName = args[6];

	// 값이 안들어 없을때 Default 값
	if (argc!=7) {
		strURL 		= "127.0.0.1";
		strDBName 		= "tin";
		strID 			= "root";
		strPW 			= "root";
		strVClassName = "Vt";
		strVIndexName = "";
		strEClassName = "Eg";
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
	tinDbStorage.SetCleanNRamdomVertexs(10000);
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
