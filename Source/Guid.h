// Copyright © 2019-2020 Dmitriy Lukovenko. All rights reserved.
// Contacts: <mludima23@gmail.com>
// License: http://opensource.org/licenses/MIT

#pragma once

namespace ion
{
	template<typename ...TArgs>
	inline std::string format(const std::string& fmt, TArgs&& ...args)
	{
		int size = std::snprintf(nullptr, 0, fmt.c_str(), std::forward<TArgs>(args)...) + 1;

		if (size <= 0)
		{
			throw std::logic_error("Error during formatting");
		}

		std::unique_ptr<char[]> buffer = std::make_unique<char[]>(size);
		std::snprintf(buffer.get(), static_cast<size_t>(size), fmt.c_str(), std::forward<TArgs>(args)...);
		return std::string(buffer.get(), buffer.get() + size - 1);
	}

	struct Guid
	{
		uint32_t data1;
		uint16_t data2;
		uint16_t data3;
		std::array<uint8_t, 8> data4;

		constexpr Guid() : data1(0), data2(0), data3(0), data4{ } { }

		constexpr Guid(const Guid& rhs) : 
			data1(rhs.data1), data2(rhs.data2), data3(rhs.data3), 
			data4{ rhs.data4[0], rhs.data4[1], rhs.data4[2], rhs.data4[3], rhs.data4[4], 
			rhs.data4[5], rhs.data4[6], rhs.data4[7] } { }

		constexpr Guid(const uint32_t& data1_, const uint16_t& data2_, const uint16_t& data3_,
			const uint8_t& data4_0, const uint8_t& data4_1, const uint8_t& data4_2, const uint8_t& data4_3,
			const uint8_t& data4_4, const uint8_t& data4_5, const uint8_t& data4_6, const uint8_t& data4_7) : 
			data1(data1_), data2(data2_), data3(data3_),
			data4{ data4_0, data4_1, data4_2, data4_3, data4_4, data4_5, data4_6, data4_7 } { }

		struct Utils
		{
			static constexpr uint8_t hexCharToUInt8(char c)
			{
				return c >= '0' && c <= '9' ? c - '0' :
					c >= 'a' && c <= 'f' ? c - 'a' + 10 :
					c >= 'A' && c <= 'F' ? c - 'A' + 10 : 0;
			}

			static constexpr uint16_t hexCharToUInt16(char c)
			{
				return static_cast<uint16_t>(hexCharToUInt8(c));
			}

			static constexpr uint32_t hexCharToUInt32(char c)
			{
				return static_cast<uint32_t>(hexCharToUInt8(c));
			}

			static constexpr uint8_t hexStringToUInt8(const char* s)
			{
				return (hexCharToUInt8(s[0]) << 4) + hexCharToUInt8(s[1]);
			}

			static constexpr uint16_t hexStringToUInt16(const char* s)
			{
				return (hexCharToUInt16(s[0]) << 12) + (hexCharToUInt16(s[1]) << 8) +
					(hexCharToUInt16(s[2]) << 4) + hexCharToUInt16(s[3]);
			}

			static constexpr uint32_t hexStringToUInt32(const char* s)
			{
				return (hexCharToUInt32(s[0]) << 28) + (hexCharToUInt32(s[1]) << 24) +
					(hexCharToUInt32(s[2]) << 20) + (hexCharToUInt32(s[3]) << 16) +
					(hexCharToUInt32(s[4]) << 12) + (hexCharToUInt32(s[5]) << 8) +
					(hexCharToUInt32(s[6]) << 4) + hexCharToUInt32(s[7]);
			}
		};

		static constexpr Guid construct(const char* guid)
		{
			uint32_t data1 = Utils::hexStringToUInt32(guid);
			uint16_t data2 = Utils::hexStringToUInt16(guid + 9);
			uint16_t data3 = Utils::hexStringToUInt16(guid + 14);
			uint8_t data4[8] = {
				Utils::hexStringToUInt8(guid + 19), Utils::hexStringToUInt8(guid + 21),
				Utils::hexStringToUInt8(guid + 24), Utils::hexStringToUInt8(guid + 26),
				Utils::hexStringToUInt8(guid + 28), Utils::hexStringToUInt8(guid + 30),
				Utils::hexStringToUInt8(guid + 32), Utils::hexStringToUInt8(guid + 34)
			};

			return Guid(data1, data2, data3,
				data4[0], data4[1], data4[2], data4[3], 
				data4[4], data4[5], data4[6], data4[7]);
		}

		static constexpr Guid null()
		{
			return Guid(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		}

		inline const std::string toString() const
		{
			return format("%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
				data1, data2, data3,
				data4[0], data4[1], data4[2],
				data4[3], data4[4], data4[5],
				data4[6], data4[7]);
		}

		inline static Guid generate()
		{
			const char* temp = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
			const char* hex = "0123456789abcdef";

			char buf[37] = {};

			for (int i = 0; i < strlen(temp); i++)
			{
				int rand = std::rand() % 16;
				char symbol = ' ';

				switch (temp[i])
				{
					case 'x': { symbol = hex[rand]; } break;
					case 'y': { symbol = hex[rand & 0x03 | 0x08]; } break;
					default: { symbol = temp[i]; } break;
				}

				buf[i] = symbol;
			}

			return Guid::construct(buf);
		}

		constexpr bool operator==(const Guid& rhs) const
		{
			return data1 == rhs.data1 && data2 == rhs.data2 && data3 == rhs.data3 && data4 == rhs.data4;
		}
	};

	constexpr Guid operator""_guid(const char* guid, size_t)
	{
		return (guid[0] == '{') ? Guid::construct(guid + 1) : Guid::construct(guid);
	}

	inline std::ostream& operator<<(std::ostream& os, const Guid& rhs)
	{
		os << rhs.toString();
		return os;
	}
}

namespace std 
{
	template <>
	struct hash<ion::Guid>
	{
		size_t operator()(const ion::Guid& rhs) const
		{
			return hash<uint32_t>()(rhs.data1) ^ hash<uint16_t>()(rhs.data2) ^ hash<uint16_t>()(rhs.data3) ^ 
				hash<uint8_t>()(rhs.data4[0]) ^ hash<uint8_t>()(rhs.data4[1]) ^ hash<uint8_t>()(rhs.data4[2]) ^
				hash<uint8_t>()(rhs.data4[3]) ^ hash<uint8_t>()(rhs.data4[4]) ^ hash<uint8_t>()(rhs.data4[5]) ^
				hash<uint8_t>()(rhs.data4[6]) ^ hash<uint8_t>()(rhs.data4[7]);
		}
	};
}
