#include "switch.h"

HalfSwitch::HalfSwitch() {};
HalfSwitch::HalfSwitch(std::string nm,char tp){
	name = nm;
	type = tp==0?HalfSwitch::BOOLEAN:HalfSwitch::INTEGER;
}
HalfSwitch::HalfSwitch(std::string nm,HalfSwitch::SwitchType tp){
	name = nm;
	type = tp;
}
HalfSwitch::HalfSwitch(std::string nm,bool val){
	name = nm;
	type = HalfSwitch::BOOLEAN;
	bval = val;
}
HalfSwitch::HalfSwitch(std::string nm,int val){
	name = nm;
	type = HalfSwitch::INTEGER;
	ival = val;
}
HalfSwitch::HalfSwitch(const HalfSwitch& old){
	name = old.name;
	type = old.type;
	ival = old.ival;
}

bool HalfSwitch::Equals(const HalfSwitch& other){
	if (type == HalfSwitch::BOOLEAN)
		return bval==other.bval?false:true;
	else if(type == HalfSwitch::INTEGER)
		return ival==other.ival?false:true;
}

FullSwitch::FullSwitch (HalfSwitch& half, std::string filename, std::string regstring, std::string replace) : HalfSwitch(half) {
	ftarget = filename;
	regstr = regstring;
	repstr = replace;
}
FullSwitch::FullSwitch(const FullSwitch& other){
	name = other.name;
	type = other.type;
	ival = other.ival;
	ftarget = other.ftarget;
	regstr = other.regstr;
	repstr = other.repstr;
}

wxRegEx* FullSwitch::GetRegex(){
	return new wxRegEx(regstr,wxRE_ADVANCED);
}

