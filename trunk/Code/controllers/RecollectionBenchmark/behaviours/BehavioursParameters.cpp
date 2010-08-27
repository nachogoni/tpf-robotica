#include "BehavioursParameters.h"
#include <string>
using namespace std;

namespace behaviours{
	
	static std::map<char *,double,ltstr> parameters;
		
	struct ltstr
	{
	  bool operator()(const char* s1, const char* s2) const
	  {
		return strcmp(s1, s2) < 0;
	  }
	};
	void BehavioursParameters::Init(FILE * config)
	{
		char  variablename [200] = {0};
		double variablevalue;
		int ret = fscanf (config, "%s %lf\n", variablename, &variablevalue);
		while ( ret != -1 ){
			if ( ret == 2 ){
				string* aux=new string ( variablename);
				char * a=(char*)aux->c_str();
				parameters[a] = variablevalue;
			}
			ret = fscanf (config, "%s %lf\n", variablename, &variablevalue);
		}
				
	}
		
		BehavioursParameters::~BehavioursParameters(){}
		
		double BehavioursParameters::getParameter(char * param){
			return parameters[param];
		}
		
		void BehavioursParameters::setParameter(char * param,double val){
			parameters[param] = val;
		}


}
