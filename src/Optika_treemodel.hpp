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
#ifndef OPTIKA_MODEL_HPP_
#define OPTIKA_MODEL_HPP_
/*
 * Optika_model.hpp
 *
 *  Created on: Apr 21, 2009
 *      Author: Kurtis Nusbaum
 */
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QDir>
#include "Optika_treeitem.hpp"
#include "Optika_DependencySheet.hpp"
#include "Optika_StandardDependencies.hpp"


namespace Optika{

class DependencySheet;
class TreeItem;

/**
 * TreeModel is a type of QAbstractItemModel that has a Tree like structure.
 *
 * Note: For all undocumented functions, please refer to the Qt api. They will have a good desciption.
 */
class TreeModel : public QAbstractItemModel{
	Q_OBJECT
public:
	/**
	 * Constructs the TreeModel.
	 * 
	 * @param validParameters A list of parameters for which the users must enter values. Note the Parameter List will be edited.
	 * All user input will be stored in it.
	 * @param saveFileName Name of a save file used in a previous attempt to get values for the validParameters ParameterList.
	 * @param parent The parent object.
	 */
	TreeModel(Teuchos::RCP<Teuchos::ParameterList> validParameters, QString saveFileName=QString(), QObject *parent=0);

	/**
	 * Constructs the TreeModel.
	 * 
	 * @param validParameters A list of parameters for which the users must enter values.
	 * @param dependencySheet A sheet listing any dependencies between parameters in the validParameters
	 * ParameterList.
	 * @param saveFileName Name of a save file used in a previous attempt to get values for the validParameters ParameterList.
	 * @param parent The parent object.
	 */
	TreeModel(Teuchos::RCP<Teuchos::ParameterList> validParameters, Teuchos::RCP<Optika::DependencySheet> dependencySheet,
		  QString saveFileName=QString(), QObject *parent=0);

	/**
	 *
	 * Deconstructor for the TreeModel.
	 */
	~TreeModel();

	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	/**
	 * If this TreeModel has a dependent Parameter List, then all the depndencies need to be evaluated before the Parameter List may be displayed.
	 * Certain items might need to be hidden before the user even starts entering data. 
	 * This function goes through all of the depndees in the Dependent Parameter List and issues a signal saying they've changed.
	 * They really haven't changed yet, but this allows all the depndencies to be evaluated and any initial visual settings to be
	 * displayed correctly.
	 */
	void issueInitilizationSignals();

	/**
	 * Prints out the model.
	 */
	void printOut() const;

	/**
	 * Writes out the state of the current parameters in xml format.
	 *
	 * @param fileName The name of the file to which the TreeModel should write the XML output.
	 */
	bool writeOutput(QString fileName);

	/**
	 * Reads an xml file that describes the state of current parameters in xml format.
	 *
	 * @param fileName The name of the file from which the TreeModel should read parameter values.
	 */
	void readInput(QString fileName);

	/**
	 * Gets the name of the save file with which the TreeModel is associated.
	 * If the TreeModel has yet to be saved and thus has no save file associated with it, the funtion will return an empty string.
	 *
	 * @return The name of the save file with which the TreeModel is associated.
	 */
	QString getSaveFileName();

	/**
	 * Determines wether or not the current state of TreeModel has been saved.
	 *
	 * @return True if the current state of the TreeModel has been saved. False otherwise.
	 */
	bool isSaved();

	/**
	 * Resets all the inputs to their default values.
	 */
	void reset();

	/**
	 * Returns the type of item located at the specified QModelIndex.
	 *
	 * @param index The index of the TreeItem.
	 * @return The type of the item at the index.
	 */
	QString itemType(const QModelIndex &index) const;

	/**
	 * Determines whether or not a Dependent Parameter List is being used in the TreeModel.
	 *
	 * @return True if the TreeModel has dependencies, false otherwise.
	 */
	bool hasDependencies();

	/**
	 * Determines whether or not the value at the valueToCheck 
	 * is valid.
	 *
	 * @param valueToCheck The index of the item whose valididty
	 * is in questions.
	 * @return True if the value at index is valid, false otherwise.
	 */
	bool hasValidValue(QModelIndex valueToCheck) const;

	/**
	 * Gets the validator for a particular TreeItem.
	 *
	 * @param index The index of the TreeItem whose validators is sought.
	 * @return The validator at the given index.
	 */
	Teuchos::RCP<const Teuchos::ParameterEntryValidator> getValidator(const QModelIndex &index) const;

	/**
	 * Gets the array for a particular TreeItem.
	 *
	 * @param index The index of the TreeItem whose arrays is sought.
	 * @return The array at the given index.
	 */
	template <class S>
	Teuchos::Array<S> getArray(const QModelIndex& index){
		return Teuchos::any_cast<Teuchos::Array<S> >(itemEntry(index)->getAny()); 
	}

	/**
	 * Get a ParameterList containing all of the parameters at their current settings.
	 *
	 * @return A ParameterList containing all of the parameters at their current settings.
	 */
	Teuchos::RCP<const Teuchos::ParameterList> getCurrentParameters();
signals:
	/**
	 * Emitted when a row should be hidden.
	 *
	 * @param row The row of the item that should be hidden.
	 * @param parent The parent of the item that should be hidden.
	 */
	void hideData(int row, const QModelIndex& parent);

	/**
	 * Emitted when a row should be shown.
	 *
	 * @param row The row of the item that should be shown.
	 * @param parent The parent of the item that should be shown.
	 */
	void showData(int row, const QModelIndex& parent);

	/**
	 * Emitted when it has been determined that a TreeItem no longer has a valid value.
	 *
	 * @param badItem The index of the item that now has a bad value.
	 * @param message A message describing what happened to cause the
	 * item to obtain an invalid value.
	 */
	void badValue(QModelIndex badItem,  QString message);

private:
	/**
	 * Whether or not the model has been saved since it was last modified.
	 */
	bool saved;

	/**
	 * Whether or not the model has any dependencies.
	 */
	bool dependencies;

	/**
	 * The name of the savefile associated with the model.
	 */
	QString saveFileName;

	/**
	 * The Root item of the model.
	 */
	TreeItem *rootItem;

	/**
	 * The list of valid parameters.
	 */
	Teuchos::RCP<Teuchos::ParameterList> validParameters;

	/**
	 * A canonical list of what the validParameters were when they were first
	 * passed to the treemodel. Used by the reset function.
	 */
	Teuchos::RCP<const Teuchos::ParameterList> canonicalList;

	/**
	 * The dependency sheet being used to determine any
	 * depdendencies between parameters.
	 */
	Teuchos::RCP<Optika::DependencySheet> dependencySheet;

	/**
	 * Finds the index of a particular parameter entry.
	 *
	 * @param parameterEntry The ParameterEntry whose index is being sought.
	 * @param parameterName The name of the parameter whose index is being sought.
	 */
	QModelIndex findParameterEntryIndex(const Teuchos::ParameterEntry *parameterEntry, std::string parameterName);

	/**
	 * Gets the ParameterEntry object given a QModelIndex.
	 *
	 * @param index Index of the TreeItem for which the ParameterEntry is desired.
	 * @return The ParameterEntry associated with the QModelIndex.
	 */
	const Teuchos::ParameterEntry* itemEntry(const QModelIndex &index) const;

	/**
	 * Reads in the parameter list to be represented by the model.
	 * 
	 * @param validParameters The list to be read.
	 * @param parentItem The initial parent tree item to be used.
	 */
	void readInParameterList(Teuchos::RCP<Teuchos::ParameterList> validParameters, TreeItem *parentItem);

	/**
	 * Inserts a new parameter list into the model.
	 *
	 * @param parameterList The ParameterList to be inserted.
	 * @param listEntry The ParameterEntry of the ParameterList to be inserted.
	 * @param name The name of the ParameterList.
	 * @param The parent TreeItem.
	 */
	void insertParameterList(Teuchos::RCP<Teuchos::ParameterList> parameterList, Teuchos::ParameterEntry *listEntry, std::string name, TreeItem *parent);

	/**
	 * Inserts a new parameter into the model.
	 *
	 * @param listEntry The ParameterEntry of the Parameter to be inserted.
	 * @param name The name of the Parameter.
	 * @param The parent TreeItem.
	 */
	void insertParameter(Teuchos::ParameterEntry *parameter, std::string name, TreeItem *parent);

	/**
	 * Basic setup shared by each of the constructors
	 *
	 * @param saveFileName The saveFileName parameter passed to the constructors.
	 */
	void basicSetup(QString saveFileName);

	/**
	 * Checks the state of a dependent after it's dependency has been evaluated. Takes
	 * appropriate action if any more modifications to the model need to be made or if
	 * the view needs to know anything as a result of the change.
	 */
	void checkDependentState(const QModelIndex dependee, Teuchos::RCP<Dependency> dependency);

	/**
	 * Redraws the array at arrayIndex if it's length has changed. 
	 *
	 * @param arrayIndex The index of the array to be redrawn.
	 */
	void redrawArray(const QModelIndex arrayIndex);

private slots:
	/**
	 * When the state of any of the MainTree's items is changed, this slot should be called
	 */
	void currentFileNowModified();

	/**
	 * Listens to see if any data has changed. If so and the item has dependencies, this function will make sure all appropriate signals are emitted,
	 * and any changes that need to be made to the model are made.
	 * 
	 * @param index1 The start index of the data that changed.
	 * @param index2 The end index of the data that changed.
	 */
	void dataChangedListener(const QModelIndex& index1, const QModelIndex& index2);
};


}

#endif /* OPTIKA_MODEL_HPP_ */
