#pragma once

#include <bitset>
#include <cstdint>

// ECS sets
using Entity = std::uint32_t;
const inline Entity MAX_ENTITIES = 5000;
using ComponentType = std::uint8_t;
const inline ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;
const inline int MAX_SYSTEMS = 128;

// Source: https://gist.github.com/Lee-R/3839813
constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
{
	return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
}

constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count)
{
	return fnv1a_32(s, count);
}

// Events
using EventId = std::uint32_t;
using ParamId = std::uint32_t;

#define METHOD_LISTENER(EventType, Listener) EventType, std::bind(&Listener, this, std::placeholders::_1)
#define FUNCTION_LISTENER(EventType, Listener) EventType, std::bind(&Listener, std::placeholders::_1)

#if defined(__cpp_lib_unreachable)
#define CAKE_UNREACHABLE std::unreachable()
#elif defined(__GNUC__) || defined(__clang__)
#define CAKE_UNREACHABLE __builtin_unreachable()
#elif defined(_MSC_VER)
#define CAKE_UNREACHABLE __assume(0)
#else
#define CAKE_UNREACHABLE
#endif // _MSC_VER