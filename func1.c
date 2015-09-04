/*
	func1.c
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "header1.h"


int CheckClear(STAGE* stage) {
	int w, h;
	for (h = 0; h < stage->height; h++)
		for (w = 0; w < stage->width; w++) 
			if(stage->grids[w + h * stage->width] == '.') return 0;
			
	return 1;
}

int CheckClash(MOVE_OBJECT* objList, int nMoveObj) {
	MOVE_OBJECT* myobj;
	MOVE_OBJECT* enemy;
	int n;
	
	for (n = 0; n < nMoveObj; n++)
		if (objList[n].type == M) { myobj = &objList[n]; break; }

	for (n = 0; n < nMoveObj; n++) 
		if(objList[n].type != M) 
		{
			enemy = &objList[n];
			if(isClashed(myobj, enemy)) return 1;
		}
			
	return 0;
}

int isClashed(MOVE_OBJECT* myobj, MOVE_OBJECT* enemy) {
	if (myobj->x == enemy->x)
	{
		if (myobj->y == enemy->y) return 1;
		else if (myobj->y == enemy->prey && myobj->prey == enemy->y) return 1;
	}
	else if (myobj->y == enemy->y)
	{
		if (myobj->x == enemy->prex && myobj->prex == enemy->x) return 1;
	}
	
	return 0;
}


void Show(STAGE* stage) {
	int w, h;
	for (h = 0; h < stage->height; h++) {
		for (w = 0; w < stage->width; w++) printf("%c", stage->grids[w + h * stage->width]);
		printf("\n");
	}
	printf("\n");
}

void ShowAll(STAGE* stage, MOVE_OBJECT* objList, int nMoveObj) {
	int w, h, n;
	MOVE_OBJECT* obj;
	
	for (h = 0; h < stage->height; h++) {
		for (w = 0; w < stage->width; w++) {
			char c = stage->grids[w + h * stage->width];
			for (n = 0; n < nMoveObj; n++) {
				obj = &objList[n];
				if (obj->x == w && obj->y == h) c = myGetChar(obj->type);
			}
			printf("%c", c);
		}
		printf("\n");
	}
	printf("\n");
}


int ReadStage(char* fname, STAGE* stage) {
	FILE *fp;

	if ((fp = fopen(fname, "r")) == NULL) {	/* オープン */
		printf("\aファイルをオープンできません。\n");
	} else {
		int nMoveObj = 0;
	    int n = 0;
		char* buf = (char*)malloc( sizeof( char ) * 100 );
		
	    /* ファイルの終端まで1行ずつ文字を読み取る */
	    while ( fgets(buf, 100, fp) != NULL ) {
	    	int i = 0;
	    	while (buf[i] != '\n') {
	    		char c = buf[i];
	    		stage->grids[n] = c;
	    		if (c != '.' && c != ' ' && c != '#') nMoveObj++;
	    		n++; i++;
	    	}
	    	if (stage->width == 0) stage->width = i;
	    	else if (stage->width != i) printf("Warning.\n");
	    	stage->height++;
	    }
	    if (n / stage->width != stage->height || n % stage->width != 0) printf("Warning.\n");
	    
	    free(buf);
	    fclose(fp);	/* クローズ */
	    
	    return nMoveObj;
	}
	return 0;

}

int MakeMoveObjects(STAGE* stage, MOVE_OBJECT* objList) {
	int nMoveObj = 0;
	int stageWidth = stage->width;
	int stageHeight = stage->height;
	int w, h;

	for (h = 0; h < stageHeight; h++) {
		for (w = 0; w < stageWidth; w++) {
			char c = stage->grids[w + h * stageWidth];
			OBJECT_TYPE t = GetType(c);
			if (t != D && t != E && t != W) {
				MOVE_OBJECT obj;
				obj.prex = -1; obj.prey = -1;
				obj.x = w; obj.y = h; obj.type = t;
				objList[nMoveObj] = obj;

				nMoveObj++;
				stage->grids[w + h * stageWidth] = ' ';
			}
		}
	}
	return nMoveObj;
}


OBJECT_TYPE GetType(char c) {
	OBJECT_TYPE t;
	switch(c) {
		case '@':
			t = M;
			break;
		case 'V':
			t = V;
			break;
		case 'H':
			t = H;
			break;
		case 'L':
			t = L;
			break;
		case 'R':
			t = R;
			break;
		case 'J':
			t = J;
			break;
		case '.':
			t = D;
			break;
		case ' ':
			t = E;
			break;
		case '#':
		default:
			t = W;
			break;
	}
	return t;
}

static char myGetChar(OBJECT_TYPE t) {
	char c;
	switch(t) {
		case M:
			c = '@';
			break;
		case V:
			c = 'V';
			break;
		case H:
			c = 'H';
			break;
		case L:
			c = 'L';
			break;
		case R:
			c = 'R';
			break;
		case J:
			c = 'J';
			break;
		case D:
			c = '.';
			break;
		case E:
			c = ' ';
			break;
		case W:
			c = '#';
			break;
		default:
			c = '?';
			break;
	}
	return c;
}

static POSITION_TYPE GetPositionType(STAGE* stage, int x, int y) {
	POSITION_TYPE pt;
	int count = 0;
	int d;
	
	for (d = 0; d < COUNT; d++) {
		if (isProceedableD(stage, x, y, d)) count++;
	}
	
	if (count < 3) pt = count;
	else pt = INTERSECTION;
	
	return pt;
}



static int isProceedable(STAGE* stage, int x, int y) {
	int stageWidth = stage->width;
	int stageHeight = stage->height;
	int idx = x + stage->width * y;
	
	if (idx < 0 || idx >= stageWidth * stageHeight) return 0;
	if (GetType(stage->grids[idx]) == W) { return 0; }
	else { return 1; }
}

static int isProceedableD(STAGE* stage, int prex, int prey, DIRECTION d) {
	int x, y;
	switch(d) {
		case DOWN:
			x = prex;
			y = prey + 1;
			break;
		case LEFT:
			x = prex - 1;
			y = prey;
			break;
		case UP:
			x = prex;
			y = prey - 1;
			break;
		case RIGHT:
			x = prex + 1;
			y = prey;
			break;
		default:
			x = -1;
			y = -1;
			break;
	}
	return isProceedable(stage, x, y);
}

int ProceedMine(DIRECTION d, STAGE* stage, MOVE_OBJECT* objList, int nMoveObj) {
	int i;
	
	for (i = 0; i < nMoveObj; i++) {
		MOVE_OBJECT* obj = &objList[i];
		if (obj->type == M) {
			if (isProceedableD(stage, obj->x, obj->y, d)) {
				Proceed(obj, d);
				if (stage->grids[obj->x + obj->y * stage->width] == '.')
					stage->grids[obj->x + obj->y * stage->width] = ' ';
				return 1;
			}
		}
	}
	return 0;
}


void ProceedEnemy(STAGE* stage, MOVE_OBJECT* objList, int nMoveObj) {
	int i;
	
	for (i = 0; i < nMoveObj; i++) {
		MOVE_OBJECT* obj = &objList[i];
		POSITION_TYPE pt = GetPositionType(stage, obj->x, obj->y);
		DIRECTION d = STAY;
		
		if (obj->type != V && obj->type != H && obj->type != L && obj->type != R && obj->type != J) continue;
		
		if (obj->prex == -1 || obj->prey == -1) {
			int j;
			for (j = 0; j < COUNT; j++) {
				if (isProceedableD(stage, obj->x, obj->y, j)) { d = j; break; }
			}
		}
		
		else {
			int dx = obj->x - obj->prex;
			int dy = obj->y - obj->prey;
			
			switch(pt) {	
			/* 行き止まり */
			case END:
			{
				for (d = 0; d < COUNT; d++) { if (isProceedableD(stage, obj->x, obj->y, d)) break; }
				if (d == STAY) printf("Warning: END\n");
				break;
			}
			/* 通路 */
			case PATH:
			{
				DIRECTION notd;
				if (dx == 0 && dy != 0) notd = (dy > 0) ? UP : DOWN;
				else if (dx != 0 && dy == 0) notd = (dx > 0) ? LEFT : RIGHT;
				for (d = 0; d < COUNT; d++) { if (d != notd && isProceedableD(stage, obj->x, obj->y, d)) break; }
				if (d == STAY) printf("Warning: PATH\n");
				break;
			}
			/* 交差点 */
			case INTERSECTION:
				switch(obj->type) {
				case V:
				case H:
					d = ProceedEnemyVHOnIS(obj, stage, objList, nMoveObj);
					break;
				case L:
				case R:
					d = ProceedEnemyLROnIS(obj, stage);
					break;
				case J:
					d = ProceedEnemyJOnIS(obj, stage);
					break;
				default:
					printf("Warning: Position\n");
					break;
				}
				
				break;
			/* 動作不可 */
			default:
				printf("Warning: 4Walls\n");
				break;
			}
		}
		
		if (d == STAY) printf("Warning: No Direction\n");
		else Proceed(obj, d);
	}
}

void Proceed(MOVE_OBJECT* obj, DIRECTION d) {
	obj->prex = obj->x; obj->prey = obj->y;
	
	switch(d) {
	case DOWN:
		obj->y += 1;
		break;
	case LEFT:
		obj->x += -1;
		break;
	case UP:
		obj->y += -1;
		break;
	case RIGHT:
		obj->x += 1;
		break;
	default:
		printf("Warning: %c\n", d);
		break;
	}
}

DIRECTION ProceedEnemyVHOnIS(MOVE_OBJECT* obj, STAGE* stage, MOVE_OBJECT* objList, int nMoveObj) {
	DIRECTION d = STAY;
	int x = obj->x;
	int y = obj->y;
	int mx = -1;
	int my = -1;
	int j;

	if (obj->type != V && obj->type != H) { printf("Warning VH: %c\n", myGetChar(obj->type)); return d; }

	/* 自機位置の把握 */
	for (j = 0; j < nMoveObj; j++) {
		MOVE_OBJECT* o = &objList[j];
		if (o->type == M) {
			mx = o->x; my = o->y;
			break;
		}
	}
				
	/* 移動方向の決定 */
	if (mx != -1 && my != -1) {
		int dx = x - mx;
		int dy = y - my;
		
		/* 敵V */
		if (obj->type == V) {
			/* 条件1 */
			if (d == STAY && dy != 0) {
				if (dy > 0 && isProceedable(stage, x, y - 1)) d = UP;
				else if (dy < 0 && isProceedable(stage, x, y + 1)) d = DOWN;
			}
			/* 条件2 */
			if (d == STAY && dx != 0) {
				if (dx > 0 && isProceedable(stage, x - 1, y)) d = LEFT;
				else if (dy < 0 && isProceedable(stage, x + 1, y)) d = RIGHT;
			}
		}
		/* 敵H */
		else if (obj->type == H) {
			/* 条件2 */
			if (d == STAY && dx != 0) {
				if (dx > 0 && isProceedable(stage, x - 1, y)) d = LEFT;
				else if (dy < 0 && isProceedable(stage, x + 1, y)) d = RIGHT;
			}
			/* 条件1 */
			if (d == STAY && dy != 0) {
				if (dy > 0 && isProceedable(stage, x, y - 1)) d = UP;
				else if (dy < 0 && isProceedable(stage, x, y + 1)) d = DOWN;
			}
		}
		/* 敵V･H共通 */
		/* 条件3 */
		if (d == STAY) {
			for (j = 0; j < COUNT; j++) {
				if (isProceedableD(stage, x, y, j)) d = j;
			}
		}
	}
	else printf("Warning.\n");
	
	return d;
}


DIRECTION ProceedEnemyLROnIS(MOVE_OBJECT* obj, STAGE* stage) {
	DIRECTION pred = STAY;
	DIRECTION d = STAY;
	int dx, dy;
	int i;

	if (obj->type != L && obj->type != R) { printf("Warning LR: %c\n", myGetChar(obj->type)); return d; }

	dx = obj->x - obj->prex;
	dy = obj->y - obj->prey;

	if (dx == 0 && dy != 0) {
		if (dy > 0) pred = DOWN;
		else if (dy < 0) pred = UP;
	}
	else if (dx != 0 && dy == 0) {
		if (dx > 0) pred = RIGHT;
		else if (dx < 0) pred = LEFT;
	}
	else {
		printf("Warning: Not Proceeded\n");
		return d;
	}
	switch (obj->type) {
	case L:
		for (i = rLEFT; i <= rRIGHT; i++) {
			d = GetAbsoluteDirection(pred, i);
			if (isProceedableD(stage, obj->x, obj->y, d)) return d;
		}
		break;
	case R:
		for (i = rRIGHT; i >= rLEFT; i--) {
			d = GetAbsoluteDirection(pred, i);
			if (isProceedableD(stage, obj->x, obj->y, d)) return d;
		}
		break;
	default:
		printf("Warning: Not Decided\n");
		break;
	}
	
	printf("Warning: Not Decided\n");
	d = STAY;
	return d;
}


DIRECTION ProceedEnemyJOnIS(MOVE_OBJECT* obj, STAGE* stage) {
	static int counter = 0;
	MOVE_OBJECT tmp = *obj;
	
	if (counter % 2 == 0) tmp.type = L;
	else tmp.type = R;
	counter++;
	return ProceedEnemyLROnIS(&tmp, stage);
}

static DIRECTION GetAbsoluteDirection(DIRECTION pred, rDIRECTION rd) {
	int d = (pred + rd - 1) % COUNT;
	return (d >= 0) ? d : d + COUNT;
}
