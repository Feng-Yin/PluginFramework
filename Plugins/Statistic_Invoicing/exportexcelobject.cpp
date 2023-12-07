#include "exportexcelobject.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QVariant>
#include <QDateTime>

#include "xlsxdocument.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

int ExportExcelObject::export2Excel()
{
    if (fieldList.size() <= 0) {
        qDebug() << "ExportExcelObject::export2Excel failed: No fields defined.";
        return -1;
    }

    QXlsx::Document xlsxW;
    int row = 1;
    int col = 1;

    for (int i = 0; i < fieldList.size(); i++) {
        xlsxW.write(row, col + i, fieldList.at(i)->sFieldName);
    }

    int r, iRet = 0;
    for (r = 0; r < tableView->model()->rowCount(); r++) {
        for (int c = 0; c < fieldList.size(); c++) {
            xlsxW.write(row + r + 1,
                        col + c,
                        tableView->model()
                            ->data(tableView->model()->index(r, fieldList.at(c)->iCol))
                            .toString());
        }

        iRet++;

        if (r % 10 == 0)
            emit exportedRowCount(r);
    }

    emit exportedRowCount(r);

    if (xlsxW.saveAs(excelFilePath)) {
        qDebug() << "[debug] success to write xlsx file";
    } else {
        qDebug() << "[debug][error] failed to write xlsx file";
        return -2;
    }

    return iRet;
}
