#include <iostream>
#include <ctime>
#include <vector>
#include <conio.h>
#include <random>

using namespace std;

double** newOne(int fils, int cols) {
	double** m = new double* [fils];
	for (int j = 0; j < fils; j++)
		m[j] = new double[cols];
	return m;
}

double** Cofactor(double** m, int fila, int col, int n) {
	int auxi = 0, auxj = 0;
	double** aux = newOne(n - 1, n - 1);
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

double** m_copy(double** matrix, int n, int cols) {
	double** aux = newOne(n, cols);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < cols; j++)
			aux[i][j] = matrix[i][j];
	return aux;
}

void show(double** matrix, int n, int cols) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < cols; j++)
			cout << matrix[i][j] << "\t";
		cout << endl;
	}
}

void generate(double** matrix, int filas, int cols) {
	for (int i = 0; i < filas; i++)
		for (int j = 0; j < cols; j++)
			matrix[i][j] = rand() % 10;
}

double DetM2x2(double** m) {
	return ((m[0][0] * m[1][1]) - (m[0][1] * m[1][0]));
}

double DetLaplace(double** m, int n, int f) {
	if (n < 3) return DetM2x2(m);
	double ansr = 0; 
	for (int j = 0; j < n; j++)
		ansr += m[f][j] * DetLaplace(Cofactor(m, f, j, n), n - 1, f) * (((f + j) % 2 == 0) ? 1 : -1);
		//Verificamos si i + j es par (positivo) o impar (negativo)
	return ansr;
}

double Cramer(double** m, double** b, int n, int col) {
	double** aux = m_copy(m, n, n);
	for (int i = 0; i < n; i++)
		aux[i][col] = b[i][0];
	return DetLaplace(aux, n, 0);
	//Cramer realmente devuelve DetLaplace(aux, n) / DetLaplace(m, n);
	//Para fines de presentación del resultado, esta división se realiza en otro bloque de código
}

void opArr(double* arr, int n, int value, string tipo) {

	for (short i = 0; i < n; i++) {
		if("+" == tipo)
			arr[i] += value;
		else if ("-" == tipo)
			arr[i] -= value;
		else if ("*" == tipo)
			arr[i] *= value;
		else if ("/" == tipo)
			arr[i] /= value;
	}
}

int numFVacias(double** Matrix, int n) {
	int rango = n;
	int cont = 0;
	for (char i = 0; i < n; i++) {
		for (char c = 0; c < n; c++)
			if (Matrix[i][c] == 0)
				cont++;
		if (cont == n)
			rango--;
		cont = 0;
	}
	return rango;
}

int Rango(int n, double** Matrix) {
	double* aux = new double[n];
	double **A = m_copy(Matrix, n, n);

	for (int f = 0; f < n - 2; f++) {
		//convertir el primer elemento de la fila en 1
		for (int bajada = f; bajada < n - 1; bajada++) {
			for (int i = f; i < n; i++) {
				aux[i] = (A[f][i] / A[f][f]);		
			}
			//multiplicalo por el número que quieres convertir en 0
			opArr(aux, n, A[bajada + 1][f], "*");
			for (int i = f; i < n; i++) {
				A[bajada + 1][i] -= aux[i];
			}
		}
		//esto es para mostrar como queda la matriz
		//show(A, n, n); cout << endl; _getch();
	}
	return numFVacias(A, n);
}

double** Traspuesta(double** m, int n) {
	double** aux = newOne(n, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			aux[i][j] = m[j][i];
	return aux;
}

double** MCofactor(double** m, int n) {
	double** aux = newOne(n, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			aux[i][j] = DetLaplace(Cofactor(m, i, j, n), n - 1, 0);
	return aux;
}

double** inversa(double** mcof, int n, double det) {
	double** aux = Traspuesta(mcof, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			aux[i][j] /= (det * (((i + j) % 2 == 0) ? 1 : -1));
	return aux;
}

int main() {
	int n;
	double** A, ** b;
	
	srand(time(NULL));
	do {
		cout << "Ingrese un numero entre 4 y 10:\t";
		cin >> n;
	} while (n < 4 || n > 10);

	cout << "La matriz generada aleatoriamente A:\n" << endl;
	A = newOne(n, n); generate(A, n, n); show(A, n, n);
	cout << endl;

	cout << "El vector generado aleatoriamente B:\n" << endl;
	b = newOne(n, 1); generate(b, n, 1); show(b, n, 1);
	cout << endl;

	int ran = Rango(n, A);
	cout << "El rango de la matriz A es: \t" << ran << endl;
	auto detA = DetLaplace(A, n, 0);
	if (ran == n && detA != 0) {
		cout << "La inversa de la matriz A es:" << endl;
		show(inversa(MCofactor(A, n), n, detA), n, n);
	}

	cout << "DetA: " << detA << endl;
	if (detA != 0) {
		auto detBx = 0;
		for (int i = 0; i < n; i++) {
			detBx = Cramer(A, b, n, i);
			cout << "x" << i + 1 << "\t" << detBx <<
				"/" << detA << " = " << (detBx / detA) << endl;
		}
	}
	else { _exit(1); }

	_getch();
	return 0;
}