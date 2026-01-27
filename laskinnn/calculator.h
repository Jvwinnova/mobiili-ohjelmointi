#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QString>

class Calculator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString display READ display NOTIFY displayChanged)



public:
    explicit Calculator(QObject *parent = nullptr);

    enum Enum_Operations {
        Null,
        Square,
        SquareRoot,
        Inverse,
        Multiplication,
        Division,
        Subtraction,
        Addition,
        Sine,
        Cosine,
        Tangent,
        Ln,
        Log,
        Factorial,
        Power
    };
    Q_ENUM(Enum_Operations)

public slots:
    void digitClicked(QString clickedOperator);
    void unaryOperatorClicked(Enum_Operations clickedOperator);
    void additiveOperatorClicked(Enum_Operations clickedOperator);
    void multiplicativeOperatorClicked(Enum_Operations clickedOperator);

    void equalClicked();
    void pointClicked();
    void changeSignClicked();
    void backspaceClicked();
    void clear();
    void clearAll();
    void clearMemory();
    void readMemory();
    void setMemory();
    void addToMemory();

    QString display();

signals:
    void displayChanged(QString);

private:
    void abortOperation();
    bool calculate(double rightOperand, const Enum_Operations &pendingOperator);

    double sumInMemory;
    double sumSoFar;
    double factorSoFar;
    Enum_Operations pendingAdditiveOperator;
    Enum_Operations pendingMultiplicativeOperator;
    bool waitingForOperand;

    QString m_display;
};

#endif // CALCULATOR_H
