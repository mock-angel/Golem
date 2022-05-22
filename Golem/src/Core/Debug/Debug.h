/*
 * Debug.h
 *
 *  Created on: 10-May-2022
 *      Author: anantha
 */

#pragma once

#include <string>
#include <iostream>

#include <Core/Time/Timer.h>

namespace Debug
{
	// void log(std::string text);
	// void log(std::string first, std::string second);

	template <class... Args>
	void log(Args... args)
	{
		(std::cout << ... << args) << "\n";
	}

} // namespace Debug
namespace Golem
{

	namespace TimekeeperUtils
	{
		template <size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char (&expr)[N], const char (&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
}
#define GOLEM_PROFILE 1
#if GOLEM_PROFILE
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define GOLEM_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define GOLEM_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define GOLEM_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define GOLEM_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define GOLEM_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define GOLEM_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define GOLEM_FUNC_SIG __func__
#else
#define GOLEM_FUNC_SIG "GOLEM_FUNC_SIG unknown!"
#endif

//#define GOLEM_PROFILE_BEGIN_SESSION(name, filepath) ::Hazel::Instrumentor::Get().BeginSession(name, filepath)
//#define GOLEM_PROFILE_END_SESSION() ::Hazel::Instrumentor::Get().EndSession()
#define GOLEM_PROFILE_SCOPE_LINE2(name, line)                                                         \
	constexpr auto fixedName##line = ::Golem::TimekeeperUtils::CleanupOutputString(name, "__cdecl "); \
	::Golem::Timer timer##line(fixedName##line.Data)
#define GOLEM_PROFILE_SCOPE_LINE(name, line) GOLEM_PROFILE_SCOPE_LINE2(name, line)
#define GOLEM_PROFILE_SCOPE(name) GOLEM_PROFILE_SCOPE_LINE(name, __LINE__)
#define GOLEM_PROFILE_FUNCTION() GOLEM_PROFILE_SCOPE(GOLEM_FUNC_SIG)
#else
#define GOLEM_PROFILE_BEGIN_SESSION(name, filepath)
#define GOLEM_PROFILE_END_SESSION()
#define GOLEM_PROFILE_SCOPE(name)
#define GOLEM_PROFILE_FUNCTION()
#endif
