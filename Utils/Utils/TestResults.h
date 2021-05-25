#ifndef TESTRESULT_H
#define TESTRESULT_H

#include "AsyncResult.h"

class TestResultEmpty : public CAsyncResult
{
public:
	TestResultEmpty()
		: CAsyncResult("ResultEmpty")
	{}
};

class TestResultComputeFib : public CAsyncResult
{
	int m_fibVal;
public:
	TestResultComputeFib()
		: CAsyncResult("ResultComputeFib")
	{}

	int getVal() const { return m_fibVal; }
	void setVal(int val) { m_fibVal = val; }
};

class TestResultSimple : public CAsyncResult
{
	int			m_intData;
	float		m_floatingData;
	std::string m_stringData;
	bool		m_boolData;

public:
	TestResultSimple()
		: CAsyncResult("ResultSimple")
		, m_intData(0)
		, m_floatingData(0.0f)
		, m_stringData("")
		, m_boolData(false)
	{}


	int			getIntData()    const { return m_intData;      }
	float		getFloatData()  const { return m_floatingData; }
	std::string getStringData() const { return m_stringData;   }
	bool		getBoolData()   const { return m_boolData;	   }

	void setIntData(int val)            { m_intData = val; }
	void setFloatData(float val)        { m_floatingData = val; }
	void setStringData(std::string val) { m_stringData = val; }
	void setBoolData(bool val)			{ m_boolData = val; }
};
#endif // !TESTRESULT_H