// Copyright © 2019-2020 Dmitriy Lukovenko. All rights reserved.
// Contacts: <mludima23@gmail.com>
// License: http://opensource.org/licenses/MIT

#pragma once

#include "Guid.h"
#include "TypeID.h"

namespace ion
{
	class IUnknown;
	typedef std::shared_ptr<IUnknown> IUnknownPtr;

	class IFactory
	{
	public:

		virtual ~IFactory() = default;

		virtual const ClassID& ciid() const = 0;
		virtual IUnknownPtr createClassInstance() const = 0;
	};
}
