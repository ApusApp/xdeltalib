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
#ifndef __XDELTA_SERVER_H__
#define __XDELTA_SERVER_H__
/// @file
/// �����������˴�����ࡣ
namespace xdelta {
/// \def XDELTA_DEFAULT_PORT
/// Ĭ�Ϸ�����ͬ���˿ڣ��û�����ָ�������˿ڡ�
#define XDELTA_DEFAULT_PORT 1366
/// \def XDELTA_ADDR
/// ���ط����ַ
#define XDELTA_ADDR "127.0.0.1"
/// \def NO_MULTI_ROUND_FILSIZE
/// Ĭ�϶��� Hash ���ļ��ߴ��С��һ�㲻�����ļ����������С�����ָ���������ʱ�������ļ���ִֻ�е��� Hash��
#define NO_MULTI_ROUND_FILSIZE ((uint64_t)(-1))

/// \class
/// \brief
/// �������ڹ۲�ͬ��ִ�����̣�ʹ������Ҫ�Լ�������Ӧ�Ĺ۲����࣬��ʵ���Լ���ͳ�ƻ���������Ӧ�Ĳ�����
class xdelta_observer
{
public:
	xdelta_observer () {}
	virtual ~xdelta_observer () {}
	/// \brief
	/// ָʾ��ʼ�����ļ��� Hash ��
	/// \param[in] fname	�ļ����������·��
	/// \param[in] blk_len	�����ļ��Ŀ鳤��
	/// \return û�з���
	virtual void start_hash_stream (const std::string & fname, const int32_t blk_len) = 0;
	/// \brief
	/// ָʾ����һ���ļ��� Hash ���Ĵ���
	/// \param[in] filsize		Դ�ļ��Ĵ�С��
	/// \return û�з���
	virtual void end_hash_stream (const uint64_t filesize) = 0;
	/// \brief
	/// ָʾ�������� Hash �е�һ�֣�����Ӧ����൱�ڵ��� Hash �е� end_hash_stream��
	/// \param[in] file_hash		�����ļ��� MD4 Hash ֵ��
	/// \return ���Դ�ļ����ж���Ҫ������һ�֣��򷵻��棬���򷵻ؼ١�
	virtual void end_first_round () = 0;
	/// \brief
	/// ָʾ��һ�� Hash ����ʼ��
	/// \param[in] blk_len		��һ�� Hash �Ŀ鳤�ȡ�
	/// \return û�з���
	virtual void next_round (const int32_t blk_len) = 0;
	/// \brief
	/// ָʾ����һ�� Hash��ֻ�ڶ��� Hash �е���
	/// \return û�з���
	virtual void end_one_round () = 0;
	/// \brief
	/// ָʾ��������еĴ���
	/// \param[in] errmsg		������Ϣ��
	/// \param[in] errorno		�����롣
	/// \return û�з���
	virtual void on_error (const std::string & errmsg, const int errorno) = 0;
	/// \brief
	/// ָʾ���͵��ֽ���������������������ͳ�ơ�
	/// \param[in] bytes		�ֽ�����
	/// \return û�з���
	virtual void bytes_send (const uint32_t bytes) = 0;
	/// \brief
	/// ָʾ���յ��ֽ���������������������ͳ�ơ�
	/// \param[in] bytes		�ֽ�����
	/// \return û�з���
	virtual void bytes_recv (const uint32_t bytes) = 0;
	/// \brief
	/// ָʾ�����һ����ͬ�Ŀ�
	/// \param[in] blk_len	�鳤�ȡ�
	/// \param[in] s_offset	��ͬ����Դ�ļ��е�λ��ƫ�ơ�
	/// \return û�з���
	virtual void on_equal_block(const uint32_t blk_len
								, const uint64_t s_offset) = 0;
	/// \brief
	/// ָʾ�����һ�������
	/// \param[in] blk_len	���ݿ鳤�ȡ�
	/// \return û�з���
	virtual void on_diff_block (const uint32_t blk_len) = 0;
};


/// \class
/// \brief
/// ����������Ϊ����˽���ͬ�����ݣ���ִ�� run ��һ����������Ż᷵�ء�
class DLL_EXPORT xdelta_server
{
	CPassiveSocket server_;					///<	������ Socket ����
	uint64_t		auto_multiround_filsize_;///<	���� Hash �ı߽�ֵ��
	bool			inplace_;				///<	���þ͵ع����ļ��ķ�ʽͬ�������� Hash ��͵ع��첻��ͬʱ���֡�
	bool			compress_;				///< �����ݴ����Ƿ���Ҫѹ����
	void _start_task (file_operator & foperator
					, xdelta_observer & observer
					, uint16_t port);
public:
	//
	// @auto_multiround_filsize if file's size excess this size
	// will used multiround xdelta.
	//
	xdelta_server (bool compress) 
		: server_ (compress)
		, auto_multiround_filsize_ (NO_MULTI_ROUND_FILSIZE)
		, inplace_ (false)
		, compress_ (compress)
	{
		// less than this size will cause no difference with single round.
		if (MULTIROUND_MAX_BLOCK_SIZE > auto_multiround_filsize_)
			auto_multiround_filsize_ = NO_MULTI_ROUND_FILSIZE;
	}
	/// \brief
	/// ָʾͬ��������ļ�����ͨ���͵صķ�ʽ���С�
	/// \return û�з���
	void set_inplace ();
	/// \brief
	/// ������Ҫ���ж��� Hash ͬ�����ļ���С�߽硣���û�����ã���Ĭ�ϲ����ж��� Hash��
	/// \param[in]	multi_round_size ִ�ж��� Hash ͬ���ı߽��С��
	/// \return û�з���
	void set_multiround_size (uint64_t multi_round_size);
	~xdelta_server () {}

	/// \brief
	/// ��ʼִ��ͬ����ͬ����ɺ󣬱��ӿڲŻ᷵�ء�
	/// \param[in]	foperator �ļ���������
	/// \param[in]	observer �۲��߶���
	/// \param[in]	port	�������˿ڡ�
	/// \return û�з���
	void run (file_operator & foperator
				, xdelta_observer & observer
				, uint16_t port = XDELTA_DEFAULT_PORT);
};

inline void wait_to_exit (std::vector<thread*> & thrds)
{
	std::for_each (thrds.begin (),thrds.end (), std::mem_fun (&thread::join));
	std::for_each (thrds.begin (), thrds.end (), delete_obj<thread>);
	thrds.clear ();
}

void init_passive_socket (CPassiveSocket & passive, uint16_t port);
void init_active_socket (CActiveSocket & active, const uchar_t * addr, uint16_t port);

inline void read_block (char_buffer<uchar_t> & buff
						, CSimpleSocket & client
						, int32_t len
						, xdelta_observer & observer)
{
	if (len == 0)
		return;

	int32_t nbytes = client.Receive (buff, len);
	if (nbytes <= 0) {
		std::string errmsg = fmt_string ("Socket is broken or closed!");	
		THROW_XDELTA_EXCEPTION (errmsg);
	}
	observer.bytes_recv (len);
}

inline block_header read_block_header (CSimpleSocket & client, xdelta_observer & observer)
{
	DEFINE_STACK_BUFFER (buff);
	read_block (buff, client, BLOCK_HEAD_LEN, observer);
	block_header header;
	buff >> header;
	return header;
}

inline void send_block (CSimpleSocket & socket, char_buffer<uchar_t> & data, xdelta_observer & observer)
{
	int32_t tbytes = data.data_bytes ();
	if (tbytes > 0) {
		if (!socket.Send (data.rd_ptr (), tbytes)) {
			std::string errmsg = fmt_string ("Send data to client failed.");
			THROW_XDELTA_EXCEPTION (errmsg);
		}
		observer.bytes_send (tbytes);
	}
	data.reset ();
}

void buffer_or_send (CSimpleSocket & socket
							, char_buffer<uchar_t> & stream_buff
							, char_buffer<uchar_t> & buff
							, xdelta_observer & observer
							, bool now = false);
} //namespace xdelta

#endif //__XDELTA_SERVER_H__

