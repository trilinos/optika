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
#ifndef OPTIKA_GUI_HPP_
#define OPTIKA_GUI_HPP_

#include "Optika_metawindow.hpp"

/*! \file Optika_GUI.hpp
    \brief A collection of functions and an
    Object that serve as the primary interface to the
    Optika package allowing application developers to
    obtain user input.*/

namespace Optika{

  //! @name Basic Input Getting Function
  //@{

  /**
   * \brief Retreives the input for a Teuchos Parameter List using a GUI. Note the 
   * Parameter List will be edited. All user input will be stored in it. 
   * Also runs the function specified whenever the user clicks the action
   * button and uses the specified dependency list.
   *
   * @param validParameters A list of parameters from which the users may 
   * specify values.
   * @param dependencySheet A sheet listing any dependencies between parameters
   * in the validParameters ParameterList.
   * @param customFunc Custom function to run whenever the user clicks the 
   * action button.
   */
  void getInput(
    RCP<ParameterList> validParameters,
    RCP<DependencySheet> dependencySheet=null,
    void (*customFunc)(RCP<const ParameterList>)=NULL);

  /**
   * \brief Reads in a set of parameters and dependencies from the specified xmlfile,
   * displays a GUI, and stores the users input in the sprecified
   * ParameterList. If a custom function is provided, it is run upon the user
   * clicking the action button.
   *
   * @param namOfXmlFile The name of the xml file from which parameters and
   * dependencies will be read in.
   * @param userInput A ParameterList into which all user input should be
   * stored.
   * @param customFunc A custom function for Optika to run upon the user 
   * clicking the action button.
   */
  void getInput(
    const std::string& nameOfXmlFile,
    RCP<ParameterList>& userInput,
    void (*customFunc)(RCP<const ParameterList>)=NULL);

  //@}

/**
 * \brief A class that allows the user to create and customize their Optika GUI.
 */
class OptikaGUI{
public:
  /** \name Constructors */
  //@{

  /**
   * \brief Constructs an OptikaGUI object.
   *
   * @param validParameters A list of parameters from which the users may 
   * specify values.
   * @param dependencySheet A sheet listing any dependencies between parameters
   * in the validParameters ParameterList.
   * @param customFunc A custom function for Optika to run upon the user 
   * clicking the action button.
   */
  OptikaGUI(
    RCP<ParameterList> validParameters, 
    RCP<DependencySheet> dependencySheet=null,
    void (*customFunc)(RCP<const ParameterList>)=NULL);

  /**
   * \brief Constructs an OptikaGUI object.
   *
   * @param xmlFileName Name of an XML file describing the GUI.
   * @param customFunc A custom function for Optika to run upon the user 
   * clicking the action button.
   */
  OptikaGUI(const std::string& xmlFileName,
    void (*customFunc)(RCP<const ParameterList>)=NULL);

  //@}

  //! @name Execution Functions
  //@{
  
  /**
   * \brief Runs the GUI and gets the user input.
   */
  void exec();

  //@}

  //! @name Getters and Setters
  //@{
  
  /**
   * \brief Adds the information specified to the about dialog of the GUI.
   *
   * @param aboutInfo Information to be added to the about dialog of the GUI.
   */
  void setAboutInfo(const std::string& aboutInfo);

  /**
   * \brief Sets the text in the "action" button"
   *
   * @param text The text for the action button
   */
  void setActionButtonText(const std::string& text);

  /**
   * \brief Sets the title of the GUI window that is displayed to the user.
   *
   * @param title A string containing what the title of the GUI window 
   * should be.
   */
  void setWindowTitle(const std::string& title);

  /**
   * \brief Sets the window icon to the image specified in the filePath.
   *
   * @param filePath File path to the image that should be used as
   *  the window icon.
   */
  void setWindowIcon(const std::string& filePath);

  /**
   * \brief Sets the QT style sheet that should be used for the GUI.
   *
   * @param filePath File path to the QT style sheet to be used for
   * the GUI.
   */
  void setStyleSheet(const std::string& filePath);

  /**
   * \brief Sets the custom function to be used in the GUI. When ever the
   * user clicks the action button, this function will be run.
   *
   * @param The custom function to be run whenever the user clicks the action 
   * button.
   */
  void setCustomFunction(void (*customFunc)(RCP<const ParameterList>));

  /**
   * \brief Gets the window title.
   * 
   * @return A string containing the window title.
   */
  std::string getWindowTitle();

  /**
   * \brief Gets the file path describing the location of the file
   * being used for the window icon.
   *
   * @return The file path describing the location of the file
   * being used for the window icon.
   */
  std::string getWindowIcon();

  /**
   * \brief Gets the file path describing the location of the file
   * being used as the QT Style Sheet.
   *
   * @return The file path describing the location of the file
   * being used as the QT Style Sheet.
   */
  std::string getStyleSheet();

  /**
   * \brief Gets the information to be added to the about dialog of the GUI.
   *
   * @return the information to be added to the about dialog of the GUI.
   */
  std::string getAboutInfo();

  //@}

private:
  /** \name Private Members */
  //@{
  
  /**
   * \brief A list of parameters from which the users may specify values.
   */
  RCP<ParameterList> validParameters;

  /**
   * \brief A sheet listing any dependencies between parameters in the validParameters
   */
  RCP<DependencySheet> dependencySheet;

  /**
   * \brief A string containing the window title.
   */
  std::string title;

  /**
   * \brief File path to the image that should be used as the window icon.
   */
  std::string iconFilePath;

  /**
   * \brief File path to the QT style sheet to be used for the GUI.
   */
  std::string styleSheetFilePath;

  /**
   * \brief Information to be added to the about dialog of the GUI.
   */
  std::string aboutInfo;

  /**
   * \brief Text to display in the action button.
   */
  std::string actionButtonText;

  /**
   * \brief The custom function to be run whenever the user clicks the action button.
   */
  void (*customFunc)(RCP<const ParameterList>);

  //@}
};

}

#endif //OPTIKA_GUI_HPP_
