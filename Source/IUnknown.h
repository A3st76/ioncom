// Copyright © 2019-2020 Dmitriy Lukovenko. All rights reserved.
// Contacts: <mludima23@gmail.com>
// License: http://opensource.org/licenses/MIT

#pragma once

#include "Guid.h"
#include "TypeID.h"

#define _FRIEND_IIDOF()								\
	private:										\
		template<class TClass>						\
		friend const ion::InterfaceID& iidof();

#define _INTERFACE_DECLARE(class, guid)																				\
	private:																										\
		friend struct std::default_delete<class>;																	\
		static const ion::InterfaceID& iid() { static constexpr ion::InterfaceID guid_ = guid; return guid_; }

#define IONINTERFACE_DECLARE_GUID(class, guid)																		\
	_FRIEND_IIDOF()																									\
	_INTERFACE_DECLARE(class, guid)

namespace ion
{
	class IUnknown
	{
		IONINTERFACE_DECLARE_GUID(IUnknown, "5a4edf92-6c8d-4a95-b2f2-38ec03e587d0"_guid)

	public:

		virtual ~IUnknown() = default;
		virtual void* queryInterface(const InterfaceID& guid) const = 0;
	};
}
