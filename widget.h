#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "serialport.h"
#include "tcpserver.h"
#include "tcpclient.h"
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QRadioButton>

class RadioButton : public QRadioButton
{
    Q_OBJECT
public:
    RadioButton(const QString &text,QWidget *parent = 0,int id = 0) : QRadioButton(text, parent)
    {
        this->id = id;
        setAutoExclusive(false);
    }
public:
    int id;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
private:
    void initView();
    void connections();
    void on_open();
    void on_radio(bool checked);
    void getPara(Para *para, int id);
    void setAllEnabled(bool enabled);
private:
    QVector<BaseParent*> bases;
private:
    QSpinBox *com_spin1;
    QSpinBox *baud_spin1;
    QComboBox *parity_combo1;
    QSpinBox *com_spin2;
    QSpinBox *baud_spin2;
    QComboBox *parity_combo2;

    QSpinBox *server_port_spin1;
    QSpinBox *server_port_spin2;

    QLineEdit *client_ip_line1;
    QSpinBox *client_port_spin1;
    QLineEdit *client_ip_line2;
    QSpinBox *client_port_spin2;

    QVector<RadioButton*> radios;

    QPushButton *open_btn;

    QVector<int> fifo;
};

#endif // WIDGET_H
