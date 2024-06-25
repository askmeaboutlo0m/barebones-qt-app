// SPDX-License-Identifier: MIT
#include <QAction>
#include <QApplication>
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

    void openMainWindow()
    {
        QMainWindow *mw = new QMainWindow;

        QMenu *fileMenu = mw->menuBar()->addMenu(QStringLiteral("&File"));
        QAction *quitAction = fileMenu->addAction(QStringLiteral("&Quit"));
        quitAction->setMenuRole(QAction::QuitRole);
        quitAction->setShortcut(QStringLiteral("Ctrl+Q"));
        connect(quitAction, &QAction::triggered, mw, &QMainWindow::close);

        QMenu *helpMenu = mw->menuBar()->addMenu(tr("&Help"));
        QAction *aboutQtAction = helpMenu->addAction(tr("About &Qt"));
        aboutQtAction->setMenuRole(QAction::AboutQtRole);
        connect(aboutQtAction, &QAction::triggered, &QApplication::aboutQt);

        QWidget *centralWidget = new QWidget;
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);
        QLabel *label = new QLabel(QStringLiteral("Hello, Qt!"));
        layout->addWidget(label);
        layout->addStretch();
        mw->setCentralWidget(centralWidget);

        mw->showMaximized();
    }
};

int main(int argc, char **argv)
{
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
