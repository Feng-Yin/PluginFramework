#ifndef STATISTIC_INVOICING_GLOBAL_H
#define STATISTIC_INVOICING_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(STATISTIC_INVOICING_LIBRARY)
#  define STATISTIC_INVOICINGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define STATISTIC_INVOICINGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // STATISTIC_INVOICING_GLOBAL_H
