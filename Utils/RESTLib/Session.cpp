#include "stdafx.h"
#include "Session.h"

SplitQueries SessionBase::extractQueries(beast::string_view target)
{
    // find '?' to identify a key and '=' to identify values, all deliminated by '&'
    // find first query key
    SplitQueries queries{};

    size_t searchOffset = 0;
    size_t keyIndex = 0;
    size_t valueIndex = 0;
    size_t nextKey = 0;

    std::string str(target.data(), target.length());

    keyIndex = str.find_first_of('?', keyIndex);
    while (keyIndex != beast::string_view::npos && valueIndex != beast::string_view::npos && nextKey != beast::string_view::npos)
    {
        std::string key = "";
        std::string val = "";
        if (keyIndex == beast::string_view::npos) return queries;
        // look for the ampersand pos
        nextKey = str.find_first_of('&', keyIndex + 1);

        // look for a value for that key
        valueIndex = str.find_first_of('=', keyIndex);

        // there is no value for this key but maybe one for a future key
        if (valueIndex >= nextKey)
        {
            key = str.substr(keyIndex + 1, nextKey - keyIndex - 1).data();
        }
        else
        {
            key = str.substr(keyIndex + 1, valueIndex - keyIndex - 1).data();
            val = str.substr(valueIndex + 1, nextKey - valueIndex - 1).data();
        }

        keyIndex = nextKey;
        queries.insert_or_assign(key, val);
    }
    return queries;
}

std::string SessionBase::extractEndpoint(beast::string_view target)
{
    target.length();
    std::string ep(target.data(), target.length());
    auto index = 0;// ep.find_last_of("/api/");
    auto queryIndex = ep.find_first_of('?', index);
    if (index < ep.length())
    {
        ep = ep.substr(index, queryIndex - index).data();
        return ep;
    }
    return "";
}
