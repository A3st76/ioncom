// Copyright © 2019-2020 Dmitriy Lukovenko. All rights reserved.
// Contacts: <mludima23@gmail.com>
// License: http://opensource.org/licenses/MIT

#pragma once

#include "IFactory.h"

namespace ion
{
	class FactoryRegistry
	{
	public:

		inline FactoryRegistry() { }

		inline static IUnknownPtr getClassInstance(ClassID guid)
		{
			for(const auto& factory : factories_)
			{
				if (guid == factory->ciid())
					return factory->createClassInstance();
			}
			return nullptr;
		}

		inline static void* operator new(size_t, void* instance)
		{
			return factories_.emplace_back(reinterpret_cast<IFactory*>(instance));
		}

		inline static void operator delete(void*, void*) { }

	private:
		
		static std::vector<IFactory*> factories_;
	};

	inline std::vector<IFactory*> FactoryRegistry::factories_;
}
