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
#ifndef OPTIKA_DEPENDENCYSHEET_HPP_
#define OPTIKA_DEPENDENCYSHEET_HPP_
#include "Optika_Dependency.hpp"

namespace Optika{

/**
 * A Dependency sheet keeps track of dependencies between various elements located
 * somewhere within a "Root List". All dependencies added to a DependencySheet
 * must have dependents and dependees who are either in the Root List or one of
 * its sublists.
 *
 * Note that a DependencySheet never acts on these dependencies. It mearly keeps
 * track of them.
 */
class DependencySheet{
public:
	/**
	 * Convience typedef representing a set of dependencies.
	 */
	typedef std::set<Teuchos::RCP<Optika::Dependency>, Optika::Dependency::DepComp > DepSet;

	/**
	 * Convience typedef. Maps dependee parameter entries to a set of their corresponding
	 * dependencies.
	 */
	typedef Teuchos::map<const Teuchos::ParameterEntry*, DepSet > DepMap;

	/**
	 * Constructs an empty DependencySheet with the name DEP_ANONYMOUS.
	 * 
	 * @param rootList The Parameter List containing all parameters and sublists
	 * for which this Dependency will keep track of dependencies.
	 */
	DependencySheet(Teuchos::RCP<Teuchos::ParameterList> rootList);

	/**
	 * Constructs a DependencySheet.
	 *
	 * @param rootList The Parameter List containing all parameters and sublists
	 * for which this Dependency will keep track of dependencies.
	 * @param name Name of the Dependency Sheet.
	 */
	DependencySheet(Teuchos::RCP<Teuchos::ParameterList> rootList, const std::string &name);

	/**
	 * Adds a dependency to the sheet.
	 * 
	 * @param dependency The dependency to be added.
	 * @return True if the addition was sucessful, false otherwise.
	 */
	bool addDependency(const Teuchos::RCP<Optika::Dependency> dependency);

	/**
	 * Removes a particular dependency between two parameters.
	 *
	 * @param dependency The dependency to be removed.
	 * @return True if the removal was sucessfull, false otherwise.
	 */
	bool removeDependency(Teuchos::RCP<Optika::Dependency> dependency);

	/**
	 * Determines whether or not a parameter is depended upon by any another
	 * parameters or parameter lists.
	 *
	 * @param name The paramteter to be checked for dependents.
	 * @return True if the parameter you're checking has other dependents, false otherwise.
	 */
	bool hasDependents(const Teuchos::ParameterEntry *dependee) const;

	/**
	 * Returns a set of all the dependencies associated with a particular dependee.
	 *
	 * @param dependee The parameter whose dependencies are sought. 
	 * @return A set of all dependencies associated with the dependee parameter.
	 * */
	const DepSet& getDependenciesForParameter(const Teuchos::ParameterEntry *dependee) const;

	/**
	 * Returns an iterator to the beginning of all the dependees in the sheet.
	 *
	 * @return An iterator to the beginning of all the dependees in the sheet.
	 */
	DepMap::iterator depBegin();

	/**
	 * Returns an iterator to the end of all of the dependees in the sheet.
	 *
	 * @return An iterator to the end of all of the dependees in the sheet.
	 */
	DepMap::iterator depEnd();

	/**
	 * Returns a const iterator to the beginning of all the dependees in the sheet.
	 *
	 * @return A const iterator to the beginning of all the dependees in the sheet.
	 */
	DepMap::const_iterator depBegin() const;

	/**
	 * Returns a const iterator to the end of all of the dependees in the sheet.
	 *
	 * @return A const iterator to the end of all of the dependees in the sheet.
	 */
	DepMap::const_iterator depEnd() const;

	/**
	 * Prints out a list of the dependencies in the DependencySheet
	 */
	void printDeps();

private:
	/**
	 * A map containing all the depenecies for a list. Dependencies with multiple dependees will be found in multiple
	 * places  within the map. Essentially, for each dependee, there will be a pointer to the dependency.
	 */
	DepMap dependencies;

	/**
	 * The Name of the dependency sheet.
	 */
	std::string name;

	/**
	 * The root parameterlist that this dependency sheet is associated with.
	 */
	Teuchos::RCP<Teuchos::ParameterList> rootList;

	/**
	 * Validates whether or not the dependee and dependet of a dependency exist
	 * within the root ParameterList.
	 */
	void validateExistanceInRoot(Teuchos::RCP<Optika::Dependency> dependency);
};


}
#endif //OPTIKA_DEPENDENCYSHEET_HPP_
