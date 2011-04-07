// @HEADER
// ***********************************************************************
// 
//         Optika: A Tool For Developing Parameter Obtaining GUIs
//                Copyright (2009) Sandia Corporation
// 
// Under terms of Contract DE-AC04-94AL85000, with Sandia Corporation, the 
// U.S. Government retains certain rights in this software.
// 
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
// Questions? Contact Kurtis Nusbaum (klnusbaum@gmail.com) 
// 
// ***********************************************************************
// @HEADER
#include <iostream>
#include "Optika_treemodel.hpp"
#include "Teuchos_XMLParameterListWriter.hpp"
#include <QTextStream>

namespace Optika{


TreeModel::TreeModel(RCP<ParameterList> validParameters, QString saveFileName, QObject *parent):
	QAbstractItemModel(parent),
	dependencies(false),
	validParameters(validParameters)
{
	basicSetup(saveFileName);
}

TreeModel::TreeModel(RCP<ParameterList> validParameters, RCP<DependencySheet> dependencySheet,
     QString saveFileName, QObject *parent):
	 QAbstractItemModel(parent),
	 dependencies(true),
	 validParameters(validParameters),
	 dependencySheet(dependencySheet)
{
	basicSetup(saveFileName);
	connect(this, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), 
		this, SLOT(dataChangedListener(const QModelIndex&, const QModelIndex&)));
}

TreeModel::~TreeModel() {
	delete rootItem;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const {
	if(!index.isValid()){
		return QVariant();
	}
	if(role != Qt::DisplayRole && role != Qt::ToolTipRole){
		return QVariant();
	}
	TreeItem *item = (TreeItem*)(index.internalPointer());
	return item->data(index.column(), role);
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const {
	if(!index.isValid()){
		return Qt::ItemIsEnabled;
	}
	else if(index.column() == 1){
		return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
	}
	else{
		return QAbstractItemModel::flags(index);
	}
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const{
	if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
		return rootItem->data(section);
	}
	return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const{
	if(!hasIndex(row, column, parent)){
		return QModelIndex();
	}
	TreeItem *parentItem;

	if(!parent.isValid()){
		parentItem = rootItem;
	}
	else{
		parentItem = (TreeItem*)(parent.internalPointer());
	}
	TreeItem *childItem = parentItem->child(row);

	if(childItem){
		return createIndex(row, column, childItem);
	}
	return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const{
	if(!index.isValid()){
		return QModelIndex();
	}

	TreeItem *childItem = (TreeItem*)(index.internalPointer());
	TreeItem *parentItem = childItem->parent();

	if(parentItem == rootItem){
		return QModelIndex();
	}

	return createIndex(parentItem->row(), 0, parentItem);
}

bool TreeModel::setData(const QModelIndex & index, const QVariant &value, int role){
	if(index.isValid() && index.column() == 1 && role == Qt::EditRole){
		TreeItem *item = (TreeItem*)(index.internalPointer());
		if(item->changeValue(value)){
			emit dataChanged(index, index);
		}
		return true;
	}
	return false;
}

int TreeModel::rowCount(const QModelIndex &parent) const{
	TreeItem *parentItem;
	if(parent.column() > 0){
		return 0;
	}

	if (!parent.isValid()){
		parentItem = rootItem;
	}
	else{
		parentItem = (TreeItem*)(parent.internalPointer());
	}

	return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const {
	if(parent.isValid()){
		return ((TreeItem*)(parent.internalPointer()))->columnCount();
	}
	else{
		return rootItem->columnCount();
	}
}

void TreeModel::issueInitilizationSignals(){
	for(
    DependencySheet::DepSet::const_iterator it = 
      dependencySheet->depBegin(); 
    it != dependencySheet->depEnd(); 
    ++it)
  {
    for(
      Dependency::ConstParameterEntryList::const_iterator it2=
        (*it)->getDependees().begin();
      it2 != (*it)->getDependees().end();
      ++it2)
    {
		  QModelIndex dependeeIndex = findParameterEntryIndex(*it2);
      TEST_FOR_EXCEPTION(!dependeeIndex.isValid(), std::logic_error,
        "Could not find the index of the dependee. This is an internal error. "
        "Please contact the Optika team.");
		  dataChangedListener(dependeeIndex, dependeeIndex);
        
    }
	}
}

void TreeModel::printOut() const{
	rootItem->printOut();
}

bool TreeModel::writeOutput(QString fileName){
	QFile *file = new QFile(fileName);
	if(!file->open(QIODevice::WriteOnly)){
		return false;
	}
	std::ofstream outputFile;
	XMLParameterListWriter plWriter;
	XMLObject xmlOutput = 
    plWriter.toXML(*validParameters, dependencySheet);
	QTextStream outStream(file);
	outStream << QString::fromStdString(xmlOutput.toString());
	file->close();
	delete file;
	saved = true;
	saveFileName = fileName;
	return true;
}

void TreeModel::readInput(QString fileName){
	QFile *file = new QFile(fileName);
	file->open(QIODevice::ReadOnly);
	QXmlStreamReader xmlReader(file);
	while(!xmlReader.atEnd()){
		xmlReader.readNext();
		if(xmlReader.name().toString() == "Parameter" && xmlReader.isStartElement()){
			QList<QModelIndex> matches = match(index(0,0), Qt::DisplayRole, xmlReader.attributes().value("name").toString(),
							   1, Qt::MatchExactly | Qt::MatchRecursive);
			if(matches.size() !=0){
				QModelIndex valueToEdit = matches.at(0).sibling(matches.at(0).row(), 1);
				setData(valueToEdit,xmlReader.attributes().value("value").toString(), Qt::EditRole);
			}
		}
	}
	file->close();
	delete file;
	saved = true;
	saveFileName = fileName;
}

QString TreeModel::getSaveFileName(){
	return saveFileName;
}

bool TreeModel::isSaved(){
	return saved;
}

void TreeModel::reset(){
	delete rootItem;
	QList<QVariant> headers;
	headers  << "Parameter" << "Value" << "Type";
	rootItem = new TreeItem(headers, null, 0, true);	
	validParameters->setParameters(*canonicalList);
	readInParameterList(validParameters, rootItem);
	this->saveFileName = saveFileName;
	if(saveFileName != ""){
		saved = true;
		readInput(saveFileName);
	}
	else{
		saved = false;
	}
	if(dependencies){
		issueInitilizationSignals();
	}
	currentFileNowModified();
}

QString TreeModel::itemType(const QModelIndex &index) const{
	int row = index.row(); 
	QModelIndex itemTypeIndex = index.sibling(row, 2);
	return index.model()->data(itemTypeIndex, Qt::DisplayRole).toString();
}

bool TreeModel::hasDependencies(){
	return dependencies;
}

bool TreeModel::hasValidValue(QModelIndex valueToCheck) const{
	TreeItem *item = static_cast<TreeItem*>(valueToCheck.internalPointer());
	return item->hasValidValue();
}

RCP<const ParameterEntryValidator> TreeModel::getValidator(const QModelIndex &index) const{
	return itemEntry(index)->validator();
}

RCP<const ParameterList> TreeModel::getCurrentParameters(){
	return validParameters;
}

QModelIndexList TreeModel::parameterEntryMatch(const QModelIndex &start,
  const RCP<const ParameterEntry> &parameterEntry) const
{
  QModelIndexList result;
  QModelIndex p = parent(start);
  int from = start.row();
  int to = rowCount(p);

  for (int r = from; (r < to) && (result.size() < 1); ++r) {
    QModelIndex idx = index(r, start.column(), p);
    if (!idx.isValid())
      continue;
    RCP<const ParameterEntry> entry = itemEntry(idx);
    if(entry != null && entry.get() == parameterEntry.get()){
      result.append(idx);
    }  
            
    if (hasChildren(idx)) { // search the hierarchy
      result += 
        parameterEntryMatch(index(0, idx.column(), idx), parameterEntry);
    }
  }
  return result;
}


QModelIndex TreeModel::findParameterEntryIndex(
  RCP<const ParameterEntry> parameterEntry)
{
	QList<QModelIndex> potentialMatches = parameterEntryMatch(
    index(0,0),
    parameterEntry);
  
  if(potentialMatches.size() == 1){
    return potentialMatches.first();
  }
	return QModelIndex();
}


RCP<const ParameterEntry> 
TreeModel::itemEntry(const QModelIndex &index) const{
  if(!index.isValid()){
    return null;
  }
  TreeItem* item = (TreeItem*)index.internalPointer();
  if(item->hasEntry()){
    return item->getEntry();
  }
  else{
    return null;
  }
}

void TreeModel::readInParameterList(RCP<ParameterList> parameterList, TreeItem *parentItem){
	for(ParameterList::ConstIterator itr = parameterList->begin(); itr != parameterList->end(); ++itr){
		std::string name = parameterList->name(itr);
		if(parameterList->isSublist(name)){
			insertParameterList(sublist(parameterList, name), parameterList->getEntryRCP(name), name, parentItem);
		}
		else if(parameterList->isParameter(name)){
			insertParameter(parameterList->getEntryRCP(name), name, parentItem);
		}
	}
}

void TreeModel::insertParameterList(RCP<ParameterList> parameterList, RCP<ParameterEntry> listEntry, 
				    std::string name, TreeItem *parent)
{
	QList<QVariant> values = QList<QVariant>() << QString::fromStdString(name).section("->",-1) << QString("") << listId;

	TreeItem *newList = new TreeItem(values, listEntry, parent);
	parent->appendChild(newList);
	for(ParameterList::ConstIterator itr = parameterList->begin(); itr != parameterList->end(); ++itr){
		std::string name = parameterList->name(itr);
		if(parameterList->isSublist(name)){
			insertParameterList(sublist(parameterList, name), parameterList->getEntryRCP(name), name,  newList);
		}
		else if(parameterList->isParameter(name)){
			insertParameter(parameterList->getEntryRCP(name), name, newList);
		}
	}
}

void TreeModel::insertParameter(RCP<ParameterEntry> parameter, std::string name, TreeItem *parent){
	QList<QVariant> values;
	values.append(QString::fromStdString(name));
	if(parameter->isType<int>()){
		values.append(getValue<int>(*parameter));
		values.append(intId);
	}
	else if(parameter->isType<short>()){
		values.append(getValue<short>(*parameter));
		values.append(shortId);
	}
	/*else if(parameter->isType<long long>()){
		values.append(getValue<long long>(*parameter));
		value.append(longlongId);
	}*/
	else if(parameter->isType<double>()){
		values.append(getValue<double>(*parameter));
		values.append(doubleId);
	}
	else if(parameter->isType<float>()){
		values.append(getValue<float>(*parameter));
		values.append(floatId);
	}
	else if(parameter->isType<bool>()){
		values.append(getValue<bool>(*parameter));
		values.append(boolId);
	}
	else if(parameter->isType<std::string>()){
		values.append(QString::fromStdString(getValue<std::string>(*parameter)));
		values.append(stringId);
	}
	else if(parameter->isArray()){
		QString determinedId = determineArrayType(parameter);
		if( determinedId != unrecognizedId){
			values.append(QString::fromStdString(toString(parameter->getAny())));
			values.append(QString(arrayId + " "+ determinedId));
		}
		else{
			values.append("");
			values.append("");
			parent->appendChild(new TreeItem(values, parameter, parent, true));
			return;
		}
	}
	else{
		values.append("");
		values.append("");
		parent->appendChild(new TreeItem(values, parameter, parent, true));
		return;
	}
	parent->appendChild(new TreeItem(values, parameter, parent));
}

void TreeModel::basicSetup(QString saveFileName){
	QList<QVariant> headers;
	headers  << "Parameter" << "Value" << "Type";
	rootItem = new TreeItem(headers, null, 0);	
	canonicalList = RCP<const ParameterList>(new ParameterList(*validParameters));
	readInParameterList(validParameters, rootItem);
	this->saveFileName = saveFileName;
	if(saveFileName != ""){
		saved = true;
		readInput(saveFileName);
	}
	else{
		saved = false;
	}
}

void TreeModel::checkDependentState(const QModelIndex dependee, RCP<Dependency> dependency){
	QModelIndex dependent;
	Dependency::ParameterEntryList dependents= dependency->getDependents();
	for(Dependency::ParameterEntryList::iterator it = dependents.begin(); it != dependents.end(); ++it ){ 
		dependent = findParameterEntryIndex(*it);
		//if(!is_null(rcp_dynamic_cast<NumberArrayLengthDependency>(dependency))){
    if((*it)->isArray()){
			redrawArray(dependent.sibling(dependent.row(),1));
		}
		else if(!is_null(rcp_dynamic_cast<VisualDependency>(dependency))){
			RCP<VisualDependency> visDep = rcp_static_cast<VisualDependency>(dependency);
			visDep->isDependentVisible() ? emit showData(dependent.row(), dependent.parent()) :
					       emit hideData(dependent.row(), dependent.parent());
		}

		if(!hasValidValue(dependent)){
			QString message = "Because you recently modified the " + data(dependee, Qt::DisplayRole).toString() +
			" parameter, the valid values for the " + data(dependent, Qt::DisplayRole).toString() +
			" parameter have changed.\n\nPlease modify the " +  data(dependent,Qt::DisplayRole).toString() + " value.\n";
			emit badValue(dependent.sibling(dependent.row(), 1), message);
		}
	}
}

void TreeModel::redrawArray(const QModelIndex arrayIndex){
	if(toString(itemEntry(arrayIndex)->getAny()).size() <= 2){
		emit hideData(arrayIndex.row(), arrayIndex.parent());
	}
	else{
		setData(arrayIndex, QString::fromStdString(toString(itemEntry(arrayIndex)->getAny())));
		emit showData(arrayIndex.row(), arrayIndex.parent());
	}
}

void TreeModel::currentFileNowModified(){
	saved = false;
}

void TreeModel::dataChangedListener(const QModelIndex& index1, const QModelIndex& /*index2*/){
	RCP<const ParameterEntry> changedIndexEntry = 
    itemEntry(index1);	
	QModelIndex dependee = index1.sibling(index1.row(), 0);
	if(dependencySheet->hasDependents(changedIndexEntry)){
		RCP<const DependencySheet::DepSet> deps =  
      dependencySheet->getDependenciesForParameter(changedIndexEntry);
		for(
      DependencySheet::DepSet::const_iterator it = deps->begin();
      it != deps->end(); 
      ++it)
    {
			(*it)->evaluate();
			checkDependentState(dependee,*it);
		}
	}
}



}

