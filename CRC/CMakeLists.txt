cmake_minimum_required(VERSION 3.5)
project(CRC LANGUAGES C)

add_executable(test
    crc8.c
    crc8.h
    crc16.c
    crc16.h
    crc32.c
    crc32.h
    test.c
)
