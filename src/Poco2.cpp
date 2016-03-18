//============================================================================
// Name        : Poco2.cpp
// Author      : Claus Guttesen
// Version     :
// Copyright   : Just let me know.
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

#include <pqxx/pqxx>

#include "MyHandler.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <username> <password>" << endl;
        return EXIT_FAILURE;
    }
    try {
        try {
            // Connect to database.
            pqxx::connection D("dbname=weather user=claus hostaddr=127.0.0.1 port=5432");
            if (!D.is_open()) {
                cerr << "Unable to connect to database" << D.dbname() << endl;
                return EXIT_FAILURE;
            }

            // Get this using curl.
            const string url = "https://www.vegvesen.no/ws/no/vegvesen/veg/trafikkpublikasjon/vaer/1/GetMeasurementWeatherSiteTable/";

            std::string username = argv[1];
            std::string password = argv[2];
            std::string credentials = username + ":" + password;

            curlpp::Cleanup cleaner;
            curlpp::Easy request;

            request.setOpt(new curlpp::options::Url(url));
            request.setOpt(new curlpp::options::UserPwd(credentials));

            ostringstream out;
            out << request;

            // Convert html-encoded letters to uft-8 equivalent.
            auto ss = out.str();
            boost::algorithm::replace_all(ss, "&#230;", "æ");
            boost::algorithm::replace_all(ss, "&#248;", "ø");
            boost::algorithm::replace_all(ss, "&#229;", "å");
            boost::algorithm::replace_all(ss, "&#198;", "Æ");
            boost::algorithm::replace_all(ss, "&#216;", "Ø");
            boost::algorithm::replace_all(ss, "&#197;", "Å");

            // Parse response.
            MyHandler handler {};
            Poco::XML::SAXParser parser {};
            parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
            parser.setContentHandler(&handler);

            try {
                parser.parseString(ss);
            } catch (Poco::Exception& e) {
                cerr << e.displayText() << endl;
            }
            auto l = handler.locations();

            // Get a list of current locations.
            pqxx::connection C("dbname=weather user=claus hostaddr=127.0.0.1 port=5432");
            if (!C.is_open()) {
                cerr << "Unable to connect to database " << C.dbname() << endl;
                return EXIT_FAILURE;
            }

            std::string query = "select * from locations";
            pqxx::nontransaction N(C);
            pqxx::result R(N.exec(query));

            //  Remove locations already present.
            for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
                l.erase(to_string(c[1].as<int>()));
            }
            C.disconnect();

            // Insert new locations.
            std::string coordinate {};
            std::string prepared_table = "prep_locations";
            for (auto& i : l) {
                pqxx::work W(D);
                // Set to default values.
                query = "insert into locations(site_id,measurementsitename,coordinate) values ($1,$2,$3)";
                coordinate = "(" + i.second.longitude() + "," + i.second.latitude() + ")";
                D.prepare(prepared_table, query);
                try {
                    W.prepared(prepared_table)(i.first)(i.second.measurementSiteName())(coordinate).exec();
                    W.commit();
                } catch (const pqxx::sql_error& e) {
                    cerr << "unable to insert, error: " << e.what() << endl;
                }
            }

            D.disconnect();
            return EXIT_SUCCESS;
        } catch (const exception& e) {
            cerr << e.what() << endl;
            return EXIT_FAILURE;
        }

    } catch (curlpp::LogicError& e) {
        cout << e.what() << endl;
    } catch (curlpp::RuntimeError& e) {
        cout << e.what() << endl;
    }

    return EXIT_FAILURE;
}
