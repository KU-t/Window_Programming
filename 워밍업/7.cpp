#include <stdio.h>
#include <windows.h>
#include <ctime>

void white_move(char command, int *white, int *black);
void black_move(char command, int *black, int *white);

int main(void) {
	
	srand((unsigned int)time(NULL));
	
	int white[2];
	int black[2];
	char command;
	int order = 0;
	
	for (int i = 0; i < 2; i++) {
		white[i] = rand() % 9;
		black[i] = rand() % 9;
	}
	
	do {
		for (int i = 0; i < 41; i++) { printf("- "); }
		printf("\n");

		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {

				if (white[0] == i && white[1] == j) {
					printf("|   ¡Û\t");
				}
				else if (black[0] == i && black[1] == j) {
					printf("|   ¡Ü\t");
				}
				else
					printf("| \t");
			}
			printf("| \t");
			printf("\n\n");
		}

		for (int i = 0; i < 41; i++) 
			printf("- "); 

		printf("\n");

		printf("Input command : ");
		scanf_s("%c", &command);

		if (command == 'w' || command == 's' || command == 'a' || command == 'd') {
			if (order == 0) {
				//printf("comm mIN:%c\n", command);
				white_move(command, white, black);
				order = 1;
			}
		}
		else if (command == 'i' || command == 'j' || command == 'k' || command == 'l') {
			if (order == 1) {
				black_move(command, black, white);
				order = 0;
			}
		}
		system("cls");
	} while (command != 'q');

}

void white_move(char command, int *white, int *black) {
	
	printf("comm:%c\n", command);
	
	switch (command){
		case 'w': {
			white[0]--;
			if (white[0] == black[0] && white[1] == black[1])
				white[0]++;
			printf("test");
			break;
		}

		case 's': {
			white[0]++;
			if (white[0] == black[0] && white[1] == black[1])
				white[0]--;
			break;
		}

		case 'a': {
			white[1]--;
			if (white[1] == black[1] && white[0] == black[0])
				white[1]++;
			break;
		}

		case 'd': {
			white[1]++;
			if (white[1] == black[1] && white[0] == black[0])
				white[1]--;
			break;
		}
	
	}
	
	if (white[0] > 9 || white[0] < 0 || white[1] > 9 || white[1] < 0) {
		printf("\a");
		if (white[0] > 9)
			white[0]--;
		else if (white[0] < 0)
			white[0]++;
		else if (white[1] > 9)
			white[1]--;
		else if (white[1] < 0)
			white[1]++;
	}
}

void black_move(char command, int *black, int *white) {
	switch (command) {
		case 'i': {
			black[0]--;
			if (black[0] == white[0] && white[1] == black[1])
				black[0]++;
			break;
		}

		case 'k': {
			black[0]++;
			if (black[0] == white[0] && white[1] == black[1])
				black[0]--;
			break;
		}

		case 'j': {
			black[1]--;
			if (black[1] == white[1] && white[0] == black[0])
				black[1]++;
			break;
		}

		case 'l': {
			black[1]++;
			if (black[1] == white[1] && white[0] == black[0])
				black[1]--;
			break;
		}
	}

	if (black[0] > 9 || black[0] < 0 || black[1] > 9 || black[1] < 0) {
		printf("\a");
		if (black[0] > 9)
			black[0]--;
		else if (black[0] < 0)
			black[0]++;
		else if (black[1] > 9)
			black[1]--;
		else if (black[1] < 0)
			black[1]++;
	}
}