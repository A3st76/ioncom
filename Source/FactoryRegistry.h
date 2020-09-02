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

		FactoryRegistry() { }

		static inline IUnknownPtr getClassInstance(const ClassID& guid)
		{
			for(const auto& factory : factories_)
			{
				if (guid == factory->ciid())
				{
					return factory->createClassInstance();
				}
			}
			return nullptr;
		}

		static inline void* operator new(size_t, void* instance)
		{
			return factories_.emplace_back(reinterpret_cast<IFactory*>(instance));
		}

		static inline void operator delete(void*, void*) { }

	private:
		
		static std::vector<IFactory*> factories_;
	};

	inline std::vector<IFactory*> FactoryRegistry::factories_;
}
