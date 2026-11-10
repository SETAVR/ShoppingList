#include "purchase_delegate.h"

#include <QColor>
#include <QDebug>
#include <QLineEdit>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QStyle>

PurchaseDelegate::PurchaseDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    qDebug() << "[DELEGATE] Created PurchaseDelegate";
}

QWidget *PurchaseDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &,
                                       const QModelIndex &index) const
{
    // The view owns the editor through its supplied parent.
    qDebug() << "[EDITOR] createEditor, row:" << index.row();
    return new QLineEdit(parent);
}

void PurchaseDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (auto *lineEdit = qobject_cast<QLineEdit *>(editor)) {
        const QString value = index.data(Qt::EditRole).toString();
        lineEdit->setText(value);
        qDebug() << "[EDITOR] setEditorData:" << value;
    }
}

void PurchaseDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    if (auto *lineEdit = qobject_cast<QLineEdit *>(editor)) {
        // Change the model, not the view: dataChanged triggers the update.
        const QString value = lineEdit->text();
        model->setData(index, value, Qt::EditRole);
        qDebug() << "[EDITOR] setModelData, saved:" << value;
    }
}

void PurchaseDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

void PurchaseDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
    const int gutter = 18;
    const int dot = 6;
    const bool selected = option.state & QStyle::State_Selected;

    // Restore every painter setting, including those changed by the overlay.
    painter->save();
    if (selected)
        painter->fillRect(option.rect, option.palette.brush(QPalette::Highlight));

    // Reserve a left gutter so the marker does not overlap the text.
    QStyleOptionViewItem textOption(option);
    textOption.rect.adjust(gutter, 0, 0, 0);
    QStyledItemDelegate::paint(painter, textOption, index);

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setBrush(selected ? option.palette.color(QPalette::HighlightedText)
                               : QColor("#2ecc71"));
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(option.rect.left() + (gutter - dot) / 2,
                         option.rect.center().y() - dot / 2, dot, dot);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(0, 0, 0, 40), 1));
    painter->drawRect(option.rect.adjusted(1, 1, -1, -1));
    painter->restore();

    qDebug() << "[PAINT] row:" << index.row()
             << "text:" << index.data(Qt::DisplayRole).toString();
}
