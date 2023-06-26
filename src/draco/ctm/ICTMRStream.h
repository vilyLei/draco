#pragma once
#include <string>
#include <memory>
#include "ctmDef.h"
#include "draco/stream/Uint8Array.h"
#include "draco/stream/Int32Array.h"
#include "draco/stream/Float32Array.h"

namespace draco
{
	class ICTMRStream
	{
	public:
		ICTMRStream() :
			data(nullptr), offset(0) {}

		virtual ~ICTMRStream() {
			data = nullptr;
			offset = 0;
			//std::cout << "ICTMRStream::deconstructor()..." << std::endl;
		}
		/**
		 * uint8 array
		 */
		std::shared_ptr<draco::Uint8Array> data;
		Int32                                    offset;

		virtual void                                       showInfo() = 0;
		virtual Uint8                                      readByte() = 0;
		virtual Int32                                      readInt32() = 0;
		virtual Float32                                    readFloat32() = 0;
		virtual std::string                                readString() = 0;
		virtual std::shared_ptr<draco::Int32Array>   readArrayInt32(std::shared_ptr<draco::Int32Array> arr) = 0;
		virtual std::shared_ptr<draco::Float32Array> readArrayFloat32(std::shared_ptr<draco::Float32Array> arr) = 0;
	};
}