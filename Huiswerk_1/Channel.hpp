#ifndef CHANNEL
#define CHANNEL
#include <list>
#include <condition_variable>
#include <allocators>

template <class item>

class Channel
{
private:
	std::list<item> queue;
	std::mutex m;
	std::condition_variable cv;
	bool closed;

public:
	Channel(): closed(false)
	{
	}

	void close()
	{
		std::unique_lock<stdext::mutex> lock(m);
		closed = true;
		cv.notify_all();
	}

	bool is_closed()
	{
		std::unique_lock<std::mutex> lock(m);
		return closed;
	}

	void put(const item& i)
	{
		std::unique_lock<std::mutex> lock(m);
		if (closed)
			throw std::logic_error("put called on closed channel");
		queue.push_back(i);
		cv.notify_one();
	}

	bool get(item& out, bool wait = true)
	{
		std::unique_lock<std::mutex> lock(m);
		if (wait)
			cv.wait(lock, [&]() { return closed || !queue.empty(); });
		if (queue.empty())
			return false;
		out = queue.front();
		queue.pop_front();
		return true;
	}
};

#endif
