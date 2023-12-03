#pragma once

#include <mutex>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <map>

using namespace std;

class ThreadPool {
private:
	void _MainLoop();

	bool isRunning = true;
	std::mutex queueMutex = {};
	condition_variable new_job_cond;
	vector<thread> threads = {};

	// map of priority to lambda func
	multimap<int, function<void()>> jobs = {};
	//queue<function<void()>> jobs;
public:
	string namingScheme = "";
	unsigned int thread_count = 4;

	void Init();
	void Queue(function<void()> func, int priority = 0);
	void Stop();
	bool IsBusy();
};