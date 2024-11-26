#pragma once
#include <functional>
#include <vector>

namespace base {
	template <typename... Args>
	class Signal {
	public:
		using Slot = std::function<void(Args...)>;

		void connect(const Slot& slot) {
			m_slots.push_back(slot);
		}

		void emit(Args... args) {
			for (const auto& slot : m_slots) {
				slot(args...);
			}
		}

	private:
		std::vector<Slot> m_slots;
	};
}