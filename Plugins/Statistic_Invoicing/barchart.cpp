#include "barchart.h"
#include <qwt_plot_renderer.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_multi_barchart.h>
#include <qwt_column_symbol.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <QtGlobal>
#include <QtGui>

BarChart::BarChart(QWidget *parent):
    QwtPlot(parent),
    d_barChartItem(NULL),
    maxVolume(0),
    maxQuantity(0)
{
    //init();
}

void BarChart::updatePlot(QMap<QString, SalesResult> data, SortArg sArg)
{
    salesData = data;
    sortArg = sArg;
    setAutoFillBackground(true);

    setPalette(Qt::white);
    canvas()->setPalette(QColor("LemonChiffon"));

    setTitle(tr("Sales Statistic"));

    if(!d_barChartItem) {
        d_barChartItem = new QwtPlotMultiBarChart("Sales Statistic");
    }
    else {
        d_barChartItem->detach();
    }

    d_barChartItem->setLayoutPolicy(QwtPlotMultiBarChart::AutoAdjustSamples);

    d_barChartItem->setSpacing(20);
    d_barChartItem->setMargin(3);

    d_barChartItem->attach(this);

    insertLegend(new QwtLegend());

    populate();
    setOrientation(0);

    setAutoReplot(true);
}

void BarChart::populate()
{
    static const char *colors[] = {"SteelBlue", "Gold"};

    const int numSamples = salesData.count();
    const int numBars = sizeof(colors)/sizeof(colors[0]);

    QList<QwtText> titles;
    titles += QString(tr("Sales Volume"));
    titles += QString(tr("Sales Quantity"));
    d_barChartItem->setBarTitles(titles);
    d_barChartItem->setLegendIconSize(QSize(10, 14));

    for (int i = 0; i < numBars; i++) {
        QwtColumnSymbol *symbol = new QwtColumnSymbol(QwtColumnSymbol::Box);
        symbol->setLineWidth(2);
        symbol->setFrameStyle(QwtColumnSymbol::Raised);
        symbol->setPalette(QPalette(colors[i]));

        d_barChartItem->setSymbol(i, symbol);
    }
    
    QVector< QVector<double> > series;
    QList<SalesResult> results = salesData.values();
    for (int i = 0; i < numSamples; i++)
    {
        maxVolume =  (results[i].volume>maxVolume)?results[i].volume:maxVolume;
        maxQuantity =  (results[i].quantity>maxQuantity)?results[i].quantity:maxQuantity;
    }
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

    int finalRange = salesData.count()>sortArg.peopleRange?
                sortArg.peopleRange:salesData.count();
    for (int i = 0; i < finalRange; i++)
    {
        QVector<double> values;
        values += salesData.value(keys[i]).volume;
        values += salesData.value(keys[i]).quantity/maxQuantity*maxVolume;
        series += values;
    }

    d_barChartItem->setSamples(series);
}

void BarChart::setMode(int mode)
{
    if (mode == 0)
    {
        d_barChartItem->setStyle(QwtPlotMultiBarChart::Grouped);
    }
    else
    {
        d_barChartItem->setStyle(QwtPlotMultiBarChart::Stacked);
    }
}

void BarChart::setOrientation(int orientation)
{
    QwtPlot::Axis axisAssistant, axisVolume, axisQuantity;

    if (orientation == 0)
    {
        axisAssistant = QwtPlot::xBottom;
        axisVolume = QwtPlot::yLeft;
        axisQuantity = QwtPlot::yRight;

        d_barChartItem->setOrientation(Qt::Vertical);

        enableAxis(QwtPlot::yRight);
        setAxisTitle(QwtPlot::yLeft, tr("Sales Volume"));
        setAxisTitle(QwtPlot::yRight, tr("Sales Quantity"));
        setAxisTitle(QwtPlot::xBottom, tr("Shop Assistant(Volume, Quantity)"));

        setAxisScaleDraw(QwtPlot::yLeft, new DefaultScaleDraw());
        setAxisScaleDraw(QwtPlot::yRight, new DefaultScaleDraw());
        setAxisScaleDraw(QwtPlot::xBottom, new AssistantScaleDraw(salesData, sortArg));
    }
    else
    {
        axisAssistant = QwtPlot::yLeft;
        axisVolume = QwtPlot::xBottom;
        axisQuantity = QwtPlot::xTop;

        d_barChartItem->setOrientation(Qt::Horizontal);

        enableAxis(QwtPlot::xTop);
        setAxisTitle(QwtPlot::yLeft, tr("Shop Assistant(Volume, Quantity)"));
        setAxisTitle(QwtPlot::xBottom, tr("Sales Volume"));
        setAxisTitle(QwtPlot::xTop, tr("Sales Quantity"));

        setAxisScaleDraw(QwtPlot::xTop, new DefaultScaleDraw());
        setAxisScaleDraw(QwtPlot::xBottom, new DefaultScaleDraw());
        setAxisScaleDraw(QwtPlot::yLeft, new AssistantScaleDraw(salesData, sortArg));
    }

    int finalRange = salesData.count()>sortArg.peopleRange?
                sortArg.peopleRange:salesData.count();
    setAxisScale(axisAssistant, 0, finalRange-1, 1.0);
    setAxisScale(axisVolume, 0, maxVolume);
    setAxisScale(axisQuantity, 0, maxQuantity);

    QwtScaleDraw *scaleDraw1 = axisScaleDraw(axisAssistant);
    scaleDraw1->enableComponent(QwtScaleDraw::Backbone, false);
    scaleDraw1->enableComponent(QwtScaleDraw::Ticks, false);

    QwtScaleDraw *scaleDraw2 = axisScaleDraw(axisVolume);
    scaleDraw2->enableComponent(QwtScaleDraw::Backbone, true);
    scaleDraw2->enableComponent(QwtScaleDraw::Ticks, true);

    QwtScaleDraw *scaleDraw3 = axisScaleDraw(axisQuantity);
    scaleDraw3->enableComponent(QwtScaleDraw::Backbone, true);
    scaleDraw3->enableComponent(QwtScaleDraw::Ticks, true);

    plotLayout()->setAlignCanvasToScale(axisAssistant, true);
    plotLayout()->setAlignCanvasToScale(axisVolume, false);
    plotLayout()->setAlignCanvasToScale(axisQuantity, false);

    plotLayout()->setCanvasMargin(0);
    updateCanvasMargins();

    replot();
}

void BarChart::exportChart()
{
    QwtPlotRenderer renderer;
    renderer.exportTo(this, "barchart.pdf");
}

void BarChart::printChart()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOrientation(QPrinter::Landscape);

    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Statistic Chart"));
    if (dialog->exec() == QDialog::Accepted) {
        QPainter painter;
        painter.begin(&printer);
        double xscale = printer.pageRect().width()/double(width());
        double yscale = printer.pageRect().height()/double(height());
        double scale = qMin(xscale, yscale);
        painter.translate(printer.paperRect().x() + printer.pageRect().width()/2,
                          printer.paperRect().y() + printer.pageRect().height()/2);
        painter.scale(scale, scale);
        painter.translate(-width()/2, -height()/2);

        render(&painter);
        painter.end();
    }

}
