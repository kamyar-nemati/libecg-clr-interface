/*
* @author: Kamyar Nemati <kamyarnemati@gmail.com>
* @description: This class provides wrapper functions for libecg library.
*/

// libecg-clr-interface.h

#pragma once

/* Include libecg headers */
#include "../../../libecg/libecg/encode.h"
#include "../../../libecg/libecg/decode.h"
/**/

#include <msclr\marshal_cppstd.h>

using namespace System;
using namespace libecg; //Open up libecg namespace

namespace libecgclrinterface {

	//A managed wrapper class for unmanaged Encode class. Each EncodeWrapper shall handle only one Encode object at a time.
	public ref class EncodeWrapper
	{
		// TODO: Add your methods for this class here.
	private:
		typedef std::list<int>* pList;
		Encode* object = nullptr; //Represents a reference to to Encode class.

	public:
		//Wrapper constructor (managed arguments).
		EncodeWrapper(
			System::Int32 dataset_len,
			System::String^ dataset_path,
			System::Int32 bitsThreshold,
			System::Int32 aperture,
			System::Boolean% stat)
		{
			//Parsing arguments into unmanaged types (native).
			unsigned int dataset_length = dataset_len;
			std::string dataset_file_path = msclr::interop::marshal_as<std::string>(dataset_path);
			unsigned int threshold = bitsThreshold;
			unsigned int apertureSize = aperture;
			bool status;
			//Prevent memory leak on re-initialization.
			if (this->object != nullptr)
				delete this->object;
			//Re-initialization
			this->object = new Encode(dataset_length, dataset_file_path, threshold, apertureSize, status);
			//Update back re-initialization status.
			stat = status;
		};

		//Reference deletion.
		~EncodeWrapper() {
			delete this->object;
		};

		//Wrapper function.
		System::Void _getOriginal(System::Collections::Generic::List<int>^% lst) {
			pList list = new std::list<int>();
			this->object->getOriginal(list);
			for (std::list<int>::iterator it = list->begin(); it != list->end(); ++it)
				lst->Add(*it);
			return;
		}

		//Wrapper function.
		System::Boolean _encode() {
			return this->object->encode();
		}

		//Wrapper function.
		System::String^ _getBinarySequeneCompressed() {
			std::string str = this->object->getBinarySequeneCompressed();
			return gcnew String(str.c_str());
		}

		//Wrapper function.
		System::Single _getBinarySequeneCompressionRatio() {
			float cr = this->object->getBinarySequeneCompressionRatio();
			return cr;
		}
	};

	//A managed wrapper class for unmanaged Decode class. Each DecodeWrapper shall handle only one Decode object at a time.
	public ref class DecodeWrapper
	{
		// TODO: Add your methods for this class here.
	private:
		typedef std::list<int>* pList;
		Decode* object = nullptr; //Represents a reference to to Decode class.

	public:
		//Wrapper constructor (managed arguments).
		DecodeWrapper(
			System::String^ sequence,
			System::Boolean% stat)
		{
			//Parsing arguments into unmanaged types (native).
			std::string& compressed_sequence = msclr::interop::marshal_as<std::string>(sequence);
			bool status;
			//Prevent memory leak on re-initialization.
			if (this->object != nullptr)
				delete this->object;
			//Re-initialization.
			this->object = new Decode(compressed_sequence, status);
			//Update back re-initialization status.
			stat = status;
		}

		//Reference deletion.
		~DecodeWrapper() {
			delete this->object;
		}

		//Wrapper function.
		System::Boolean _decode() {
			return this->object->decode();
		}

		//Wrapper function.
		System::Void _getReconstructed(System::Collections::Generic::List<int>^% lst) {
			pList list = new std::list<int>();
			this->object->getReconstructed(list);
			for (std::list<int>::iterator it = list->begin(); it != list->end(); ++it)
				lst->Add(*it);
			return;
		}

		//Wrapper function.
		System::Single _getPercentRootMeanSquareDifference(System::Collections::Generic::List<int>^ original_samples) {
			pList originalSet = new std::list<int>();
			for (int i = 0; i < original_samples->Count; ++i)
				originalSet->push_back(original_samples[i]);
			float prd = this->object->getPercentRootMeanSquareDifference(originalSet);
			return prd;
		}
	};
}
