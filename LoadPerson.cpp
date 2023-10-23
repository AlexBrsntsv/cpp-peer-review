
#include<vector>
#include<iostream>
#include<sstream>

using namespace std;

struct DbConnectParams {
    string_view name;
    int connection_timeout;
    bool allow_exceptions;
    DBLogLevel log_level;
};

struct SearchFilterParams {
    int min_age;
    int max_age;
    string_view name;
};

DBHandler ConnectToDb(DbConnectParams params) {
    DBConnector connector(params.allow_exceptions, params.log_level);
    DBHandler db;
    if (db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(params.name, params.connection_timeout);
    }
    else {
        db = connector.Connect(params.name, params.connection_timeout);
    }
    return db;
}

DBQuery ConstructQuery(SearchFilterParams params, const DBHandler& db) {
    ostringstream query_str;
    query_str << "from Persons "s
        << "select Name, Age "s
        << "where Age between "s << params.min_age << " and "s << params.max_age << " "s
        << "and Name like '%"s << db.Quote(params.name) << "%'"s;
    return DBQuery (query_str.str());

}

vector<Person> LoadPersons(DbConnectParams connection_params, SearchFilterParams search_filter_params) {    
    DBHandler db = ConnectToDb(connection_params);
    if (!connection_params.allow_exceptions && !db.IsOK()) {
        return {};
    }
    DBQuery query = ConstructQuery(search_filter_params, db);
    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({ move(name), age });
    }
    return persons;
}

