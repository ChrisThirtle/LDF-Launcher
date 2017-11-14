#ifndef XMLPARSER
#define XMLPARSER

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/xrc/xmlres.h".
#include "wx/xml/xml.h"

namespace XmlParser {
//	const wxXmlNode* Traverse(const wxXmlNode* current);
//	const wxXmlNode* getNextMeta(const wxXmlNode* current);
	const wxXmlNode* getMeta(const wxString name); //Gets the single meta node from an object name
}

#endif