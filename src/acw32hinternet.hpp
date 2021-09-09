/*
	Auto Closing Win32 HINTERNET wrappers
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

#ifndef ACW32HINTERNET_HPP
#define ACW32HINTERNET_HPP

/*
 * Why no #include <winhttp.h>?
 * The choice is up to the calling application to include the proper file.
 * There are cases where <winhttp.h> or <wininet.h> is required prior to <windows.h>
 * Yeah.. windows.h include order is quite finicy!
 */

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
namespace siddiqsoft
{
	/// @brief Automatically close the Win32 HINTERNET object.
	class ACW32HINTERNET
	{
		HINTERNET _ih {NULL};

		/// @brief Close the HINTERNET handle
		void internalClose()
		{
			if ((_ih != NULL) || (_ih != INVALID_HANDLE_VALUE)) { WinHttpCloseHandle(_ih); }
			_ih = NULL;
		}

	public:
		ACW32HINTERNET()
			: _ih(NULL) {};
		~ACW32HINTERNET() noexcept { internalClose(); }

		// Forbidden: non-owning construction
		explicit ACW32HINTERNET(const HINTERNET&) = delete;

		// Forbidden: non-owning assignment
		ACW32HINTERNET& operator=(const HINTERNET& arg) = delete;

		constexpr operator HINTERNET() noexcept { return _ih; }
		constexpr operator bool() { return ((_ih != NULL) && (_ih != INVALID_HANDLE_VALUE)); }
				  operator std::string() { return std::to_string(reinterpret_cast<uint64_t>(_ih)); }
				  operator std::wstring() { return std::to_wstring(reinterpret_cast<uint64_t>(_ih)); }

		/// @brief Constructor takes ownership of the HINSTANCE clearing out the argument.
		/// @param arg An open HINSTANCE from Windows; set to INVALID_HANDLE_VALUE upon return
		explicit ACW32HINTERNET(HINTERNET&& arg) noexcept
			: _ih(std::exchange(arg, INVALID_HANDLE_VALUE))
		{
		}

		/// @brief Constructor takes ownership of the HINSTANCE clearing out the argument.
		/// @param arg An open HINSTANCE from Windows; set to INVALID_HANDLE_VALUE upon return
		explicit ACW32HINTERNET(ACW32HINTERNET&& arg) noexcept
			: _ih(std::move(arg))
		{
		}

		/// @brief Owning assignment operator
		/// @param arg An open HINSTANCE from Windows; set to INVALID_HANDLE_VALUE upon return
		/// @return Self
		ACW32HINTERNET& operator=(HINTERNET&& arg)
		{
			internalClose();
			// Takes ownership by setting the arg to INVALID_HANDLE_VALUE
			_ih = std::exchange(arg, INVALID_HANDLE_VALUE);
			return *this;
		}


		ACW32HINTERNET& operator=(ACW32HINTERNET&& arg)
		{
			internalClose();
			// Takes ownership by setting the arg to INVALID_HANDLE_VALUE
			_ih = std::exchange(arg._ih, INVALID_HANDLE_VALUE);
			return *this;
		}
	};
} // namespace siddiqsoft
#else
#pragma message("Requires Windows platform.")
#endif

#endif
