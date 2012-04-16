#ifndef _BAR_CHART_H_

#include <qwt_plot.h>
#include <qwt_scale_draw.h>
#include <QMap>
#include <QList>
#include <productmanagement_interface.h>

class QwtPlotMultiBarChart;

struct SortByVolumeAscend {
    SortByVolumeAscend(QMap<QString, SalesResult> &data)
        :salesData(data) { }
    QMap<QString, SalesResult> &salesData;

    bool operator()(const QString &s1, const QString &s2)
    {
        return salesData.value(s1).volume<salesData.value(s2).volume;
    }
};

struct SortByVolumeDescend {
    SortByVolumeDescend(QMap<QString, SalesResult> &data)
        :salesData(data) { }
    QMap<QString, SalesResult> &salesData;

    bool operator()(const QString &s1, const QString &s2)
    {
        return salesData.value(s1).volume>salesData.value(s2).volume;
    }
};

struct SortByQuantityAscend {
    SortByQuantityAscend(QMap<QString, SalesResult> &data)
        :salesData(data) { }
    QMap<QString, SalesResult> &salesData;

    bool operator()(const QString &s1, const QString &s2)
    {
        return salesData.value(s1).quantity<salesData.value(s2).quantity;
    }
};

struct SortByQuantityDescend {
    SortByQuantityDescend(QMap<QString, SalesResult> &data)
        :salesData(data) { }
    QMap<QString, SalesResult> &salesData;

    bool operator()(const QString &s1, const QString &s2)
    {
        return salesData.value(s1).quantity>salesData.value(s2).quantity;
    }
};

enum {
    SORTBYVOLUMEASCEND = 0,
    SORTBYVOLUMEDESCEND = 1,
    SORTBYQUANTITYASCEND = 2,
    SORTBYQUANTITYDESCEND = 3
};

typedef struct _SortArg {
    int sortType;
    int peopleRange;
} SortArg, *pSortArg;

class BarChart: public QwtPlot
{
    Q_OBJECT

public:
    BarChart(QWidget * = NULL);

public Q_SLOTS:
    void setMode(int);
    void setOrientation(int);
    void exportChart();
    void printChart();

    void updatePlot(QMap<QString, SalesResult> data, SortArg sArg);
private:
    void populate();

private:
    QwtPlotMultiBarChart *d_barChartItem;
    QMap<QString, SalesResult> salesData;
    SortArg sortArg;
    double maxVolume;
    double maxQuantity;
};

class AssistantScaleDraw: public QwtScaleDraw
{
public:
    AssistantScaleDraw(QMap<QString, SalesResult> &data, SortArg sArg) :
        salesData(data),
        sortArg(sArg)
    {
        setTickLength(QwtScaleDiv::MajorTick, 0);
        setTickLength(QwtScaleDiv::MinorTick, 0);
        setTickLength(QwtScaleDiv::MediumTick, 6);

        setLabelRotation(-90.0);
        setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        setSpacing(15);
    }

    virtual QwtText label(double value) const
    {
        QList<QString> keys = salesData.keys();
        switch (sortArg.sortType){
        case SORTBYVOLUMEASCEND:
            qSort(keys.begin(), keys.end(), SortByVolumeAscend(salesData));
            break;
        case SORTBYVOLUMEDESCEND:
            qSort(keys.begin(), keys.end(), SortByVolumeDescend(salesData));
            break;
        case SORTBYQUANTITYASCEND:
            qSort(keys.begin(), keys.end(), SortByQuantityAscend(salesData));
            break;
        case SORTBYQUANTITYDESCEND:
            qSort(keys.begin(), keys.end(), SortByQuantityDescend(salesData));
            break;
        default:
            qSort(keys.begin(), keys.end(), SortByVolumeDescend(salesData));
        }
        QString key = keys.at(value);
        SalesResult result = salesData.value(key);
        return QwtText(QString("%1(%2, %3)").arg(key).arg(result.volume).arg(result.quantity));
    }
private:
    QMap<QString, SalesResult> &salesData;
    SortArg sortArg;
};

#endif
