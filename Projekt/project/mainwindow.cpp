#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStateMachine>
#include <QHistoryState>
#include <QFileDialog>
#include <QListView>
#include <QTreeView>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto stateMachine = new QStateMachine{this};

    //ALL STATES
    auto stateStartup = new QState(stateMachine);
    auto stateOpen = new QState{stateMachine};
    auto stateError = new QState{stateMachine};
    auto stateView = new QState{stateMachine};
    auto stateChoose = new QState{stateMachine};
    auto stateCompare = new QState{stateMachine};

    //STARTUP
    stateStartup->assignProperty(ui->pbOpen, "enabled", true);
    stateStartup->assignProperty(ui->pbStart, "enabled", false);
    stateStartup->assignProperty(ui->taCompare, "enabled", false);
    stateStartup->assignProperty(ui->leLabel, "text", "Load directory to start...");
    stateStartup->assignProperty(ui->cbBox1, "enabled", false);
    stateStartup->assignProperty(ui->cbBox2, "enabled", false);

    stateStartup->addTransition(ui->pbOpen, SIGNAL(clicked()), stateOpen);


    //OPEN
    connect(stateOpen, SIGNAL(entered()), this, SLOT(open()));
    stateOpen->addTransition(this, SIGNAL(error(QString)), stateError);
    stateOpen->addTransition(this, SIGNAL(opened()), stateView);


    //ERROR OPEN
    stateError->assignProperty(ui->pbOpen, "enabled", true);
    stateError->assignProperty(ui->pbStart, "enabled", false);
    stateError->assignProperty(ui->cbBox1, "enabled", false);
    stateError->assignProperty(ui->cbBox2, "enabled", false);
    stateError->assignProperty(ui->taCompare, "enabled", false);

    connect(stateError, SIGNAL(entered()), this, SLOT(errorFunction()));
    stateError->addTransition(ui->pbOpen, SIGNAL(clicked()), stateOpen);


    //VIEW
    stateView->assignProperty(ui->pbOpen, "enabled", true);
    stateView->assignProperty(ui->pbStart, "enabled", true);
    stateView->assignProperty(ui->taCompare, "enabled", false);
    stateView->assignProperty(ui->cbBox1, "enabled", true);
    stateView->assignProperty(ui->cbBox2, "enabled", true);

    stateView->addTransition(ui->pbOpen, SIGNAL(clicked()), stateOpen);
    stateView->addTransition(ui->pbStart, SIGNAL(clicked()), stateChoose);


    //CHOOSE
    connect(stateChoose, SIGNAL(entered()), this, SLOT(checkChoose()));
    stateChoose->addTransition(this, SIGNAL(mustChoose()), stateView);
    stateChoose->addTransition(this, SIGNAL(choosed()), stateView);


    //COMPARE
    stateCompare->assignProperty(ui->taCompare, "enabled", true);
    connect(stateCompare, SIGNAL(entered()), this, SLOT(compare()));
    stateCompare->addTransition(this, SIGNAL(error(QString)), stateError);

    stateMachine->setInitialState(stateStartup);
    stateMachine->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open(){

    QFileDialog w;
    w.setFileMode(QFileDialog::DirectoryOnly);
    w.setOption(QFileDialog::DontUseNativeDialog,true);
    w.setOption(QFileDialog::ShowDirsOnly,false);

    QListView *lView = w.findChild<QListView*>("listView");
    if (lView)
        lView->setSelectionMode(QAbstractItemView::MultiSelection);
    QTreeView *tView = w.findChild<QTreeView*>();
    if (tView)
        tView->setSelectionMode(QAbstractItemView::MultiSelection);
    w.exec();

    //1. Just gets a single directory, simple, no complixations
    //QString filepath = QFileDialog::getExistingDirectory(this, tr("Open directory"), "..", QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog);

    //2. Able to select multiple paths, shows other files (but can also choose them, so Project::ctor just ignores them)
    try {
        for(auto&& filepath: w.selectedFiles())
            projects.emplace_back(filepath.toStdString());

    } catch (const std::exception& e) {
        emit error(e.what());
    }

    emit opened();
    //TODO: Otwieranie katalogu drugiego
    //TODO: Wyswietlenie plikow w oknach
}

void MainWindow::checkChoose(){
    emit mustChoose();
    emit choosed();
    //TODO: Wyswietlic messagebox z informacja, ze trzeba wybrac algorytm jak nie jest nic zatikowane
}

void MainWindow::errorFunction(){
    //QMessageBOX
    //TODO: Wyswietlic MessageBox co poszlo nie tak z otwieraniem katalogu i plikow w nim
}

void MainWindow::compare(){
    emit error("compare");
    //TODO: Porownywanie i wprowadzenie wynikow do tabeli
}

