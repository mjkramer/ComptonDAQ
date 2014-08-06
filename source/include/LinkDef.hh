// For generating ROOT dictionary for std::map<std::string, std::string>
// rootcint -f dict.cxx -c LinkDef.hh

#include <map>

#ifdef __CINT__

#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedefs;

#pragma link C++ class pair<string, string>+;
#pragma link C++ class pair<string, string>::*+;

#pragma link C++ class pair<const string,string>+;
#pragma link C++ class pair<const string,string>::*+;

#pragma link C++ class allocator<pair<const string,string> >+;
#pragma link C++ class allocator<pair<const string,string> >::*+;

#pragma link C++ class map<string,string,less<string>,allocator<pair<const string,string> > >+;
#pragma link C++ class map<string,string,less<string>,allocator<pair<const string,string> > >::*;
#pragma link C++ operators map<string,string,less<string>,allocator<pair<const string,string> > >::iterator;
#pragma link C++ operators map<string,string,less<string>,allocator<pair<const string,string> > >::const_iterator;
#pragma link C++ operators map<string,string,less<string>,allocator<pair<const string,string> > >::reverse_iterator;

#endif
