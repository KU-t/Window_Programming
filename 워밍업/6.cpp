#include <iostream>

using namespace std;

void array_space(int, char**);
void array_star(int, char**);

void show(int,char**);

void show_1(int,char**);
void show_2(int,char**);
void show_3(int,char**);
void show_4(int,char**);
void show_5(int,char**);
void show_6(int,char**);

int main(void) {

	int number = 0, n = 0;
	char **array;

	cout << "Input command: ";
	cin >> number >> n;

	array = new char*[n/2];
	for (int i = 0; i < n / 2; i++) {
		array[i] = new char[n];
	}

	switch (number) {
	case 1:
		show_1(n, array);
		break;
	case 2:
		show_2(n, array);
		break;
	case 3:
		show_3(n, array);
		break;
	case 4:
		show_4(n, array);
		break;
	case 5:
		show_5(n, array);
		break;
	}
	for(int i =0; i < n/2 ; i++){
		delete [] array[i]; 
	}
	delete [] array;
}

void array_space(int n,char **array)
{
	for (int i = 0; i < n / 2; i++) {
		for (int j = 0; j < n; j++) {
			array[i][j] = ' ';
		}
	}
}

void array_star(int n, char **array) {
	for (int i = 0; i < n / 2; i++) {
		for (int j = 0; j < n; j++) {
			array[i][j] = '*';
		}
	}
}

void show(int n, char **array) {
	for (int i = 0; i < n / 2; i++) {
		for (int j = 0; j < n; j++) {
			cout << array[i][j];
		}
		cout << endl;
	}
}

void show_1(int n, char ** array) {
	array_space(n,array);
	
	array[(n/4)][(n/2)+1] = '*';
	
	if((n/2)%2 == 0){
		for(int i = 1; i < n / 4 ; i++){
			array[(n/4)+i][(n/2)-i] = '*';
			array[(n/4)+i][(n/2)+1+i] = '*';
		}
	}

	else {
		for(int i = 1; i <= n / 4 ; i++){
			array[(n/4)+i][(n/2)-i] = '*';
			array[(n/4)+i][(n/2)+1+i] = '*';
		}
	}
	
	for(int i = 1; i <= n / 4 ; i++){
			array[(n/4)-i][(n/2)-i] = '*';
			array[(n/4)-i][(n/2)+1+i] = '*';

	
	}
	
	show(n, array);
}


/*
0010000100
0001001000
0000010000
0001001000
0010000100
*/


void show_2(int n, char ** array) {
	array_space(n, array);
	for (int i = 0; i < n / 2; i++) {
		for (int j = i; j < n - i; j++) {
			array[i][j] = '*';
		}
	}
	show(n, array);
}

void show_3(int n, char **array) {
	array_space(n, array);
	for (int i = 0; i < n / 4; i++) {
		for (int j = (n / 2)-(2 * i)-1 ; j < (n / 2) + (2 * i) +1 ; j++) 
			array[i][j] = '*';
	}
	
	for (int i = n/4; i < n / 2; i++) {
		for (int j = (n / 2) - (2 * ((n/2)-i))+1; j < (n / 2) + (2 * ((n/2)-i)) -1; j++)
			array[i][j] = '*';
	}
	array[0][(n/2)-1] = ' ';
	array[(n/2)-1][(n/2)-1] = ' ';
	show(n, array);
}

void show_4(int n, char **array) {
	array_star(n, array);
	for (int i = 0; i < n / 4; i++) {
		for (int j = 1+(2*i); j < (n - 1) - (2 * i); j++ ) 
			array[i][j] = ' ';
			
	}

	for (int i = (n / 2) -1 ; i >= (n / 4); i--) {
		for (int j = (2*((n/2)-i)) -1; j < (n - 1) - (2 * ((n/2)-i))+2; j++ )
			array[i][j] =  ' ';
	}
	show(n, array);
}

void show_5(int n, char **array) {
	array_space(n, array);
	for (int i = 0; i < n / 4; i++) {
		for (int j = (2*i); j < (n - 1) - (2 * i)+1; j++ ) {
			array[i][j] = '*';
		}
	}

	for (int i = (n / 2) -1 ; i >= (n / 4); i--) {
		for (int j = (2*((n/2)-i)) -2 ; j < (n - 1) - (2 * ((n/2)-i))+3; j++ )
			array[i][j] =  '*';
	}

	show(n, array);
}

void show_6(int, char **) {
	
}
