#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "calculator.h"
#include <QPushButton>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    calculator = new Calculator(this);

    // Connect display
    connect(calculator, &Calculator::displayChanged, ui->displayLabel, &QLabel::setText);

    // Connect all buttons
    connectButtons();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectButtons()
{
    // Digit buttons
    for(int i = 0; i <= 9; ++i) {
        QString btnName = "button" + QString::number(i);
        QPushButton *btn = ui->centralwidget->findChild<QPushButton*>(btnName);
        if(btn) {
            connect(btn, &QPushButton::clicked, [=](){ calculator->digitClicked(QString::number(i)); });
        }
    }

    // Decimal point and sign
    connect(ui->buttonDot, &QPushButton::clicked, calculator, &Calculator::pointClicked);
    connect(ui->buttonPlusMinus, &QPushButton::clicked, calculator, &Calculator::changeSignClicked);

    // Clear / Backspace
    connect(ui->buttonBackspace, &QPushButton::clicked, calculator, &Calculator::backspaceClicked);
    connect(ui->buttonClear, &QPushButton::clicked, calculator, &Calculator::clear);
    connect(ui->buttonClearAll, &QPushButton::clicked, calculator, &Calculator::clearAll);

    // Additive operators
    connect(ui->buttonAdd, &QPushButton::clicked, [=](){ calculator->additiveOperatorClicked(Calculator::Addition); });
    connect(ui->buttonSub, &QPushButton::clicked, [=](){ calculator->additiveOperatorClicked(Calculator::Subtraction); });

    // Multiplicative operators
    connect(ui->buttonMul, &QPushButton::clicked, [=](){ calculator->multiplicativeOperatorClicked(Calculator::Multiplication); });
    connect(ui->buttonDiv, &QPushButton::clicked, [=](){ calculator->multiplicativeOperatorClicked(Calculator::Division); });

    // Unary operators
    connect(ui->buttonSqrt, &QPushButton::clicked, [=](){ calculator->unaryOperatorClicked(Calculator::SquareRoot); });
    connect(ui->buttonSquare, &QPushButton::clicked, [=](){ calculator->unaryOperatorClicked(Calculator::Square); });
    connect(ui->buttonInverse, &QPushButton::clicked, [=](){ calculator->unaryOperatorClicked(Calculator::Inverse); });

    // Equal
    connect(ui->buttonEqual, &QPushButton::clicked, calculator, &Calculator::equalClicked);

    // Memory buttons
    connect(ui->buttonMC, &QPushButton::clicked, calculator, &Calculator::clearMemory);
    connect(ui->buttonMR, &QPushButton::clicked, calculator, &Calculator::readMemory);
    connect(ui->buttonMS, &QPushButton::clicked, calculator, &Calculator::setMemory);
    connect(ui->buttonMPlus, &QPushButton::clicked, calculator, &Calculator::addToMemory);
}
