/*
	Auto Closing Win32 HANDLE wrappers
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

/*
 * Why no #include <windows.h>?
 * The choice is up to the calling application to include the proper file.
 * There are cases where <winhttp.h> or <wininet.h> is required prior to <windows.h>
 * Yeah.. windows.h include order is quite finicy!
 */

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
namespace siddiqsoft
{
	/// @brief Automatically close the Win32 HANDLE object.
	class ACW32HANDLE
	{
		HANDLE _h {NULL};

		/// @brief Close the Win32 handle
		void internalClose()
		{
			try
			{
				if ((_h != NULL) && (_h != INVALID_HANDLE_VALUE)) { CloseHandle(_h); }
				_h = NULL;
			}
			catch (...)
			{
			}
		}

	public:
		ACW32HANDLE()
			: _h(NULL) {};
		~ACW32HANDLE() noexcept { internalClose(); }

		// Forbidden: non-owning construction
		explicit ACW32HANDLE(const HANDLE&) = delete;

		// Forbidden: non-owning assignment
		ACW32HANDLE& operator=(const HANDLE& arg) = delete;

		constexpr operator HANDLE() noexcept { return _h; }
		constexpr operator bool() { return ((_h != NULL) && (_h != INVALID_HANDLE_VALUE)); }
				  operator std::string() { return std::to_string(reinterpret_cast<uint64_t>(_h)); }
				  operator std::wstring() { return std::to_wstring(reinterpret_cast<uint64_t>(_h)); }

		/// @brief Constructor takes ownership of the HANDLE clearing out the argument.
		/// @param arg An open HANDLE from Windows; set to INVALID_HANDLE_VALUE upon return
		explicit ACW32HANDLE(HANDLE&& arg) noexcept
			: _h(std::exchange(arg, INVALID_HANDLE_VALUE))
		{
		}

		/// @brief Constructor takes ownership of the HANDLE clearing out the argument.
		/// @param arg An open HANDLE from Windows; set to INVALID_HANDLE_VALUE upon return
		explicit ACW32HANDLE(ACW32HANDLE&& arg) noexcept
			: _h(std::exchange(arg._h, INVALID_HANDLE_VALUE))
		{
		}

		/// @brief Owning assignment operator
		/// @param arg An open HANDLE/HINSTANCE from Windows; set to INVALID_HANDLE_VALUE upon return
		/// @return Self
		ACW32HANDLE& operator=(HANDLE&& arg)
		{
			internalClose();
			// Takes ownership by setting the arg to INVALID_HANDLE_VALUE
			_h = std::exchange(arg, INVALID_HANDLE_VALUE);
			return *this;
		}

		/// @brief Owning assignment operator
		/// @param arg An existing ACW32HANDLE instance
		/// @return Self
		ACW32HANDLE& operator=(ACW32HANDLE&& arg)
		{
			internalClose();
			// Takes ownership by setting the arg to INVALID_HANDLE_VALUE
			_h = std::exchange(arg._h, INVALID_HANDLE_VALUE);
			return *this;
		}
	};
} // namespace siddiqsoft
#else
#pragma message("Requires Windows platform.")
#endif

#endif
