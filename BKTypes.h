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


#if defined(_BK_DEBUG_)
	#if defined(_BK_RELEASE_)
		#error	You are making a _BK_DEBUG_ build but _BK_RELEASE_ is also defined.
	#endif
#elif defined(_BK_RELEASE_)
	#if defined(_BK_DEBUG_)
		#error	You are making a _BK_RELEASE_ build but _BK_DEBUG_ is also defined.
 	#endif
#endif


#if defined(_BK_IOS_)
	#if defined(_BK_MAC_)
		#error	You are making a _BK_IOS_ build but _BK_MAC_ is also defined.
	#elif defined(_BK_WIN_)
		#error	You are making a _BK_IOS_ build but _BK_WIN_ is also defined.
	#endif
#elif defined(_BK_MAC_)
	#if defined(_BK_IOS_)
		#error	You are making a _BK_MAC_ build but _BK_IOS_ is also defined.
	#elif defined(_BK_WIN_)
		#error	You are making a _BK_MAC_ build but _BK_WIN_ is also defined.
	#endif
#elif defined(_BK_WIN_)
	#if defined(_BK_IOS_)
		#error	You are making a _BK_WIN_ build but _BK_IOS_ is also defined.
	#elif defined(_BK_MAC_)
		#error	You are making a _BK_WIN_ build but _BK_MAC_ is also defined.
	#endif
#endif


#if defined(_BK_IOS_) || defined(_BK_MAC_)
	#include	<stdbool.h>
#endif


/* base types */
#if !defined(s64)
	typedef	signed long long	s64;
#endif
#if !defined(u64)
	typedef	unsigned long long	u64;
#endif
#if !defined(s32)
	typedef	signed int			s32;
#endif
#if !defined(u32)
	typedef	unsigned int		u32;
#endif
#if !defined(s16)
	typedef	signed short		s16;
#endif
#if !defined(u16)
	typedef	unsigned short		u16;
#endif
#if !defined(s8)
	typedef	signed char			s8;
#endif
#if !defined(u8)
	typedef	unsigned char		u8;
#endif

#define	staticarray_count(array)	(sizeof(array) / sizeof((array)[0]))

/* merged types to allow individual access to specific bits */
/* 32 bits */
typedef union
{
	u32		m_u32;
	s32		m_s32;

	void *	m_ptr;

	struct
	{
		u16	m_DC;
		u16	m_BA;
	} m_u16;

	struct
	{
		s16	m_DC;
		s16	m_BA;
	} m_s16;

	struct
	{
		u8	m_D;
		u8	m_C;
		u8	m_B;
		u8	m_A;
	} m_u8;

	struct
	{
		s8	m_D;
		s8	m_C;
		s8	m_B;
		s8	m_A;
	} m_s8;
} merged32;

/* 16 bits */
typedef union
{
	u16	m_u16;
	s16	m_s16;

	struct
	{
		u8	m_B;
		u8	m_A;
	} m_u8;

	struct
	{
		s8	m_B;
		s8	m_A;
	} m_s8;
} merged16;

/* 8 bits */
typedef union
{
	u8	m_u8;
	s8	m_s8;
} merged8;
