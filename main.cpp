#include "AdjMatGraph.h"

void main() {
	AdjMatGraph g;

	for (int i = 0; i < 4; i++)
	g.insertVertex('A' + i);
	g.inserEdge(1, 0);
	g.inserEdge(0, 3);
	g.inserEdge(1, 2);
	g.inserEdge(1, 3);
	g.inserEdge(2, 3);
	printf("인접 행렬로 표현한 그래프 \n");
	g.display();

	FILE* fp;
	fopen_s(&fp, "../text_out.txt", "w"); 
	// (..:이 파일보다 한 단계 위 폴더 내의 text_out이라는 txt 파일을 write모드로 열겠다)
	g.display(fp);
	fclose(fp);

}