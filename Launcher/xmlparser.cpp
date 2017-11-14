#include "xmlparser.h"
/*
const wxXmlNode* XmlParser::Traverse(const wxXmlNode* current) {
	const wxXmlNode* next = current;
	if(next->GetChildren() != NULL && next->GetChildren()->GetType() == wxXML_ELEMENT_NODE) {
		next = next->GetChildren(); //Get your first child if you have children
	}
	else if(next->GetNext() != NULL) {
		next = next->GetNext(); //Get your sibling if you have a sibling
	}
	else { 
		while(next->GetNext() == NULL){
			if(next->GetParent() == NULL)
				return NULL;
			next = next->GetParent();
			}
		next = next->GetNext(); //Get your first uncle
	}
	return next;
}


const wxXmlNode* XmlParser::getNextMeta(const wxXmlNode* current) {
	const wxXmlNode* next = current;
	while(next != NULL && next->GetName().Lower() != "meta")
		next = XmlParser::Traverse(next);
	return next;
}
*/
const wxXmlNode* XmlParser::getMeta(const wxString name) {
	if(name == "")
		return NULL;
	wxXmlNode* node = wxXmlResource::Get()->GetResourceNode(name)->GetChildren();
	while(node != NULL && node->GetName().Lower() != "meta")
		node = node->GetNext();
	if(node == NULL) return XmlParser::getMeta(wxXmlResource::Get()->GetResourceNode(name)->GetAttribute("ref","")); //If there's no meta in this unit, head to its structural ancestor
	else return node;
}