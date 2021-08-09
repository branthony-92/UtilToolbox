#include "stdafx.h"
#include "PersistorTest.h"
#include "StateMachineTest.h"
#include "DirectoryTree.h"
#include "Logger.h"
#include "ServerTest.h"
#include "RESTServer.h"

int main()
{
	::testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}