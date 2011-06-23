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


} //end namespace
