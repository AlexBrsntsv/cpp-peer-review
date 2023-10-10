#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <assert.h>
#include <algorithm>

using namespace std;

void Tests();

class Domain {
public:
    Domain(std::string s) {
        std::copy(s.rbegin(), s.rend(), std::back_inserter(domain_));
        domain_.push_back('.');
    }
    bool operator==(const Domain& other) const {
        return other.domain_ == domain_;
    }

    bool operator<(const Domain& other) const {
        return domain_ < other.domain_;
    }

    bool IsSubdomain(const Domain& other) const {
        return (other.domain_.size() <= domain_.size()) &&
            std::equal(
                other.domain_.begin(),
                other.domain_.end(),
                domain_.begin()
            );
    }
    std::string GetName() {
        return domain_;
    }

private:
    std::string domain_;
};

class DomainChecker {
public:
    template<typename It>
    DomainChecker(const It begin, const It end) : forbidden_domains_(begin, end) {
        std::sort(forbidden_domains_.begin(), forbidden_domains_.end());

        const auto it = std::unique(forbidden_domains_.begin(), forbidden_domains_.end(),
            [](const Domain& lhs, const Domain& rhs) {
                return lhs.IsSubdomain(rhs) || rhs.IsSubdomain(lhs);
            }
        );
        forbidden_domains_.erase(it, forbidden_domains_.end());
    }

    bool IsForbidden(const Domain& domain) const {
        const auto it = std::upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain);
        if (it == forbidden_domains_.begin()) {
            return false;
        }
        return domain.IsSubdomain(*std::prev(it));
    }

    const  std::vector<Domain>& GetForbiddenDomains() const {
        return forbidden_domains_;
    }

private:
    std::vector<Domain> forbidden_domains_;
};

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);
    Number num;
    std::istringstream(line) >> num;
    return num;
}

std::vector<Domain> ReadDomains(std::istream& input, size_t domains_amount) {
    std::vector<Domain> result;
    result.reserve(domains_amount);
    for (size_t i = 0; i < domains_amount; ++i) {
        std::string s;
        std::getline(input, s);
        result.emplace_back(s);
    }
    return result;
}

void PrintV(std::vector<int> v) {
    for (int i : v) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}

void TestDomainConstructor() {
    Domain domain("alg.m.gdz.ru");
    assert(domain.GetName() == "ur.zdg.m.gla."s);
    std::cout << "TestDomainConstructor is OK" << std::endl;
}

void TestDomainIsSubdomain() {
    Domain domain("gdz.ru");
    assert(Domain("math.gdz.ru").IsSubdomain(domain));
    assert(Domain("history.gdz.ru").IsSubdomain(domain));
    assert(Domain("biology.gdz.ru").IsSubdomain(domain));
    assert(Domain("free.gdz.ru").IsSubdomain(domain));
    std::cout << "TestDomainIsSubdomain is OK" << std::endl;

}

void TestDomainEqualOperator() {
    Domain domain_1("alg.m.gdz.ru");
    Domain domain_2("alg.m.gdz.ru");
    Domain domain_3("gdz.ru");
    assert(domain_1 == domain_2);
    assert(domain_2 == domain_1);
    assert(!(domain_1 == domain_3));
    assert(!(domain_2 == domain_3));
    std::cout << "TestDomainAssignOperator is OK" << std::endl;
}
void TestSortForbiddenDomens() {
    std::istringstream is(
        "30\n"s
        "g.wbo\n"s
        "g.wbo\n"s
        "cbj.g.wbo\n"s
        "dw.jgzm.cbj.g.wbo\n"s
        "tipa.onw.lr.g.wbo\n"s
        "q.cl.nmn.zo.tm.vz\n"s
        "b.z.usa.nndj.abbi.g.wbo\n"s
        "z.bi.k.vw.sak.w.shr.diba\n"s
        "os.uc.ztqp.tipa.onw.lr.g.wbo\n"s
        "z.bi.k.vw.sak.w.shr.diba\n"s
        "n.b.z.usa.nndj.abbi.g.wbo\n"s
        "b.z.usa.nndj.abbi.g.wbo\n"s
        "dw.jgzm.cbj.g.wbo\n"s
        "w.shr.diba\n"s
        "nfg.tipa.onw.lr.g.wbo\n"s
        "ag.fnv.c.dw.jgzm.cbj.g.wbo\n"s
        "drz.u.bfvv.dc.lr.g.wbo\n"s
        "ijre.r.vqa.x.edco.cbj.g.wbo\n"s
        "qqx.q.cl.nmn.zo.tm.vz\n"s
        "z.bi.k.vw.sak.w.shr.diba\n"s
        "arfb.nfg.tipa.onw.lr.g.wbo\n"s
        "g.wbo\n"s
        "z.bi.k.vw.sak.w.shr.diba\n"s
        "z.bi.k.vw.sak.w.shr.diba\n"s
        "j.q.cl.nmn.zo.tm.vz\n"s
        "qqx.q.cl.nmn.zo.tm.vz\n"s
        "zft.q.cl.nmn.zo.tm.vz\n"s
        "arfb.nfg.tipa.onw.lr.g.wbo\n"s
        "vubi.nfg.tipa.onw.lr.g.wbo\n"s
        "tm.vz\n"s
    );
    std::vector<Domain> ethalon = {
        Domain("w.shr.diba"s),
        Domain("g.wbo"s),
        Domain("tm.vz"s),
    };

    const std::vector<Domain> forbidden_domains = ReadDomains(is, ReadNumberOnLine<size_t>(is));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    assert(checker.GetForbiddenDomains() == ethalon);
    assert(checker.IsForbidden(Domain("z.bi.k.vw.sak.w.shr.diba"s)));
    assert(!checker.IsForbidden(Domain("all.diba"s)));
    assert(!checker.IsForbidden(Domain("wa.shr.diba"s)));
    assert(checker.IsForbidden(Domain("w.shr.diba"s)));
    assert(!checker.IsForbidden(Domain("ab.diba"s)));
    for (const auto& element : forbidden_domains) {
        assert(checker.IsForbidden(element));
    }
    std::cout << "TestSortForbiddenDomens is OK" << std::endl;

}

void Tests() {
    TestSortForbiddenDomens();
    TestDomainConstructor();
    TestDomainIsSubdomain();
    TestDomainEqualOperator();
}


