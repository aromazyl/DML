#pragma once

#include <string>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cthreads>

namespace DML {
namespace base {
namespace string {
// String output formated
const std::string& StringPrintf(const std::string& format, ...);

// split string
bool StringSplit(const std::string& str, const std::string& slides,
                 bool isValid, std::vector<std::string>* terms);
}

}
}
