// cl: /EHs /EHc- /MD /D_STLP_USE_STATIC_LIB
// stlport

namespace _STL
{

class ios_base
{
public:
	typedef int fmtflags;

	enum
	{
		dec = 0x0008,
		hex = 0x0010,
		oct = 0x0020,
		basefield = 0x0038,
		showbase = 0x0200,
		showpos = 0x0800,
		uppercase = 0x4000
	};
};

extern const char __hex_char_table_lo[];
extern const char __hex_char_table_hi[];

struct __false_type {};
struct __true_type {};

template <class Integer>
struct integer_sign;

template <>
struct integer_sign<unsigned __int64>
{
	typedef __false_type type;
};

template <>
struct integer_sign<__int64>
{
	typedef __true_type type;
};

template <class Integer>
__forceinline char *__cdecl __write_decimal_backward(
		char *current, Integer value, ios_base::fmtflags flags,
		const __false_type &)
{
	for (; value != 0; value /= 10)
		*--current = (int)(value % 10) + '0';
	if (flags & ios_base::showpos)
		*--current = '+';
	return current;
}

template <class Integer>
inline char *__cdecl __write_decimal_backward(
		char *current, Integer value, ios_base::fmtflags flags,
		const __true_type &)
{
	__int64 temporary = value;
	const bool negative = value < 0;

	if (negative)
		temporary = -temporary;

	for (; temporary != 0; temporary /= 10)
		*--current = (int)(temporary % 10) + '0';

	if (negative)
		*--current = '-';
	else if (flags & ios_base::showpos)
		*--current = '+';
	return current;
}

template <class Integer>
char *__cdecl __write_integer_backward(
		char *buffer, ios_base::fmtflags flags, Integer value)
{
	char *current = buffer;
	unsigned __int64 temporary;

	if (value == 0)
	{
		*--current = '0';
		if ((flags & ios_base::showpos) &&
				((flags & (ios_base::hex | ios_base::oct)) == 0))
			*--current = '+';
	}
	else
	{
		switch (flags & ios_base::basefield)
		{
		case ios_base::oct:
			temporary = value;
			for (; temporary != 0; temporary >>= 3)
				*--current = ((unsigned)temporary & 0x7) + '0';
			if (flags & ios_base::showbase)
				*--current = '0';
			break;

		case ios_base::hex:
			{
				const char *table = (flags & ios_base::uppercase)
						? __hex_char_table_hi : __hex_char_table_lo;
				temporary = value;
				for (; temporary != 0; temporary >>= 4)
					*--current = table[(unsigned)temporary & 0xf];
				if (flags & ios_base::showbase)
				{
					*--current = table[16];
					*--current = '0';
				}
			}
			break;

		default:
			current = __write_decimal_backward(
					current, value, flags,
					typename integer_sign<Integer>::type());
			break;
		}
	}

	return current;
}

template char *__cdecl __write_integer_backward<unsigned __int64>(
		char *, ios_base::fmtflags, unsigned __int64);

template char *__cdecl __write_integer_backward<__int64>(
		char *, ios_base::fmtflags, __int64);

template char *__cdecl __write_decimal_backward<__int64>(
		char *, __int64, ios_base::fmtflags, const __true_type &);

}
