#ifndef EXCEPTIONS_HPP_
#define EXCEPTIONS_HPP_
#include <exception>
const class UnknownDest: public std::exception { } unknownDest;
const class HostUnreachable: public std::exception { } hostUnreachable;

#endif
