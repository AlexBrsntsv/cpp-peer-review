#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <numeric>

enum class QueryType {
	INVALID,
	READ,
	CHEER
};

struct Query {
	int id = 0;
	int pages = 0;
	QueryType type = QueryType::INVALID;
};

Query GetQuery(std::istream& input) {
	Query query;
	std::string cmd;
	input >> cmd;
	if (cmd == "READ") {
		query.type = QueryType::READ;
		input >> query.id >> query.pages;		
	}
	else if (cmd == "CHEER") {
		query.type = QueryType::CHEER;
		input >> query.id;
	}
	return query;
}

class EBook {
	inline const static int max_readers = 100000;
	inline const static int max_pages = 1000;
public:
	EBook()
		: pages_(std::vector<int>(max_pages + 1, 0))
		, readers_(std::vector<int>(max_readers + 1, 0))
	{		
	}
	void AddReader(int id, int pages) {	
		int prev_pages_amount = readers_[id];
		--pages_[prev_pages_amount];		
		readers_[id] = pages;
		++pages_[pages];
	}
	
	double GetReaderAchiev(int id) const {
		int reader_pages = readers_[id];
		if (reader_pages == 0) {
			return 0.0;
		}		
		const auto it_reader = std::next(pages_.begin(), reader_pages);
		int readers_less_pages = std::accumulate(std::next(pages_.begin()), it_reader, 0);
		int total_readers = std::accumulate(it_reader, pages_.end(), readers_less_pages);
		return total_readers == 1 ? 1.0 : ( 1.0 * readers_less_pages / (total_readers - 1) );
	}

private:
	std::vector<int> pages_;
	std::vector<int> readers_;	
};

int main() {
	std::cout << std::setprecision(6);
	EBook ebook;
	int query_amount = 0;
	std::cin >> query_amount;
	for (int i = 0; i < query_amount; ++i) {
		Query query = GetQuery(std::cin);
		if (query.type == QueryType::CHEER) {
			std::cout << ebook.GetReaderAchiev(query.id) << std::endl;
		}
		else if (query.type == QueryType::READ) {
			ebook.AddReader(query.id, query.pages);
		}		
		else {
			std::cout << "Wrong data input format" << std::endl;
			break;
		}
	}

}