// author:yeyouqun@163.com
#ifndef __XDELTA_CLIENT_H__
#define __XDELTA_CLIENT_H__
/// @file
/// ��������ͻ��˴�����ࡣ
namespace xdelta {
struct client_slot_t;

/// \class
/// \brief �ͻ���ִ��ʱ���ࡣ

class DLL_EXPORT xdelta_client
{
	CActiveSocket			client_;		///< ���˶���
	client_slot_t		*	task_slot_;		///< �����
	uint32_t				thread_nr_;		///< ��ǰ�߳�����
	bool					compress_;		///< �����ݴ����Ƿ���Ҫѹ����
public:
	xdelta_client (bool compress, uint32_t thread_nr = 0);
	~xdelta_client ();
	/// \brief
	/// ��������ִ�С�
	/// �ͻ���ִ��˳��:run -> add_task -< �����ô��>--Y--->  wait
	///                    ^               |
	///                    \----<-----N----/   
	/// \param[in] foperator	�ļ���������
	/// \param[in] observer		���Ȳ鿴����
	/// \param[in] paddr		��������ַ������Ǳ��أ���Ϊ 127.0.0.1������ 0��
	/// \param[in] port			������ִ�ж˿ڡ�
	/// \return �޷��ء�
	void run (file_operator & foperator
				, xdelta_observer & observer
				, const uchar_t * paddr
				, uint16_t port = XDELTA_DEFAULT_PORT);
	/// \brief
	/// ���һ���ļ�ͬ������
	/// \param[in] reader	�ļ���ȡ����
	/// \param[in] pdel		�ļ�����ɾ������
	/// \return �޷��ء�
	void add_task (file_reader * reader, deletor * pdel);
	/// \brief
	/// �ȴ������Ѿ���ӵ�����ִ����ɺ󷵻ء�
	/// \return �޷��ء�
	void wait ();
};

} //namespace xdelta

#endif //__XDELTA_CLIENT_H__

