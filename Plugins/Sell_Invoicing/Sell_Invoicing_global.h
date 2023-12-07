#pragma once

#include <QtCore/qglobal.h>

#if defined(SELL_INVOICING_LIBRARY)
#  define SELL_INVOICINGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SELL_INVOICINGSHARED_EXPORT Q_DECL_IMPORT
#endif
