#ifndef STATEMACHINEMGR_H
#define STATEMACHINEMGR_H

/*	This class is meant to maintain a syncronize an
*	SM/Context pair. The goal is to facilitate modular,
*	clearly defined components to operate within a larger 
*	system.
* 
*/

class CSMBase;
class CSMStateContext;

class CSMBaseMgr
{
public:
	CSMBaseMgr(std::shared_ptr<CSMBase> pSM = nullptr);
	virtual ~CSMBaseMgr();

	/* 
	 *	This should handle assignment of the SM and context 
	 *	if they have not been injected by the constructor.  
	 */
	virtual void init() = 0; 

protected:
	std::shared_ptr<CSMBase>		 m_pSM;
	std::shared_ptr<CSMStateContext> m_pCtx;

public:
	std::shared_ptr<CSMBase> getSM() const { return m_pSM;  }
	void setSM(std::shared_ptr<CSMBase> pSM) { m_pSM = pSM; }

	std::shared_ptr<CSMStateContext> getContext() const { return m_pCtx; }
	void setContext(std::shared_ptr<CSMStateContext> pCtx) { m_pCtx = pCtx; }

	void reset();
};
typedef std::shared_ptr<CSMBaseMgr> TSMManagerPtr;
#endif