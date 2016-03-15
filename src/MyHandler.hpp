#ifndef MYHANDLER_HPP_
#define MYHANDLER_HPP_

#include <iostream>
#include <unordered_map>

#include <boost/algorithm/string/replace.hpp>
#include <Poco/SAX/Attributes.h>
#include <Poco/SAX/ContentHandler.h>
#include <Poco/SAX/SAXParser.h>

#include "Location.hpp"

class MyHandler: public Poco::XML::ContentHandler {
private:
    bool location_data {false};  // Set to true when we reach the actual readings.
    int x {0};                   // Used in index in startElement().
    int counter {0};             // Increment by one. When it reaches three we have the site name.
    std::string _site_id {0};
    std::string _measurementSiteName {};
    std::string _latitude {};
    std::string _longitude {};
    bool _lat {false};
    bool _lon {false};
    std::unordered_map<std::string, Location> _locations {};

public:
    MyHandler();
    virtual ~MyHandler();

    std::unordered_map<std::string, Location> locations();

    // ContentHandler overrides, begin.
    void setDocumentLocator(const Poco::XML::Locator* loc);
    void startDocument();
    void endDocument();
    void startElement(
            const Poco::XML::XMLString&,
            const Poco::XML::XMLString&,
            const Poco::XML::XMLString&,
            const Poco::XML::Attributes&);
    void endElement(
            const Poco::XML::XMLString&,
            const Poco::XML::XMLString&,
            const Poco::XML::XMLString&);
    void characters(const Poco::XML::XMLChar ch[], int, int);
    void ignorableWhitespace(const Poco::XML::XMLChar ch[], int, int);
    void processingInstruction(const Poco::XML::XMLString&, const Poco::XML::XMLString&);
    void startPrefixMapping(const Poco::XML::XMLString&, const Poco::XML::XMLString&);
    void endPrefixMapping(const Poco::XML::XMLString&);
    void skippedEntity(const Poco::XML::XMLString&);
    // ContentHandler overrides, end

};

#endif /* MYHANDLER_HPP_ */
