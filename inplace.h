// definition for class and method for in-place synchronizing file
// author:yeyouqun@eisoo.com

#ifndef __INPLACE_H__
#define __INPLACE_H__

/// @file
/// �����͵��ع��ļ������ͼ���ص� stream ���͡�
namespace xdelta {
/// \class
/// �͵��ļ��ع���
class DLL_EXPORT in_place_reconstructor : public reconstructor
{
	virtual void start_hash_stream (const std::string & fname, const int32_t blk_len);
	virtual void add_block (const uchar_t * data, const uint32_t blk_len, const uint64_t s_offset);
	virtual void end_hash_stream (const uint64_t filsize);
	virtual void on_error (const std::string & errmsg, const int errorno);
public:
	in_place_reconstructor (file_operator & foperator) : reconstructor (foperator)
	{}
	~in_place_reconstructor () {}
};

/// \struct
/// ����һ����ͬ�����ݶ����͡�
struct equal_node
{
	uint64_t	s_offset;	///< Դ�ļ��е�ƫ��
	target_pos	tpos;		///< Ŀ���ļ��е�λ����Ϣ
	uint32_t	blength:29; ///< �鳤�ȣ������ᳬ�� MAX_XDELTA_BLOCK_BYTES ���� MULTIROUND_MAX_BLOCK_SIZE
	uint32_t	visited:1;  ///< ����������ʾ�Ŀ��Ƿ��Ѿ��������
	uint32_t	stacked:1;	///< ����������ʾ�Ŀ��Ƿ��Ѿ��ڴ���ջ�С�
	uint32_t	deleted:1;	///< ����������ʾ�Ŀ��Ƿ��Ѿ�ɾ������ѭ����������
};

/// \struct
/// ����һ����ͬ�����ݶ�����
struct diff_node
{
	uint64_t s_offset;	///< Դ�ļ��е�ƫ�ƣ����洢��Ŀ���ļ�����ͬ��λ�á�
	uint32_t blength;	///< �鳤�ȡ�
};

} // namespace xdelta

NAMESPACE_STD_BEGIN

template <> struct less<xdelta::equal_node *> {
	bool operator () (const xdelta::equal_node * left, const xdelta::equal_node * right) const
	{
		return left->tpos.index < right->tpos.index;
	}
};

NAMESPACE_STD_END

namespace xdelta {

/// \class
/// �͵� xdelta �����ࡣ
class DLL_EXPORT in_place_stream : public xdelta_stream
{
	std::list<diff_node>	diff_nodes_;
	std::list<equal_node *>	equal_nodes_;
	xdelta_stream &			output_;
	file_reader &			reader_;
	char_buffer<uchar_t>	buff_;
private:
	virtual void start_hash_stream (const std::string & fname, const int32_t blk_len);
	virtual void add_block (const target_pos & tpos
							, const uint32_t blk_len
							, const uint64_t s_offset);
	virtual void add_block (const uchar_t * data
							, const uint32_t blk_len
							, const uint64_t s_offset);
	virtual void next_round (const int32_t blk_len) { output_.next_round (blk_len); }
	virtual void end_one_round () { output_.end_one_round (); }
	virtual void end_hash_stream (const uint64_t filsize);
	virtual void set_holes (std::set<hole_t> * holeset) {}
	virtual void on_error (const std::string & errmsg, const int errorno);
	void _clear ();
	//
	// calculate the dependency
	//
	void _calc_send ();
	void _handle_node (std::set<equal_node *> & enode_set
						, equal_node * node
						, std::list<equal_node*> & result);
public:
	in_place_stream (xdelta_stream & output, file_reader & reader)
		: output_ (output)
		, reader_ (reader) 
		, buff_ (XDELTA_BUFFER_LEN){}
	~in_place_stream () { _clear (); }
};


} // namespace xdelta

#endif //__INPLACE_H__

