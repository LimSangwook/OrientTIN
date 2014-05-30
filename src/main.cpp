#include "UstTinLib/Include.h"
#include <time.h>

int main() {
	clock_t StartClock = clock(), LoadingClock, TinClock;
	///////////////////////////////////////
	// TinEdgeMaker 생성
	CTinEdgeMaker tinEdgeMaker;

	///////////////////////////////////////
	// Memory를 사용하여 Edge를 만들 경우
//	CTinMemStorage  tinMemStorage;
//	tinMemStorage.SetRamdomVertexs(100);
//	tinEdgeMaker.AttachTinStorage(&tinMemStorage);

	///////////////////////////////////////
	// OrientDB를 사용하여 edge를 만들 경우
	CTinOrientDBStorage tinDbStorage;
	tinDbStorage.InitDB("127.0.0.1", "tintest", "root", "root", "TestVertex", "HalfEdge");
	tinDbStorage.SetCleanNRamdomVertexs(100);
	tinEdgeMaker.AttachTinStorage((ITinStorageManager*)&tinDbStorage);

	tinEdgeMaker.PrintVertexList();		// Vertex 갯수 출력

	LoadingClock = clock();
	std::cout << "Load Points Time : " << (double)(LoadingClock - StartClock)/CLOCKS_PER_SEC << " sec" << std::endl;

	tinEdgeMaker.MakeDelaunayEdge();	// Edge 만들기

	TinClock = clock();
	tinEdgeMaker.PrintEdgeList();		// Edge 갯수 출력
	std::cout << "Make Tin Time : " << (double)(TinClock - LoadingClock)/CLOCKS_PER_SEC << " sec" << std::endl;

	return 0;
}
