/*
	ヘッダファイル
*/
#include "header1.h"

#ifndef H2
#define H2

typedef struct _node {
	STAGE stage;
	MOVE_OBJECT* objList;
	struct _node* parent;
} NODE;


void Search(STAGE* stage, MOVE_OBJECT* objList, int nMoveObj);
void EstimateLRJ(int* enemyPosHistory, MOVE_OBJECT* obj, STAGE* stage, int nMoveObj);
int deleteObjectLRJ(MOVE_OBJECT* objList, int nMoveObj);
void ObjListCopy(const MOVE_OBJECT* objList1, MOVE_OBJECT* objList2, int nMoveObj);
void ProceedMineTechnically(void);
void FreeNodeMemory(NODE* root, int n);
#endif