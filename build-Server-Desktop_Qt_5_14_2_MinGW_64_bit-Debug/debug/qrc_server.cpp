/****************************************************************************
** Resource object code
**
** Created by: The Resource Compiler for Qt version 5.14.2
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

static const unsigned char qt_resource_data[] = {
  // E:/QTWORK/Server/server.config.txt
  0x0,0x0,0x0,0x11,
  0x31,
  0x32,0x37,0x2e,0x30,0x2e,0x30,0x2e,0x31,0xd,0xa,0x35,0x30,0x30,0x30,0xd,0xa,
  
  
};

static const unsigned char qt_resource_name[] = {
  // server.config.txt
  0x0,0x11,
  0x5,0xb8,0x28,0x54,
  0x0,0x73,
  0x0,0x65,0x0,0x72,0x0,0x76,0x0,0x65,0x0,0x72,0x0,0x2e,0x0,0x63,0x0,0x6f,0x0,0x6e,0x0,0x66,0x0,0x69,0x0,0x67,0x0,0x2e,0x0,0x74,0x0,0x78,0x0,0x74,
  
  
};

static const unsigned char qt_resource_struct[] = {
  // :
  0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x1,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  // :/server.config.txt
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,
0x0,0x0,0x1,0x8f,0xed,0x26,0xde,0xc9,

};

#ifdef QT_NAMESPACE
#  define QT_RCC_PREPEND_NAMESPACE(name) ::QT_NAMESPACE::name
#  define QT_RCC_MANGLE_NAMESPACE0(x) x
#  define QT_RCC_MANGLE_NAMESPACE1(a, b) a##_##b
#  define QT_RCC_MANGLE_NAMESPACE2(a, b) QT_RCC_MANGLE_NAMESPACE1(a,b)
#  define QT_RCC_MANGLE_NAMESPACE(name) QT_RCC_MANGLE_NAMESPACE2( \
        QT_RCC_MANGLE_NAMESPACE0(name), QT_RCC_MANGLE_NAMESPACE0(QT_NAMESPACE))
#else
#   define QT_RCC_PREPEND_NAMESPACE(name) name
#   define QT_RCC_MANGLE_NAMESPACE(name) name
#endif

#ifdef QT_NAMESPACE
namespace QT_NAMESPACE {
#endif

bool qRegisterResourceData(int, const unsigned char *, const unsigned char *, const unsigned char *);
bool qUnregisterResourceData(int, const unsigned char *, const unsigned char *, const unsigned char *);

#ifdef QT_NAMESPACE
}
#endif

int QT_RCC_MANGLE_NAMESPACE(qInitResources_server)();
int QT_RCC_MANGLE_NAMESPACE(qInitResources_server)()
{
    int version = 3;
    QT_RCC_PREPEND_NAMESPACE(qRegisterResourceData)
        (version, qt_resource_struct, qt_resource_name, qt_resource_data);
    return 1;
}

int QT_RCC_MANGLE_NAMESPACE(qCleanupResources_server)();
int QT_RCC_MANGLE_NAMESPACE(qCleanupResources_server)()
{
    int version = 3;
    QT_RCC_PREPEND_NAMESPACE(qUnregisterResourceData)
       (version, qt_resource_struct, qt_resource_name, qt_resource_data);
    return 1;
}

namespace {
   struct initializer {
       initializer() { QT_RCC_MANGLE_NAMESPACE(qInitResources_server)(); }
       ~initializer() { QT_RCC_MANGLE_NAMESPACE(qCleanupResources_server)(); }
   } dummy;
}
