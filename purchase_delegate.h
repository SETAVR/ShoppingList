#ifndef PURCHASE_DELEGATE_H
#define PURCHASE_DELEGATE_H

#include <QStyledItemDelegate>

// No Q_OBJECT: this class only overrides virtual methods, without new signals.
class PurchaseDelegate : public QStyledItemDelegate
{
public:
    explicit PurchaseDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
};

#endif
