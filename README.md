# ISF Utilities

## Introduction

This repository contains a number of C++ classes and C modules that have proven useful in various projects and are being made available for general use. They are described here in alphabetical order.

### Base64
A C implementation of the Base64 algorithm. Both encode and decode functions are provided.

### CRC
C implementations of CRC-8, CRC-16 and CRC-32. Keep in mind that these algorithms come in many different variations. The implementations given here may therefore need to be adjusted for specific purposes.

### ElapsedTimer
A simple C++ timer class used to measure execution times.

### FreeStoreArray
A fixed-size array similar to std::array but allocated on the free store (aka heap). Often a fixed-size array is desired, but using a large std::array locally would exceed the available stack size. In these cases FreeStoreArray provides an equivalent heap-based alternative.

### SerialPort
A C++ class encapsulating operating-system-specific APIs and providing synchronous access to serial ports. Currently Linux and Windows are supported.

### SHA-256
A C implementation of the SHA-256 cryptographic hash algorithm.

### SincFilter
A C++ class allowing various windowed sinc filters to be constructed in an intuitive manner.

### StaticString
Provides a C++ class template for strings using fixed-size storage. This class is intended for use on microcontrollers where dynamic memory allocation is often undesirable and therefore std::string is not an option. Typically this class will be used with a project-specific alias such as `using String = StaticString<64>;`

### TcpSocket
Another C++ class encapsulating operating-system-specific APIs and providing a synchronous interface to TCP sockets. Currently Linux and Windows are supported.


## License

This collection of utilities is released under the 2-Clause BSD License. See [COPYING](COPYING) for details.

## Contributing

Some of the code provided here will likely continue to be improved over time. Patches to fix bugs are always welcome and can be submitted by [email](mailto:mahasler@gmail.com).
