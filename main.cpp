// SPDX-License-Identifier: MIT
#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QDockWidget>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QQuickView>
#include <QVBoxLayout>
#include <QWidget>

class GlRect : public QOpenGLWidget {
  public:
    explicit GlRect(QWidget *parent = nullptr) : QOpenGLWidget(parent)
    {
    }

    ~GlRect() override
    {
        makeCurrent();
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        if (m_vbo != 0) {
            f->glDeleteBuffers(1, &m_vbo);
        }
        if (m_program != 0) {
            f->glDeleteProgram(m_program);
        }
        doneCurrent();
    }

    void initializeGL() override
    {
        QOpenGLContext *ctx = QOpenGLContext::currentContext();
        QOpenGLFunctions *f = ctx->functions();

        const char *vShaderSrc = "#version 100\n"
                                 "attribute vec3 v_pos;\n"
                                 "void main() {\n"
                                 "   gl_Position = vec4(v_pos, 1.0);\n"
                                 "}\n";
        GLuint vShader = f->glCreateShader(GL_VERTEX_SHADER);
        f->glShaderSource(vShader, 1, &vShaderSrc, nullptr);
        f->glCompileShader(vShader);

        const char *fShaderSrc = "#version 100\n"
                                 "precision mediump float;\n"
                                 "void main() {\n"
                                 "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
                                 "}\n";
        GLuint fShader = f->glCreateShader(GL_FRAGMENT_SHADER);
        f->glShaderSource(fShader, 1, &fShaderSrc, nullptr);
        f->glCompileShader(fShader);

        m_program = f->glCreateProgram();
        f->glAttachShader(m_program, vShader);
        f->glAttachShader(m_program, fShader);

        f->glBindAttribLocation(m_program, 0, "v_pos");
        f->glLinkProgram(m_program);

        f->glDeleteShader(vShader);
        f->glDeleteShader(fShader);

        GLfloat vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
        };

        f->glGenBuffers(1, &m_vbo);
        f->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                        GL_STATIC_DRAW);
    }

    void resizeGL(int w, int h) override
    {
        QOpenGLContext *ctx = QOpenGLContext::currentContext();
        QOpenGLFunctions *f = ctx->functions();
        f->glViewport(0, 0, w, h);
    }

    void paintGL() override
    {
        QOpenGLContext *ctx = QOpenGLContext::currentContext();
        QOpenGLFunctions *f = ctx->functions();

        f->glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
        f->glClear(GL_COLOR_BUFFER_BIT);

        f->glUseProgram(m_program);

        f->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        f->glEnableVertexAttribArray(0);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                                 reinterpret_cast<void *>(0));

        f->glDrawArrays(GL_TRIANGLES, 0, 3);

        f->glDisableVertexAttribArray(0);
    }

  private:
    GLuint m_program = 0;
    GLuint m_vbo = 0;
};

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

        QLineEdit *lineEdit = new QLineEdit;
        layout->addWidget(lineEdit);

        GlRect *glRect = new GlRect;
        glRect->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(glRect);

        mw->setCentralWidget(centralWidget);
    }

    void buildDockWidget(QMainWindow *mw, const QString &title,
                         Qt::DockWidgetArea area)
    {
        QDockWidget *dw = new QDockWidget;
        dw->setWindowTitle(title);

        QWidget *content = new QWidget;
        dw->setWidget(content);

        QVBoxLayout *layout = new QVBoxLayout(content);

        QLabel *label = new QLabel(title);
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label);

        if (area == Qt::LeftDockWidgetArea) {
            QLineEdit *lineEdit = new QLineEdit;
            layout->addWidget(lineEdit);
            layout->addStretch();
        }
        else if (area == Qt::RightDockWidgetArea) {
            QQuickView *view = new QQuickView;
            QWidget *container = QWidget::createWindowContainer(view);
            container->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Expanding);
            layout->addWidget(container);
            view->setSource(QUrl(QStringLiteral("qrc:/main.qml")));
        }
        else {
            layout->addStretch();
        }

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
            QMenu *nestedMenu =
                menu->addMenu(QStringLiteral("Nested Menu %1").arg(i));
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
    // qputenv("QT_QUICK_BACKEND", "software");
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
