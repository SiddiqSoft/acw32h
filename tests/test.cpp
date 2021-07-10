#include "gtest/gtest.h"

#include <windows.h>
#include "../src/acw32h.hpp"


TEST(basic, TestAutoClose)
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
		//ACW32HANDLE ach(eh);

		// We share ownership with the other handle
		ACW32HANDLE ach(std::move(eh));

		// The original handle is cleared
		EXPECT_EQ(INVALID_HANDLE_VALUE, eh);
		// The originalHandleValue and the ach must match
		EXPECT_EQ(HANDLE(originalHandleValue), HANDLE(ach));
	} // the object will be closed upon destructor
	catch (const std::exception& e)
	{
		EXPECT_TRUE(FALSE) << e.what();
	}

	// The handle must be closed
	// If the CloseHandle is called twice, we throw.
	auto rc = CloseHandle(HANDLE(originalHandleValue));
	// Failure should be expected
	EXPECT_EQ(FALSE, rc);
	// Error code should be ERROR_INVALID_HANDLE
	EXPECT_EQ(ERROR_INVALID_HANDLE, GetLastError());
}