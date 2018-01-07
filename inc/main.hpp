#include <iostream>
#include <vector>
#include "optionparser.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <sstream>

#ifndef __MAIN_H__
#define __MAIN_H__
enum  optionIndex { UNKNOWN, HELP,  XML_FILE};
const option::Descriptor usage[] =
{
 {UNKNOWN, 0, "", "",option::Arg::None, "USAGE: tally [options]\n\n"
                                        "Options:" },
 {HELP, 0,"", "help",option::Arg::None, "  --help  \tPrint usage and exit." },
 {XML_FILE, 0, "d", "accounts", option::Arg::Optional, "--accounts, -d \txml sheet of the month"},
 {UNKNOWN, 0, "", "",option::Arg::None, "\nExamples:\n"
                               "  tally --unknown -- --this_is_no_option\n"
                               "  tally -unk --plus -ppp file1 file2\n" },
 {0,0,0,0,0,0}
};

typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned char uint8_t;

#endif //__MAIN_H__