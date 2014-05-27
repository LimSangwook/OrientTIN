#include <stdio.h>
#include <liborient/liborient.h>
#include "UstTinLib/UstTinDB.h"

void my_debug(const char *msg) {
	fprintf(stderr, "program: %s", msg);
}
int main() {
	CUstTinDB tinDB;

	tinDB.InitFromOrientDB("127.0.0.1", "USTTin", "root", "root", "V", "HalfEdge");
	tinDB.MakeTin();

	return 0;
}
