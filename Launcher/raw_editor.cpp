#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <iterator>
#include <iostream>
#include <fstream>
#include <regex>
#include "rapidxml\rapidxml.hpp"
#include "rapidxml\rapidxml_print.hpp"


const static std::string XML_FILE = "doc.xml";

class DfTokenDef {
	//Provides an abstract definition for a token, not an instance of the token
	//Basically just gives the name and the meaning of the attributes of the token.
	private:
	std::string NAME;
	std::vector<std::string> ATTRIBUTES;
	public:
	bool operator == (DfTokenDef &other) {
		return this->NAME == other.NAME;
	}
	DfTokenDef(std::string name, std::string* attribute_names, int atrcount) {
		NAME = name;
		
		std::copy(attribute_names,attribute_names+atrcount,std::back_inserter(ATTRIBUTES));
		ATTRIBUTES.shrink_to_fit();
	}
	DfTokenDef(std::string name, std::vector<std::string> attribute_names) {
		NAME = name;
		std::copy(attribute_names.begin(),attribute_names.end(),std::back_inserter(ATTRIBUTES));
		ATTRIBUTES.shrink_to_fit();
	}
	DfTokenDef() {}
	std::string getName() {return NAME;}
	int getCount() {return ATTRIBUTES.size();}
	std::string getAttribute(int index) {
		if (index-1 > ATTRIBUTES.size())
			throw std::invalid_argument("Index out of bounds");
		return ATTRIBUTES[index-1];
	}
	friend bool operator==(const DfTokenDef &T1, const DfTokenDef &T2)	{return T1.NAME==T2.NAME;}
	friend bool operator!=(const DfTokenDef &T1, const DfTokenDef &T2)	{return !(T1==T2);}
	friend bool operator<(const DfTokenDef &T1, const DfTokenDef &T2)	{return T1.NAME<T2.NAME;}
	friend bool operator<=(const DfTokenDef &T1, const DfTokenDef &T2)	{return T1.NAME<=T2.NAME;}
	friend bool operator>(const DfTokenDef &T1, const DfTokenDef &T2)	{return T1.NAME>T2.NAME;}
	friend bool operator>=(const DfTokenDef &T1, const DfTokenDef &T2)	{return T1.NAME>=T2.NAME;}
	void print(){
		std::cout << this->getName() + '\n';
		int i;
		for(i=1;i<=this->getCount();i++){
			std::cout << this->getAttribute(i) + "\t";
		}
	}
	
	//Returns a set of DfTokenDefs from a file
	static std::vector<DfTokenDef> populateFromFile(std::string filename){
		std::ifstream file(filename);
		std::vector<char> filetext;
		std::copy(std::istream_iterator<char>(file), //Read from beginning of file
				  std::istream_iterator<char>(),     //Stop reading when you equal the EOS iterator
				  std::back_inserter(filetext)); //Insert to the end of the text file
		filetext.push_back(0);
		
		file.close();
		
		rapidxml::xml_document<> doc;
		doc.parse<0>(filetext.data());
		
		//Parse the definitions
		rapidxml::xml_node<>* currDef = doc.first_node()->first_node("def",0,false);
		std::vector<DfTokenDef> definitions;
		do {
			std::string name = currDef->first_node("name",0,false)->value();
			std::vector<std::string> attributes;
			rapidxml::xml_node<>* currNode = currDef->first_node("Attribute");
			do {
				attributes.push_back(currNode->value());
			} while((currNode = currNode->next_sibling("Attribute")) != 0);
			definitions.emplace_back(name,attributes);
		} while((currDef = currDef->next_sibling("Def")) != 0);
		return definitions;
	}
};
class DfToken {
	//Combined with input values and a token definition, defines a raw token
	//Objects of this class are actual token instances, not merely definitions. This includes the token's values and text.
	private:
	DfTokenDef DEFINITION;
	std::string* attributeValues;
	
	public:
	DfToken(DfTokenDef definition, std::string* vals, int valcount){
		if (valcount != definition.getCount())
			throw std::invalid_argument("Incorrect number of values");
		else
			DEFINITION = definition;
			std::copy(vals,vals+valcount,attributeValues);
	}
	DfToken() {}
	std::string getName() {return DEFINITION.getName();}
	std::string getAttributeName(int index) {return DEFINITION.getAttribute(index);}
	int getCount() {return DEFINITION.getCount();}
	std::string getAttributeValue(int index) {
		if (index-1 > this->getCount())
			throw std::invalid_argument("Index out of bounds");
		else
			return attributeValues[index-1];
	}
	std::string getAttributeFromName(std::string index) {
		int i;
		for (i=1;i<=this->getCount();i++) {
			if(index.compare(this->getAttributeName(i)) == 0)
				return attributeValues[i-1];
		}
		throw std::invalid_argument("Not found");
	}
};

class DfRawParser {
	//Begin definitions for the actual scanner
	private: 
		static std::vector<DfTokenDef> definitions;
		std::string filename;
		std::string filetext;
	
	public:	
		DfRawParser(std::string fname){
			if(definitions.size() == 0) { //If definitions is empty or hasn't been initialized
				definitions = DfTokenDef::populateFromFile(XML_FILE); //Fill it from the XML file
				std::sort(definitions.begin(),definitions.end()); //And sort it
			}
			filename = fname;
			std::ifstream file(filename);
			std::copy(std::istream_iterator<char>(file), //Read from beginning of file
			     	  std::istream_iterator<char>(),     //Stop reading when you equal the EOS iterator
				      std::back_inserter(this->filetext)); //Insert to the end of the text file
			filetext.push_back(0); //Ensure the string is null-terminated
			filetext.shrink_to_fit();
		}
		~DfRawParser() {
			std::ofstream(filename) << filetext; //When the object is deleted, write the text to the related file
		}
		/*
		bool isInFile(std::string tname) {
		if(filetext.find(tname + ':') == std::basic_string::npos) //If it cannot find the string NAME:
			return false;										  //It's not in the file at all
		return true;
		bool isActive(std::string tname) {
			if(filetext.find('[' + tname + ':') == std::basic_string::npos) //If it cannot find the string [NAME:
				return false;												//It's not active
			return true;
		}
		*/
		/*
		void activateTokenByName(std::string tname){
			int pos = filetext.find('!' tname + ':');
			if(pos == std::basic_string::npos) return;
			filetext[pos] = '[';
			return;
		}
		void deactivateTokenByName(std::string tname){
			int pos = filetext.find('[' tname + ':');
			if(pos == std::basic_string::npos) return;
			filetext[pos] = '!';
			return;
		}
		void activateToken(std::string tname, std::vector<std::string> attr){
			std:string searchfor = '!' + tname;
			for(int i=0;i<attr.size();i++) searchfor.append(":"+attr.at(i));
			searchfor.push_back(']')
			int pos = filetext.find(searchfor);
			if(pos == std::basic_string::npos) return;
			filetext[pos] = '[';
			return;
		}
		void deactivateToken(std::string tname, std::vector<std::string> attr){
			std:string searchfor = '[' + tname;
			for(int i=0;i<attr.size();i++) searchfor.append(":"+attr.at(i));
			searchfor.push_back(']')
			int pos = filetext.find(searchfor);
			if(pos == std::basic_string::npos) return;
			filetext[pos] = '!';
			return;
		}
		*/
		
		//Assume somewhere within the file there is a token or tokens 'prefix'. Use that to identify if this is the right place. Then modify just the second token received.
		void activateTokenFromRegex(std::string prefix, std::string token){
			std::string str = "(\\[" + prefix + "\\][.|\\s]*)\\!(" + token + "\\])";
			std::regex regstr(str,std::regex_constants::icase | std::regex_constants::ECMAScript);
			std::regex_replace(filetext,
							   regstr,
							   "\\1\\[\\2");
		}
		void deactivateTokenFromRegex(std::string prefix, std::string token){
			std::string str = "(\\[" + prefix + "\\][.|\\s]*)\\[(" + token + "\\])";
			std::regex regstr(str,std::regex_constants::icase | std::regex_constants::ECMAScript);
			std::regex_replace(filetext,
							   regstr,
							   "\\1\\!\\2");
		}
		
		//Assume somewhere within the file there is a token or tokens 'prefix', used to identify the correct token
		//Use a regex string 'token' to split the token into substrings
		//Modify the selected substring
		void modifyTokenFromRegex(std::string prefix, std::string token, int substring=0){
			std::string str = "(\\[" + prefix + "\\][.|\\s]*)(\\[" + token + "\\])";
			std::regex regstr(str,std::regex_constants::icase | std::regex_constants::ECMAScript);
		}
};

int main() {
	std::vector<DfTokenDef> defs = DfTokenDef::populateFromFile((char*)"doc.xml");
	for(int i=0;i<defs.size();i++) {
		defs[i].print();
		std::cout << "\n\n";
	}
	return 1;
}