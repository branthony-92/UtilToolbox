// ServerTestApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include <iostream>

#include "ServerTest.h"
#include "TestEndpoints.h"
#include "RESTServer.h"

int main()
{
	TestApp test;

	test.init();
	std::cout << "Server Listening at: " << test.URL() << "\n";
	test.run();
	std::cout << "Server timed out";
}