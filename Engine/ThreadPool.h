#pragma once

#include <mutex>
#include <vector>
#include <thread>
#include <queue>
#include <functional>

using namespace std;

class ThreadPool {
private:
	void _MainLoop();

	bool isRunning = true;
	std::mutex queueMutex;
	condition_variable new_job_cond;
	vector<thread> threads = {};
	queue<function<void()>> jobs;
public:
	string namingScheme = "";
	unsigned int thread_count = 4;

	void Init();
	void Queue(function<void()> func);
	void Stop();
	bool IsBusy();
};