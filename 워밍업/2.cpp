#include <iostream>
#include <ctime>

using namespace std;

int m = 0, n = 0;


void show(int **array);
void CH_1(int **array);
void CH_2(int **array);
void CH_3(int **array);
void CH_4(int **array);
void CH_5(int **array);

void show(int **array) {
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			cout << array[i][j] << " ";
		}
		cout << endl;
	}
}

void CH_1(int **array) {
	for (int i = 0; i < m; i += 2) {
		for (int j = 0; j < n; j++) {
			array[i][j] = 2 * array[i][j];
		}
	}
	show(array);
}

void CH_2(int **array) {
	for (int i = 1; i < m; i += 2) {
		for (int j = 0; j < n; j++) {
			array[i][j] = 3 * array[i][j];
		}
	}
	show(array);
}

void CH_3(int **array) {
	int M = m, N = n;

	if (M > N)
		M = N;

	for (int i = 0; i < M; i++) {
		array[i][i] += 4;
	}
	show(array);
}

void CH_4(int **array) {
	for (int i = 1; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (array[i][j] % 4 == 0) {
				array[i][j] = (array[i][j] - (2 * array[i][j]));
			}
		}
	}
	show(array);
}

void CH_5(int **array) {
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			array[i][j] = rand() % 10;
			cout << array[i][j] << " ";
		}
		cout << endl;
	}
}





int main(void) {

	int choose, **array; // m = 행, n = 열

	srand((unsigned)time(NULL));


reset:
	while (1) {

		m = 0, n = 0;

		cout << "행, 열값 입력 : ";
		cin >> m >> n;

		if ((m >= 10) || (n >= 10)) {
			cout << "error : 10보다 작은 수를 입력하시오" << endl;
			goto reset;
		}

		array = new int *[m];

		for (int i = 0; i < m; i++) {
			array[i] = new int[n];
		}

		cout << "--행렬값--" << endl;

		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				array[i][j] = rand() % 10;
				cout << array[i][j] << " ";
			}
			cout << endl;
		}
	GO:
		cout << "명령어 : ";
		cin >> choose;

		switch (choose) {

		case 1:
			CH_1(array);
			break;

		case 2:
			CH_2(array);
			break;

		case 3:
			CH_3(array);
			break;

		case 4:
			CH_4(array);
			break;

		case 5:
			CH_5(array);
			break;

		case 6:
			exit(1);
		}
		cout << endl;
	}
}