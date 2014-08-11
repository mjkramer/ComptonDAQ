#ifndef ARGPARSER_HH
#define ARGPARSER_HH

#include <map>
#include <vector>
#include <string>

using namespace std;

class ArgParser {
public:
  void Parse(int argc, char** argv);
  bool HasOpt(char opt);

  const char* progname;
  string cmd;
  vector<const char*> cmdargs;
  map<char, const char*> options;
};

#endif
