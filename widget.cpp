#include "widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    initView();
    connections();
}

Widget::~Widget()
{

}

void Widget::initView()
{
    RadioButton *radio1 = new RadioButton("串口1:",this,radios.size());
    radios.append(radio1);
    com_spin1 = new QSpinBox(this);
    com_spin1->setPrefix("COM");
    baud_spin1 = new QSpinBox(this);
    baud_spin1->setPrefix("波特率:");
    baud_spin1->setMaximum(10000000);
    parity_combo1 = new QComboBox(this);
    parity_combo1->addItems(QStringList()<<"无"<<"奇"<<"偶");
    QHBoxLayout *hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(radio1);
    hLayout1->addWidget(com_spin1);
    hLayout1->addWidget(baud_spin1);
    hLayout1->addWidget(parity_combo1);
    RadioButton *radio2 = new RadioButton("串口2:",this,radios.size());
    radios.append(radio2);
    com_spin2 = new QSpinBox(this);
    com_spin2->setPrefix("COM");
    baud_spin2 = new QSpinBox(this);
    baud_spin2->setPrefix("波特率:");
    baud_spin2->setMaximum(10000000);
    parity_combo2 = new QComboBox(this);
    parity_combo2->addItems(QStringList()<<"无"<<"奇"<<"偶");
    QHBoxLayout *hLayout2 = new QHBoxLayout;
    hLayout2->addWidget(radio2);
    hLayout2->addWidget(com_spin2);
    hLayout2->addWidget(baud_spin2);
    hLayout2->addWidget(parity_combo2);
    RadioButton *radio3 = new RadioButton("服务器1:",this,radios.size());
    radios.append(radio3);
    server_port_spin1 = new QSpinBox(this);
    server_port_spin1->setPrefix("端口:");
    server_port_spin1->setMaximum(65535);
    QHBoxLayout *hLayout3 = new QHBoxLayout;
    hLayout3->addWidget(radio3);
    hLayout3->addWidget(server_port_spin1);
    RadioButton *radio4 = new RadioButton("服务器2:",this,radios.size());
    radios.append(radio4);
    server_port_spin2 = new QSpinBox(this);
    server_port_spin2->setPrefix("端口:");
    server_port_spin2->setMaximum(65535);
    QHBoxLayout *hLayout4 = new QHBoxLayout;
    hLayout4->addWidget(radio4);
    hLayout4->addWidget(server_port_spin2);
    RadioButton *radio5 = new RadioButton("客户端1:",this,radios.size());
    radios.append(radio5);
    client_ip_line1 = new QLineEdit(this);
    client_ip_line1->setInputMask("000.000.000.000; ");
    client_port_spin1 = new QSpinBox(this);
    client_port_spin1->setPrefix("端口:");
    client_port_spin1->setMaximum(65535);
    QHBoxLayout *hLayout5 = new QHBoxLayout;
    hLayout5->addWidget(radio5);
    hLayout5->addWidget(client_ip_line1);
    hLayout5->addWidget(client_port_spin1);
    RadioButton *radio6 = new RadioButton("客户端2:",this,radios.size());
    radios.append(radio6);
    client_ip_line2 = new QLineEdit(this);
    client_ip_line2->setInputMask("000.000.000.000; ");
    client_port_spin2 = new QSpinBox(this);
    client_port_spin2->setPrefix("端口:");
    client_port_spin2->setMaximum(65535);
    QHBoxLayout *hLayout6 = new QHBoxLayout;
    hLayout6->addWidget(radio6);
    hLayout6->addWidget(client_ip_line2);
    hLayout6->addWidget(client_port_spin2);


    open_btn = new QPushButton("打开",this);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addLayout(hLayout3);
    vLayout->addLayout(hLayout4);
    vLayout->addLayout(hLayout5);
    vLayout->addLayout(hLayout6);
    vLayout->addWidget(open_btn);
    setLayout(vLayout);

    BaseParent *serial1 = new SerialPort(this);
    BaseParent *serial2 = new SerialPort(this);
    BaseParent *server1 = new TcpServer(this);
    BaseParent *server2 = new TcpServer(this);
    BaseParent *client1 = new TcpClient(this);
    BaseParent *client2 = new TcpClient(this);
    bases.append(serial1);
    bases.append(serial2);
    bases.append(server1);
    bases.append(server2);
    bases.append(client1);
    bases.append(client2);
}

void Widget::connections()
{
    connect(open_btn,&QPushButton::clicked,this,&Widget::on_open);
    for(int i = 0 ; i < radios.size(); i++) {
        connect(radios[i], &RadioButton::clicked,this,&Widget::on_radio);
    }
}

void Widget::on_open()
{
    QVector<int> ids;
    for(int i = 0; i< radios.size();i++) {
        if(radios[i]->isChecked()) {
            ids.append(i);
        }
    }
    if(ids.size() != 2) {
        QMessageBox::warning(this,"提示","通道数不等于2!");
        return;
    }
    if(open_btn->text() == "打开") {
        Para para;
        getPara(&para, ids[0]);
        open_btn->setEnabled(false);
        if(!bases[ids[0]]->Open(&para)) {
            QMessageBox::information(this,"提示",QString("通道%1打不开!").arg(ids[0]+1));
            goto label1;
        }
        getPara(&para, ids[1]);
        if(!bases[ids[1]]->Open(&para)) {
            QMessageBox::information(this,"提示",QString("通道%1打不开!").arg(ids[1]+1));
            bases[ids[0]]->Close();
            goto label1;
        }
        connect(bases[ids[0]],&BaseParent::sig_recv_data,bases[ids[1]],&BaseParent::slot_send_data);
        connect(bases[ids[1]],&BaseParent::sig_recv_data,bases[ids[0]],&BaseParent::slot_send_data);

        open_btn->setText("关闭");
        setAllEnabled(false);
    } else {
        disconnect(bases[ids[0]],&BaseParent::sig_recv_data,bases[ids[1]],&BaseParent::slot_send_data);
        disconnect(bases[ids[1]],&BaseParent::sig_recv_data,bases[ids[0]],&BaseParent::slot_send_data);
        bases[ids[0]]->Close();
        bases[ids[1]]->Close();
        open_btn->setText("打开");
        setAllEnabled(true);
    }
label1:
    open_btn->setEnabled(true);
}

void Widget::on_radio(bool checked)
{
    RadioButton *radio = (RadioButton*)sender();
    int sum = 0;
    for(int i = 0; i< radios.size();i++) {
        if(radios[i]->isChecked())
            sum++;
    }
    if(!checked) {
        int idx = fifo.indexOf(radio->id);
        if(idx >= 0) {
            fifo.removeAt(idx);
        }
    } else {
        if(sum == 3) {
           int id = fifo[0];
           fifo.removeAt(0);
           radios[id]->setChecked(false);
        }
        fifo.append(radio->id);
    }
}

void Widget::getPara(Para *para, int id)
{
    QSerialPort::Parity parity[] = {QSerialPort::NoParity, QSerialPort::OddParity, QSerialPort::EvenParity};
    switch (id) {
    case 0:
        para->com = com_spin1->value();
        para->baud = baud_spin1->value();
        para->parity = parity[parity_combo1->currentIndex()];
        break;
    case 1:
        para->com = com_spin2->value();
        para->baud = baud_spin2->value();
        para->parity = parity[parity_combo2->currentIndex()];
        break;
    case 2:
        para->server_port = server_port_spin1->value();
        break;
    case 3:
        para->server_port = server_port_spin2->value();
        break;
    case 4:
        para->ip = client_ip_line1->text();
        para->client_port = client_port_spin1->value();
        break;
    default://5
        para->ip = client_ip_line2->text();
        para->client_port = client_port_spin2->value();
        break;
    }
}

void Widget::setAllEnabled(bool enabled)
{
    com_spin1->setEnabled(enabled);
    baud_spin1->setEnabled(enabled);
    parity_combo1->setEnabled(enabled);
    com_spin2->setEnabled(enabled);
    baud_spin2->setEnabled(enabled);
    parity_combo2->setEnabled(enabled);
    server_port_spin1->setEnabled(enabled);
    server_port_spin2->setEnabled(enabled);
    client_ip_line1->setEnabled(enabled);
    client_port_spin1->setEnabled(enabled);
    client_ip_line2->setEnabled(enabled);
    client_port_spin2->setEnabled(enabled);
    for(int i = 0; i < radios.size(); i++) {
        radios[i]->setEnabled(enabled);
    }
}
