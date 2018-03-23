// libecg-clr-interface.h

/*
* Author: Kamyar Nemati
* Email: kamyarnemati at gmail.com
*
* Description:
* This file contains managed-C++ code (Common Language Runtime classes).
* It provides wrappers for libecg library to interface CLR-based languages such as C#.
*/

#pragma once

/* Include libecg headers. These classes are implemented in native C++ 14. */
#include "../../libecg/libecg/encoder.h" //Encoder class
#include "../../libecg/libecg/decoder.h" //Decoder class
/**/

/* Needed in order to convert managed-type of string back into native standard string. */
#include <msclr\marshal_cppstd.h>

using namespace System;
using namespace libecg; //libecg namespace

namespace libecgclrinterface {

	/* A wrapper class that contains managed code to interface unmanaged Encoder class from libecg. */
	/* Each instance of EncoderWrapper class shall handle only one instance of Encoder class at a time. */
	/* Re-instantiation of this class will get its state destroyed.*/
	public ref class EncoderWrapper
	{
		// TODO: Add your methods for this class here.
	private:
		typedef std::list<int>* pList;
		Encoder* object = nullptr; //Represents an instance to Encoder class.

	public:
		//Wrapper constructor (arguments are meant to interface managed language).
		EncoderWrapper(
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
			this->object = new Encoder(dataset_length, dataset_file_path, threshold, apertureSize, status);
			//Update back re-initialization status.
			stat = status;
		};

		//Memory deallocation.
		~EncoderWrapper() {
			delete this->object;
		};

		//Wrapper function
		System::Void _getOriginal(System::Collections::Generic::List<int>^% lst) {
			pList list = new std::list<int>();
			this->object->getOriginal(list);
			for (std::list<int>::iterator it = list->begin(); it != list->end(); ++it)
				lst->Add(*it);
			return;
		}

		//Wrapper function
		System::Boolean _encode() {
			return this->object->encode();
		}

		//Wrapper function
		System::String^ _getBinarySequeneCompressed() {
			std::string str = this->object->getBinarySequeneCompressed();
			return gcnew String(str.c_str());
		}

		//Wrapper function
		System::Single _getBinarySequeneCompressionRatio() {
			float cr = this->object->getBinarySequeneCompressionRatio();
			return cr;
		}
	};

	/* A wrapper class that contains managed code to interface unmanaged Decoder class from libecg. */
	/* Each instance of DecoderWrapper class shall handle only one instance of Decoder class at a time. */
	/* Re-instantiation of this class will get its state destroyed.*/
	public ref class DecoderWrapper
	{
		// TODO: Add your methods for this class here.
	private:
		typedef std::list<int>* pList;
		Decoder* object = nullptr; //Represents an instance to Decoder class.

	public:
		//Wrapper constructor (arguments are meant to interface managed language).
		DecoderWrapper(
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
			this->object = new Decoder(compressed_sequence, status);
			//Update back re-initialization status.
			stat = status;
		}

		//Memory deallocation.
		~DecoderWrapper() {
			delete this->object;
		}

		//Wrapper function
		System::Boolean _decode() {
			return this->object->decode();
		}

		//Wrapper function
		System::Void _getReconstructed(System::Collections::Generic::List<int>^% lst) {
			pList list = new std::list<int>();
			this->object->getReconstructed(list);
			for (std::list<int>::iterator it = list->begin(); it != list->end(); ++it)
				lst->Add(*it);
			return;
		}

		//Wrapper function
		System::Single _getPercentRootMeanSquareDifference(System::Collections::Generic::List<int>^ original_samples) {
			pList originalSet = new std::list<int>();
			for (int i = 0; i < original_samples->Count; ++i)
				originalSet->push_back(original_samples[i]);
			float prd = this->object->getPercentRootMeanSquareDifference(originalSet);
			return prd;
		}
	};
}
