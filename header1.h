/*
	header1.h
*/

#ifndef H1
#define H1

typedef enum { M, V, H, L, R, J, D, E, W } OBJECT_TYPE;
typedef enum { NO, END, PATH, INTERSECTION } POSITION_TYPE;

typedef enum { DOWN, LEFT, UP, RIGHT, COUNT, STAY } DIRECTION;
typedef enum { rLEFT, rSTRAIGHT, rRIGHT, rCOUNT } rDIRECTION;

typedef struct {
	int width;
	int height;
	char* grids;
} STAGE;


typedef struct {
	int prex;
	int prey;
	int x;
	int y;
	OBJECT_TYPE type;
} MOVE_OBJECT;

void Show(STAGE* stage);
int ReadStage(char* fname, STAGE* stage);
void ShowAll(STAGE* stage, MOVE_OBJECT* objList, int nMoveObj);

int isClashed(MOVE_OBJECT* myobj, MOVE_OBJECT* enemy);
int CheckClear(STAGE* stage);
int CheckClash(MOVE_OBJECT* objList, int nMoveObj);


OBJECT_TYPE GetType(char c);
static char myGetChar(OBJECT_TYPE t);
static POSITION_TYPE GetPositionType(STAGE* stage, int x, int y);

static int isProceedable(STAGE* stage, int x, int y);
static int isProceedableD(STAGE* stage, int prex, int prey, DIRECTION d);


int MakeMoveObjects(STAGE* stage, MOVE_OBJECT* objList);
int ProceedMine(DIRECTION d, STAGE* stage, MOVE_OBJECT* objList, int nMoveObj);
void ProceedEnemy(STAGE* stage, MOVE_OBJECT* objList, int nMoveObj);
DIRECTION ProceedEnemyVHOnIS(MOVE_OBJECT* obj, STAGE* stage, MOVE_OBJECT* objList, int nMoveObj);
DIRECTION ProceedEnemyLROnIS(MOVE_OBJECT* obj, STAGE* stage);
DIRECTION ProceedEnemyJOnIS(MOVE_OBJECT* obj, STAGE* stage);
static void Proceed(MOVE_OBJECT* obj, DIRECTION d);
static DIRECTION GetAbsoluteDirection(DIRECTION pred, rDIRECTION rd);

#endif
