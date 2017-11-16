#ifndef LDFSWITCHES
#define LDFSWITCHES

#include <string>
#include "wx/regex.h"
#include "cereal/archives/portable_binary.hpp"


#define REGEXFLAGS wxRE_DEFAULT


class HalfSwitch {
	public:
	std::string name;
	enum SwitchType : char {BOOLEAN, INTEGER} type;
	union {
		bool bval;
		int ival;
	};
	HalfSwitch();
	HalfSwitch(std::string nm,char tp);
	HalfSwitch(std::string nm,HalfSwitch::SwitchType tp);
	HalfSwitch(std::string nm,bool val);
	HalfSwitch(std::string nm,int val);
	HalfSwitch(const HalfSwitch& old);
	
	bool Equals(const HalfSwitch& other);
	
	template<class Archive>
	void serialize(Archive &archive){
		archive(name,type,ival);
	};
};

class FullSwitch : public HalfSwitch {
	private:
	std::string regstr;
	public:
	std::string ftarget;
	
	std::string repstr;
	
	FullSwitch(HalfSwitch& half,std::string filename, std::string regstring, std::string replace);
	FullSwitch(const FullSwitch& other);
	
	wxRegEx* GetRegex();
};


#endif