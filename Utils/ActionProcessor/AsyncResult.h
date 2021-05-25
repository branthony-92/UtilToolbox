#ifndef ASYNCRESULT_H
#define ASYNCRESULT_H

typedef std::lock_guard<std::mutex> TLock;

class CAsyncResult
{
public:
	enum class Status {
		Invalid = -1,

		Idle,
		Executing,
		Complete,
		Error,
		Timeout,
	};

	CAsyncResult(std::string name);
	CAsyncResult(const CAsyncResult& other);
	CAsyncResult& operator=(const CAsyncResult& other);
	virtual ~CAsyncResult();

	Status getStatus() const { TLock lock(m_mutex); return m_status; }
	void setStatus(Status s) { TLock lock(m_mutex); m_status = s; }

	std::string getError() const      { TLock lock(m_mutex); return m_error; }
	void setError(std::string errMsg) { TLock lock(m_mutex); m_error = errMsg; }

	const std::string c_name;

protected:
	Status	    m_status;
	std::string m_error;

	mutable std::mutex m_mutex;

	// convenience template function to wrape dynamic pointer casts for our results
public:
	template <typename T>
	static std::shared_ptr<T> convertTo(std::shared_ptr<CAsyncResult> pBaseResult) {
		return std::dynamic_pointer_cast<T>(pBaseResult);
	}
};
typedef std::shared_ptr<CAsyncResult> TResultPtr;
#endif                                                                                                                