// Copyright © 2019-2020 Dmitriy Lukovenko. All rights reserved.
// Contacts: <mludima23@gmail.com>
// License: http://opensource.org/licenses/MIT

#pragma once

#include "Guid.h"

namespace ion
{
	typedef Guid InterfaceID;
	typedef Guid ClassID;

	template<class TClass>
	inline const ClassID& ciidof()
	{
		return TClass::ciid();
	}

	template<class TClass>
	inline const InterfaceID& iidof()
	{
		return TClass::iid();
	}
}
