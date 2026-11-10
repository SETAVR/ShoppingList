#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QStringListModel;
class QListView;
class QLineEdit;
class QLabel;
class QPushButton;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void addItem();
    void removeSelected();
    void clearAll();

private:
    void setupUi();
    void refreshStatus();

    QStringListModel *m_model = nullptr;
    QListView *m_view = nullptr;
    QLineEdit *m_input = nullptr;
    QLabel *m_status = nullptr;
    QPushButton *m_removeButton = nullptr;
};

#endif
