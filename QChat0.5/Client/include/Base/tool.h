#ifndef __TOOL_H
#define __TOOL_H


#include <string>
#include <cstring>
#include <string.h>
#include <iostream>



int __64_10(const std::string& s);
int __64_10(const char& c);

char __10_64_char(int now);

std::string __10_64_string(int temp, int len);
std::string get_time_64();
std::string get_time_10();
std::string get_time(int year, int month, int day, int hour, int minu, int seconds);


int getNum(int l, int r);
std::string getstr(int l, int r, bool ok);

int GetID();
int GetGID();




#endif
