#include <iostream>
#include <limits>
#include <time.h>
#include <ctime>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

int N = 10000;
int* queens;
const int INF = numeric_limits<int>::max();

int* colCount;

int* upperDiagCount;
int* lowerDiagCount;

int conflicts(int row) {
	int conflicts = 0;

	for (int i = 0; i < 2 * N - 1; i++) {
		if (i < N) {
			colCount[i] = 0;
		}
		upperDiagCount[i] = lowerDiagCount[i] = 0;
	}

	for (int i = 0; i < N; i++) {
		if (i != row) {
			colCount[queens[i]] += 1;
			upperDiagCount[queens[i] + i] += 1;
			lowerDiagCount[(N - queens[i]) + i - 1] += 1;
		}
	}

	for (int i = 0; i < 2 * N - 1; i++) {
		if (i < N) {
			conflicts += ((colCount[i] - 1) * colCount[i]) / 2;
		}
		conflicts += ((upperDiagCount[i] - 1) * upperDiagCount[i]) / 2;
		conflicts += ((lowerDiagCount[i] - 1) * lowerDiagCount[i]) / 2;
	}
	return conflicts;
}

int conflicts() {
	return conflicts(-1);
}


void Init() {
	queens = new int[N];

	colCount = new int[N];
	upperDiagCount = new int[(2 * N) - 1];
	lowerDiagCount = new int[(2 * N) - 1];

	for (int i = 0; i < 2 * N - 1; i++) {
		if (i < N) {
			colCount[i] = 0;
		}
		upperDiagCount[i] = lowerDiagCount[i] = 0;
	}

	vector<int> minConflictCols;
	int minConflicts = INF;
	int tempConflicts;

	// избираме първата царица случайно
	queens[0] = rand() % N;

	colCount[queens[0]] += 1;
	upperDiagCount[queens[0] + 0] += 1;
	lowerDiagCount[(N - queens[0]) + 0 - 1] += 1;

	// i = индекса на реда
	for (int i = 1; i < N; i++) {
		minConflictCols.clear();
		minConflicts = INF;
		// j= индекса на колоната
		for (int j = 0; j < N; j++) {
			tempConflicts = ((colCount[j] + 1) * colCount[j]) / 2;
			tempConflicts += ((upperDiagCount[j + i] + 1) * upperDiagCount[j + i]) / 2;
			tempConflicts += ((lowerDiagCount[(N - j) + i - 1] + 1) * lowerDiagCount[(N - j) + i - 1]) / 2;

			if (tempConflicts < minConflicts) {
				minConflicts = tempConflicts;
				minConflictCols.clear();
				minConflictCols.push_back(j);
			}
			else if (tempConflicts == minConflicts) {
				minConflictCols.push_back(j);
			}
		}
		queens[i] = minConflictCols[rand() % minConflictCols.size()];

		colCount[queens[i]] += 1;
		upperDiagCount[queens[i] + i] += 1;
		lowerDiagCount[(N - queens[i]) + i - 1] += 1;
	}
}

void Print() {
	cout << "==========================================" << endl;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < queens[i]; j++) {
			cout << "- ";
		}
		cout << "* ";
		for (int j = 0; j < N - queens[i] - 1; j++) {
			cout << "- ";
		}
		cout << endl;
		
	}
	cout << "==========================================" << endl;
}

// изчисляваме реда с най-много конфликти
int HighestConflicts() {
	int rowConflicts = 0;
	int tempConflicts;
	vector<int> maxConflictRows;

	for (int i = 0; i < N; i++) {
		tempConflicts = ((colCount[queens[i]] - 1) * colCount[queens[i]]) / 2;
		tempConflicts += ((upperDiagCount[queens[i] + i] - 1) * upperDiagCount[queens[i] + i]) / 2;
		tempConflicts += ((lowerDiagCount[(N - queens[i]) + i - 1] - 1) * lowerDiagCount[(N - queens[i]) + i - 1]) / 2;

		if (tempConflicts > rowConflicts) {
			rowConflicts = tempConflicts;
			maxConflictRows.clear();
			maxConflictRows.push_back(i);
		}
		else if (tempConflicts == rowConflicts) {
			maxConflictRows.push_back(i);
		}
	}
	return maxConflictRows[rand() % maxConflictRows.size()];
}

void min_conflicts() {
	int highestConflictRow = HighestConflicts();

	int minConflicts = INF;
	int tempConflicts;
	//колоните с най-малко конфликти
	vector<int> minConflictCols;
	conflicts(highestConflictRow);

	// i= индекса на колоната
	for (int i = 0; i < N; i++) {
		tempConflicts = ((colCount[i] + 1) * colCount[i]) / 2;
		tempConflicts += ((upperDiagCount[i + highestConflictRow] + 1) * upperDiagCount[i + highestConflictRow]) / 2;
		tempConflicts += ((lowerDiagCount[(N - i) + highestConflictRow - 1] + 1) * lowerDiagCount[(N - i) + highestConflictRow - 1]) / 2;

		if (tempConflicts < minConflicts) {
			minConflicts = tempConflicts;
			minConflictCols.clear();
			minConflictCols.push_back(i);
		}
		else if (tempConflicts == minConflicts) {
			minConflictCols.push_back(i);
		}
	}
	queens[highestConflictRow] = minConflictCols[rand() % minConflictCols.size()];
}

int main() {


	cout << "Number of queens: " << N << endl;
	auto timeStart = std::chrono::high_resolution_clock::now();
	srand(time(0));
	Init();

	if (N <= 20) {
		Print();
	}

	int previousConflicts = conflicts();
	int newConflicts;


	int count = 0;
	int steps = 0;

	while (previousConflicts != 0) {
		min_conflicts();
		steps++;
		newConflicts = conflicts();
		if (previousConflicts == newConflicts) {
			count++;
			if (count > 1) {
				queens[rand() % N] = rand() % N;
				count = 0;
			}
		}
		previousConflicts = newConflicts;
	}

	if (N <= 20) {
		Print();
	}
	auto timeEnd = chrono::high_resolution_clock::now();
	printf("Total execution time was %f millis.\n",
		chrono::duration<double, milli>(timeEnd - timeStart).count());
	return 0;
}
