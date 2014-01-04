#pragma	once
/*

Copyright (c) 2003 Bob Koon

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*/


#if defined(_BK_IOS_)
	#include	<sys/sysctl.h>
	#include	<unistd.h>
	#include	"TargetConditionals.h"
#elif defined(_BK_MAC_)
	#include	<sys/sysctl.h>
	#include	<unistd.h>
	#include	<assert.h>
#elif defined(_BK_WIN_)
	#if	defined(_BK_DEBUG_)
		#define		_Pragma(args)
	#endif
#else
	#include	<signal.h>
	#if	defined(_BK_DEBUG_)
		#define		_Pragma(args)
	#endif
#endif

#include	<stdio.h>
#include	"BKTypes.h"


#if	defined(_BK_DEBUG_)
	/* check an expression for "truth" */
	#define	BKDEBUG_CHECK(expression, ...) \
			{ \
				if	((expression) == false) \
				{ \
					BKDEBUG_MESSAGE("%s\n", "Check failed, should be non-zero: " __VA_ARGS__); \
					BKDEBUG_MESSAGE("    %s\n", #expression); \
					BKDEBUG_MESSAGE("    %s\n", __FILE__); \
					BKDEBUG_MESSAGE("    Line %d\n", __LINE__); \
					_BKDEBUG_CHECK_DEBUGBREAK(); \
				} \
			}

	/* check two expressions for equality */
	#define	BKDEBUG_CHECK_EQUAL(left_expression, right_expression, ...) \
			{ \
				if	((left_expression) != (right_expression)) \
				{ \
					BKDEBUG_MESSAGE("%s\n", "Check failed, should be = : " __VA_ARGS__); \
					BKDEBUG_MESSAGE("    %s != %s\n", #left_expression, #right_expression); \
					_Pragma("clang diagnostic push"); \
					_Pragma("clang diagnostic ignored \"-Wint-to-void-pointer-cast\""); \
					BKDEBUG_MESSAGE("    %p != %p\n", (_BKDEBUG_PERCENTP_TYPE)(left_expression), (_BKDEBUG_PERCENTP_TYPE)(right_expression)); \
					_Pragma("clang diagnostic pop"); \
					BKDEBUG_MESSAGE("    %s\n", __FILE__); \
					BKDEBUG_MESSAGE("    Line %d\n", __LINE__); \
					_BKDEBUG_CHECK_DEBUGBREAK(); \
				} \
			}

	/* check two expressions for inequality */
	#define	BKDEBUG_CHECK_NOTEQUAL(left_expression, right_expression, ...) \
			{ \
				if	((left_expression) == (right_expression)) \
				{ \
					BKDEBUG_MESSAGE("%s\n", "Check failed, should be != : " __VA_ARGS__); \
					BKDEBUG_MESSAGE("    %s == %s\n", #left_expression, #right_expression); \
					_Pragma("clang diagnostic push"); \
					_Pragma("clang diagnostic ignored \"-Wint-to-void-pointer-cast\""); \
					BKDEBUG_MESSAGE("    %p == %p\n", (_BKDEBUG_PERCENTP_TYPE)(left_expression), (_BKDEBUG_PERCENTP_TYPE)(right_expression)); \
					_Pragma("clang diagnostic pop"); \
					BKDEBUG_MESSAGE("    %s\n", __FILE__); \
					BKDEBUG_MESSAGE("    Line %d\n", __LINE__); \
					_BKDEBUG_CHECK_DEBUGBREAK(); \
				} \
			}

	/* check if the left expression is less than the right expression */
	#define	BKDEBUG_CHECK_LESSTHAN(left_expression, right_expression, ...) \
			{ \
				if	((left_expression) >= (right_expression)) \
				{ \
					BKDEBUG_MESSAGE("%s\n", "Check failed, should be < : " __VA_ARGS__); \
					BKDEBUG_MESSAGE("    %s >= %s\n", #left_expression, #right_expression); \
					_Pragma("clang diagnostic push"); \
					_Pragma("clang diagnostic ignored \"-Wint-to-void-pointer-cast\""); \
					BKDEBUG_MESSAGE("    %p >= %p\n", (_BKDEBUG_PERCENTP_TYPE)(left_expression), (_BKDEBUG_PERCENTP_TYPE)(right_expression)); \
					_Pragma("clang diagnostic pop"); \
					BKDEBUG_MESSAGE("    %s\n", __FILE__); \
					BKDEBUG_MESSAGE("    Line %d\n", __LINE__); \
					_BKDEBUG_CHECK_DEBUGBREAK(); \
				} \
			}

	/* check if the left expression is greater than the right expression */
	#define	BKDEBUG_CHECK_GREATERTHAN(left_expression, right_expression, ...) \
			{ \
				if	((left_expression) <= (right_expression)) \
				{ \
					BKDEBUG_MESSAGE("%s\n", "Check failed, should be > : " __VA_ARGS__); \
					BKDEBUG_MESSAGE("    %s <= %s\n", #left_expression, #right_expression); \
					_Pragma("clang diagnostic push"); \
					_Pragma("clang diagnostic ignored \"-Wint-to-void-pointer-cast\""); \
					BKDEBUG_MESSAGE("    %p <= %p\n", (_BKDEBUG_PERCENTP_TYPE)(left_expression), (_BKDEBUG_PERCENTP_TYPE)(right_expression)); \
					_Pragma("clang diagnostic pop"); \
					BKDEBUG_MESSAGE("    %s\n", __FILE__); \
					BKDEBUG_MESSAGE("    Line %d\n", __LINE__); \
					_BKDEBUG_CHECK_DEBUGBREAK(); \
				} \
			}

	/* check if the left expression is less than or equal to the right expression */
	#define	BKDEBUG_CHECK_LESSTHANEQUAL(left_expression, right_expression, ...) \
			{ \
				if	((left_expression) > (right_expression)) \
				{ \
					BKDEBUG_MESSAGE("%s\n", "Check failed, should be <= : " __VA_ARGS__); \
					BKDEBUG_MESSAGE("    %s > %s\n", #left_expression, #right_expression); \
					_Pragma("clang diagnostic push"); \
					_Pragma("clang diagnostic ignored \"-Wint-to-void-pointer-cast\""); \
					BKDEBUG_MESSAGE("    %p > %p\n", (_BKDEBUG_PERCENTP_TYPE)(left_expression), (_BKDEBUG_PERCENTP_TYPE)(right_expression)); \
					_Pragma("clang diagnostic pop"); \
					BKDEBUG_MESSAGE("    %s\n", __FILE__); \
					BKDEBUG_MESSAGE("    Line %d\n", __LINE__); \
					_BKDEBUG_CHECK_DEBUGBREAK(); \
				} \
			}

	/* check if the left expression is greater than or equal to the right expression */
	#define	BKDEBUG_CHECK_GREATERTHANEQUAL(left_expression, right_expression, ...) \
			{ \
				if	((left_expression) < (right_expression)) \
				{ \
					BKDEBUG_MESSAGE("%s\n", "Check failed, should be >= : " __VA_ARGS__); \
					BKDEBUG_MESSAGE("    %s < %s\n", #left_expression, #right_expression); \
					_Pragma("clang diagnostic push"); \
					_Pragma("clang diagnostic ignored \"-Wint-to-void-pointer-cast\""); \
					BKDEBUG_MESSAGE("    %p < %p\n", (_BKDEBUG_PERCENTP_TYPE)(left_expression), (_BKDEBUG_PERCENTP_TYPE)(right_expression)); \
					_Pragma("clang diagnostic pop"); \
					BKDEBUG_MESSAGE("    %s\n", __FILE__); \
					BKDEBUG_MESSAGE("    Line %d\n", __LINE__); \
					_BKDEBUG_CHECK_DEBUGBREAK(); \
				} \
			}

	/* check if the left expression is within a certain value range of the right expression */
	#define	BKDEBUG_CHECK_WITHINRANGE(left_expression, right_expression, tolerance, ...) \
			{ \
				if	( \
						((left_expression) < ((right_expression) - (tolerance))) || \
						((left_expression) > ((right_expression) + (tolerance))) \
					) \
				{ \
					BKDEBUG_MESSAGE("%s\n", "Check failed, outside of tolerance : " __VA_ARGS__); \
					BKDEBUG_MESSAGE("    %s != %s +- %s\n", #left_expression, #right_expression, #tolerance); \
					_Pragma("clang diagnostic push"); \
					_Pragma("clang diagnostic ignored \"-Wint-to-void-pointer-cast\""); \
					BKDEBUG_MESSAGE("    %p != %p +- %p\n", (_BKDEBUG_PERCENTP_TYPE)(left_expression), (_BKDEBUG_PERCENTP_TYPE)(right_expression), (_BKDEBUG_PERCENTP_TYPE)(tolerance)); \
					_Pragma("clang diagnostic pop"); \
					BKDEBUG_MESSAGE("    %s\n", __FILE__); \
					BKDEBUG_MESSAGE("    Line %d\n", __LINE__); \
					_BKDEBUG_CHECK_DEBUGBREAK(); \
				} \
			}

	/* check if the left expression is within a certain value range of the right expression; evaluated as floats */
	#define	BKDEBUG_CHECK_WITHINRANGE_FLOAT(left_expression, right_expression, tolerance, ...) \
			{ \
				if	( \
						((left_expression) < ((right_expression) - (tolerance))) || \
						((left_expression) > ((right_expression) + (tolerance))) \
					) \
				{ \
					BKDEBUG_MESSAGE("%s\n", "Check failed, outside of tolerance : " __VA_ARGS__); \
					BKDEBUG_MESSAGE("    %s != %s +- %s\n", #left_expression, #right_expression, #tolerance); \
					BKDEBUG_MESSAGE("    %f != %f +- %f\n", (float)(left_expression), (float)(right_expression), (float)(tolerance)); \
					BKDEBUG_MESSAGE("    %s\n", __FILE__); \
					BKDEBUG_MESSAGE("    Line %d\n", __LINE__); \
					_BKDEBUG_CHECK_DEBUGBREAK(); \
				} \
			}

	/* display a printf()-style message to the debugging console */
	#define	BKDEBUG_MESSAGE			printf

	/* halt execution and interrupt the debugger as if a breakpoint was triggered */
	#define	BKDEBUG_BREAKPOINT()	_BKDEBUG_DEBUGBREAK

	/* execute one code statement in the debug build only */
	/* this can be abused, so be careful */
	#define	BKDEBUG_CODE(code)		code

	/* check an array at compile time for validity */
	#define	BKDEBUG_COMPILETIME_ARRAY_CHECK(condition, message)		typedef char static_assertion_##message[(condition) ? 1 : -1]

	/*----------------------------------------------------------------------------*/
	/* support macros; do NOT use them directly */
	#if defined(_BK_IOS_)
		#define	_BKDEBUG_PERCENTP_TYPE	void *

		#if TARGET_CPU_ARM
			#define	AmIBeingDebugged(flag_var) \
			{ \
				int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() }; \
				struct kinfo_proc info = { 0 }; \
				size_t size = sizeof(info); \
				sysctl (mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0); \
				flag_var = ((info.kp_proc.p_flag & P_TRACED) != 0); \
			}
			#define _BKDEBUGSTOP(signal)	__asm__ __volatile__ ("mov r0, %0\nmov r1, %1\nmov r12, #37\nswi 128\n" : : "r" (getpid ()), "r" (signal) : "r12", "r0", "r1", "cc")

			#define _BKDEBUG_DEBUGBREAK		\
						do \
						{ \
							bool is_being_debugged; \
							AmIBeingDebugged(is_being_debugged); \
							if (is_being_debugged == true) \
							{ \
								int trapSignal = SIGSTOP; \
								if (trapSignal) \
								{ \
									trapSignal = SIGINT; \
								} \
								_BKDEBUGSTOP(trapSignal); \
								if (trapSignal == SIGSTOP) \
								{ \
									_BKDEBUGSTOP(SIGINT); \
								} \
							} \
						} while (false)
		#else
			#define	AmIBeingDebugged(flag_var) \
			{ \
				int                 junk; \
				int                 mib[4]; \
				struct kinfo_proc   info; \
				size_t              size; \
				info.kp_proc.p_flag = 0; \
				mib[0] = CTL_KERN; \
				mib[1] = KERN_PROC; \
				mib[2] = KERN_PROC_PID; \
				mib[3] = getpid(); \
				size = sizeof(info); \
				junk = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0); \
				assert(junk == 0); \
				flag_var = ((info.kp_proc.p_flag & P_TRACED) != 0); \
			}
			#define	_BKDEBUGSTOP()		__asm__("int $3\n" : : )

			#define _BKDEBUG_DEBUGBREAK	do { bool is_being_debugged; AmIBeingDebugged(is_being_debugged); if (is_being_debugged == true) _BKDEBUGSTOP(); } while (false)
		#endif
	#elif defined(_BK_MAC_)
		#define	_BKDEBUG_PERCENTP_TYPE	void *

		#define	AmIBeingDebugged(flag_var) \
		{ \
			int                 junk; \
			int                 mib[4]; \
			struct kinfo_proc   info; \
			size_t              size; \
			info.kp_proc.p_flag = 0; \
			mib[0] = CTL_KERN; \
			mib[1] = KERN_PROC; \
			mib[2] = KERN_PROC_PID; \
			mib[3] = getpid(); \
			size = sizeof(info); \
			junk = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0); \
			assert(junk == 0); \
			flag_var = ((info.kp_proc.p_flag & P_TRACED) != 0); \
		}
		#define	_BKDEBUGSTOP()			__asm__("int $3\n" : : )

		#define _BKDEBUG_DEBUGBREAK		do { bool is_being_debugged; AmIBeingDebugged(is_being_debugged); if (is_being_debugged == true) _BKDEBUGSTOP(); } while (false)
	#elif defined(_BK_WIN_)
		#define	_BKDEBUG_PERCENTP_TYPE	size_t

		#define	_BKDEBUG_DEBUGBREAK		__debugbreak()
	#else
		#define	_BKDEBUG_PERCENTP_TYPE	void *

		#define	_BKDEBUG_DEBUGBREAK		raise(SIGINT)
	#endif

	#if defined(_BK_DISABLE_CHECK_BREAKPOINTS_)
		#define	_BKDEBUG_CHECK_DEBUGBREAK()
	#else
		#define	_BKDEBUG_CHECK_DEBUGBREAK()	{ _BKDEBUG_DEBUGBREAK; }
	#endif
#else
	#define	BKDEBUG_CHECK(expression, ...)
	#define	BKDEBUG_CHECK_EQUAL(left_expression, right_expression, ...)
	#define	BKDEBUG_CHECK_NOTEQUAL(left_expression, right_expression, ...)
	#define	BKDEBUG_CHECK_LESSTHAN(left_expression, right_expression, ...)
	#define	BKDEBUG_CHECK_GREATERTHAN(left_expression, right_expression, ...)
	#define	BKDEBUG_CHECK_LESSTHANEQUAL(left_expression, right_expression, ...)
	#define	BKDEBUG_CHECK_GREATERTHANEQUAL(left_expression, right_expression, ...)
	#define	BKDEBUG_CHECK_WITHINRANGE(left_expression, right_expression, tolerance, ...)
	#define	BKDEBUG_CHECK_WITHINRANGE_FLOAT(left_expression, right_expression, tolerance, ...)
	#define	BKDEBUG_MESSAGE(format, ...)
	#define	BKDEBUG_BREAKPOINT()
	#define	BKDEBUG_CODE(code)
	#define	BKDEBUG_COMPILETIME_ARRAY_CHECK(condition, message)
#endif
