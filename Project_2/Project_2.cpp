#include "pch.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <math.h>
#include <fstream>
#include <string>
#include <cstdlib>

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

vector<string> getFromFile(string filePath)
{
	string str;
	vector<string> fileContents;

	ifstream myfile;
	myfile.open(filePath);

	while (getline(myfile, str))
	{
		fileContents.push_back(str);
	}

	myfile.close();
	return fileContents;
}

vector<string> split(const string& str, const string& delim)
{
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == string::npos) pos = str.length();
		string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());

	return tokens;
}

void saveToFile(
	string extraInfo,
	double resultRectangles,
	double measurementTimeRectangle, 
	double resultTrapezoidal, 
	double measurementTimeTrapezoidal,
	double resultSimpson,
	double measurementTimeSimpson
)
{
	ofstream myFile;
	myFile.open("result.csv", ios_base::app);
	myFile << "\n" 
		+ to_string(resultRectangles)
		+ "," + extraInfo
		+ "," + to_string(measurementTimeRectangle)
		+ "," + to_string(resultTrapezoidal)
		+ "," + to_string(measurementTimeTrapezoidal)
		+ "," + to_string(resultSimpson)
		+ "," + to_string(measurementTimeSimpson);
	myFile.close();
}

int main(int argc, char *argv[])
{
	string filePath;
	cin >> filePath;
	const int numberOfSteps = 100000;
	double integralRangeArr[numberOfSteps];

	vector<string> allLinesFromFile = getFromFile(filePath);
	int countLines = allLinesFromFile.size();

	auto step_size = 100ul;
	auto total_steps = countLines;
	size_t steps_completed = 0;

	double measurementOneThread = 0;
	double measurementFourThread = 0;

	clock_t beginTimeOneThread = clock();
	// For 1 Thread
	for (int i = 0; i < countLines; i++)
	{
		vector<string> splitResultLine = split(allLinesFromFile[i], ",");

		double a = stod(splitResultLine[0]);
		double b = stod(splitResultLine[1]);
		double c = stod(splitResultLine[2]);
		double beginPoint = stod(splitResultLine[3]);
		double endPoint = stod(splitResultLine[4]);

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
		double measurementTimeRectangle = 0;

		clock_t beginTime = clock();
		double resultRectangles = rectanglesMethod(numberOfSteps, a, b, c, beginPoint, endPoint, integralRangeArr);
		clock_t endTime = clock();
		measurementTimeRectangle = double(endTime - beginTime) / CLOCKS_PER_SEC;

		// --- METODA TRAPEZOW ---
		double measurementTimeTrapezoidal = 0;

		beginTime = clock();
		double resultTrapezoidal = trapezoidalMethod(numberOfSteps, a, b, c, beginPoint, endPoint, integralRangeArr);
		endTime = clock();
		measurementTimeTrapezoidal = double(endTime - beginTime) / CLOCKS_PER_SEC;

		// --- METODA SIMPSONA ---
		double measurementTimeSimpson = 0;

		beginTime = clock();
		double resultSimpson = simpsonMethod(numberOfSteps, a, b, c, beginPoint, endPoint, integralRangeArr);
		endTime = clock();
		measurementTimeSimpson = double(endTime - beginTime) / CLOCKS_PER_SEC;

		saveToFile("Thread 1", resultRectangles, measurementTimeRectangle, resultTrapezoidal, measurementTimeTrapezoidal, resultSimpson, measurementTimeSimpson);

		++steps_completed;

		if (steps_completed % 10 == 1)
		{
			system("cls");
			int j = 0;
			cout << "[";
			int percent = 100 * steps_completed / total_steps;
			int percentStep = 1;
			if (percent > 0 && percent < 10)
			{
				percentStep = 1;
			}
			else if (percent >= 10 && percent < 20)
			{
				percentStep = 2;
			}
			else if (percent >= 20 && percent < 30)
			{
				percentStep = 3;
			}
			else if (percent >= 40 && percent < 50)
			{
				percentStep = 4;
			}
			else if (percent >= 50 && percent < 60)
			{
				percentStep = 5;
			}
			else if (percent >= 60 && percent < 70)
			{
				percentStep = 6;
			}
			else if (percent >= 70 && percent < 80)
			{
				percentStep = 7;
			}
			else if (percent >= 80 && percent < 90)
			{
				percentStep = 8;
			}
			else if (percent >= 90 && percent < 100)
			{
				percentStep = 9;
			}
			for (j; j < percentStep; j++)
			{
				cout << "* ";
			}
			for (int k = 0; k < 10 - percentStep; k++)
			{
				cout << ". ";
			}
			cout << "] ";
			cout << (100.0 * steps_completed / total_steps) << "%";
		}
	}
	clock_t endTimeOneThread = clock();
	measurementOneThread = double(endTimeOneThread - beginTimeOneThread) / CLOCKS_PER_SEC;

	system("cls");
	cout << "[* * * * * * * * * * ] 100%";

	steps_completed = 0;

	// For 4 Thread
	clock_t beginTimeFourThread = clock();
	#pragma omp parallel for
	for (int i = 0; i < countLines; i++)
	{
		vector<string> splitResultLine = split(allLinesFromFile[i], ",");

		double a = stod(splitResultLine[0]);
		double b = stod(splitResultLine[1]);
		double c = stod(splitResultLine[2]);
		double beginPoint = stod(splitResultLine[3]);
		double endPoint = stod(splitResultLine[4]);

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
		double measurementTimeRectangle = 0;

		clock_t beginTime = clock();
		double resultRectangles = rectanglesMethod(numberOfSteps, a, b, c, beginPoint, endPoint, integralRangeArr);
		clock_t endTime = clock();
		measurementTimeRectangle = double(endTime - beginTime) / CLOCKS_PER_SEC;

		// --- METODA TRAPEZOW ---
		double measurementTimeTrapezoidal = 0;

		beginTime = clock();
		double resultTrapezoidal = trapezoidalMethod(numberOfSteps, a, b, c, beginPoint, endPoint, integralRangeArr);
		endTime = clock();
		measurementTimeTrapezoidal = double(endTime - beginTime) / CLOCKS_PER_SEC;

		// --- METODA SIMPSONA ---
		double measurementTimeSimpson = 0;

		beginTime = clock();
		double resultSimpson = simpsonMethod(numberOfSteps, a, b, c, beginPoint, endPoint, integralRangeArr);
		endTime = clock();
		measurementTimeSimpson = double(endTime - beginTime) / CLOCKS_PER_SEC;

		saveToFile("Thread 4", resultRectangles, measurementTimeRectangle, resultTrapezoidal, measurementTimeTrapezoidal, resultSimpson, measurementTimeSimpson);
		
		#pragma omp atomic
		++steps_completed;

		#pragma omp critical
		if (steps_completed % 10 == 1)
		{
			system("cls");
			int j = 0;
			cout << "[";
			int percent = 100 * steps_completed / total_steps;
			int percentStep = 1;
			if (percent > 0 && percent < 10)
			{
				percentStep = 1;
			}
			else if (percent >= 10 && percent < 20)
			{
				percentStep = 2;
			}
			else if (percent >= 20 && percent < 30)
			{
				percentStep = 3;
			}
			else if (percent >= 40 && percent < 50)
			{
				percentStep = 4;
			}
			else if (percent >= 50 && percent < 60)
			{
				percentStep = 5;
			}
			else if (percent >= 60 && percent < 70)
			{
				percentStep = 6;
			}
			else if (percent >= 70 && percent < 80)
			{
				percentStep = 7;
			}
			else if (percent >= 80 && percent < 90)
			{
				percentStep = 8;
			}
			else if (percent >= 90 && percent < 100)
			{
				percentStep = 9;
			}
			for (j; j < percentStep; j++)
			{
				cout << "* ";
			}
			for (int k = 0; k < 10 - percentStep; k++)
			{
				cout << ". ";
			}
			cout << "] ";
			cout << (100.0 * steps_completed / total_steps) << "%";
		}
	}
	clock_t endTimeFourThread = clock();
	measurementFourThread = double(endTimeFourThread - beginTimeFourThread) / CLOCKS_PER_SEC;

	system("cls");
	cout << "[* * * * * * * * * * ] 100%" << endl << endl;
	cout << "Czas dla 1 watku: " << measurementOneThread << endl;
	cout << "Czas dla 4 watku: " << measurementFourThread << endl;

	return 0;
}