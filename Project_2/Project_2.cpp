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

double rectanglesMethod(int numberOfSteps, double a, double b, double c, double beginPoint, double endPoint, double integralRangeArr[])
{
	double sumResult = 0;
	double distanceBetweenPoints = getDistanceBetweenPoints(beginPoint, endPoint, numberOfSteps);

	for (int i = 0; i < numberOfSteps; i++)
	{
		double functionResult = getFunctionResult(a, b, c, integralRangeArr[i]);
		sumResult += functionResult;
	}

	double result = distanceBetweenPoints * sumResult;
	return result;
}

double trapezoidalMethod(int numberOfSteps, double a, double b, double c, double beginPoint, double endPoint, double integralRangeArr[])
{
	double sumResult = 0;
	double distanceBetweenPoints = getDistanceBetweenPoints(beginPoint, endPoint, numberOfSteps);

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

	double result = distanceBetweenPoints * sumResult;
	return result;
}

double simpsonMethod(int numberOfSteps, double a, double b, double c, double beginPoint, double endPoint, double integralRangeArr[])
{
	double st = 0;
	double s = 0;
	double distanceBetweenPoints = getDistanceBetweenPoints(beginPoint, endPoint, numberOfSteps);

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

	double result = (distanceBetweenPoints / 6) * (getFunctionResult(a, b, c, beginPoint) +
		getFunctionResult(a, b, c, endPoint) + (2 * s) + (4 * st));
	return result;
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
	double measurementTime = 0;

	clock_t beginTime = clock();
	double resultRectangles = rectanglesMethod(numberOfSteps, a, b, c, beginPoint, endPoint, integralRangeArr);
	clock_t endTime = clock();
	measurementTime = double(endTime - beginTime) / CLOCKS_PER_SEC;

	cout << "--- METODA PROSTAKATOW ---" << endl << endl;

	cout << "Całka z funkcji:" << endl <<
		"f(x) = " << a << "x^2 + " << b << "x + " << c << " wynosi: " << resultRectangles << endl;

	cout << "Czas dla jednego wątku: " << measurementTime << endl;

	// --- METODA TRAPEZOW ---
	// Wyzerowanie czasu
	measurementTime = 0;

	beginTime = clock();
	double resultTrapezoidal = trapezoidalMethod(numberOfSteps, a, b, c, beginPoint, endPoint, integralRangeArr);
	endTime = clock();
	measurementTime = double(endTime - beginTime) / CLOCKS_PER_SEC;

	cout << endl << "--- METODA TRAPEZÓW ---" << endl << endl;
	cout << "Całka z funkcji:" << endl <<
		"f(x) = " << a << "x^2 + " << b << "x + " << c << " wynosi: " << resultTrapezoidal << endl;
	cout << "Czas dla jednego wątku: " << measurementTime << endl;

	// --- METODA SIMPSONA ---
	// Wyzerowanie czasu
	measurementTime = 0;

	beginTime = clock();
	double resultSimpson = simpsonMethod(numberOfSteps, a, b, c, beginPoint, endPoint, integralRangeArr);
	endTime = clock();
	measurementTime = double(endTime - beginTime) / CLOCKS_PER_SEC;

	cout << endl << "--- METODA SIMPSONA ---" << endl << endl;
	cout << "Całka z funkcji:" << endl <<
		"f(x) = " << a << "x^2 + " << b << "x + " << c << " wynosi: " << resultSimpson << endl;
	cout << "Czas dla jednego wątku: " << measurementTime << endl;

	return 0;
}