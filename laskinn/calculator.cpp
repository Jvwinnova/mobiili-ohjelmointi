#include "calculator.h"
#include <QtMath>

Calculator::Calculator(QObject *parent)
    : QObject(parent)
    , sumInMemory(0.0)
    , sumSoFar(0.0)
    , factorSoFar(0.0)
    , waitingForOperand(true)
{
    pendingMultiplicativeOperator = Null;
    pendingAdditiveOperator = Null;
}


QString Calculator::display() {
    return m_display;
}


void Calculator::digitClicked(QString sNumber)
{
    int digitValue = sNumber.toInt();

    if (m_display == "0" && digitValue == 0.0)
        return;

    if (waitingForOperand) {
        m_display.clear();
        waitingForOperand = false;
    }
    m_display += QString::number(digitValue);
    emit displayChanged(m_display);
}


void Calculator::unaryOperatorClicked(Enum_Operations clickedOperator)
{
    double operand = m_display.toDouble();
    double result = 0.0;

    if (clickedOperator == SquareRoot) {
        if (operand < 0.0) {
            abortOperation();
            return;
        }
        result = std::sqrt(operand);
    } else if (clickedOperator == Square) {
        result = std::pow(operand, 2.0);
    } else if (clickedOperator == Inverse) {
        if (operand == 0.0) {
            abortOperation();
            return;
        }
        result = 1.0 / operand;
    }
    m_display = QString::number(result);
    emit displayChanged(m_display);
    waitingForOperand = true;
}


void Calculator::additiveOperatorClicked(Enum_Operations clickedOperator)
{
    double operand = m_display.toDouble();


    if (pendingMultiplicativeOperator != Null) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            abortOperation();
            return;
        }

        m_display = QString::number(factorSoFar);
        emit displayChanged(m_display);
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator = Null;
    }

    if (pendingAdditiveOperator != Null) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            abortOperation();
            return;
        }

        m_display = QString::number(sumSoFar);
        emit displayChanged(m_display);
    } else {
        sumSoFar = operand;
    }

    pendingAdditiveOperator = clickedOperator;
    waitingForOperand = true;
}


void Calculator::multiplicativeOperatorClicked(Enum_Operations clickedOperator)
{
    double operand = m_display.toDouble();

    if (pendingMultiplicativeOperator != Null) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            abortOperation();
            return;
        }

        m_display = QString::number(factorSoFar);
        emit displayChanged(m_display);
    } else {
        factorSoFar = operand;
    }

    pendingMultiplicativeOperator = clickedOperator;
    waitingForOperand = true;
}


void Calculator::equalClicked()
{
    double operand = m_display.toDouble();

    if (pendingMultiplicativeOperator != Null) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            abortOperation();
            return;
        }
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator = Null;
    }
    if (pendingAdditiveOperator != Null) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            abortOperation();
            return;
        }
        pendingAdditiveOperator = Null;
    } else {
        sumSoFar = operand;
    }


    m_display = QString::number(sumSoFar);
    emit displayChanged(m_display);
    sumSoFar = 0.0;
    waitingForOperand = true;
}


void Calculator::pointClicked()
{
    if (waitingForOperand) {

        m_display = "0";
        emit displayChanged(m_display);
    }
    if (!m_display.contains('.')) {

        m_display += tr(".");
        emit displayChanged(m_display);
    }
    waitingForOperand = false;
}


void Calculator::changeSignClicked()
{
    QString text = m_display;
    double value = text.toDouble();

    if (value > 0.0) {
        text.prepend(tr("-"));
    } else if (value < 0.0) {
        text.remove(0, 1);
    }

    m_display = text;
    emit displayChanged(m_display);
}


void Calculator::backspaceClicked()
{
    if (waitingForOperand)
        return;

    QString text = m_display;
    text.chop(1);
    if (text.isEmpty()) {
        text = "0";
        waitingForOperand = true;
    }

    m_display = text;
    emit displayChanged(m_display);
}


void Calculator::clear()
{
    if (waitingForOperand)
        return;


    m_display = "0";
    emit displayChanged(m_display);
    waitingForOperand = true;
}


void Calculator::clearAll()
{
    sumSoFar = 0.0;
    factorSoFar = 0.0;
    pendingAdditiveOperator = Null;
    pendingMultiplicativeOperator = Null;

    m_display = "0";
    emit displayChanged(m_display);
    waitingForOperand = true;
}


void Calculator::clearMemory()
{
    sumInMemory = 0.0;
}

void Calculator::readMemory()
{

    m_display = QString::number(sumInMemory);
    emit displayChanged(m_display);
    waitingForOperand = true;
}

void Calculator::setMemory()
{
    equalClicked();
    sumInMemory = m_display.toDouble();
}

void Calculator::addToMemory()
{
    equalClicked();
    sumInMemory += m_display.toDouble();
}




void Calculator::abortOperation()
{
    clearAll();

    m_display = tr("####");
    emit displayChanged(m_display);
    QString::number(sumInMemory);
}


bool Calculator::calculate(double rightOperand, const Enum_Operations &pendingOperator)
{
    if (pendingOperator == Addition) {
        sumSoFar += rightOperand;
    } else if (pendingOperator == Subtraction) {
        sumSoFar -= rightOperand;
    } else if (pendingOperator == Multiplication) {
        factorSoFar *= rightOperand;
    } else if (pendingOperator == Division) {
        if (rightOperand == 0.0)
            return false;
        factorSoFar /= rightOperand;
    }
    return true;
}
