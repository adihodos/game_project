#ifndef BASETYPES_H__
#define BASETYPES_H__

#ifndef NO_CPY_CONSTRUCTORS
#define NO_CPY_CONSTRUCTORS(type_name)        \
  type_name(const type_name&);                \
  type_name& operator=(const type_name&)
#endif

#endif // MISC_H__