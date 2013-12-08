/*
* Copyright (C) 2013- yeyouqun@163.com
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, visit the http://fsf.org website.
*/
#ifndef __DIGEST_H__
#define __DIGEST_H__

/// \file
/// \brief
/// ���ļ��ṩ������ MD4 Hash �㷨�ӿڡ�

namespace xdelta {

/// \def MD4_BLOCK_LENGTH
/// MD4 ժҪ����鳤��
#define	MD4_BLOCK_LENGTH		64
/// \def MD4_DIGEST_LENGTH
/// MD4 ժҪ����
#define	MD4_DIGEST_LENGTH		16
/// \def MD4_DIGEST_LENGTH
/// MD4 ժҪ����
#define DIGEST_BYTES			MD4_DIGEST_LENGTH


typedef struct rs_mdfour {
    int                 A, B, C, D;
#if HAVE_UINT64
    uint64_t            totalN;
#else
    uint32_t            totalN_hi, totalN_lo;
#endif
    int                 tail_len;
    uchar_t       tail[64];
}rs_mdfour_t;

/// \fn void DLL_EXPORT rs_mdfour(uchar_t *out, void const *in, size_t);
/// \brief �������ݿ�� MD ֵ��
/// \param[in] out MD4 �������
/// \param[in] in  �������ݿ�ָ�롣
/// \param[in] bytes  ���ݿ鳤�ȡ�
/// \return �޷���
void DLL_EXPORT rs_mdfour(uchar_t *out, void const *in, size_t bytes);

/// \fn void DLL_EXPORT rs_mdfour_begin(rs_mdfour_t * md);
/// \brief ��ʼ�� MD4 �����ġ�
/// \param[out] md MD4 ���������ļ�
/// \return �޷���
void DLL_EXPORT rs_mdfour_begin(rs_mdfour_t * md);
/// \fn void void DLL_EXPORT rs_mdfour_update(rs_mdfour_t * md, void const * buf,size_t n);
/// \brief �����ݿ��е�����д�뵽 MD �������С�
/// \param[in] md   MD4 ���������ļ���
/// \param[in] buf	���ݿ�ָ�롣
/// \param[in] n	���ݿ鳤�ȡ�
/// \return �޷���
void DLL_EXPORT rs_mdfour_update(rs_mdfour_t * md, void const * buf,size_t n);
/// \fn rs_mdfour_result(rs_mdfour_t * md, uchar_t *out);
/// \brief ��� MD44 �����ļ��е� MD4 ֵ��
/// \param[in] md   MD4 ���������ġ�
/// \param[out] out  ������塣
/// \return �޷���
void DLL_EXPORT rs_mdfour_result(rs_mdfour_t * md, uchar_t *out);

/// \fn void get_slow_hash (const uchar_t *buf1, uint32_t len, uchar_t hash[DIGEST_BYTES])
/// \brief �������ݿ���� Hash ֵ��
/// \param[in] buf1 ���ݿ�ָ�롣
/// \param[in] len  �鳤�ȡ�
/// \param[out] hash  Hash �����������
/// \return �޷���
inline void get_slow_hash (const uchar_t *buf1, uint32_t len, uchar_t hash[DIGEST_BYTES])
{
	rs_mdfour (hash, buf1, len);
}

} // namespace xdelta

#endif //__DIGEST_H__

