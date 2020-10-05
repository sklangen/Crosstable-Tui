#ifndef CONVERT_HPP_
#define CONVERT_HPP_

#include <locale>
#include <codecvt>

std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>& getWcharConverter();

#endif /* CONVERT_HPP_ */
