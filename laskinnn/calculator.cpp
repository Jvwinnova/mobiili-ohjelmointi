#include "calculator.h"
#include <QtMath>

Calculator::Calculator(QObject *parent)
    : QObject(parent),
    sumInMemory(0.0),
    sumSoFar(0.0),
    factorSoFar(0.0),
    waitingForOperand(true),
    m_display("0"),
    pendingAdditiveOperator(Null),
    pendingMultiplicativeOperator(Null)
{
}

// ---------- Display ----------
QString Calculator::display() { return m_display; }

// ---------- Digit ----------
void Calculator::digitClicked(QString sNumber)
{
    int digitValue = sNumber.toInt();
    if (m_display == "0" && digitValue == 0) return;

    if (waitingForOperand) {
        m_display.clear();
        waitingForOperand = false;
    }

    m_display += QString::number(digitValue);
    emit displayChanged(m_display);
}

// ---------- Unary Operators ----------
void Calculator::unaryOperatorClicked(Enum_Operations op)
{
    double operand = m_display.toDouble();
    double result = 0.0;

    switch (op) {
    case SquareRoot:
        if (operand < 0) { abortOperation(); return; }
        result = std::sqrt(operand); break;
    case Square:
        result = std::pow(operand, 2); break;
    case Inverse:
        if (operand == 0) { abortOperation(); return; }
        result = 1.0 / operand; break;
    case Sine:
        result = std::sin(operand); break;
    case Cosine:
        result = std::cos(operand); break;
    case Tangent:
        result = std::tan(operand); break;
    case Ln:
        if (operand <= 0) { abortOperation(); return; }
        result = std::log(operand); break;
    case Log:
        if (operand <= 0) { abortOperation(); return; }
        result = std::log10(operand); break;
    case Factorial:
        if (operand < 0 || operand != floor(operand)) { abortOperation(); return; }
        result = 1;
        for (int i = 1; i <= operand; ++i) result *= i;
        break;
    default:
        return;
    }

    m_display = QString::number(result);
    emit displayChanged(m_display);
    waitingForOperand = true;
}

// ---------- Additive Operators ----------
void Calculator::additiveOperatorClicked(Enum_Operations op)
{
    double operand = m_display.toDouble();

    if (pendingMultiplicativeOperator != Null) {
        if (!calculate(operand, pendingMultiplicativeOperator)) { abortOperation(); return; }
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator = Null;
    }

    if (pendingAdditiveOperator != Null) {
        if (!calculate(operand, pendingAdditiveOperator)) { abortOperation(); return; }
        m_display = QString::number(sumSoFar);
        emit displayChanged(m_display);
    } else {
        sumSoFar = operand;
    }

    pendingAdditiveOperator = op;
    waitingForOperand = true;
}

// ---------- Multiplicative Operators ----------
void Calculator::multiplicativeOperatorClicked(Enum_Operations op)
{
    double operand = m_display.toDouble();

    if (op == Power) {
        sumSoFar = operand;
        pendingMultiplicativeOperator = Power;
        waitingForOperand = true;
        return;
    }

    if (pendingMultiplicativeOperator != Null) {
        if (!calculate(operand, pendingMultiplicativeOperator)) { abortOperation(); return; }
        m_display = QString::number(factorSoFar);
        emit displayChanged(m_display);
    } else {
        factorSoFar = operand;
    }

    pendingMultiplicativeOperator = op;
    waitingForOperand = true;
}

// ---------- Equal ----------
void Calculator::equalClicked()
{
    double operand = m_display.toDouble();

    if (pendingMultiplicativeOperator != Null) {
        if (!calculate(operand, pendingMultiplicativeOperator)) { abortOperation(); return; }
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator = Null;
    }

    if (pendingAdditiveOperator != Null) {
        if (!calculate(operand, pendingAdditiveOperator)) { abortOperation(); return; }
        pendingAdditiveOperator = Null;
    } else {
        sumSoFar = operand;
    }

    m_display = QString::number(sumSoFar);
    emit displayChanged(m_display);
    sumSoFar = 0.0;
    waitingForOperand = true;
}

// ---------- Decimal Point ----------
void Calculator::pointClicked()
{
    if (waitingForOperand) {
        m_display = "0";
        emit displayChanged(m_display);
    }

    if (!m_display.contains('.')) {
        m_display += ".";
        emit displayChanged(m_display);
    }

    waitingForOperand = false;
}

// ---------- Change Sign ----------
void Calculator::changeSignClicked()
{
    double value = m_display.toDouble();

    if (value > 0.0)
        m_display.prepend("-");
    else if (value < 0.0)
        m_display.remove(0, 1);

    emit displayChanged(m_display);
}

// ---------- Backspace ----------
void Calculator::backspaceClicked()
{
    if (waitingForOperand) return;

    m_display.chop(1);
    if (m_display.isEmpty()) {
        m_display = "0";
        waitingForOperand = true;
    }

    emit displayChanged(m_display);
}

// ---------- Clear ----------
void Calculator::clear()
{
    m_display = "0";
    emit displayChanged(m_display);
    waitingForOperand = true;
}

// ---------- Clear All ----------
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

// ---------- Memory ----------
void Calculator::clearMemory() { sumInMemory = 0.0; }
void Calculator::readMemory() { m_display = QString::number(sumInMemory); emit displayChanged(m_display); waitingForOperand = true; }
void Calculator::setMemory() { equalClicked(); sumInMemory = m_display.toDouble(); }
void Calculator::addToMemory() { equalClicked(); sumInMemory += m_display.toDouble(); }

// ---------- Abort Operation ----------
void Calculator::abortOperation()
{
    clearAll();
    m_display = "####";
    emit displayChanged(m_display);
}

// ---------- Calculate Helper ----------
bool Calculator::calculate(double rightOperand, const Enum_Operations &op)
{
    if (op == Addition)
        sumSoFar += rightOperand;
    else if (op == Subtraction)
        sumSoFar -= rightOperand;
    else if (op == Multiplication)
        factorSoFar *= rightOperand;
    else if (op == Division) {
        if (rightOperand == 0.0) return false;
        factorSoFar /= rightOperand;
    }
    else if (op == Power)
        factorSoFar = std::pow(factorSoFar, rightOperand);

    return true;
}
