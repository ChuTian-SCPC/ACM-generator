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


#endif // !_SGPCET_MACRO_H_
