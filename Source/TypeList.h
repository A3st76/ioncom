// Copyright © 2019-2020 Dmitriy Lukovenko. All rights reserved.
// Contacts: <mludima23@gmail.com>
// License: http://opensource.org/licenses/MIT

#pragma once

namespace ion
{
	struct NullType { };

	template<typename... TArgs>
	struct TypeList 
	{ 
		typedef NullType Head;
		typedef NullType Tail;
	};

	typedef TypeList<> EmptyTypeList;

	template<typename H, typename... T>
	struct TypeList<H, T...>
	{
		typedef H Head;
		typedef TypeList<T...> Tail;
	};
}
