#ifndef SCRIBUSAPISTRING_H
#define SCRIBUSAPISTRING_H

#include <string>
#include <sstream>

namespace ScribusAPI
{
namespace String
{

/**
 * https://codereview.stackexchange.com/a/142912
 *
 * TODO:
 * - check if the stringstream is the fastest solution...
 */
template<typename Iterator>
std::string join(Iterator begin,
                 Iterator end,
                 const std::string & separator =", ")
{
    std::ostringstream ss;

    if(begin != end)
    {
        ss << *begin++;
    }    

    while(begin != end)
    {
        ss << separator;
        ss << *begin++;
    }

    return ss.str();
}

}
}

#endif // SCRIBUSAPISTRING_H
