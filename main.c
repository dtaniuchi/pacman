/*
	DevQuiz Pacman
	 : http://koriym.github.io/gdd2010-pacman/
	 : https://github.com/koriym/gdd2010-pacman
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "header1.h"
#include "header2.h"

#define STAGE_SIZE 1000
#define FILENAME_SIZE 20

int main(int argc, char *argv[])
{
	STAGE* stage;
	MOVE_OBJECT* objList;
	int nMoveObj;

	char* fname = NULL;
	DIRECTION d;
	char input;

	stage->width = 0; stage->height = 0;
	stage->grids = (char*)malloc( sizeof( char ) * STAGE_SIZE );

	/* Read stage */
	if (argc == 2) {
		nMoveObj = ReadStage(argv[1], stage); 
	}
	else
	{
		fname = (char*)malloc(sizeof(char) * FILENAME_SIZE);
		printf("Input stage filename.\n");
		if (fname != NULL) {
			scanf("%s", fname);
			nMoveObj = ReadStage(fname, stage);
		}
		else{ printf("Unable to get memory.\n"); }
		free(fname);

	}

	/* Display stage */ 
	Show(stage);
	
	objList = (MOVE_OBJECT*)malloc(sizeof(MOVE_OBJECT) * nMoveObj);
	if (nMoveObj != MakeMoveObjects(stage, objList))
		printf("Warning\n");

//	Search(stage, objList, nMoveObj);

	while (1) {
		d = COUNT;
		input = ' ';
		
		printf("w:up, a:left, z: down, s:right, q:stay\n");
		while (scanf("%c", &input)) { if(input != '\n') break; }
		
		switch(input) {
			case 'w':
				d = UP;
				break;
			case 'a':
				d = LEFT;
				break;
			case 'z':
				d = DOWN;
				break;
			case 's':
				d = RIGHT;
				break;
			case 'q':
				d = STAY;
				break;
			default:
				printf("Warning?\n");
				break;
		}

		if(d != COUNT) {
			ProceedEnemy(stage, objList, nMoveObj);
			ProceedMine(d, stage, objList, nMoveObj);
			ShowAll(stage, objList, nMoveObj);
		}
		
		if (CheckClash(objList, nMoveObj))
		 { printf("Clashed...!\n"); break; }
		if (CheckClear(stage))
		 { printf("Cleared!\n"); break; }
	}

	free(stage->grids);
	free(objList);

	return 0;
}

