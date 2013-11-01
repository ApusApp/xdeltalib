/*---------------------------------------------------------------------------*/
/*                                                                           */
/* PassiveSocket.cpp - Passive Socket Implementation		 			     */
/*                                                                           */
/* Author : Mark Carrier (mark@carrierlabs.com)                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Copyright (c) 2007-2009 CarrierLabs, LLC.  All rights reserved.
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

#include <stdlib.h>
#include <string.h>
#include <string>
#include "mytypes.h"
#include "platform.h"
#include "buffer.h"
#include "PassiveSocket.h"


namespace xdelta {

CPassiveSocket::CPassiveSocket(CSocketType nType) : CSimpleSocket(nType)
{
}

bool CPassiveSocket::BindMulticast(const uchar_t *pInterface, const uchar_t *pGroup, int16_t nPort)
{
    bool		   bRetVal = false;
#ifdef _WIN32
    ULONG          inAddr;
#else
    in_addr_t      inAddr;
#endif

    //--------------------------------------------------------------------------
    // Set the following socket option SO_REUSEADDR.  This will allow the file
    // descriptor to be reused immediately after the socket is closed instead
    // of setting in a TIMED_WAIT state.
    //--------------------------------------------------------------------------
    memset(&m_stMulticastGroup,0,sizeof(m_stMulticastGroup));
    m_stMulticastGroup.sin_family = AF_INET;
    m_stMulticastGroup.sin_port = htons(nPort);
    
    //--------------------------------------------------------------------------
    // If no IP Address (interface ethn) is supplied, or the loop back is 
    // specified then bind to any interface, else bind to specified interface.
    //--------------------------------------------------------------------------
    if ((pInterface == NULL) || (!strlen((const char *)pInterface)))
    {
        m_stMulticastGroup.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        if ((inAddr = inet_addr((const char *)pInterface)) != INADDR_NONE)
        {
            m_stMulticastGroup.sin_addr.s_addr = inAddr;
        }
    }
    
    //----------------------------------------------------------------------
    // Specify the multicast group and bind the specified interface.
    //----------------------------------------------------------------------
    m_stMulticastRequest.imr_multiaddr.s_addr = inet_addr((const char *)pGroup);
    m_stMulticastRequest.imr_interface.s_addr = m_stMulticastGroup.sin_addr.s_addr;

    //--------------------------------------------------------------------------
    // Bind to the specified port 
    //--------------------------------------------------------------------------
    if (bind(m_socket, (struct sockaddr *)&m_stMulticastGroup, sizeof(m_stMulticastGroup)) == 0)
    {
        //----------------------------------------------------------------------
        // Join the multicast group
        //----------------------------------------------------------------------
        if (SETSOCKOPT(m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
                       (void *)&m_stMulticastRequest,
                       sizeof(m_stMulticastRequest)) == CSimpleSocket::SocketSuccess)
        {
            bRetVal = true;
        }
    }

    //--------------------------------------------------------------------------
    // If there was a socket error then close the socket to clean out the 
    // connection in the backlog.
    //--------------------------------------------------------------------------
    TranslateSocketError();

    if (bRetVal == false)
    {
        Close();
    }

    return bRetVal;
}

//------------------------------------------------------------------------------
//
// Listen() - 
//
//------------------------------------------------------------------------------
bool CPassiveSocket::Listen(const uchar_t *pAddr, int16_t nPort, int32_t nConnectionBacklog)
{
    bool		   bRetVal = false;
#ifdef _WIN32
    ULONG          inAddr;
#else
    int32_t          nReuse;
    in_addr_t      inAddr;
    nReuse = IPTOS_LOWDELAY;
#endif

    //--------------------------------------------------------------------------
    // Set the following socket option SO_REUSEADDR.  This will allow the file
    // descriptor to be reused immediately after the socket is closed instead
    // of setting in a TIMED_WAIT state.
    //--------------------------------------------------------------------------
#ifdef _LINUX
    SETSOCKOPT(m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&nReuse, sizeof(int32_t));
    SETSOCKOPT(m_socket, IPPROTO_TCP, IP_TOS, &nReuse, sizeof(int32_t));
#endif

    memset(&m_stServerSockaddr,0,sizeof(m_stServerSockaddr));
    m_stServerSockaddr.sin_family = AF_INET;
    m_stServerSockaddr.sin_port = htons(nPort);
    
    //--------------------------------------------------------------------------
    // If no IP Address (interface ethn) is supplied, or the loop back is 
    // specified then bind to any interface, else bind to specified interface.
    //--------------------------------------------------------------------------
    if ((pAddr == NULL) || (!strlen((const char *)pAddr)))
    {
        m_stServerSockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        if ((inAddr = inet_addr((const char *)pAddr)) != INADDR_NONE)
        {
            m_stServerSockaddr.sin_addr.s_addr = inAddr;
        }
    }
    
    //--------------------------------------------------------------------------
    // Bind to the specified port 
    //--------------------------------------------------------------------------
    if (bind(m_socket, (struct sockaddr *)&m_stServerSockaddr, sizeof(m_stServerSockaddr)) != CSimpleSocket::SocketError)
    {
        socklen_t nSockLen = sizeof(struct sockaddr);
        memset(&m_stServerSockaddr, 0, nSockLen);
        getsockname(m_socket, (struct sockaddr *)&m_stServerSockaddr, &nSockLen);

        if (m_nSocketType == CSimpleSocket::SocketTypeTcp)
        {
            if (listen(m_socket, nConnectionBacklog) != CSimpleSocket::SocketError)
            {
                bRetVal = true;
            }
        }  
        else      
        {
            bRetVal = true;
        }
    }

    //--------------------------------------------------------------------------
    // If there was a socket error then close the socket to clean out the 
    // connection in the backlog.
    //--------------------------------------------------------------------------
    TranslateSocketError();

    if (bRetVal == false)
    {
        Close();
    }

    return bRetVal;
}


//------------------------------------------------------------------------------
//
// Accept() - 
//
//------------------------------------------------------------------------------
CActiveSocket *CPassiveSocket::Accept(uint32_t timeout_sec)
{
    uint32_t         nSockLen;
    CActiveSocket *pClientSocket = NULL;
    SOCKET         socket = CSimpleSocket::SocketError;

    if (m_nSocketType != CSimpleSocket::SocketTypeTcp)
    {
        SetSocketError(CSimpleSocket::SocketProtocolError);
        return pClientSocket;
    }

    pClientSocket = new CActiveSocket();

    //--------------------------------------------------------------------------
    // Wait for incoming connection.
    //--------------------------------------------------------------------------
    if (pClientSocket != NULL)
    {
        CSocketError socketErrno = SocketSuccess;

        nSockLen = sizeof(m_stClientSockaddr);
        
        do
        {
            errno = 0;
			if (!Select (timeout_sec, 0)) {
				delete pClientSocket;
	            pClientSocket = NULL;
				return 0;
			}
			
			socket = accept(m_socket, (struct sockaddr *)&m_stClientSockaddr, (socklen_t *)&nSockLen);
            if (socket != -1)
            {
                pClientSocket->SetSocketHandle(socket);
                pClientSocket->TranslateSocketError();
                socketErrno = pClientSocket->GetSocketError();
                socklen_t nSockLen = sizeof(struct sockaddr);

                //-------------------------------------------------------------
                // Store client and server IP and port information for this
                // connection.
                //-------------------------------------------------------------
                getpeername(m_socket, (struct sockaddr *)&pClientSocket->m_stClientSockaddr, &nSockLen);
                memcpy((void *)&pClientSocket->m_stClientSockaddr, (void *)&m_stClientSockaddr, nSockLen);

                memset(&pClientSocket->m_stServerSockaddr, 0, nSockLen);
                getsockname(m_socket, (struct sockaddr *)&pClientSocket->m_stServerSockaddr, &nSockLen);
            }
            else
            {
                TranslateSocketError();
                socketErrno = GetSocketError();
            }

        } while (socketErrno == CSimpleSocket::SocketInterrupted);
        
        if (socketErrno != CSimpleSocket::SocketSuccess)
        {
            delete pClientSocket;
            pClientSocket = NULL;
        }
    }

    return pClientSocket;
}


//------------------------------------------------------------------------------
//
// Send() - Send data on a valid socket
//
//------------------------------------------------------------------------------
int32_t CPassiveSocket::Send(const uchar_t *pBuf, int32_t bytesToSend)
{
    SetSocketError(SocketSuccess);
    int32_t BytesSent = 0;

    switch(m_nSocketType)
    {
        case CSimpleSocket::SocketTypeUdp:
        {
			if (IsSocketValid() && (bytesToSend > 0) && (pBuf != NULL)) {
                BytesSent = SENDTO(m_socket, pBuf, bytesToSend, 0, 
                                      (const sockaddr *)&m_stClientSockaddr, 
                                      sizeof(m_stClientSockaddr));
                if (BytesSent == CSimpleSocket::SocketError)
                {
                    TranslateSocketError();
                }
            }
            break;
        }
        case CSimpleSocket::SocketTypeTcp:
           CSimpleSocket::Send(pBuf, bytesToSend);
           break;
       default:
           SetSocketError(SocketProtocolError);
           break;
    }

    return BytesSent;
}

} // namespace xdelta