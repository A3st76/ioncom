// Copyright © 2019-2020 Dmitriy Lukovenko. All rights reserved.
// Contacts: <mludima23@gmail.com>
// License: http://opensource.org/licenses/MIT

#pragma once

#include "IFactory.h"
#include "TypeList.h"
#include "FactoryRegistry.h"

namespace ion
{
	namespace internal
	{
		template <typename Head>
		struct interfaceCast;

		template <typename Head>
		struct interfaceCast
		{
			template<class TClass>
			static void* cast(TClass* instance)
			{
				return const_cast<Head*>(static_cast<const Head*>(static_cast<const void*>(instance)));
			}
		};

		template<>
		struct interfaceCast<IUnknown>
		{
			template<class TClass>
			static void* cast(TClass* instance)
			{
				return const_cast<IUnknown*>(static_cast<const IUnknown*>(static_cast<const void*>(instance)));
			}
		};
	}

	template <typename TList>
	struct interfaceCast;

	template<>
	struct interfaceCast<EmptyTypeList>
	{
		template<class TClass>
		static void* cast(TClass*, const InterfaceID&)
		{
			return 0;
		}
	};

	template<typename Head, typename... Tail>
	struct interfaceCast<TypeList<Head, Tail...>>
	{
		template<class TClass>
		static void* cast(TClass* instance, const InterfaceID& iid)
		{
			if (iidof<TypeList<Head, Tail...>::Head>() == iid)
				return internal::interfaceCast<TypeList<Head, Tail...>::Head>::cast(instance);

			return interfaceCast<TypeList<Head, Tail...>::Tail>::cast(instance, iid);
		}
	};

	template<class TClass>
	class Factory : public IFactory
	{
	public:

		Factory() 
		{ 
			new (this)FactoryRegistry();
		}

		virtual const ClassID& ciid() const
		{
			return TClass::ciid();
		}

		virtual IUnknownPtr createClassInstance() const
		{
			return std::reinterpret_pointer_cast<IUnknown>(std::make_shared<TClass>());
		}
	};

	template<class TClass>
	class SingletonFactory : public Factory<TClass>
	{
	public:

		SingletonFactory() : Factory<TClass>() { }

		virtual IUnknownPtr createClassInstance() const
		{
			std::lock_guard<std::mutex> lock(mutex_);
			static IUnknownPtr instance = std::reinterpret_pointer_cast<IUnknown>(std::make_shared<TClass>());
			return instance;
		}

	private:
		mutable std::mutex mutex_;
	};
}

#define IONINTERFACE_BEGIN()					\
	private:									\
		typedef ion::TypeList<ion::IUnknown

#define IONINTERFACE_ADD(interface) , interface
#define IONINTERFACE_END() > InterfaceList;

#define _IMPLEMENT_IUNKNOWN()												\
	public:																	\
		void* queryInterface(const InterfaceID& guid) const override		\
		{																	\
			return ion::interfaceCast<InterfaceList>::cast(this, guid);		\
		}

#define _FRIEND_CIIDOF()																					\
	private:																								\
		template<class TClass>																				\
		friend const ion::ClassID& ciidof();																\
																											\
		template<class TClass>																				\
		friend const ion::ClassID& ion::Factory<TClass>::ciid() const;

#define _FACTORY_DECLARE_SINGLETON(class, guid)																\
	private:																								\
		static ion::SingletonFactory<class> factory_;														\
																											\
		static const ion::ClassID& ciid() { static constexpr ion::ClassID guid_ = guid; return guid_; }

#define _FACTORY_DECLARE(class, guid)																		\
	private:																								\
		static ion::Factory<class> factory_;																\
																											\
		static const ion::ClassID& ciid() { static constexpr ion::ClassID guid_ = guid; return guid_; }

#define IONGENERATE_SINGLETONCLASS_GUID(class, guid)									\
_IMPLEMENT_IUNKNOWN()																	\
_FRIEND_CIIDOF()																		\
_FACTORY_DECLARE_SINGLETON(class, guid)

#define IONGENERATE_CLASS_GUID(class, guid)												\
_IMPLEMENT_IUNKNOWN()																	\
_FRIEND_CIIDOF()																		\
_FACTORY_DECLARE(class, guid)

#define IONREGISTER_SINGLETON_CLASS(class)		\
ion::SingletonFactory<class> class::factory_;

#define IONREGISTER_CLASS(class)		\
ion::Factory<class> class::factory_;
