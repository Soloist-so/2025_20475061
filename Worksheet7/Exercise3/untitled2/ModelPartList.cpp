/**     @fileModelPartList.h
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model part list that will be used to create the trewview.
  *
  *     P Evans 2022
  */

#include "ModelPartList.h"
#include "ModelPart.h"
#include <QIcon>
#include <QPixmap>
#include <QColor>

ModelPartList::ModelPartList( const QString& data, QObject* parent ) : QAbstractItemModel(parent) {
    /* Have option to specify number of visible properties for each item in tree - the root item
     * acts as the column headers
     */
    rootItem = new ModelPart({ tr("Part"), tr("Visible?"), tr("Colour") });
}

QVariant ModelPartList::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    ModelPart* item = static_cast<ModelPart*>(index.internalPointer());

    // Colour square in column 2
    if (role == Qt::DecorationRole && index.column() == 2) {
        QColor c(item->getColourR(), item->getColourG(), item->getColourB());
        QPixmap pix(16, 16);
        pix.fill(c);
        return QIcon(pix);
    }

    // Checkbox in column 1
    if (role == Qt::CheckStateRole && index.column() == 1) {
        const bool vis = (item->data(1).toString().toLower() == "true");
        return vis ? Qt::Checked : Qt::Unchecked;
    }

    // Display text
    if (role == Qt::DisplayRole) {
        if (index.column() == 1 || index.column() == 2)
            return QVariant(); // no text for checkbox + colour columns
        return item->data(index.column());
    }

    return QVariant();
}

ModelPartList::~ModelPartList() {
    delete rootItem;
}


int ModelPartList::columnCount( const QModelIndex& parent ) const {
    Q_UNUSED(parent);

    return rootItem->columnCount();
}



Qt::ItemFlags ModelPartList::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags f = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if (index.column() == 1)
        f |= Qt::ItemIsUserCheckable;

    if (index.column() == 0)
        f |= Qt::ItemIsEditable;

    return f;
}



QVariant ModelPartList::headerData( int section, Qt::Orientation orientation, int role ) const {
    if( orientation == Qt::Horizontal && role == Qt::DisplayRole )
        return rootItem->data( section );

    return QVariant();
}

QModelIndex ModelPartList::index(int row, int column, const QModelIndex& parent) const
{
    ModelPart* parentItem;

    if (!parent.isValid() || !hasIndex(row, column, parent))
        parentItem = rootItem;
    else
        parentItem = static_cast<ModelPart*>(parent.internalPointer());

    ModelPart* childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}


QModelIndex ModelPartList::parent( const QModelIndex& index ) const {
    if (!index.isValid())
        return QModelIndex();

    ModelPart* childItem = static_cast<ModelPart*>(index.internalPointer());
    ModelPart* parentItem = childItem->parentItem();

    if( parentItem == rootItem )
        return QModelIndex();

    return createIndex( parentItem->row(), 0, parentItem );
}


int ModelPartList::rowCount( const QModelIndex& parent ) const {
    ModelPart* parentItem;
    if( parent.column() > 0 )
        return 0;

    if( !parent.isValid() )
        parentItem = rootItem;
    else
        parentItem = static_cast<ModelPart*>(parent.internalPointer());

    return parentItem->childCount();
}


ModelPart* ModelPartList::getRootItem() {
    return rootItem; 
}



QModelIndex ModelPartList::appendChild(QModelIndex& parent, const QList<QVariant>& data) {      
    ModelPart* parentPart;

    if (parent.isValid())
        parentPart = static_cast<ModelPart*>(parent.internalPointer());
    else {
        parentPart = rootItem;
        parent = createIndex(0, 0, rootItem );
    }

    beginInsertRows( parent, rowCount(parent), rowCount(parent) ); 

    ModelPart* childPart = new ModelPart( data, parentPart );

    parentPart->appendChild(childPart);

    QModelIndex child = createIndex(0, 0, childPart);

    endInsertRows();

    emit layoutChanged();

    return child;
}
bool ModelPartList::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
        return false;

    ModelPart* item = static_cast<ModelPart*>(index.internalPointer());

    if (role == Qt::CheckStateRole && index.column() == 1) {
        bool vis = (value.toInt() == Qt::Checked);

        item->set(1, vis ? "true" : "false");
        item->setVisible(vis);

        emit dataChanged(index, index, {Qt::CheckStateRole, Qt::DisplayRole});
        return true;
    }

    if (role == Qt::EditRole && index.column() == 0) {
        item->set(0, value);
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
        return true;
    }

    return false;
}

