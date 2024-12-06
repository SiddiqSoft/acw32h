#include "gtest/gtest.h"

#include <Windows.h>
#include "../include/siddiqsoft/acw32handle.hpp"
#include <winhttp.h>
#include "../include/siddiqsoft/acw32hinternet.hpp"

#pragma comment(lib, "winhttp.lib")


TEST(basic, OwningAutoClose_HANDLE)
{
	HANDLE eh = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	// Copy/store this for later
	auto originalHandleValue = reinterpret_cast<uint64_t>(eh);

	// The handle must be non-null
	EXPECT_FALSE(NULL == eh);
	EXPECT_NE(INVALID_HANDLE_VALUE, eh);

	try
	{
		// Will not compile! The eh cannot be copied and must be moved.
		siddiqsoft::ACW32HANDLE ach(std::move(eh));

		// The original handle is cleared
		EXPECT_EQ(INVALID_HANDLE_VALUE, eh);
		// The originalHandleValue and the ach must match
		EXPECT_EQ(HANDLE(originalHandleValue), HANDLE(ach));
	} // the object will be closed upon destructor
	catch (const std::exception& e)
	{
		EXPECT_TRUE(FALSE) << e.what();
	}

	// If the CloseHandle is called twice, we throw.
	BOOL rc = TRUE;
	try
	{
		auto rc = CloseHandle(HANDLE(originalHandleValue));
	}
	catch (...)
	{
		SetLastError(ERROR_INVALID_HANDLE);
		rc = FALSE;
	}
	// Failure should be expected
	EXPECT_EQ(FALSE, rc);
	// Error code should be ERROR_INVALID_HANDLE or ERROR_INVALID_STATE
	auto lerr = GetLastError();
	EXPECT_TRUE((ERROR_INVALID_HANDLE == lerr) || (lerr == ERROR_INVALID_STATE)) << "rc: " << rc << " lerr: " << lerr;
}


TEST(basic, OwningAutoClose_HINTERNET)
{
	HINTERNET ih = ::WinHttpOpen(L"", WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	// Copy/store this for later
	auto originalHandleValue = reinterpret_cast<uint64_t>(ih);

	// The handle must be non-null
	EXPECT_FALSE(NULL == ih);
	EXPECT_NE(INVALID_HANDLE_VALUE, ih);

	try
	{
		// Will not compile! The eh cannot be copied and must be moved.
		//siddiqsoft::ACW32HINTERNET ach(eh);

		// We share ownership with the other handle
		siddiqsoft::ACW32HINTERNET ach(std::move(ih));

		// The original handle is cleared
		EXPECT_EQ(INVALID_HANDLE_VALUE, ih);
		// The originalHandleValue and the ach must match
		EXPECT_EQ(HINTERNET(originalHandleValue), HINTERNET(ach));
	} // the object will be closed upon destructor
	catch (const std::exception& e)
	{
		EXPECT_TRUE(FALSE) << e.what();
	}

	// The handle must be closed
	// If the WinHttpCloseHandle is called twice, we throw.
	auto rc = WinHttpCloseHandle(HANDLE(originalHandleValue));
	// Failure should be expected
	EXPECT_EQ(FALSE, rc);
	// Error code should be ERROR_INVALID_HANDLE
	auto lerr = GetLastError();
	EXPECT_TRUE((ERROR_INVALID_HANDLE == lerr) || (lerr == ERROR_INVALID_STATE));
}
