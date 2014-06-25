#include "UstTinLib/Include.h"
#include <time.h>

int main(int argc, char* args[]) {
	clock_t StartClock, LoadingClock, TinClock;

	///////////////////////////////////////
	// TinEdgeMaker 생성
	CTinEdgeMaker tinEdgeMaker;
	time_t htime;
	tm* pTime;

	///////////////////////////////////////
	// Memory를 사용하여 Edge를 만들 경우
//	CTinMemStorage  tinMemStorage;
//	tinMemStorage.SetRamdomVertexs(10000);
//	StartClock = clock();
//	tinEdgeMaker.AttachTinStorage(&tinMemStorage);

	///////////////////////////////////////
	// OrientDB를 사용하여 edge를 만들 경우
	CTinOrientDBStorage tinDbStorage;
	tinDbStorage.InitDB("127.0.0.1", "tin3", "root", "root", "xyz", "xyUniq", "HalfEdge");
	StartClock = clock();
	//tinDbStorage.SetCleanNRamdomVertexs(100);
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
