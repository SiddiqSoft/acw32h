/*
	Auto Closing Win32 HANDLE and HINTERNET wrappers
	Repo: https://github.com/SiddiqSoft/acw32handle
	Wiki: https://github.com/SiddiqSoft/acw32handle/wiki/Auto-Close-Release-Win32-Handle

	BSD 3-Clause License

	Copyright (c) 2020, Siddiq Software LLC
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this
	list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the documentation
	and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived from
	this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#ifndef ACW32HANDLE_HPP
#define ACW32HANDLE_HPP

#include <type_traits>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)

namespace siddiqsoft
{
	/// @brief Automatically close the Win32 HANDLE or HINTERNET object.
	/// There cannot be an implementation of the reference
	/// @tparam T Must be either HANDLE or HINTERNET
	template <class T> struct acw32h
	{
		T _h {NULL};

		acw32h() = default;

		/// @brief Constructor takes ownership of the HANDLE clearing out the argument.
		/// @param arg An open HANDLE/HINSTANCE from Windows; set to INVALID_HANDLE_VALUE upon return
		explicit acw32h(T&& arg) noexcept
		{
#ifdef HINTERNET
			if ((_h != NULL) && (_h != INVALID_HANDLE_VALUE) && std::is_same_v<T, HINTERNET>) { WinHttpCloseHandle(_h); }
#endif

			if ((_h != NULL) && (_h != INVALID_HANDLE_VALUE) && std::is_same_v<T, HANDLE>) { CloseHandle(_h); }

			// Takes ownership by setting the arg to INVALID_HANDLE_VALUE
			_h = std::exchange(arg, INVALID_HANDLE_VALUE);
		}

		~acw32h() noexcept
		{
#ifdef HINTERNET
			if ((_h != NULL) && (_h != INVALID_HANDLE_VALUE) && std::is_same_v<T, HINTERNET>) { WinHttpCloseHandle(_h); }
#endif
			if ((_h != NULL) && (_h != INVALID_HANDLE_VALUE) && std::is_same_v<T, HANDLE>) { CloseHandle(_h); }
		}


		/// @brief Owning assignment operator
		/// @param arg An open HANDLE/HINSTANCE from Windows; set to INVALID_HANDLE_VALUE upon return
		/// @return Self
		acw32h& operator=(T&& arg)
		{
#ifdef HINTERNET
			if ((_h != NULL) && (_h != INVALID_HANDLE_VALUE) && std::is_same_v<T, HINTERNET>) { WinHttpCloseHandle(_h); }
#endif

			if ((_h != NULL) && (_h != INVALID_HANDLE_VALUE) && std::is_same_v<T, HANDLE>) { CloseHandle(_h); }

			// Takes ownership by setting the arg to INVALID_HANDLE_VALUE
			_h = std::exchange(arg, INVALID_HANDLE_VALUE);
			return *this;
		}

		constexpr operator T() noexcept { return _h; }
		constexpr operator bool() { return ((_h != NULL) && (_h != INVALID_HANDLE_VALUE)); }
				  operator std::string() { return std::to_string(reinterpret_cast<uint64_t>(_h)); }
				  operator std::wstring() { return std::to_wstring(reinterpret_cast<uint64_t>(_h)); }
	};
} // namespace siddiqsoft

using ACW32HANDLE = siddiqsoft::acw32h<HANDLE>;
#ifdef HINTERNET
using ACW32HINTERNET = siddiqsoft::acw32h<HINTERNET>;
#endif

#else
#pragma message("Requires Windows platform.")
#endif

#endif
