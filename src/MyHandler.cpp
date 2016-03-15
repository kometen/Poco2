#include "MyHandler.hpp"

MyHandler::MyHandler() {}
MyHandler::~MyHandler() {}

void MyHandler::setDocumentLocator(const Poco::XML::Locator* loc) {
}

void MyHandler::startDocument() {
}

void MyHandler::endDocument() {
}

void MyHandler::startElement(const Poco::XML::XMLString& namespaceURI, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname, const Poco::XML::Attributes& attributes) {
    // We have reached the actual weather data in the xml string.
    if (qname == "measurementSiteTable") {
        location_data = true;
    }
    // Set counter to zero. When counter reaches three the current qname value is used as key.
    if (qname == "measurementSiteName" && location_data) {
        counter = 0;
    }
    if (location_data) {
        ++counter;
    }

    // Get id of site.
    if (qname == "measurementSiteRecord") {
        if (attributes.getLength()) {
            x = attributes.getLength();
            for (int i = 0; i < x; ++i) {
                if (attributes.getQName(i) == "id") {
                    _site_id = attributes.getValue(i);
//                    std::cout << "site id: " << _site_id << std::endl;
                }
            }
        }
    }

    // Get latitude and longitude.
    if (qname == "latitude") {
        _lat = true;
    }
    if (qname == "longitude") {
        _lon = true;
    }
}

void MyHandler::endElement(const Poco::XML::XMLString& uri, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname) {
    if (qname == "measurementSiteRecord") {
        // We reached the end of one site, emplace the vector in the unordered_map.
        Location l {_measurementSiteName, _latitude, _longitude};
        _locations.emplace(_site_id, l);
        // Reset the counter.
        counter = 0;
    }
}

void MyHandler::characters(const Poco::XML::XMLChar ch[], int start, int length) {
    // We have a site name. XXX Convert html entity to æøå. Boost.
    if (counter == 3) {
        _measurementSiteName = std::string(ch + start, length);
//        std::cout << "site name: " << _measurementSiteName << ", start: " << start <<", length: " << length << std::endl;
    }

    // Grab latitude and longitude.
    if (_lat) {
        _latitude = std::string(ch + start, length);
//        std::cout << "latitude: " << _latitude << std::endl;
        _lat = false;
    }
    if (_lon) {
        _longitude = std::string(ch + start, length);
//        std::cout << "longitude: " << _longitude << std::endl;
        _lon = false;
    }
}

void MyHandler::ignorableWhitespace(const Poco::XML::XMLChar ch[], int start, int length) {
}

void MyHandler::processingInstruction(const Poco::XML::XMLString& target, const Poco::XML::XMLString& data) {
}

void MyHandler::startPrefixMapping(const Poco::XML::XMLString& prefix, const Poco::XML::XMLString& uri) {
}

void MyHandler::endPrefixMapping(const Poco::XML::XMLString& prefix) {
}

void MyHandler::skippedEntity(const Poco::XML::XMLString& name) {
}

std::unordered_map<std::string, Location> MyHandler::locations() {
    return _locations;
}
