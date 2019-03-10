#include "pch.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <math.h>

using namespace std;

double getFunctionResult(double a, double b, double c, double x)
{
	double res = ((a * pow(x, 2)) + (b * x) + c);
	return res;
}

double getDistanceBetweenPoints(double beginPoint, double endPoint, int numberOfSteps)
{
	if ((endPoint > beginPoint) && (numberOfSteps > 0))
	{
		return (endPoint - beginPoint) / numberOfSteps;
	}
	return false;
}

int main()
{
	// Deklaracja danych poczatkowych
	double beginPoint = -2;
	double endPoint = 2;
	const int numberOfSteps = 100000;

	// Deklaracja zmiennych a, b i c dla funkcji
	double a = 2;
	double b = 3;
	double c = 4;

	double sumResult = 0;
	double integralRangeArr[numberOfSteps];

	for (int i = 0; i < numberOfSteps; i++)
	{
		double result = 0;
		double iterator = i + 1;

		result = beginPoint + ((iterator / numberOfSteps) * (endPoint - beginPoint));
		if (i == 1)
		{
			result = (int)beginPoint;
		}
		integralRangeArr[i] = result;
	}

	double distanceBetweenPoints = getDistanceBetweenPoints(beginPoint, endPoint, numberOfSteps);

	// --- METODA PROSTAKATOW ---
	double result = 0;
	double measurementTime = 0;

	clock_t beginTime = clock();

	for (int i = 0; i < numberOfSteps; i++)
	{
		double functionResult = getFunctionResult(a, b, c, integralRangeArr[i]);
		sumResult += functionResult;
	}

	result = distanceBetweenPoints * sumResult;

	clock_t endTime = clock();
	measurementTime = double(endTime - beginTime) / CLOCKS_PER_SEC;

	cout << "--- METODA PROSTAKATOW ---" << endl << endl;

	cout << "Całka z funkcji:" << endl <<
		"f(x) = " << a << "x^2 + " << b << "x + " << c << " wynosi: " << result << endl;

	cout << "Czas dla jednego wątku: " << measurementTime << endl;

	// --- METODA TRAPEZOW ---
	// Wyzerowanie wynikow
	sumResult = 0;
	result = 0;
	// Wyzerowanie czasu
	measurementTime = 0;

	beginTime = clock();

	for (int i = 0; i < (numberOfSteps - 1); i++)
	{
		double functionResult = 0;

		if (i == 0)
		{
			double firstResult = getFunctionResult(a, b, c, integralRangeArr[0]);
			double lastResult = getFunctionResult(a, b, c, integralRangeArr[numberOfSteps - 1]);
			sumResult += ((firstResult + lastResult) / 2);
			continue;
		}

		functionResult = getFunctionResult(a, b, c, integralRangeArr[i]);
		sumResult += functionResult;
	}

	result = distanceBetweenPoints * sumResult;

	endTime = clock();
	measurementTime = double(endTime - beginTime) / CLOCKS_PER_SEC;

	cout << endl << "--- METODA TRAPEZÓW ---" << endl << endl;
	cout << "Całka z funkcji:" << endl <<
		"f(x) = " << a << "x^2 + " << b << "x + " << c << " wynosi: " << result << endl;
	cout << "Czas dla jednego wątku: " << measurementTime << endl;

	// --- METODA SIMPSONA ---
	// Wyzerowanie wynikow
	sumResult = 0;
	result = 0;
	// Wyzerowanie czasu
	measurementTime = 0;

	beginTime = clock();

	double st = 0;
	double s = 0;

	for (int i = 1; i <= numberOfSteps; i++)
	{
		double x = 0;
		double integral = i;

		x = beginPoint + (integral * distanceBetweenPoints);

		st += getFunctionResult(a, b, c, (x - distanceBetweenPoints / 2));

		if (i < numberOfSteps)
		{
			s += getFunctionResult(a, b, c, x);
		}
	}

	result = (distanceBetweenPoints / 6) * (getFunctionResult(a, b, c, beginPoint) +
		getFunctionResult(a, b, c, endPoint) + (2 * s) +
		(4 * st));

	endTime = clock();
	measurementTime = double(endTime - beginTime) / CLOCKS_PER_SEC;

	cout << endl << "--- METODA SIMPSONA ---" << endl << endl;
	cout << "Całka z funkcji:" << endl <<
		"f(x) = " << a << "x^2 + " << b << "x + " << c << " wynosi: " << result << endl;
	cout << "Czas dla jednego wątku: " << measurementTime << endl;


	return 0;
}