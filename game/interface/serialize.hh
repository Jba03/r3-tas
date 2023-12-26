/************************************************************
 # serialize.h: Object serialization and deserialization
 ************************************************************/

#ifndef serialize_h
#define serialize_h

struct serialization;

#define T_bool      "boolean"
#define T_int8      "int8"
#define T_uint8     "uint8"
#define T_int16     "int16"
#define T_uint16    "uint16"
#define T_int32     "int32"
#define T_uint32    "uint32"
#define T_int64     "int64"
#define T_uint64    "uint64"
#define T_float     "float"
#define T_double    "double"
#define T_pointer   "pointer"
#define T_string    "string"



struct serialization_field {
    const char *type;
    const int length;
    const char *name;
};

struct serialization_template {
    const char *type;
};

#define T_START _Generic(typeof(S),
#define T_END )

#define SERIALIZABLE_TYPE(T) \
    /* Default types */ \
    T("const char*", const char*, ) \
    /* Objects types */ \
    T("HIE_tdstSuperObject", struct stSuperObject)


#define T(name,st) st: st_serialize(),
#define serialize(S) _Generic(S, SERIALIZABLE_TYPE(T) default: 0)


#define unserialize(S)

#endif /* serialize_h */
