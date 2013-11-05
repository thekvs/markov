#include <string>

#include <boost/locale.hpp>

#include "exc.hpp"

using namespace markov;

void
test()
{
    boost::locale::generator gen;
    std::locale locale = gen("ru_RU.UTF-8");

    std::string expected = "ёлки-палки лес густой";
    std::string data = "ЁЛКИ-ПАЛКИ ЛЕС ГУСТОЙ";

    std::string result = boost::locale::to_lower(data, locale);

    THROW_EXC_IF_FAILED(result == expected, "unit test failed: got %s, expected %s",
        result.c_str(), expected.c_str());
}

int
main()
{
    int status = EXIT_SUCCESS;

    try {
        test();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        status = EXIT_FAILURE;
    }

    return status;
}
