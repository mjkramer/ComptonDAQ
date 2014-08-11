#include "ArgParser.hh"

void ArgParser::Parse(int argc, char** argv)
{
  this->progname = argv[0];

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      char opt = argv[i][1];
      if (i < argc-1)
	this->options[opt] = argv[++i];
    }

    else if (this->cmd.empty())
      this->cmd = argv[i];

    else this->cmdargs.push_back(argv[i]);
  }
}

bool ArgParser::HasOpt(char opt)
{
  return options.find(opt) != options.end();
}
