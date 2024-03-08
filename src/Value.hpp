#ifndef SED_COMPILER_VALUE_HPP
#define SED_COMPILER_VALUE_HPP
#include <cstdint>
namespace Compiler
{
    struct Value
    {
        virtual ~Value() = default;
    };

    struct IntValue : public Value
    {
        explicit IntValue(int32_t value);
        int32_t value;
    };

    struct FloatValue : public Value
    {
        explicit FloatValue(float value);
        float value;
    };

}

#endif // SED_COMPILER_VALUE_HPP