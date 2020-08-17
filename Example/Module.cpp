// Copyright © 2019-2020 Dmitriy Lukovenko. All rights reserved.
// Contacts: <mludima23@gmail.com>
// License: http://opensource.org/licenses/MIT

#pragma once

#include "Source/Weaver.h"

namespace ion
{
	class Module : public IModule
	{
		IONINTERFACE_BEGIN()
			IONINTERFACE_ADD(ion::IModule)
		IONINTERFACE_END()

		IONGENERATE_CLASS_GUID(Module, "26e46204-6db2-4f02-b8ff-bea4de2ee060"_guid)

	public:
		Module() { std::cout << "Created!" << std::endl; }
		~Module() { std::cout << "Deleted!" << std::endl; }

		virtual void initialize() { std::cout << "Initialize!" << std::endl; };
	};
}

IONREGISTER_CLASS(ion::Module)
