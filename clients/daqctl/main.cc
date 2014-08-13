#include <cstdlib>
#include <iostream>

#include <TApplication.h>

#include "Client.hh"
#include "ArgParser.hh"

#define PORT_DEFAULT 8520

bool gRunROOT = false;

using namespace std;

void help(const char* progname)
{
  cerr << "Compton DAQ montoring/control client" << endl << endl;
  cerr << "Usage: " << progname
       << " [<option>...] <command> [<arguments>...]" << endl;

  cerr << endl << "Available commands:" << endl;
  cerr << "\tinfo          Display DAQ status and info" << endl;
  cerr << "\tlisthists     List available histograms" << endl;
  cerr << "\thist <ID>     Display histogram (e.g., hist 3)" << endl;

  cerr << endl << "Available options:" << endl;
  cerr << "\t-h <host>     Address of DAQ machine (default: localhost)" << endl;
  cerr << "\t-p <port>     Connect to DAQ on port <port> (default: "
       << PORT_DEFAULT << ")" << endl;
}

void configure(Client& client, ArgParser& ap)
{
  if (ap.HasOpt('h'))
    client.SetHost(ap.options['h']);

  if (ap.HasOpt('p'))
    client.SetPort(atoi(ap.options['p']));
}

void runcmd(Client& client, ArgParser& ap)
{
  try {
    if (ap.cmd == "info") {
      client.Connect();
      cout << client.Info();
    }

    else if (ap.cmd == "listhists") {
      client.Connect();
      cout << client.ListHists();
    }

    else if (ap.cmd == "hist") {
      if (ap.cmdargs.size() == 0)
	cerr << "Specify a histogram ID!" << endl;
      else {
	int histId = atoi(ap.cmdargs[0]);
	client.Connect();
	client.ShowHist(histId);
	gRunROOT = true;
      }
    }

    else help(ap.progname);
  }

  catch (Client::CommError e) {
    cerr << "Problem talking to DAQ" << endl;
  }
}

int main(int argc, char** argv)
{
  TApplication app("client", 0, 0);

  ArgParser ap;
  ap.Parse(argc, argv);

  Client client;
  configure(client, ap);
  runcmd(client, ap);

  if (gRunROOT) app.Run();

  return 0;
}
