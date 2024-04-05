// Stax Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "LinkedNode.h"
#include "Application.h"
#include "di.hpp"

int main()
{
    auto injector = boost::di::make_injector(
        boost::di::bind<IApplication>().to<Application>()
    );
    auto app = injector.create<std::unique_ptr<IApplication>>();
    app->run();
    return 0;
}
