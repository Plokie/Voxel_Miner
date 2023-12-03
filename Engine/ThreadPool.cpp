#include "ThreadPool.h"

#include <string>
#include <math.h>
#include "ThreadUtil.h"

void ThreadPool::_MainLoop()
{
	while (this->isRunning) {
		//if (jobs.empty()) continue;
		std::function<void()> job;
		
		{
			unique_lock<std::mutex> lock(queueMutex);
			new_job_cond.wait(lock, [this] {
				// Predicate
				return !jobs.empty() || !isRunning;
			});
			if (!isRunning) return;

			job = jobs.begin()->second;
			jobs.erase(jobs.begin());

			//job = jobs.front();
			//jobs.pop();
		}

		job();
	}
}

void ThreadPool::Init()
{
	// If the max supported system thread count is less than request thread count, go with min
	unsigned int num_threads = min(thread::hardware_concurrency()-2, thread_count); 
	for (unsigned int i = 0; i < num_threads; i++) {
		threads.emplace_back(thread(&ThreadPool::_MainLoop, this));
		if(namingScheme != "") {
			//threads[i]
			string name = namingScheme + " " + std::to_string(i);
			SetThreadName(&threads[i], name.c_str());
		}
	}
}

void ThreadPool::Queue(function<void()> func, int priority)
{
	{
		unique_lock<std::mutex> lock(queueMutex);
		//jobs.push(func);
		jobs.emplace(priority, func);
	}
	new_job_cond.notify_one();
}

void ThreadPool::Stop()
{
	if (this == nullptr) return;

	{
		unique_lock<std::mutex> lock(queueMutex);
		this->isRunning = false;
	}
	
	new_job_cond.notify_all();

	for (thread& thr : threads) {
		thr.join();
	}
	threads.clear();
}

bool ThreadPool::IsBusy()
{
	bool isBusy = false;
	{
		unique_lock<std::mutex> lock(queueMutex);
		isBusy = !jobs.empty();
	}
	return isBusy;
}
