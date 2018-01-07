#include "accounts.hpp"
using namespace std;

int main(int argc, char* argv[])
{
  Accounting *GroupAccounting = NULL;
  
  string fname = "";
  argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
  option::Stats  stats(usage, argc, argv);
  std::vector<option::Option> options(stats.options_max);
  std::vector<option::Option> buffer(stats.buffer_max);
  option::Parser parse(usage, argc, argv, &options[0], &buffer[0]);

  if (parse.error())
    return 1;

  if (options[HELP] || argc == 0) {
    option::printUsage(std::cout, usage);
    return 0;
  }

  for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next())
    std::cout << "Unknown option: " << std::string(opt->name,opt->namelen) << "\n";


  for (option::Option* opt = options[XML_FILE]; opt; opt = opt->next())
  {
 	/*
	if( 0 == opt->arg){
		cout << "No input file!" << endl;
		break;
	}
	fname = opt->arg;
	cout << "XML DB file:" << fname << endl;
	*/
	if("" != fname)
	{
		GroupAccounting = new Accounting(fname);
	}
	else
	{
		GroupAccounting = new Accounting();
	}
	
	if(NULL != GroupAccounting)
	{
		GroupAccounting->StartAccounting();
	}
  }
  cout << "Process ended!!!" << endl;
}
