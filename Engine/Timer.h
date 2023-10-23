#pragma once

#include <chrono>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#define NUM_LOOPS 10

using namespace std;
using namespace std::chrono;

class Timer {
private:
	time_point<steady_clock> currentStartTime, currentEndTime;

	vector<pair<time_point<steady_clock>, time_point<steady_clock>>> benchTimes;

public:

	/// <summary>
	/// Starts the 'stopwatch' so to speak
	/// </summary>
	void start() {
		currentStartTime = steady_clock::now();
		currentEndTime = currentStartTime; //Prevents accidental negative numbers, avoiding FUNKY behaviour
	}

	/// <summary>
	/// Stops the 'stopwatch' and records current time
	/// </summary>
	void stop() {
		currentEndTime = steady_clock::now();
	}

	/// <summary>
	/// Initialises a benchmark for repeat average times
	/// </summary>
	void start_benchmark() {
		benchTimes.clear();
	}

	/// <summary>
	/// Finish a benchmark lap and record time
	/// </summary>
	void lap_benchmark() {
		benchTimes.push_back(pair<time_point<steady_clock>, time_point<steady_clock>>(currentStartTime, steady_clock::now()));
		start();
	}

	/// <summary>
	/// Finishes the benchmark and averages the times. Access with get_elapsed
	/// </summary>
	void finish_benchmark() {
		duration<double, nano> sum{};

		for(pair<time_point<steady_clock>, time_point<steady_clock>> benchPair : benchTimes) {
			duration<double, nano> dur = benchPair.second - benchPair.first;
			sum += dur; //benchmark elapsed time
		}

		sum /= (double)benchTimes.size();

		currentStartTime = time_point<steady_clock>{};
		currentEndTime = currentStartTime + nanoseconds((long long)sum.count());
	}

	/// <summary>
	/// Exports benchmark times to a .csv file path
	/// </summary>
	/// <param name="path">Export path</param>
	template<typename _Period = milli>
	void export_benchmark(string path) {
		ofstream filestream(path, std::ios_base::trunc);

		for(int i = 0; i < benchTimes.size(); i++) {
			pair<time_point<steady_clock>, time_point<steady_clock>> benchPair = benchTimes[i];
			duration<double, _Period> dur = benchPair.second - benchPair.first;

			filestream << to_string(dur.count()) + ",\n";
		}

		filestream.close();
	}

	/// <summary>
	/// Gets the elapsed time from the called start time to the end time
	/// </summary>
	/// <typeparam name="_T">Time type format</typeparam>
	/// <typeparam name="_Period">Time period</typeparam>
	/// <returns>Time of _T in period _Period</returns>
	template<typename _T = double, typename _Period = ratio<1, 1>>
	_T get_elapsed() {
		duration<_T, _Period> elapsed = currentEndTime - currentStartTime;
		return elapsed.count();
	}

	Timer() {
		start();
		start_benchmark();
	}
};