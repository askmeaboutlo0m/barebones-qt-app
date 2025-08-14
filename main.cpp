// SPDX-License-Identifier: MIT
#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QDockWidget>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QWidget>

class BarebonesApp final : public QApplication {
    Q_OBJECT
  public:
    BarebonesApp(int &argc, char **argv) : QApplication(argc, argv)
    {
        setOrganizationName(QStringLiteral("drawpile"));
        setOrganizationDomain(QStringLiteral("barebones.drawpile.net"));
        setApplicationName(QStringLiteral("barebones"));
        setApplicationVersion(QStringLiteral("0.0.1"));
        setApplicationDisplayName(QStringLiteral("Barebones"));
    }

    void buildCentralWidget(QMainWindow *mw)
    {
        QWidget *centralWidget = new QWidget;
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);

        QLabel *label = new QLabel(QStringLiteral("Hello, Qt!"));
        layout->addWidget(label);

        QComboBox *combo = new QComboBox;
        for (int i = 1; i <= 10; ++i) {
            combo->addItem(QStringLiteral("Option %1").arg(i));
        }
        layout->addWidget(combo);

        layout->addStretch();
        mw->setCentralWidget(centralWidget);
    }

    void buildDockWidget(QMainWindow *mw, const QString &title,
                         Qt::DockWidgetArea area)
    {
        QDockWidget *dw = new QDockWidget;
        dw->setWindowTitle(title);

        QLabel *label = new QLabel(title);
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(true);
        dw->setWidget(label);

        mw->addDockWidget(area, dw);
    }

    void addTestActions(QMenu *menu, int depth)
    {
        menu->addAction(QStringLiteral("Action"));

        QAction *check1 = menu->addAction(QStringLiteral("Checkable Action 1"));
        QAction *check2 = menu->addAction(QStringLiteral("Checkable Action 2"));
        check1->setCheckable(true);
        check2->setCheckable(true);
        check1->setChecked(false);
        check2->setChecked(true);

        for (int i = 1; i < depth; ++i) {
            QMenu *nestedMenu = menu->addMenu(QStringLiteral("Nested Menu %1").arg(i));
            addTestActions(nestedMenu, i);
        }
    }

    void buildMenuBar(QMainWindow *mw)
    {
        QMenu *fileMenu = mw->menuBar()->addMenu(QStringLiteral("&File"));
        addTestActions(fileMenu, 3);
        fileMenu->addSeparator();
        QAction *quitAction = fileMenu->addAction(QStringLiteral("&Quit"));
        quitAction->setMenuRole(QAction::QuitRole);
        quitAction->setShortcut(QStringLiteral("Ctrl+Q"));
        connect(quitAction, &QAction::triggered, mw, &QMainWindow::close);

        QMenu *editMenu = mw->menuBar()->addMenu(QStringLiteral("&Edit"));
        addTestActions(editMenu, 3);

        QMenu *viewMenu = mw->menuBar()->addMenu(QStringLiteral("&View"));
        QMenu *docksMenu = viewMenu->addMenu(QStringLiteral("&Docks"));
        for (QDockWidget *dw : mw->findChildren<QDockWidget *>(
                 QString(), Qt::FindDirectChildrenOnly)) {
            docksMenu->addAction(dw->toggleViewAction());
        }
        addTestActions(viewMenu, 3);

        QMenu *helpMenu = mw->menuBar()->addMenu(QStringLiteral("&Help"));
        QAction *aboutQtAction =
            helpMenu->addAction(QStringLiteral("About &Qt"));
        aboutQtAction->setShortcut(QStringLiteral("Ctrl+T"));
        aboutQtAction->setMenuRole(QAction::AboutQtRole);
        connect(aboutQtAction, &QAction::triggered, &QApplication::aboutQt);
        helpMenu->addSeparator();
        addTestActions(helpMenu, 3);
    }

    void openMainWindow()
    {
        QMainWindow *mw = new QMainWindow;
        mw->setDockOptions(QMainWindow::AnimatedDocks
                           | QMainWindow::AllowNestedDocks
                           | QMainWindow::AllowTabbedDocks);
        buildCentralWidget(mw);
        buildDockWidget(mw, QStringLiteral("North"), Qt::TopDockWidgetArea);
        buildDockWidget(mw, QStringLiteral("East"), Qt::RightDockWidgetArea);
        buildDockWidget(mw, QStringLiteral("South"), Qt::BottomDockWidgetArea);
        buildDockWidget(mw, QStringLiteral("West"), Qt::LeftDockWidgetArea);
        buildMenuBar(mw);
        mw->showMaximized();
    }
};

int main(int argc, char **argv)
{
#ifdef Q_OS_ANDROID
    // The "native menu bar" on Android is a big, empty bar showing only the
    // application title, with a button on the right that folds out the "menu
    // bar" that tries to emulate proper menus. That's a lot of wasted space
    // for a blank bar, the menu emulation can't do everything the regular Qt
    // menu bar can do, nested menus get thrown to the other side of the screen
    // and navigation is very arduous due to slow fade effects that don't
    // adhere to the system animation scaling speed. This makes the non-native
    // menu bar the only feasible option for a real application.
    QApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
#endif
#ifdef __EMSCRIPTEN__
    BarebonesApp *app = new BarebonesApp(argc, argv);
    app->openMainWindow();
    return 0;
#else
    BarebonesApp app(argc, argv);
    app.openMainWindow();
    return app.exec();
#endif
}

#include <main.moc>
