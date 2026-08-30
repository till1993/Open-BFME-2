// ??$__put_integer@V?$ostreambuf_iterator@DV?$char_traits@D@_STL@@@_STL@@@_STL@@YA?AV?$ostreambuf_iterator@DV?$char_traits@D@_STL@@@0@PAD0V10@AAVios_base@0@HD@Z
// partial score=0.93 date=2026-08-30
// cl: /EHs /EHc- /MD /D_STLP_USE_STATIC_LIB
// stlport

typedef int ptrdiff_t;

namespace _STL
{

template <class T>
class char_traits {};

struct __true_type {};

template <class T>
class allocator {};

template <class Pointer, class Value, class Alloc>
class _STLP_alloc_proxy : public Alloc
{
public:
	Pointer _M_data;
};

template <class CharT, class Traits, class Alloc>
class basic_string
{
public:
	__declspec(dllimport) __forceinline bool empty() const
	{
		return _M_start == _M_finish;
	}

private:
	CharT *_M_start;
	CharT *_M_finish;
	_STLP_alloc_proxy<CharT *, CharT, Alloc> _M_end_of_storage;
};

typedef basic_string<char, char_traits<char>, allocator<char> > narrow_string;

template <class CharT>
class numpunct;

template <>
class numpunct<char>
{
public:
	__declspec(dllimport) __forceinline char thousands_sep() const
	{
		return do_thousands_sep();
	}

protected:
	virtual ~numpunct();
	virtual char do_decimal_point() const;
	virtual char do_thousands_sep() const;
};

class ios_base
{
public:
	typedef int fmtflags;
	typedef int streamsize;

	enum
	{
		hex = 0x0010,
		oct = 0x0020,
		basefield = 0x0038,
		showbase = 0x0200
	};

	virtual ~ios_base();
	__declspec(dllimport) __forceinline streamsize width(streamsize value)
	{
		streamsize previous = _M_width;
		_M_width = value;
		return previous;
	}
	__declspec(dllimport) __forceinline const void *_M_numpunct_facet() const
	{
		return _M_cached_numpunct;
	}
	__declspec(dllimport) __forceinline const narrow_string &_M_grouping() const
	{
		return _M_cached_grouping;
	}

private:
	fmtflags _M_fmtflags;
	int _M_iostate;
	int _M_openmode;
	int _M_seekdir;
	int _M_exception_mask;
	streamsize _M_precision;
	streamsize _M_width;
	void *_M_locale;
	void *_M_callbacks;
	unsigned int _M_num_callbacks;
	unsigned int _M_callback_index;
	long *_M_iwords;
	unsigned int _M_num_iwords;
	void **_M_pwords;
	unsigned int _M_num_pwords;
	void *_M_cached_ctype;
	void *_M_cached_numpunct;
	narrow_string _M_cached_grouping;
};

template <class CharT, class Traits>
class ostreambuf_iterator
{
private:
	void *_M_buf;
	bool _M_ok;
};

int __cdecl __insert_grouping(
		char *, char *, const narrow_string &, char, char, char, int);

template <class CharT, class OutputIter>
OutputIter __cdecl __copy_integer_and_fill(
		const CharT *, ptrdiff_t, OutputIter,
		ios_base::fmtflags, ios_base::streamsize, CharT, CharT, CharT);

template <class InputIter, class OutputIter>
OutputIter __cdecl __copy_ptrs(
		InputIter, InputIter, OutputIter, const __true_type &);

template <class OutputIter>
OutputIter __cdecl __put_integer(
		char *buffer, char *buffer_end, OutputIter out,
		ios_base &stream, ios_base::fmtflags flags, char fill)
{
	ptrdiff_t length = buffer_end - buffer;

	const numpunct<char> &punctuation =
			*static_cast<const numpunct<char> *>(stream._M_numpunct_facet());
	const narrow_string &grouping = stream._M_grouping();

	if (!grouping.empty())
	{
		union copy_state
		{
			int base_characters;
			__true_type pointer_copy;
		} state;
		if (flags & ios_base::showbase)
		{
			switch (flags & ios_base::basefield)
			{
			case ios_base::hex:
				state.base_characters = 2;
				break;
			case ios_base::oct:
				state.base_characters = 1;
				break;
			default:
				state.base_characters = 0;
			}
		}
		else
			state.base_characters = 0;

		char grouping_buffer[64];
		__copy_ptrs(buffer, buffer_end, grouping_buffer, state.pointer_copy);
		buffer = grouping_buffer;
		buffer_end = grouping_buffer + length;
		length = __insert_grouping(
				buffer, buffer_end, grouping, punctuation.thousands_sep(),
				'+', '-', state.base_characters);
	}

	return __copy_integer_and_fill(
			buffer, length, out, flags, stream.width(0), fill, '+', '-');
}

typedef ostreambuf_iterator<char, char_traits<char> > narrow_output_iterator;

template narrow_output_iterator __put_integer<narrow_output_iterator>(
		char *, char *, narrow_output_iterator,
		ios_base &, ios_base::fmtflags, char);

}
