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

#include "Teuchos_UnitTestHarness.hpp"
#include "Optika_ArrayHelperFunctions.hpp"

namespace Optika{

TEUCHOS_UNIT_TEST(Array_Helper_Functions, ArrayToQVariant){
  Array<std::string> testArray = Teuchos::tuple<std::string>("blah1", "blah2");
  ParameterEntry entry(testArray);
  QVariant testVariant = arrayEntryToVariant(rcpFromRef(entry), stringId);
  Array<std::string> extractedArray = testVariant.value<Array<std::string> >();
  TEST_EQUALITY(testArray,extractedArray);
  out << "Test Array: " << testArray << std::endl;
  out << "Extracted Array: " << extractedArray << std::endl;
}

TEUCHOS_UNIT_TEST(Array_Helper_Functions, DetermineArrayType){
  Array<int> intArray  = Teuchos::tuple<int>(5,6,7,8);
  ParameterEntry intParam(intArray);
  TEST_EQUALITY(
    determineArrayType(rcpFromRef(intParam)).toStdString(), 
    intId.toStdString());

  Array<double> doubleArray = 
    Teuchos::tuple<double>(5.0,6.4,7.5,8.9);
  ParameterEntry doubleParam(doubleArray);
  TEST_EQUALITY(
    determineArrayType(rcpFromRef(doubleParam)).toStdString(), 
    doubleId.toStdString());

  Array<std::string> stringArray = 
    Teuchos::tuple<std::string>("hello", "one");
  ParameterEntry stringParam(stringArray);
  TEST_EQUALITY(
    determineArrayType(rcpFromRef(stringParam)).toStdString(), 
    stringId.toStdString());
  TwoDArray<float> floatArray = 
    Teuchos::tuple<float>(5.0,6.4,7.5,8.9);
  ParameterEntry floatParam(floatTwoDArray);
  TEST_EQUALITY(
    determineTwoDArrayType(rcpFromRef(floatParam)).toStdString(), 
    floatId.toStdString());
}

TEUCHOS_UNIT_TEST(Array_Helper_Functions, GetArrayType){
  TEST_EQUALITY(
    intId.toStdString(), 
    getArrayType(arrayId + " " + intId).toStdString());

  TEST_EQUALITY(
    doubleId.toStdString(), 
    getArrayType(arrayId + " " + doubleId).toStdString());

  TEST_EQUALITY(
    stringId.toStdString(), 
    getArrayType(arrayId + " " + stringId).toStdString());

}

TEUCHOS_UNIT_TEST(Array_Helper_Functions, IsArrayEmpty){
  Array<int> intArray;
  ParameterEntry intArrayParam(intArray);
  TEST_ASSERT(isArrayEmpty(rcpFromRef(intArrayParam), intId));

  Array<std::string> stringArray(4, "blah");
  ParameterEntry stringArrayParam(stringArray);
  TEST_ASSERT(!isArrayEmpty(rcpFromRef(stringArrayParam), stringId));

}

} //end namespace
