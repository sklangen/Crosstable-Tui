#include "Convert.hpp"

std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>& getWcharConverter() {
	return converter;
}
