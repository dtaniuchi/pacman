/*
	func2.c
*/

#include <stdio.h>
#include <stdlib.h>
#include "header2.h"

#define EST_TIME 100
#define NODE_NUM 10000


void ObjListCopy(const MOVE_OBJECT* objList1, MOVE_OBJECT* objList2, int nMoveObj)
{
	int i;
	for (i = 0; i < nMoveObj; i++) objList2[i] = objList1[i];printf("aa\n");

}

void Search(STAGE* stage, MOVE_OBJECT* objList, int nMoveObj)
{
	int i;
	int step, dir, cnt = 0;
	int nLRJ = 0, _nMoveObj;
	int* enemyPosHistory;
	NODE* root_node = (NODE*)malloc(sizeof(NODE) * NODE_NUM);
	NODE* parent_node;

	if (root_node == NULL) return;
	
	for (i = 0; i < nMoveObj; i++)
		if (objList[i].type == L || objList[i].type == R || objList[i].type == J) nLRJ++;
	
	enemyPosHistory = (int*)malloc(sizeof(int) * nLRJ * EST_TIME);
	EstimateLRJ(enemyPosHistory, objList, stage, nMoveObj);
	if (nMoveObj - nLRJ != deleteObjectLRJ(objList, nMoveObj)) printf("Warning: %d\n", nLRJ);
	
	_nMoveObj = nMoveObj - nLRJ;
	
	root_node->stage = *stage;
	root_node->objList = (MOVE_OBJECT*)malloc(sizeof(MOVE_OBJECT) * _nMoveObj);
	free(root_node->objList);
printf("%d\n",_nMoveObj);
	ObjListCopy(objList, root_node->objList, _nMoveObj);
	root_node->parent = NULL;
	
	parent_node = root_node;

	
	for (step = 0; step < EST_TIME; step++) {
		NODE tmp_node;
		
		tmp_node.stage = parent_node->stage;
		tmp_node.objList = (MOVE_OBJECT*)malloc(sizeof(MOVE_OBJECT) * _nMoveObj);
		ObjListCopy(parent_node->objList, tmp_node.objList, _nMoveObj);
		tmp_node.parent = parent_node;
		
		ProceedEnemy(&tmp_node.stage, tmp_node.objList, _nMoveObj);
		
		for (dir = DOWN; dir < COUNT; dir++) {
			NODE child_node;
			child_node.stage = tmp_node.stage;
			child_node.objList = (MOVE_OBJECT*)malloc(sizeof(MOVE_OBJECT) * _nMoveObj);
			ObjListCopy(tmp_node.objList, child_node.objList, _nMoveObj);
			child_node.parent = parent_node;
			if(ProceedMine(dir, &child_node.stage, child_node.objList, _nMoveObj)) {
				int flg = 0;
				MOVE_OBJECT* mine;
				for (i = 0; i < _nMoveObj; i++) {
					if (child_node.objList[i].type == M) { mine = &child_node.objList[i]; break; }
				}
				for (i = 0; i < nLRJ; i++) {
					int idx = enemyPosHistory[i * EST_TIME + step];
					if (mine->x + child_node.stage.width * mine->y == idx) { flg = 1; break; }
				}
				ShowAll(&child_node.stage, child_node.objList, _nMoveObj);
				if (!flg && !CheckClash(child_node.objList, _nMoveObj)) {
					cnt++;
					*(root_node + cnt) = child_node;
					if (CheckClear(&child_node.stage)) { printf("Cleared!\n"); FreeNodeMemory(root_node, NODE_NUM); return; }
				}
				else free(child_node.objList);
			}
			else free(child_node.objList);
		}
		parent_node++;
	}
	FreeNodeMemory(root_node, NODE_NUM);
	free(enemyPosHistory);
}

void EstimateLRJ(int* enemyPosHistory, MOVE_OBJECT* objList, STAGE* stage, int nMoveObj)
{
	int i, id = 0;
	int step = 0;
	
	for (i = 0; i < nMoveObj; i++) {
		MOVE_OBJECT* myobj = &objList[i];
	
		if (myobj->type == L || myobj->type == R || myobj->type == J)
		{
			enemyPosHistory[id * EST_TIME + step] = myobj->x + myobj->y * stage->width;
			
			while (step++ < EST_TIME) {
					ProceedEnemy(stage, myobj, 1);
					enemyPosHistory[id * EST_TIME + step] = myobj->x + myobj->y * stage->width;
			}
			id++;
		}
	}
}

int deleteObjectLRJ(MOVE_OBJECT* objList, int nMoveObj) {
	int i;
	
	for (i = 0; i < nMoveObj; i++) {
		MOVE_OBJECT* obj = &objList[i];
		if (obj->type == L || obj->type == R || obj->type == J) {
			int j = 0;
			while (i + j < nMoveObj){ *(obj + j) = *(obj + j + 1); j++; }
			nMoveObj--; i--;
		}
	}
	return nMoveObj;
}

void ProceedMineTechnically(void) {
}

void FreeNodeMemory(NODE* root, int n) {
	while(n-- > 0) {
		free((root+n)->objList);
	}
}
