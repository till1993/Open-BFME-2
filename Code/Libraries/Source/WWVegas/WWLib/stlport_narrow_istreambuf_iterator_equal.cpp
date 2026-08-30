// cl: /GX- /MD /D_STLP_USE_STATIC_LIB
// stlport

namespace _STL
{

template <class T>
class char_traits
{
public:
	static int eof() { return -1; }
	static char to_char_type(int value) { return static_cast<char>(value); }
	static bool eq_int_type(int left, int right) { return left == right; }
};

struct stdio_get_area
{
	char *_M_next;
	int _M_available;
};

template <class CharT, class Traits>
class basic_streambuf
{
public:
	__declspec(dllimport) __forceinline int sgetc()
	{
		return _M_get->_M_available > 0
				? static_cast<unsigned char>(*_M_get->_M_next)
				: underflow();
	}

protected:
	virtual ~basic_streambuf();
	virtual void *setbuf();
	virtual void seekoff();
	virtual void seekpos();
	virtual int sync();
	virtual int showmanyc();
	virtual int xsgetn();
	virtual int underflow();

private:
	stdio_get_area *_M_get;
};

template <class CharT, class Traits>
class istreambuf_iterator
{
public:
	bool equal(const istreambuf_iterator &) const;

private:
	__declspec(dllimport) __forceinline void _M_getc() const
	{
		if (_M_have_c)
			return;
		int character = _M_buf->sgetc();
		_M_c = Traits::to_char_type(character);
		_M_eof = Traits::eq_int_type(character, Traits::eof());
		_M_have_c = true;
	}

	basic_streambuf<CharT, Traits> *_M_buf;
	mutable CharT _M_c;
	mutable unsigned char _M_eof;
	mutable unsigned char _M_have_c;
};

template <class CharT, class Traits>
bool istreambuf_iterator<CharT, Traits>::equal(
		const istreambuf_iterator &other) const
{
	if (_M_buf != 0)
		_M_getc();
	if (other._M_buf != 0)
		other._M_getc();
	return _M_eof == other._M_eof;
}

typedef istreambuf_iterator<char, char_traits<char> > narrow_iterator;

template bool narrow_iterator::equal(const narrow_iterator &) const;

}
