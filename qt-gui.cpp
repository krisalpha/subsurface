/* qt-gui.cpp */
/* Qt UI implementation */
#include "dive.h"
#include "display.h"
#include "qt-ui/mainwindow.h"
#include "helpers.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QNetworkProxy>
#include <QLibraryInfo>


#include "qt-gui.h"

#ifdef SUBSURFACE_MOBILE
#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSortFilterProxyModel>
#include "qt-mobile/qmlmanager.h"
#include "qt-models/divelistmodel.h"
#include "qt-mobile/qmlprofile.h"
QObject *qqWindowObject = NULL;
#endif

static MainWindow *window = NULL;

void init_ui()
{
	init_qt_late();

	window = new MainWindow();
	if (existing_filename && existing_filename[0] != '\0')
		window->setTitle(MWTF_FILENAME);
	else
		window->setTitle(MWTF_DEFAULT);
}

void run_ui()
{
#ifdef SUBSURFACE_MOBILE
	window->hide();
	qmlRegisterType<QMLManager>("org.subsurfacedivelog.mobile", 1, 0, "QMLManager");
	qmlRegisterType<QMLProfile>("org.subsurfacedivelog.mobile", 1, 0, "QMLProfile");
	QQmlApplicationEngine engine;
	DiveListModel diveListModel;
	QSortFilterProxyModel *sortModel = new QSortFilterProxyModel(0);
	sortModel->setSourceModel(&diveListModel);
	sortModel->setDynamicSortFilter(true);
	sortModel->setSortRole(DiveListModel::DiveDateRole);
	sortModel->sort(0, Qt::DescendingOrder);
	QQmlContext *ctxt = engine.rootContext();
	ctxt->setContextProperty("diveModel", sortModel);
	engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));
	qqWindowObject = engine.rootObjects().value(0);
	if (!qqWindowObject) {
		fprintf(stderr, "can't create window object\n");
		exit(1);
	}
	QQuickWindow *qml_window = qobject_cast<QQuickWindow *>(qqWindowObject);
	qml_window->setIcon(QIcon(":/subsurface-mobile-icon"));
	qqWindowObject->setProperty("messageText", QVariant("Subsurface mobile startup"));
#if !defined(Q_OS_ANDROID)
	qml_window->setHeight(1200);
	qml_window->setWidth(800);
#endif
	qml_window->show();
#else
	window->show();
#endif
	qApp->exec();
}

void exit_ui()
{
	delete window;
	delete qApp;
	free((void *)existing_filename);
	free((void *)default_dive_computer_vendor);
	free((void *)default_dive_computer_product);
	free((void *)default_dive_computer_device);
}

double get_screen_dpi()
{
	QDesktopWidget *mydesk = qApp->desktop();
	return mydesk->physicalDpiX();
}


