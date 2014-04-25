/**
* ʵ��ͨ�� SIMHASH �㷨���� Fingerprint��
*/

#ifndef __SIMHASH_H__
#define __SIMHASH_H__

namespace xdelta {

/// \class
/// HashType ��ʹ�õ� Hash ���ͣ�hash_len ��ʾ�ڼ��� Hash ʱ���ִ����ȣ�һ��Ϊ 2 ~ 6��
/// ������ hash_len �ᵼ�¼�����ʧ�档
template <typename HashType, int hash_len = 3>
class simhash {
public:
	simhash (file_reader * reader);
	/// \brief
	/// ������������֮������ƶȣ�������ƶȲ�ͬʱ rollsim ��������� similarity = ��A �� B�� / ��A �� B��
	/// ���ǣ�Hash ���� - ���������� Hash ����֮�ȡ��纺������Ϊ 8������Ϊ 128�������ƶ�Ϊ��
	/// ((128 - 8) / 128 ) * 100%
	float similarity (const simhash<HashType, hash_len> & sh);
	///\brief
	/// ������������֮���û�����롣
	uint32_t distance (const simhash<HashType, hash_len> & sh);
private:
	char_buffer<char_t>	buff_;
	file_reader & reader_;
};

}

#endif //__SIMHASH_H__
