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

#ifndef __PLATFORM_H__
#define __PLATFORM_H__
/// @file
/// ���ͼ�ƽ̨�����ļ�

namespace xdelta {
#ifdef _WIN32
	#define NAMESPACE_STD_BEGIN _STD_BEGIN
	#define NAMESPACE_STD_END	_STD_END
	#define hash_map			_STDEXT hash_map
	#ifdef XDELTALIB_EXPORTS
	#define DLL_EXPORT				__declspec(dllexport)
	#else
	#define DLL_EXPORT				__declspec(dllimport)
	#endif
#else
	#define NAMESPACE_STD_BEGIN namespace std {
    #define NAMESPACE_STD_END	}
    #if !defined (__CXX_11__)
    	#define hash_map			__gnu_cxx::hash_map
    #else
    	#define hash_map			std::unordered_map
    #endif
	#define O_BINARY            0
	#define DLL_EXPORT
	#define FILE_BEGIN SEEK_SET
	#define FILE_CURRENT SEEK_CUR 
	#define FILE_END SEEK_END 
#endif


/// \fn std::string DLL_EXPORT fmt_string (const char * fmt, ...);
/// \brief ��ʽ��������Ϣ��
/// \param[in] fmt   ��ʽ�ִ�
/// \return ���� string ��ʾ�Ĵ�����Ϣ��
std::string DLL_EXPORT fmt_string (const char * fmt, ...);
/// \fn std::string DLL_EXPORT error_msg ();
/// \brief ���ص�ǰ������ִ���ʾ��
/// \return ���ص�ǰ������ִ���ʾ��
std::string DLL_EXPORT error_msg ();

/// \fn std::string get_tmp_fname (const std::string & fname);
/// \brief ��������ļ���ȡ����ʱ�ļ�����
/// \param[in] fname   ������ļ�����
/// \return ��ʱ�ļ�����
std::string get_tmp_fname (const std::string & fname);
} // namespace xdelta
#endif //__PLATFORM_H__

