#ifndef BENCH_LAB_READ_STRINGS_H
#define BENCH_LAB_READ_STRINGS_H

#include <vector>
#include <string>
#include <random>
#include <memory>

struct StringWithViews {
    std::shared_ptr<std::string> string;
    std::vector<std::string_view> views;

    struct Builder {
        StringWithViews &owner;
        std::vector<size_t> delimiters;

        Builder(StringWithViews &owner) : owner{owner} {
            owner.string = std::make_shared<std::string>();
        }

        void push(const std::string_view &token) {
            delimiters.push_back(token.size());
            *owner.string += token;
        }

        void close() {
            auto iter = owner.string->begin();
            for (auto d : delimiters) {
                owner.views.emplace_back(&*iter, d);
                iter += d;
            }
        }
    };
};

StringWithViews readIntoOneString(const std::string &filename) {
    StringWithViews result;

    std::string line;
    std::ifstream stream{filename};
    StringWithViews::Builder builder{result};
    while (std::getline(stream, line)) {
        if (line.size() < 4) { continue; }
        builder.push(std::string_view{line}.substr(1, line.size() - 3));
    }
    builder.close();

    return result;
}

StringWithViews generateRandomStringWithViews(const std::vector<std::string_view> &strings,
    size_t size)
{
    StringWithViews result;
    StringWithViews::Builder builder{result};

    std::mt19937 generator;
    std::uniform_int_distribution<int> distr(0, strings.size() - 1);

    size_t minl = 100;
    size_t maxl = 0;
    for (const auto &s : strings) {
        if (s.size() > maxl) { maxl = s.size(); }
        if (s.size() < minl) { minl = s.size(); }
    }

    std::uniform_int_distribution<int> randomSizeDistr(minl - 1, maxl + 1);
    std::uniform_real_distribution<double> randomStringDistr(0., 1.);

    for (size_t i = 0; i < size; ++i) {
        if (randomStringDistr(generator) < 0.1) {
            auto randomSize = randomSizeDistr(generator);
            std::string res;
            for (size_t j = 0; j < randomSize; ++j) {
                auto &s = strings.at(distr(generator));
                res += s.at(generator() % s.size());
            }
            builder.push(res);
        } else {
            builder.push(strings.at(distr(generator)));
        }
    }

    builder.close();

    return result;
}

template <typename Mapping>
StringWithViews generateTestStrings() {
    std::vector<std::string_view> strings;

    for (const auto &keyValue : Mapping::values) {
        strings.emplace_back(
            std::string_view{keyValue.key, static_cast<size_t>(keyValue.keyLength)});
    }

    return generateRandomStringWithViews(strings, 560000);
}

#endif //BENCH_LAB_READ_STRINGS_H
