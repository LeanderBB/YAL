#include "yal/util/argparser.h"
#include <cstring>
#include <sstream>
namespace yal
{

const char ArgParser::NoSingleCharOption = '\0';

ArgParser::ArgParser():
    _nParsedOptions(-1),
    _nRequiredOptions(0),
    _maxOptionsLen(0)
{
    clear();
}

bool
ArgParser::add(const yal_u32 id,
               const char shortOpt,
               const char* longOpt,
               const char* help,
               const yal_u32 flags)
{
    if(!checkDuplicate(id, shortOpt, longOpt))
    {
        _maxOptionsLen = std::max(_maxOptionsLen, (yal_u32) strlen(longOpt));
        Option opt(id, shortOpt, longOpt, help, flags);
        _options.push_back(opt);

        if (flags & kArgFlagRequired)
        {
            _nRequiredOptions++;
        }
        return true;
    }
    return false;
}

bool
ArgParser::add(const yal_u32 id,
               const char* longOpt,
               const char* help,
               const yal_u32 flags)
{
    return add(id, NoSingleCharOption, longOpt, help, flags);
}


int
ArgParser::parse(const int argc,
                 const char** argv)
{
    Option* p_cur_option = nullptr;
    int n_options = 0;
    int n_options_req = 0;
    int i;
    for (i = 1; i < argc;)
    {
        size_t len = strlen(argv[i]);

        if (!len)
        {
            std::stringstream stream;
            stream << "Invalid option at index " << i << "." << std::endl;
            throw ArgParserException(stream.str());
        }


        if (len == 2 && strcmp(argv[i], "--") == 0)
        {
            // end of control options
            ++i;
            break;
        }

        if (p_cur_option)
        {
            // handle active object

            // check if it is the next argument
            if (argv[i][0] == '-')
            {
                // check if a value is already assigned

                if (p_cur_option->size <= 0 && (p_cur_option->flags & (kArgFlagSingleValue | kArgFlagMultiValue)))
                {

                    if (p_cur_option->shortopt != NoSingleCharOption)
                    {
                        std::stringstream stream;
                        stream << "Option (-" << p_cur_option->shortopt
                               << ", --" << p_cur_option->longopt
                               << ") requires at least one value." << std::endl;
                        throw ArgParserException(stream.str());
                    }
                    else
                    {
                        std::stringstream stream;
                        stream << "Option (--" << p_cur_option->longopt
                               << ") requires at least one value." << std::endl;
                        throw ArgParserException(stream.str());
                    }

                }
                p_cur_option = nullptr;
                continue;
            }

            // set value ptr , if not set before
            if (!p_cur_option->value)
            {
                p_cur_option->value = &argv[i];
                p_cur_option->size = 1;
            }
            else
            {
                if ((p_cur_option->flags & kArgFlagSingleValue) && p_cur_option->size == 1)
                {
                    p_cur_option = nullptr;
                    return -1;
                }
                else
                {
                    p_cur_option->size++;
                }
            }
            ++i;
        }

        if (!p_cur_option && len > 1 && argv[i][0] == '-')
        {
            // check for short arg
            p_cur_option = findOptionByShort(argv[i][1]);

            // try to find long option
            if (!p_cur_option && len >2 && argv[i][1] == '-')
            {
                p_cur_option = findOptionByLong(argv[i] + 2);
            }

            if (!p_cur_option)
            {
                std::stringstream stream;
                stream << "Unknown option '" << argv[i]
                          << "'." << std::endl;
                throw ArgParserException(stream.str());
            }

            // mark found
            p_cur_option->size = 0;

            if (i + 1 >= argc && (p_cur_option->flags & (kArgFlagSingleValue | kArgFlagMultiValue)))
            {
                if (p_cur_option->shortopt != NoSingleCharOption)
                {
                    std::stringstream stream;
                    stream << "Option (-" << p_cur_option->shortopt
                           << ", --" << p_cur_option->longopt
                           << ") requires at least one value." << std::endl;
                    throw ArgParserException(stream.str());
                }
                else
                {
                    std::stringstream stream;
                    stream << "Option (--" << p_cur_option->longopt
                           << ") requires at least one value." << std::endl;
                    throw ArgParserException(stream.str());
                }
            }

            if (p_cur_option->flags & kArgFlagRequired)
            {
                n_options_req++;
            }

            if (!(p_cur_option->flags & (kArgFlagSingleValue | kArgFlagMultiValue)))
            {
                p_cur_option = nullptr;
            }
            ++i;
            n_options++;
        }
        else
        {
            break;
        }
    }

    _nParsedOptions = n_options;
    if (n_options_req == _nRequiredOptions)
    {
        return i;
    }
    else
    {
        throw ArgParserException("Not all required options have been specified\n");
    }
}

bool
ArgParser::isSet(const yal_u32 id) const
{
    const Option* opt = findOption(id);
    return (opt) ? opt->size >= 0 : false;
}

const char **
ArgParser::get(const yal_u32 id,
               int &size) const
{
    const Option* opt = findOption(id);
    if(opt)
    {
        size = opt->size;
        return opt->value;
    }
    else
    {
        size = -1;
        return nullptr;
    }
}

const char*
ArgParser::getSingleArg(const yal_u32 id) const
{
    const Option* opt = findOption(id);
    if(opt && opt->size == 1)
    {
        return opt->value[0];
    }
    else
    {
        return nullptr;
    }
}

void
ArgParser::clear()
{
    _options.clear();
    _nParsedOptions = -1;
    _nRequiredOptions = 0;
    _maxOptionsLen = 0;
    add(0, 'h', "help", "Prints this message", 0);

}

void
ArgParser::printHelp(std::ostream &output,
                     const char *description) const
{
    if (description)
    {
        output << std::endl << description << std::endl;
    }

    output << "Options: " << std::endl;
    for(auto it = _options.begin(); it != _options.end(); ++it)
    {
        yal_u32 len = strlen(it->longopt);

        if (it->shortopt != NoSingleCharOption)
        {
            output << "    -"<< it->shortopt << ", --" << it->longopt;
        }
        else
        {
            output << "        --%s" << it->longopt;
        }

        while(len < _maxOptionsLen)
        {
            //TODO: Improve?
            output << " ";
            ++len;
        }
        if (it->flags & kArgFlagRequired)
        {
            output << " : [REQUIRED] ";
        }
        else
        {
            output << " : [OPTIONAL] ";
        }

        output << it->help << std::endl;
    }

     output << std::endl;
}

const ArgParser::Option*
ArgParser::findOption(const yal_u32 id) const
{
    for(auto it = _options.begin(); it != _options.end(); ++it)
    {
        if (id == it->name)
        {
            return &(*it);
        }
    }
    return nullptr;
}

ArgParser::Option*
ArgParser::findOptionByShort(const char c)
{
    for(auto it = _options.begin(); it != _options.end(); ++it)
    {
        if (it->shortopt != NoSingleCharOption && it->shortopt == c)
        {
            return &(*it);
        }
    }
    return nullptr;
}

ArgParser::Option*
ArgParser::findOptionByLong(const char* str)
{
    for(auto it = _options.begin(); it != _options.end(); ++it)
    {
        if (strcmp(it->longopt, str) == 0)
        {
            return &(*it);
        }
    }
    return nullptr;
}

bool
ArgParser::checkDuplicate(const yal_u32 id,
                          const char shortop,
                          const char* longop) const
{

    for(auto it = _options.begin(); it != _options.end(); ++it)
    {
        if(it->name == id)
        {
            return true;
        }
        if (shortop != NoSingleCharOption && it->shortopt == shortop)
        {
            return true;
        }
        if (strcmp(it->longopt, longop) == 0)
        {
            return true;
        }
    }
    return false;
}


}

