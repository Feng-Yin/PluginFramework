#pragma once

#include <QtCore/qglobal.h>

#if defined(CASH_INVOICING_LIBRARY)
#  define CASH_INVOICINGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CASH_INVOICINGSHARED_EXPORT Q_DECL_IMPORT
#endif
