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

#ifndef _DEF_GEN_FUNCTION
#define _DEF_GEN_FUNCTION \
    typedef std::function<NodeType()> NodeGenFunction; \
    typedef std::function<EdgeType()> EdgeGenFunction;
#endif // !_DEF_GEN_FUNCTION

#ifndef _CONTEXT_V
#define _CONTEXT_V(name) \
  this->_context.name()
#endif // !_CONTEXT_V

#ifndef _CONTEXT_V_REF
#define _CONTEXT_V_REF(name) \
  this->_context.name##_ref()
#endif // !_CONTEXT_V_REF

#ifndef _CONTEXT_GET
#define _CONTEXT_GET(name) \
    auto name = _CONTEXT_V(name);
#endif // !_CONTEXT_GET

#ifndef _CONTEXT_GET_REF
#define _CONTEXT_GET_REF(name) \
    auto& name = _CONTEXT_V_REF(name);
#endif // !_CONTEXT_GET

#ifndef _MUST_IS_ROOTED
#define _MUST_IS_ROOTED \
    void set_is_rooted(int is_rooted) = delete; \
    bool& is_rooted_ref() = delete;
#endif // !_MUST_IS_ROOTED

#ifndef _DEFAULT_GRAPH_GEN_FUNC
#define _DEFAULT_GRAPH_GEN_FUNC(name) \
  void __default_generator() { \
    this->_generator = new name##Gen<NodeType, EdgeType>(*this); \
  }
#endif // !_DEFAULT_GRAPH_GEN_FUNC

#ifndef _DEFAULT_GEOMETRY_GEN_FUNC
#define _DEFAULT_GEOMETRY_GEN_FUNC(name) \
  void __default_generator() { \
    this->_generator = new name##Gen<T>(*this); \
  }
#endif //!_DEFAULT_GEOMETRY_GEN_FUN

#ifndef _DEFAULT_OUTPUT
#define _DEFAULT_OUTPUT \
  _output_function = this->default_function();
#endif // !_DEFAULT_OUTPUT

#ifndef _DEFAULT_GEN
#define _DEFAULT_GEN \
  __default_generator();
#endif // !_DEFAULT_GEN

#ifndef _TREE_GRAPH_DEFAULT
#define _TREE_GRAPH_DEFAULT \
  _DEFAULT_OUTPUT \
  _DEFAULT_GEN
#endif // !_TREE_GRAPH_DEFAULT

#ifndef _GEOMETRY_DEFAULT
#define _GEOMETRY_DEFAULT \
  _TREE_GRAPH_DEFAULT
#endif //!_GEOMETRY_DEFAULT

#ifndef _DISABLE_EDGE_COUNT
#define _DISABLE_EDGE_COUNT \
    void set_edge_count(int edge_count) = delete; \
    int& edge_count_ref() = delete;
#endif // !_DISABLE_EDGE_COUNT

#ifndef _DISABLE_DIRECTION
#define _DISABLE_DIRECTION  \
    void set_direction(bool direction) = delete; \
    bool& direction_ref() = delete;
#endif // !_DISABLE_DIRECTION

#ifndef _DISABLE_MULTIPLY_EDGE
#define _DISABLE_MULTIPLY_EDGE \
    void set_multiply_edge(bool multiply_edge) = delete; \
    bool& multiply_edge_ref() = delete;
#endif // !_DISABLE_MULTIPLY_EDGE

#ifndef _DISABLE_SELF_LOOP
#define _DISABLE_SELF_LOOP \
    void set_self_loop(bool self_loop) = delete; \
    bool& self_loop_ref() = delete;
#endif // !_DISABLE_SELF_LOOP

#ifndef _DISABLE_CONNECT
#define _DISABLE_CONNECT \
    void set_connect(bool connect) = delete; \
    bool& connect_ref() = delete;
#endif // !_DISABLE_CONNECT

#ifndef _GEOMETRY_IN_RAND_FUNC
#define _GEOMETRY_IN_RAND_FUNC(Gen) \
  void rand(T x_left, T x_right, T y_left, T y_right) { \
    Gen<T> g; \
    g.set_xy_limit(x_left, x_right, y_left, y_right); \
    __rand(g); \
  } \
  void rand(T left, T right) { \
    Gen<T> g; \
    g.set_xy_limit(left, right); \
    __rand(g); \
  } \
  void rand(std::string format) { \
    Gen<T> g; \
    g.set_format(format); \
    __rand(g); \
  }
#endif //!_GEOMETRY_IN_RAND_FUNC

#ifndef _GEOMETRY_OUT_RAND_FUNC
#define _GEOMETRY_OUT_RAND_FUNC(FuncName, ReturnType) \
  template <typename T, typename = typename std::enable_if<is_point_type<T>::value>::type> \
  ReturnType<T> FuncName(T x_left, T x_right, T y_left, T y_right) { \
    ReturnType<T> r; \
    r.rand(x_left, x_right, y_left, y_right); \
    return r; \
  } \
  template <typename T, typename = typename std::enable_if<is_point_type<T>::value>::type> \
  ReturnType<T> FuncName(T left, T right) { \
    ReturnType<T> r; \
    r.rand(left, right); \
    return r; \
  } \
  template <typename T, typename = typename std::enable_if<is_point_type<T>::value>::type> \
  ReturnType<T> FuncName(std::string format) { \
    ReturnType<T> r; \
    r.rand(format); \
    return r; \
  }
#endif //!_GEOMETRY_OUT_RAND_FUNC

#endif // !_SGPCET_MACRO_H_
