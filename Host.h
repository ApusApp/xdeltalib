/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Host.h - Basic header file to provide cross-platform solutions via        */
/*                   macros, conditional compilation, etc.                   */
/*                                                                           */
/* Author : Mark Carrier (mark@carrierlabs.com)                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Copyright (c) 2007 CarrierLabs, LLC.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * 4. The name "CarrierLabs" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    mark@carrierlabs.com.
 *
 * THIS SOFTWARE IS PROVIDED BY MARK CARRIER ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL MARK CARRIER OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *----------------------------------------------------------------------------*/
#ifndef __HOST_H__
#define __HOST_H__

#include <limits.h>
/// @file
namespace xdelta {

#ifndef htonll
#ifdef _BIG_ENDIAN
#define htonll(x)   (x)
#define ntohll(x)   (x)
#else
#define htonll(x)   ((((uint64)htonl(x)) << 32) + htonl(x >> 32))
#define ntohll(x)   ((((uint64)ntohl(x)) << 32) + ntohl(x >> 32))
#endif
#endif

/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Socket Macros                                                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#ifdef _WIN32
	#define SHUT_RD                0
	#define SHUT_WR                1
	#define SHUT_RDWR              2
	#define ACCEPT(a,b,c)          accept(a,b,c)
	#define CONNECT(a,b,c)         connect(a,b,c)
	#define CLOSE(a)               closesocket(a)
	#define READ(a,b,c)            read(a,b,c)
	#define RECV(a,b,c,d)          recv(a, (char *)b, c, d)
	#define RECVFROM(a,b,c,d,e,f)  recvfrom(a, (char *)b, c, d, (sockaddr *)e, (int *)f)
	#define RECV_FLAGS             MSG_WAITALL
	#define SELECT(a,b,c,d,e)      select((int32_t)a,b,c,d,e)
	#define SEND(a,b,c,d)          send(a, (const char *)b, (int)c, d)
	#define SENDTO(a,b,c,d,e,f)    sendto(a, (const char *)b, (int)c, d, e, f)
	#define SEND_FLAGS             0
	#define SENDFILE(a,b,c,d)      sendfile(a, b, c, d)
	#define SET_SOCKET_ERROR(x,y)  errno=y
	#define SOCKET_ERROR_INTERUPT  EINTR
	#define SOCKET_ERROR_TIMEDOUT  EAGAIN
	#define WRITE(a,b,c)           write(a,b,c)
	#define WRITEV(a,b,c)          Writev(b, c)
	#define GETSOCKOPT(a,b,c,d,e)  getsockopt(a,b,c,(char *)d, (int *)e)
	#define SETSOCKOPT(a,b,c,d,e)  setsockopt(a,b,c,(char *)d, (int)e)
	#define GETHOSTBYNAME(a)       gethostbyname((const char *)a)
#elif defined (_LINUX)
	#define ACCEPT(a,b,c)          accept(a,b,c)
	#define CONNECT(a,b,c)         connect(a,b,c)
	#define CLOSE(a)               close(a)
	#define READ(a,b,c)            read(a,b,c)
	#define RECV(a,b,c,d)          recv(a, (void *)b, c, d)
	#define RECVFROM(a,b,c,d,e,f)  recvfrom(a, (char *)b, c, d, (sockaddr *)e, f)
	#define RECV_FLAGS             MSG_WAITALL
	#define SELECT(a,b,c,d,e)      select(a,b,c,d,e)
	#define SEND(a,b,c,d)          send(a, (const char_t *)b, c, d)
	#define SENDTO(a,b,c,d,e,f)    sendto(a, (const char_t *)b, c, d, e, f)
	#define SEND_FLAGS             0
	#define SENDFILE(a,b,c,d)      sendfile(a, b, c, d)
	#define SET_SOCKET_ERROR(x,y)  errno=y
	#define SOCKET_ERROR_INTERUPT  EINTR
	#define SOCKET_ERROR_TIMEDOUT  EAGAIN
	#define WRITE(a,b,c)           write(a,b,c)
	#define WRITEV(a,b,c)          writev(a, b, c)
	#define GETSOCKOPT(a,b,c,d,e)  getsockopt((int)a,(int)b,(int)c,(void *)d,(socklen_t *)e)
	#define SETSOCKOPT(a,b,c,d,e)  setsockopt((int)a,(int)b,(int)c,(const void *)d,(int)e)
	#define GETHOSTBYNAME(a)       gethostbyname((const char *)a)
#endif


/*---------------------------------------------------------------------------*/
/*                                                                           */
/* File Macros                                                               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#define STRUCT_STAT         struct stat
#define LSTAT(x,y)          lstat(x,y)
#define FILE_HANDLE         FILE *
#define CLEARERR(x)         clearerr(x)
#define FCLOSE(x)           fclose(x)
#define FEOF(x)             feof(x)
#define FERROR(x)           ferror(x)
#define FFLUSH(x)           fflush(x)
#define FILENO(s)           fileno(s)
#define FOPEN(x,y)          fopen(x, y)
#define FREAD(a,b,c,d)      fread(a, b, c, d)
#define FSTAT(s, st)        fstat(FILENO(s), st)
#define FWRITE(a,b,c,d)     fwrite(a, b, c, d)
#define STAT_BLK_SIZE(x)    ((x).st_blksize)


/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Misc Macros                                                               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#if defined(_WIN32)
  #define GET_CLOCK_COUNT(x) QueryPerformanceCounter((LARGE_INTEGER *)x)
#else
  #define GET_CLOCK_COUNT(x) gettimeofday(x, NULL)
#endif

#if defined(_WIN32)
  #define STRTOULL(x) _atoi64(x)
#else
  #define STRTOULL(x) strtoull(x, NULL, 10)
#endif

#if defined(_WIN32)
  #define SNPRINTF _snprintf
  #define PRINTF   printf
  #define VPRINTF  vprintf
  #define FPRINTF  fprintf
#else 
  #define SNPRINTF snprintf
  #define PRINTF   printf
  #define VPRINTF  vprintf
  #define FPRINTF  fprintf
#endif

} // namespace xdelta

#endif /* __HOST_H__ */

