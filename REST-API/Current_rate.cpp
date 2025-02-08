#include <iostream>
#include <string>
#include <curl/curl.h>
#include "pugixml.hpp"
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <ctime>
#include <iomanip>

using namespace std;

map<string, string> args;
string vname;
int vnom;
string date;
string date_range;
string filename;

class ArgumentParser {
public:
    ArgumentParser(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            string arg(argv[i]);
            int pos = arg.find('=');
            if (pos != string::npos) {
                args[arg.substr(2, pos - 2)] = arg.substr(pos + 1);
            }
        }
    }

    string get(const string& key, const string& default_value = "") const {
        auto it = args.find(key);
        return it != args.end() ? it->second : default_value;
    }

    bool contains(const string& key) const {
        return args.find(key) != args.end();
    }
};

class RateParser {
public:
    struct CurrencyRate {
        string code;
        int nominal;
        string date;
        double rate;
    };

    RateParser() = default;

    void fetch_and_save() {
        string url = construct_url();
        string xml_data = fetch_url(url);

        bool single_date = !date.empty();
        vector<CurrencyRate> rates = parse_currency_data(xml_data, single_date);

        if (!filename.empty()) {
            save_to_file(rates);
        } else {
            display_rates(rates);
        }
    }

private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* userp) {
        userp->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    string fetch_url(const string& url) {
        CURL* curl;
        CURLcode res;
        string readBuffer;

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        return readBuffer;
    }

    string construct_url() {
        string url;
        if (!date.empty()) {
            url = "https://www.cbr.ru/scripts/XML_daily.asp?date_req=" + date;
        } else if (!date_range.empty()) {
            size_t dash_pos = date_range.find('-');
            string start_date = date_range.substr(0, dash_pos);
            string end_date = date_range.substr(dash_pos + 1);
            url = "https://www.cbr.ru/scripts/XML_dynamic.asp?date_req1=" + start_date + "&date_req2=" + end_date + "&VAL_NM_RQ=" + vname;
        } else {
            url = "https://www.cbr.ru/scripts/XML_daily.asp";
        }
        return url;
    }

    vector<CurrencyRate> parse_currency_data(const string& xml_data, bool single_date) {
        vector<CurrencyRate> rates;
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_string(xml_data.c_str());

        if (single_date) {
            pugi::xml_node valcurs = doc.child("ValCurs");
            string date = valcurs.attribute("Date").value();

            for (pugi::xml_node valute : valcurs.children("Valute")) {
                if (string(valute.child_value("CharCode")) == vname) {
                    int nominal = stoi(valute.child_value("Nominal"));
                    string value_str = valute.child_value("Value");
                    replace(value_str.begin(), value_str.end(), ',', '.');
                    double value = stod(value_str);
                    if (vnom) {
                        value = value * (vnom / nominal);
                        nominal = vnom;
                    }
                    rates.push_back(CurrencyRate{vname, nominal, date, value});
                    break;
                }
            }
        } else {
            for (pugi::xml_node record : doc.child("ValCurs").children("Record")) {
                string date = record.attribute("Date").value();
                int nominal = stoi(record.child_value("Nominal"));
                string value_str = record.child_value("Value");
                replace(value_str.begin(), value_str.end(), ',', '.');
                double value = stod(value_str);
                if (vnom) {
                    value = value * (vnom / nominal);
                    nominal = vnom;
                }
                rates.push_back(CurrencyRate{vname, nominal, date, value});
            }
        }

        return rates;
    }

    void save_to_file(const vector<CurrencyRate>& rates) {
        ofstream file(filename);
        for (const auto& rate : rates) {
            file << rate.code << " " << rate.nominal << " " << rate.date << " " << rate.rate << "\n";
        }
    }

    void display_rates(const vector<CurrencyRate>& rates) {
        for (const auto& rate : rates) {
            cout << rate.code << " " << rate.nominal << " " << rate.date << " " << rate.rate << endl;
        }
    }
};

string get_current_date() {
    time_t t = time(nullptr);
    tm tm = *localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%d.%m.%Y");
    return oss.str();
}

int main(int argc, char* argv[]) {
    ArgumentParser args_parser(argc, argv);
    vname = args_parser.get("Vname");
    vnom = args_parser.contains("Vnom") ? stoi(args_parser.get("Vnom")) : 0;
    date = args_parser.contains("Date") ? args_parser.get("Date") : get_current_date();
    date_range = args_parser.get("DateDate");
    filename = args_parser.get("FileName");

    RateParser fetcher;
    fetcher.fetch_and_save();

    return 0;
}