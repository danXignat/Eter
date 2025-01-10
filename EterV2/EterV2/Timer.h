#pragma once
#include <chrono>
#include "Player.h"


namespace base {
    class Timer {
    public:
        Timer(int seconds) : time_limit_seconds(seconds), remaining_time_seconds(seconds) {}

        void start() {
            start_time = std::chrono::steady_clock::now();
            is_running = true;
        }

        void stop() {
            if (is_running) {
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();
                remaining_time_seconds -= elapsed;
                is_running = false;
            }
        }

        void reset() {
            remaining_time_seconds = time_limit_seconds;
            is_running = false;
        }

        int getRemainingTime() const {
            if (!is_running) return remaining_time_seconds;

            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();
            return std::max(0, remaining_time_seconds - static_cast<int>(elapsed));
        }

        bool hasTimeExpired() const {
            return getRemainingTime() <= 0;
        }

    private:
        int time_limit_seconds;
        int remaining_time_seconds;
        std::chrono::steady_clock::time_point start_time;
        bool is_running = false;
    };
}