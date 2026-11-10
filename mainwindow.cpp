#include "mainwindow.h"
#include "purchase_delegate.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QItemSelectionModel>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QStringListModel>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setupUi();
    refreshStatus();
    qDebug() << "[INIT] Data belongs to the model; the view displays it";
}

MainWindow::~MainWindow()
{
    qDebug() << "[DESTROY] MainWindow";
}

void MainWindow::setupUi()
{
    setWindowTitle(QStringLiteral("Менеджер списка покупок — Model-View"));
    resize(520, 560);
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(16, 16, 16, 16);
    root->setSpacing(10);
    root->addWidget(new QLabel(QStringLiteral("<b>Список покупок</b>"), this));

    m_model = new QStringListModel(this);
    m_model->setObjectName("shoppingModel");
    m_model->setStringList({QStringLiteral("Молоко"), QStringLiteral("Хлеб"),
                            QStringLiteral("Яблоки")});
    m_view = new QListView(this);
    m_view->setObjectName("shoppingView");
    m_view->setModel(m_model);
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_view->setEditTriggers(QAbstractItemView::DoubleClicked);
    m_view->setItemDelegate(new PurchaseDelegate(m_view));
    root->addWidget(m_view, 1);

    auto *inputRow = new QHBoxLayout;
    m_input = new QLineEdit(this);
    m_input->setObjectName("newItem");
    m_input->setPlaceholderText(QStringLiteral("Новый товар…"));
    auto *addButton = new QPushButton(QStringLiteral("Добавить"), this);
    addButton->setObjectName("addButton");
    inputRow->addWidget(m_input, 1);
    inputRow->addWidget(addButton);
    root->addLayout(inputRow);

    auto *buttonRow = new QHBoxLayout;
    m_removeButton = new QPushButton(QStringLiteral("Удалить выделенное"), this);
    m_removeButton->setObjectName("removeButton");
    auto *clearButton = new QPushButton(QStringLiteral("Очистить"), this);
    clearButton->setObjectName("clearButton");
    buttonRow->addWidget(m_removeButton);
    buttonRow->addWidget(clearButton);
    buttonRow->addStretch();
    root->addLayout(buttonRow);
    m_status = new QLabel(this);
    m_status->setObjectName("status");
    m_status->setWordWrap(true);
    root->addWidget(m_status);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::addItem);
    connect(m_input, &QLineEdit::returnPressed, this, &MainWindow::addItem);
    connect(m_removeButton, &QPushButton::clicked, this, &MainWindow::removeSelected);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearAll);
    connect(m_view->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::refreshStatus);
    connect(m_view->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::refreshStatus);
    // Counts follow the model signals, including direct changes to the model.
    connect(m_model, &QStringListModel::rowsInserted, this, &MainWindow::refreshStatus);
    connect(m_model, &QStringListModel::rowsRemoved, this, &MainWindow::refreshStatus);
    connect(m_model, &QStringListModel::modelReset, this, &MainWindow::refreshStatus);
}

void MainWindow::addItem()
{
    const QString text = m_input->text().trimmed();
    if (text.isEmpty()) {
        qDebug() << "[CRUD] addItem: empty input ignored";
        return;
    }
    const int row = m_model->rowCount();
    m_model->insertRow(row);
    m_model->setData(m_model->index(row, 0), text, Qt::EditRole);
    m_input->clear();
    m_input->setFocus();
    qDebug() << "[CRUD] addItem:" << text << "rows:" << m_model->rowCount();
}

void MainWindow::removeSelected()
{
    const QModelIndex index = m_view->currentIndex();
    if (!index.isValid() || !m_view->selectionModel()->isSelected(index)) {
        qDebug() << "[CRUD] removeSelected: no selection";
        return;
    }
    const int row = index.row();
    m_model->removeRow(row);
    qDebug() << "[CRUD] removeSelected, row:" << row << "remaining:" << m_model->rowCount();
}

void MainWindow::clearAll()
{
    m_model->setStringList({});
    qDebug() << "[CRUD] clearAll: model reset";
}

void MainWindow::refreshStatus()
{
    const QModelIndex current = m_view->currentIndex();
    const bool selected = current.isValid() && m_view->selectionModel()->isSelected(current);
    m_status->setText(QStringLiteral("Элементов в модели: %1 | выделение: %2")
                          .arg(m_model->rowCount())
                          .arg(selected ? QStringLiteral("есть") : QStringLiteral("нет")));
    m_removeButton->setEnabled(selected);
}
