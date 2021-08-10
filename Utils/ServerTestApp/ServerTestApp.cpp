// ServerTestApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include <iostream>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>

#include "ServerTest.h"
#include "TestEndpoints.h"
#include "RESTServer.h"


int main()
{
	TestApp test;

	test.init();
	test.run();
	std::cout << "Server timed out";
}
