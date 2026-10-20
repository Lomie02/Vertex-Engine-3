#pragma once
#include <iostream>
#include <functional>
#include <vector>;
#include <algorithm>
#include <cstddef>
#include <utility>

namespace VertexEngine {

	template<typename... Args>

	class VertexEvent
	{

	public:
		// Reminder: Using keyword is an alias
		using Callback = std::function<void(Args...)>;
		using ListenerID = std::size_t;


		ListenerID AddListener(Callback _call) { // Add Listener

			const ListenerID id = m_NextId++;

			m_Listeners.push_back({ id, std::move(_call) });

			return id;
		}

		void RemoveListener(ListenerID id) { // Remove a listener from the event

			std::erase_if(m_Listeners,
				[id](const EventListener& listener)
				{
					return listener.id == id;
				});

		}

		void Invoke(Args... args) { // Invoke all listeners
			auto currentListeners = m_Listeners;

			for (auto& listener : m_Listeners) {

				auto it = std::find_if(m_Listeners.begin(), m_Listeners.end(),
					[&listener](const EventListener& item) {return item.id == listener.id; });

				if (it != m_Listeners.end())
					listener.callback(args...);
			}
		}

		void ClearEvent() { m_Listeners.clear(); } // Clear all listeners in the event.

	private:

		struct EventListener {
		public:
			ListenerID id;
			Callback callback;
		};

		std::vector<EventListener> m_Listeners; // List of all listener members
		ListenerID m_NextId = 1; // listen id.

	};
}

