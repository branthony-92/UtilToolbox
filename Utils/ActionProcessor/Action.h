#ifndef ACTION_G
#define ACTION_H


template <class T, class ...>
class Action
{
	std::optional<T>	  m_result;
	std::function<T(...)> m_action;

public:
	Action(std::function<T(...)> act)
		: m_action(act)
	{}

	~Action() {}

	std::future<std::optional<T>> exececuteAsync() {
		auto future = std::async(std::launch::async, execute);
		return future;
	}
	std::optional<T> execute() { 
		T result = m_action();
		return std::optional<T>(result);
	}

};

class Action_CRTP : public Action<Action_CRTP>
{

};

#endif // !ACTION_H