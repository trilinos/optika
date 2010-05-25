// @HEADER // ***********************************************************************
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
#include "Teuchos_LocalTestingHelpers.hpp"
#include "Teuchos_VerboseObject.hpp"
#include "Teuchos_ParameterList.hpp"
#include "Optika_SpecificParameterEntryValidators.hpp"

using namespace Teuchos;
using namespace Optika;

/**
 * Tests Number Validators.
 */
int testNumberValis(Teuchos::FancyOStream &out){
	bool success = true;
	/*
	 * Testing Int Validator.
	 */
	RCP<ParameterList> intList = RCP<ParameterList>(new ParameterList("Int List"));
	RCP<EnhancedNumberValidator<int> > intVali = RCP<EnhancedNumberValidator<int> >(new EnhancedNumberValidator<int>(0,10,4));
	TEST_ASSERT(intVali->min() == 0);
	TEST_ASSERT(intVali->max() == 10);
	TEST_ASSERT(intVali->getStep() == 4);
	TEST_ASSERT(intVali->hasMin());
	TEST_ASSERT(intVali->hasMax());
	TEST_ASSERT(intVali->getType() == intId);
	RCP<EnhancedNumberValidator<int> > intVali2 = RCP<EnhancedNumberValidator<int> >(new EnhancedNumberValidator<int>());
	TEST_ASSERT(!intVali2->hasMin());
	TEST_ASSERT(!intVali2->hasMax());
	TEST_ASSERT(intVali2->min() == std::numeric_limits<int>::min());
	TEST_ASSERT(intVali2->max() == std::numeric_limits<int>::max());
	TEST_ASSERT(intVali2->getStep() == intDefaultStep);
	intList->set("Int Parameter", 5, "int parameter", intVali);
	TEST_NOTHROW(intList->validateParameters(*intList));
	TEST_THROW(intList->set("Int Parameter", 11), Exceptions::InvalidParameterValue);
	TEST_THROW(intList->set("Double Parameter", 5.0, "double parameter", intVali), Exceptions::InvalidParameterType);

	/*
	 * Testing Short Validator.
	 */
	RCP<ParameterList> shortList = RCP<ParameterList>(new ParameterList("Short List"));
	RCP<EnhancedNumberValidator<short> > shortVali = RCP<EnhancedNumberValidator<short> >(new EnhancedNumberValidator<short>(0,10,4));
	TEST_ASSERT(shortVali->min() == 0);
	TEST_ASSERT(shortVali->max() == 10);
	TEST_ASSERT(shortVali->getStep() == 4);
	TEST_ASSERT(shortVali->hasMin());
	TEST_ASSERT(shortVali->hasMax());
	TEST_ASSERT(shortVali->getType() == shortId);
	RCP<EnhancedNumberValidator<short> > shortVali2 = RCP<EnhancedNumberValidator<short> >(new EnhancedNumberValidator<short>());
	TEST_ASSERT(!shortVali2->hasMin());
	TEST_ASSERT(!shortVali2->hasMax());
	TEST_ASSERT(shortVali2->min() == std::numeric_limits<short>::min());
	TEST_ASSERT(shortVali2->max() == std::numeric_limits<short>::max());
	TEST_ASSERT(shortVali2->getStep() == shortDefaultStep);
	shortList->set("Short Parameter", (short)5, "short parameter", shortVali);
	TEST_NOTHROW(shortList->validateParameters(*shortList));
	TEST_THROW(shortList->set("Short Parameter", (short)11), Exceptions::InvalidParameterValue);
	TEST_THROW(shortList->set("Double Parameter", 5.0, "double parameter", shortVali), Exceptions::InvalidParameterType);

	/*
	 * Testing Float Validator.
	 */
	RCP<ParameterList> floatList = RCP<ParameterList>(new ParameterList("Float List"));
	RCP<EnhancedNumberValidator<float> > floatVali = RCP<EnhancedNumberValidator<float> >(new EnhancedNumberValidator<float>(0,10.0,4.0,6));
	TEST_ASSERT(floatVali->min() == 0.0);
	TEST_ASSERT(floatVali->max() == 10.0);
	TEST_ASSERT(floatVali->getStep() == 4.0);
	TEST_ASSERT(floatVali->getPrecision() == 6);
	TEST_ASSERT(floatVali->hasMin());
	TEST_ASSERT(floatVali->hasMax());
	TEST_ASSERT(floatVali->getType() == floatId);
	RCP<EnhancedNumberValidator<float> > floatVali2 = RCP<EnhancedNumberValidator<float> >(new EnhancedNumberValidator<float>());
	TEST_ASSERT(!floatVali2->hasMin());
	TEST_ASSERT(!floatVali2->hasMax());
	TEST_ASSERT(floatVali2->min() == -std::numeric_limits<float>::max());
	TEST_ASSERT(floatVali2->max() == std::numeric_limits<float>::max());
	TEST_ASSERT(floatVali2->getStep() == floatDefaultStep);
	TEST_ASSERT(floatVali2->getPrecision() == floatDefaultPrecision);
	floatList->set("Float Parameter", (float)5.0, "float parameter", floatVali);
	TEST_NOTHROW(floatList->validateParameters(*floatList));
	TEST_THROW(floatList->set("Float Parameter", (float)11.0), Exceptions::InvalidParameterValue);
	TEST_THROW(floatList->set("Int Parameter", 5, "int parameter", floatVali), Exceptions::InvalidParameterType);

	/*
	 * Testing Double Validator.
	 */
	RCP<ParameterList> doubleList = RCP<ParameterList>(new ParameterList("Double List"));
	RCP<EnhancedNumberValidator<double> > doubleVali = RCP<EnhancedNumberValidator<double> >(new EnhancedNumberValidator<double>(0,10.0,4.0,6));
	TEST_ASSERT(doubleVali->min() == 0.0);
	TEST_ASSERT(doubleVali->max() == 10.0);
	TEST_ASSERT(doubleVali->getStep() == 4.0);
	TEST_ASSERT(doubleVali->getPrecision() == 6);
	TEST_ASSERT(doubleVali->hasMin());
	TEST_ASSERT(doubleVali->hasMax());
	TEST_ASSERT(doubleVali->getType() == doubleId);
	RCP<EnhancedNumberValidator<double> > doubleVali2 = RCP<EnhancedNumberValidator<double> >(new EnhancedNumberValidator<double>());
	TEST_ASSERT(!doubleVali2->hasMin());
	TEST_ASSERT(!doubleVali2->hasMax());
	TEST_ASSERT(doubleVali2->min() == -std::numeric_limits<double>::max());
	TEST_ASSERT(doubleVali2->max() == std::numeric_limits<double>::max());
	TEST_ASSERT(doubleVali2->getStep() == doubleDefaultStep);
	TEST_ASSERT(doubleVali2->getPrecision() == doubleDefaultPrecision);
	doubleList->set("Double Parameter", (double)5.0, "double parameter", doubleVali);
	TEST_NOTHROW(doubleList->validateParameters(*doubleList));
	TEST_THROW(doubleList->set("Double Parameter", (double)11.0), Exceptions::InvalidParameterValue);
	TEST_THROW(doubleList->set("Int Parameter", 5, "int parameter", doubleVali), Exceptions::InvalidParameterType);
	return (success ? 0:1);
}

/*
 * Testing StringValidator.
 */
int testStringVali(Teuchos::FancyOStream &out){
	bool success = true;
	RCP<ParameterList> stringList = RCP<ParameterList>(new ParameterList("String List"));
	Array<std::string> stringVals = tuple<std::string>("str1", "str2", "str3");
	RCP<StringValidator> stringVali = RCP<StringValidator>(new StringValidator(stringVals));
	RCP<const Array<std::string> > valiVals = stringVali->validStringValues();
	bool local_success = true;
	for(int i=0;i<stringVals.size(); i++){
		TEST_ARRAY_ELE_EQUALITY(*valiVals, i, stringVals[i]);
	}
	if(!local_success){
		success = false;
	}
	TEST_NOTHROW(stringList->set("String param1", "str1", "a string parameter", stringVali));
	TEST_THROW(stringList->set("String param2", "not in list", "a string parameter", stringVali), Exceptions::InvalidParameterValue);
	TEST_THROW(stringList->set("int param", 5, "a int parameter", stringVali), Exceptions::InvalidParameterType);

	return (success ? 0:1);
}

/*
 * Testing FileNameValidator.
 */
int testFileNameVali(Teuchos::FancyOStream &out){
	bool success = true;
	RCP<ParameterList> fileNameList = RCP<ParameterList>(new ParameterList("Filename List"));
	RCP<FileNameValidator> fileNameVali = RCP<FileNameValidator>(new FileNameValidator(true));
	TEST_ASSERT(fileNameVali->fileMustExist());
	fileNameVali->setFileMustExist(false);
	TEST_ASSERT(!fileNameVali->fileMustExist());
	TEST_NOTHROW(fileNameList->set("File name param", "../path", "file name parameter", fileNameVali));
	TEST_THROW(fileNameList->set("int param", 5, "int parameter", fileNameVali), Exceptions::InvalidParameterType);
	fileNameVali->setFileMustExist(true);
	TEST_NOTHROW(fileNameList->set("file name param", "testFile.txt", "a file name", fileNameVali));
	TEST_THROW(fileNameList->set("file name param", "doesntexist.txt", "a file name", fileNameVali), Exceptions::InvalidParameterValue);

	return (success ? 0:1);
}

/*
 * Testing Array Validators.
 */
int testArrayValis(Teuchos::FancyOStream &out){
	bool success = true;
	/*
	 * Testing StringArrayValidator.
	 */
	RCP<ParameterList> stringList = RCP<ParameterList>(new ParameterList("String List"));
	Array<std::string> stringVals = tuple<std::string>("str1", "str2", "str3");
	RCP<StringValidator> stringVali = RCP<StringValidator>(new StringValidator(stringVals));
	RCP<ArrayStringValidator> stringArrayVali = RCP<ArrayStringValidator>(new ArrayStringValidator(stringVali));
	TEST_ASSERT(stringVali.get() == stringArrayVali->getPrototype().get());
	Array<std::string> stringArray = tuple<std::string>("str2","str3","str1","str3","str2");
	TEST_NOTHROW(stringList->set("String Array Param", stringArray, "string array parameter", stringArrayVali));
	Array<std::string> badStringArray = tuple<std::string>("not valid","str3","str1","str3","str2");
	TEST_THROW(stringList->set("String Array Param", badStringArray, "string array parameter", stringArrayVali), Exceptions::InvalidParameterValue);
	TEST_THROW(stringList->set("Int param", 5, "int parameter", stringArrayVali), Exceptions::InvalidParameterType);
	Array<long> longArray = tuple<long>((long)5,(long)5,(long)3);
	TEST_THROW(stringList->set("Long array param", longArray, "long array parameter", stringArrayVali), Exceptions::InvalidParameterType);

	
	/*
	 * Testing Int ArrayValidator.
	 */
	RCP<ParameterList> intList = RCP<ParameterList>(new ParameterList("Int List"));
	RCP<EnhancedNumberValidator<int> > intVali = RCP<EnhancedNumberValidator<int> >(new EnhancedNumberValidator<int>(0, 10));
	RCP<ArrayNumberValidator<int> > intArrayVali = RCP<ArrayNumberValidator<int> >(new ArrayNumberValidator<int>(intVali));
	TEST_ASSERT(intVali.get() == intArrayVali->getPrototype().get());
	Array<int> intArray = tuple<int>(1,4,2,5);
	TEST_NOTHROW(intList->set("int array param", intArray, "int array parameter", intArrayVali));
	Array<int> intBadArray = tuple<int>(11,4,2,5);
	TEST_THROW(intList->set("int bad array param", intBadArray, "int bad array parameter", intArrayVali), Exceptions::InvalidParameterValue);
	TEST_THROW(stringList->set("Long array param", longArray, "long array parameter", intArrayVali), Exceptions::InvalidParameterType);

	/*
	 * Testing Short ArrayValidator.
	 */
	RCP<ParameterList> shortList = RCP<ParameterList>(new ParameterList("Short List"));
	RCP<EnhancedNumberValidator<short> > shortVali = RCP<EnhancedNumberValidator<short> >(new EnhancedNumberValidator<short>(0, 10));
	RCP<ArrayNumberValidator<short> > shortArrayVali = RCP<ArrayNumberValidator<short> >(new ArrayNumberValidator<short>(shortVali));
	TEST_ASSERT(shortVali.get() == shortArrayVali->getPrototype().get());
	Array<short> shortArray = tuple<short>(1,4,2,5);
	TEST_NOTHROW(shortList->set("short array param", shortArray, "short array parameter", shortArrayVali));
	Array<short> shortBadArray = tuple<short>(11,4,2,5);
	TEST_THROW(shortList->set("short bad array param", shortBadArray, "short bad array parameter", shortArrayVali), Exceptions::InvalidParameterValue);
	TEST_THROW(stringList->set("Long array param", longArray, "long array parameter", shortArrayVali), Exceptions::InvalidParameterType);

	/*
	 * Testing Float ArrayValidator.
	 */
	RCP<ParameterList> floatList = RCP<ParameterList>(new ParameterList("Float List"));
	RCP<EnhancedNumberValidator<float> > floatVali = RCP<EnhancedNumberValidator<float> >(new EnhancedNumberValidator<float>(0.0, 10.0));
	RCP<ArrayNumberValidator<float> > floatArrayVali = RCP<ArrayNumberValidator<float> >(new ArrayNumberValidator<float>(floatVali));
	TEST_ASSERT(floatVali.get() == floatArrayVali->getPrototype().get());
	Array<float> floatArray = tuple<float>(1.0,4.0,2.0,5.0);
	TEST_NOTHROW(floatList->set("float array param", floatArray, "float array parameter", floatArrayVali));
	Array<float> floatBadArray = tuple<float>(11.0,4.0,2.0,5.0);
	TEST_THROW(floatList->set("float bad array param", floatBadArray, "float bad array parameter", floatArrayVali), Exceptions::InvalidParameterValue);
	TEST_THROW(stringList->set("Long array param", longArray, "long array parameter", floatArrayVali), Exceptions::InvalidParameterType);

	/*
	 * Testing Double ArrayValidator.
	 */
	RCP<ParameterList> doubleList = RCP<ParameterList>(new ParameterList("Double List"));
	RCP<EnhancedNumberValidator<double> > doubleVali = RCP<EnhancedNumberValidator<double> >(new EnhancedNumberValidator<double>(0.0, 10.0));
	RCP<ArrayNumberValidator<double> > doubleArrayVali = RCP<ArrayNumberValidator<double> >(new ArrayNumberValidator<double>(doubleVali));
	TEST_ASSERT(doubleVali.get() == doubleArrayVali->getPrototype().get());
	Array<double> doubleArray = tuple<double>(1.0,4.0,2.0,5.0);
	TEST_NOTHROW(doubleList->set("double array param", doubleArray, "double array parameter", doubleArrayVali));
	Array<double> doubleBadArray = tuple<double>(11.0,4.0,2.0,5.0);
	TEST_THROW(doubleList->set("double bad array param", doubleBadArray, "double bad array parameter", doubleArrayVali), Exceptions::InvalidParameterValue);
	TEST_THROW(stringList->set("Long array param", longArray, "long array parameter", doubleArrayVali), Exceptions::InvalidParameterType);

	/*
	 * Testing FileName ArrayValidator.
	 */
	RCP<ParameterList> fileNameList = RCP<ParameterList>(new ParameterList("Filename List"));
	RCP<FileNameValidator> fileNameVali = RCP<FileNameValidator>(new FileNameValidator(true));
	RCP<ArrayFileNameValidator> arrayFileNameVali = RCP<ArrayFileNameValidator>(new ArrayFileNameValidator(fileNameVali));
	TEST_ASSERT(arrayFileNameVali->getPrototype().get() == fileNameVali.get());
	Array<std::string> fileNameArray = tuple<std::string>("testFile.txt", "testFile2.txt", "testFile3.txt");
	Array<std::string> fileNameBadArray = tuple<std::string>("doesnexist.txt", "testFile2.txt", "testFile3.txt");
	TEST_NOTHROW(fileNameList->set("File name array", fileNameArray, "file name array parameter", arrayFileNameVali));
	TEST_THROW(fileNameList->set("Bad File name array", fileNameBadArray, "bad file name array parameter", arrayFileNameVali), Exceptions::InvalidParameterValue);
	TEST_THROW(stringList->set("Long array param", longArray, "long array parameter", arrayFileNameVali), Exceptions::InvalidParameterType);


	return (success ? 0:1);
}

int main(int argc, char* argv[]){
	bool success = true;
	Teuchos::RCP<Teuchos::FancyOStream> out = Teuchos::VerboseObjectBase::getDefaultOStream();
	if(testNumberValis(*out) == 1){
		success = false;
	}
	if(testStringVali(*out) == 1){
		success = false;
	}
	if(testFileNameVali(*out) == 1){
		success = false;
	}
	if(testArrayValis(*out) == 1){
		success = false;
	}
	return (success ? 0:1);
}

