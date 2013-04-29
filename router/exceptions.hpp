#ifndef EXCEPTIONS_HPP_
#define EXCEPTIONS_HPP_
#include <exception>
class UnknownDest: public std::exception { } unknownDest;
class HostUnreachable: public std::exception { } hostUnreachable;

#endif
