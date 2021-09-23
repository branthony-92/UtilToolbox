#ifndef STATEMACHINEMGR_H
#define STATEMACHINEMGR_H

#include "SMBase.h"

/*	This class is meant to maintain a syncronize an
*	SM/Context pair. The goal is to facilitate modular,
*	clearly defined components to operate within a larger 
*	system.
* 
*/


class CSMBaseMgr
{
	using SMPtr = std::shared_ptr<CSMBase>;
	using SMMap = std::map<std::string, SMPtr>;
public:
	CSMBaseMgr(std::string name, uint32_t timeout = 100);
	virtual ~CSMBaseMgr();

	/* 
	 *	This should handle assignment of the SM and context 
	 *	if they have not been injected by the constructor.  
	 */
	virtual void startStateMachines(); 
	void resetStateMachines();
	void stopStateMachines();
	void registerStateMachine(SMPtr pSM);


protected:
	const std::string c_name;
	const uint32_t c_ticTimeout;

	SMMap		        m_stateMachines;
	std::thread	        m_ticThread;
	std::atomic<bool>	m_enabled;
public:
	SMMap& getStateMachines() { return m_stateMachines;  }



};
typedef std::shared_ptr<CSMBaseMgr> TSMManagerPtr;
#endif