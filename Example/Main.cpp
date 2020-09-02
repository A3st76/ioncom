// Copyright © 2019-2020 Dmitriy Lukovenko. All rights reserved.
// Contacts: <mludima23@gmail.com>
// License: http://opensource.org/licenses/MIT

#include "Source/Weaver.h"
#include "Source/FactoryRegistry.h"
#include "Module.cpp"

using namespace ion;

int main(int argc, char** argv)
{
	auto instance = FactoryRegistry::getClassInstance(ciidof<Module>());
	auto cast = interface_cast<IModule>(instance);
	cast->initialize();

	return 0;
}
