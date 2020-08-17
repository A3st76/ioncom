// Copyright © 2019-2020 Dmitriy Lukovenko. All rights reserved.
// Contacts: <mludima23@gmail.com>
// License: http://opensource.org/licenses/MIT

#pragma once

#include "Source/IUnknown.h"

namespace ion
{
	class IModule : public IUnknown
	{
		IONINTERFACE_DECLARE_GUID(IModule, "1ebbaa9e-f715-44fa-8048-ed87c0fed7ed"_guid)

	public:

		virtual void initialize() = 0;
	};
}
