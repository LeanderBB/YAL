#ifndef __YAL_ARGPARSER_H__
#define __YAL_ARGPARSER_H__

#include <yal/yal.h>
#include <yal/util/outputsink.h>
#include <vector>

namespace yal
{
enum ArgFlag
{
    kArgFlagRequired = 1 << 0,
    kArgFlagSingleValue = 1 << 1,
    kArgFlagMultiValue = 1 << 2
};

enum
{
    KArgsOptionHelp = 0
};

class ArgParser
{
public:
    ArgParser();

    bool add(const yal_u32 id,
             const char shortOpt,
             const char* longOpt,
             const char* help,
             const yal_u32 flags);


    /// @return Number of options pared. On error, it will return -1.
    int parse(const int argc,
              const char** argv,
              OutputSink& errorOutput);

    bool isSet(const yal_u32 id) const;

    /// @return nullptr if not found, otheriwise returns the value for this
    /// option and the number of values present in size.
    const char** get(const yal_u32 id,
                     int& size) const;

    /// @return nullptr if not found or arg is not of type single
    const char* getSingleArg(const yal_u32 id) const;

    void clear();

    void printHelp(OutputSink& output,
                   const char* description = nullptr) const;

protected:

    class Option
    {
    public:
        Option(const yal_u32 id,
               const char shrtOpt,
               const char* lngOpt,
               const char* hlp,
               const yal_u32 flgs):
            name(id),
            longopt(lngOpt),
            help(hlp),
            value(nullptr),
            shortopt(shrtOpt),
            flags(flgs),
            size(-1)
        {
        }

        yal_u32 name;
        const char* longopt;
        const char* help;
        const char** value;
        const char shortopt;
        const yal_u32 flags;
        int size;

        bool operator < (const Option& other)
        {
            return shortopt < other.shortopt;
        }
    };

    const Option* findOption(const yal_u32 id) const;

    Option* findOptionByShort(const char c);

    Option* findOptionByLong(const char* str);

    bool checkDuplicate(const yal_u32 id,
                        const char shortop,
                        const char* longop) const;

protected:
    typedef std::vector<Option> OptionVec_t;

    int _nParsedOptions;
    int _nRequiredOptions;
    yal_u32 _maxOptionsLen;
    OptionVec_t _options;

};
}

#endif
