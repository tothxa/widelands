/* Public domain */

#include <chrono>
#include <string>

static const std::string timer_unit = "μs";

struct Timer {
	std::chrono::time_point<std::chrono::steady_clock> start_time_;
	std::chrono::time_point<std::chrono::steady_clock> end_time_;
	std::chrono::microseconds elapsed_;

	void start() {
		start_time_ = std::chrono::steady_clock::now();
		elapsed_ = std::chrono::microseconds::zero();
	}
	void stop() {
		end_time_ = std::chrono::steady_clock::now();
		elapsed_ = std::chrono::duration_cast<std::chrono::microseconds>(end_time_ - start_time_);
	}
	int elapsed() {
		return elapsed_.count();
	}
};

