#ifndef ACTIONCONTEXT_H
#define ACTIONCONTEXT_H

class CActionContext
{
	std::string m_name;
public:
	CActionContext(std::string name) : m_name(name) {}
	virtual ~CActionContext() {}

	std::string getName() const { return m_name; }
};

#endif // !ACTIONCONTEXT_H