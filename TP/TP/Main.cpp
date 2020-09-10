#include <iostream>
#include <ctime>
#include <vector>
#include <conio.h>
#include <random>

using namespace std;

int** newOne(int fils, int cols) {
	int** m = new int* [fils];
	for (int j = 0; j < fils; j++) {
		m[j] = new int[cols];
	}
	return m;
}

int** AuxLaplace(int** m, int fila, int col, int n) {
	int auxi = 0, auxj = 0;
	int** aux = newOne(n - 1, n - 1);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			if (!(i == fila || j == col)) {
				aux[auxi][auxj] = m[i][j];
				if (++auxj == n - 1) {
					auxj = 0;
					++auxi;
				}
			}

		}
	return aux;
}

int** m_copy(int** matrix, int n, int cols) {
	int** aux = newOne(n, cols);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < cols; j++)
			aux[i][j] = matrix[i][j];
	return aux;
}

void show(int** matrix, int n, int cols) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < cols; j++)
			cout << matrix[i][j] << " ";
		cout << endl;
	}
}

void generate(int** matrix, int filas, int cols) {
	for (int i = 0; i < filas; i++)
		for (int j = 0; j < cols; j++)
			matrix[i][j] = rand() % 10;
}

int DetM2x2(int** m) {
	return ((m[0][0] * m[1][1]) - (m[0][1] * m[1][0]));
}

double DetM3x3(int** matrix) {
	int col, aux = 1; double ansr = 0;
	for (int i = 0; i < 3; i++)
	{
		aux = 1; col = i;
		for (int fila = 0; fila < 3; fila++)
		{
			aux *= matrix[fila][col % 3];
			++col;
		}
		ansr += aux;
	}
	for (int i = 0; i < 3; i++)
	{
		aux = 1; col = 3 - 1 + i;
		for (int fila = 0; fila < 3; fila++)
		{
			aux *= matrix[fila][col % 3];
			--col;
		}
		ansr -= aux;
	}
	return ansr;
}

double DetLaplace(int** m, int n) {
	if (n < 3) return DetM2x2(m);
	double ansr = 0; //Siempre tapar la fila 0
	for (int j = 0; j < n; j++)
		ansr += m[0][j] * DetLaplace(AuxLaplace(m, 0, j, n), n - 1) * ((j % 2 == 0) ? 1 : -1);
	return ansr;
}

double Cramer(int** m, int** b, int n, int col) {
	int** aux = m_copy(m, n, n);// cout << endl << "Det " << DetLaplace(m, n);
	for (int i = 0; i < n; i++)
		aux[i][col] = b[i][0];
	return DetLaplace(aux, n) / DetLaplace(m, n);
}

void opArr(float* arr, int n,float m,string tipo) {

	for (short i = 0; i < n; i++) {
		if("+" == tipo)
			arr[i] += m;
		else if ("-" == tipo)
			arr[i] -= m;
		else if ("*" == tipo)
			arr[i] *= m;
		else if ("/" == tipo)
			arr[i] /= m;
	}
}

int numFVacias(float** mat,int n) {
	int rango = n;
	int cont = 0;
	for (char i = 0; i < n; i++) {
		for (char c = 0; c < n; c++) {
			if (mat[i][c] == 0) {
				cont++;
			}

		}
		if (cont == n)
			rango--;
		cont = 0;
	}
	return rango;
}

int rango(int n, int** Matrix) {
	float** A = new float*[n];
	float* aux = new float[n];
	
	for (short f = 0; f < n; f++) {
		A[f] = new float[n];
		for (short c = 0; c < n; c++) {
			A[f][c] = Matrix[f][c];
		}
	}


		for (short f = 0; f < n-2; f++) {
			for (short bajada = f; bajada < n-1; bajada++) {
				for (short i = f; i < n; i++) {
					/*if (A[f][f] != 1)*/
						aux[i] = (A[f][i] / A[f][f]);
					/*else
						aux[i] = A[f][i];*/

				}
				//probablemte tenga que hacer el primer numero 1 aqui
				opArr(aux, n, A[bajada + 1][f], "*");
				for (short i = f; i < n; i++) {
					A[bajada + 1][i] -= aux[i];
				}
				
			}
			
			//esto es para mostrar como queda la matriz
			/*
			for (short f = 0; f < n; f++) {
				for (short c = 0; c < n; c++) {
					Matrix[f][c] = A[f][c];
				}
			}
			show(Matrix, 4, 4);
			_getch();*/
		}

	
	

	return numFVacias(A,n);
}


int main() {
	int n;
	int** A, ** b;
	
	srand(time(NULL));
	do {
		cout << "Ingrese un numero entre 4 y 10: " << endl;
		cin >> n;
	} while (n < 4 || n > 10);

	cout << "La matriz generada aleatoriamente A: " << endl;
	A = newOne(n, n); generate(A, n, n); show(A, n, n);
	cout << endl;

	cout << "El vector generado aleatoriamente B: " << endl;
	b = newOne(n, 1); generate(b, n, 1); show(b, n, 1);
	cout << endl;

	cout << "El rango de la matriz A es: " << endl;
	
	cout << rango(n, A) << endl;


	cout << DetLaplace(A, n) << endl;

	if (DetLaplace(A, n) != 0) {
		for (int i = 0; i < n; i++)
			cout << "x" << i + 1 << "\t" << Cramer(A, b, n, i) << endl;
	}
	else { _exit(1); }

	_getch();
	return 0;
}