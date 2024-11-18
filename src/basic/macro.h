#ifndef _SGPCET_MACRO_H_
#define _SGPCET_MACRO_H_

#ifndef _GET_VALUE_CONST
#define _GET_VALUE_CONST(type, name) \
  type name() const { return _##name; }
#endif // !_GET_VALUE_CONST

#ifndef _GET_VALUE_REF
#define _GET_VALUE_REF(type, name) \
  type& name##_ref() { return _##name; }
#endif // !_GET_VALUE_REF

#ifndef _GET_VALUE
#define _GET_VALUE(type, name) \
  _GET_VALUE_CONST(type, name) \
  _GET_VALUE_REF(type, name)
#endif // !_GET_VALUE


#ifndef _SET_VALUE
#define _SET_VALUE(type, name) \
  void set_##name(type name) { _##name = name; }
#endif // !_SET_VALUE

#ifndef _SET_GET_VALUE
#define _SET_GET_VALUE(type, name) \
  _GET_VALUE(type, name) \
  _SET_VALUE(type, name)
#endif // !_SET_GET_VALUE

#ifndef _OUTPUT_FUNCTION
#define _OUTPUT_FUNCTION(_TYPE) \
    typedef std::function<void(std::ostream&, const _TYPE&)> OutputFunction; \
    OutputFunction _output_function;
#endif // !_OUTPUT_FUNCTION

#ifndef _OUTPUT_FUNCTION_SETTING
#define _OUTPUT_FUNCTION_SETTING(_TYPE) \
    void set_output(OutputFunction func) { \
        _output_function = func; \
    } \
    friend std::ostream& operator<<(std::ostream& os, const _TYPE& type) { \
        type._output_function(os, type); \
        return os; \
    } \
    void println() { \
        std::cout << *this << std::endl; \
    } \
    void set_output_default() { \
        _output_function = default_function(); \
    } \
    OutputFunction default_function() { \
        OutputFunction func = \
            [](std::ostream& os, const _TYPE& type) { \
                type.default_output(os); \
            }; \
        return func; \
    }
#endif // !_OUTPUT_FUNCTION_SETTING

#endif // !_SGPCET_MACRO_H_
