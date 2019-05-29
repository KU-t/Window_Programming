#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

void ShowArray(int*);

void ArrayUp(int*, int);
void ArrayDown(int*, int);

void SortUp(int*, int, int);
void SortDown(int*, int, int);

void ArrayRandom(int*);

void ArrayThree(int*);
void ArraySeven(int*);

void ArrayOr(int*);
void ArrayAnd(int*);

void MaxNumber(int*);
void MinNumber(int*);

void MakeArray(int *p, int* array);

void ShowArray(int* data) {
	for (int i = 0; i < 30; i++)
		cout << data[i] << " ";
	cout << endl;
}

void ArrayUp(int* data, int n) {
	SortUp(data, 0, n - 1);
}

void ArrayDown(int* data, int n) {
	SortDown(data, 0, n - 1);
}

void SortUp(int* data, int start, int end) {

	int key = start;
	int i = start + 1, j = end, tmp;

	if (start>end) {
		return;
	}



	while (i <= j) {
		while (data[i]<data[key]) { //i=1 ,6
			i++;
		}

		while (data[j] > data[key]) { // j=9 ,1
			j--;
		}

		if (i>j) {
			tmp = data[j];
			data[j] = data[key];
			data[key] = tmp;
		}

		else {
			tmp = data[i];
			data[i] = data[j];
			data[j] = tmp;
		}
	}
	SortUp(data, start, j - 1);
	SortUp(data, j + 1, end);
}

void SortDown(int* data, int start, int end) {

	int key = start;
	int i = start + 1, j = end, tmp;

	if (start > end) {
		return;
	}

	while (i <= j) {
		while (data[i] > data[key]) { //i=1 ,6
			i++;
		}

		while (data[j] < data[key]) { // j=9 ,1
			j--;
		}

		if (i>j) {
			tmp = data[j];
			data[j] = data[key];
			data[key] = tmp;
		}

		else {
			tmp = data[i];
			data[i] = data[j];
			data[j] = tmp;
		}
	}
	SortDown(data, start, j - 1);
	SortDown(data, j + 1, end);
}

void ArrayRandom(int* data) {
	int a = 0;
	int b = 0;
	int empty = 0;

	for (int i = 0; i < 10000; i++) {
		a = rand() % 30;
		b = rand() % 30;
		empty = *(data + a);
		*(data + a) = *(data + b);
		*(data + b) = empty;
	}
}

void ArrayThree(int* array) {
	for (int i = 0; i < 30; i++) {
		if (array[i] % 3 == 0) {
			cout << array[i] << " ";
		}
	}
	cout << endl;
}


void ArraySeven(int* array) {
	for (int i = 0; i < 30; i++) {
		if (array[i] % 7 == 0) {
			cout << array[i] << " ";
		}
	}
	cout << endl;
}

void ArrayOr(int* array) {
	ArrayUp(array, 30);
	for (int i = 0; i < 30; i++) {
		if ((array[i] % 3 == 0) & (array[i] % 7 == 0))
			cout << array[i] << " ";
	}
	cout << endl;
}

void ArrayAnd(int* array) {
	ArrayUp(array, 30);
	for (int i = 0; i < 30; i++) {
		if ((array[i] % 3 == 0) | (array[i] % 7 == 0))
			cout << array[i] << " ";
	}
	cout << endl;
}

void MaxNumber(int* data) {
	int empty = 0;

	for (int i = 0; i < 29; i++) {
		if (data[i] > data[i + 1]) {
			empty = data[i + 1];
			data[i + 1] = data[i];
			data[i] = empty;
		}
	}
	cout << data[29];
	cout << endl;
}

void MinNumber(int* data) {
	int empty = 0;

	for (int i = 0; i < 29; i++) {
		if (data[i] < data[i + 1]) {
			empty = data[i + 1];
			data[i + 1] = data[i];
			data[i] = empty;
		}
	}
	cout << data[29];
	cout << endl;
}

void MakeArray() {

	int *p;
	int array[30];
	int input = 0;
	int empty = 0;
	int a = 0;
	int b = 0;


	srand((unsigned)time(NULL));

	//배열 초기화
	for (int i = 0; i < 30; i++)
		array[i] = 0;

	//input값 입력받기
	cout << "Input number : ";
	cin >> input;

	//배열크기 동적 할당
	p = (int *)malloc(sizeof(int)*input);

	//배열[input] 초기화
	for (int i = 0; i < input; i++) {
		*(p + i) = i;
	}

	//배열 섞기
	for (int i = 0; i < 10000; i++) {
		a = rand() % input;
		b = rand() % input;
		empty = *(p + a);
		*(p + a) = *(p + b);
		*(p + b) = empty;
	}

	//배열 복사
	for (int i = 0; i < 30; i++)
		array[i] = *(p + i);


	for (int i = 0; i < 30; i++) {
		cout << array[i] << " ";
	}
	cout << endl;

	free(p);
}


int main() {

	int *p;
	int array[30];
	int input = 0;
	int empty = 0;
	int a = 0;
	int b = 0;
	char Command_Choose;

	srand((unsigned)time(NULL));

	//배열 초기화
	for (int i = 0; i < 30; i++)
		array[i] = 0;


	//input값 입력받기
	cout << "Input number : ";
	cin >> input;

	//배열크기 동적 할당
	p = (int *)malloc(sizeof(int)*input);

	//배열[input] 초기화
	for (int i = 0; i < input; i++) {
		*(p + i) = i;
	}

	//배열 섞기
	for (int i = 0; i < 10000; i++) {
		a = rand() % input;
		b = rand() % input;
		empty = *(p + a);
		*(p + a) = *(p + b);
		*(p + b) = empty;
	}

	//배열 복사
	for (int i = 0; i < 30; i++)
		array[i] = *(p + i);

	free(p);

	// --Generated numbers--
	cout << "Generated numbers : ";
	for (int i = 0; i < 30; i++)
		cout << array[i] << " ";


	while (1) {
		cout << endl;
		cout << "Command: ";
		cin >> Command_Choose;

		switch (Command_Choose)
		{
		case 'A':
			ArrayUp(array, 30);
			ShowArray(array);
			break;

		case 'D':
			ArrayDown(array, 30);
			ShowArray(array);
			break;

		case 'R':
			ArrayRandom(array);
			ShowArray(array);
			break;

		case 'Q':
			exit(1);

		case 't':
			ArrayThree(array);
			break;

		case 's':
			ArraySeven(array);
			break;

		case '+':
			ArrayAnd(array);
			break;

		case '-':
			ArrayOr(array);
			break;

		case 'M':
			MaxNumber(array);
			break;

		case 'I':
			MinNumber(array);
			break;

		case 'S':
			MakeArray();
			exit(1);

		default:
			break;
		}
	}
	return 1;
}