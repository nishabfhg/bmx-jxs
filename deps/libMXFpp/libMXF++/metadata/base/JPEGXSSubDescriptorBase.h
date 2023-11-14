/*
 * Copyright (C) 2023, Fraunhofer IIS
 * All Rights Reserved.
 *
 * Author: Nisha Bhaskar
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the British Broadcasting Corporation nor the names
 *       of its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MXFPP_JPEGXSSUBDESCRIPTOR_BASE_H_
#define MXFPP_JPEGXSSUBDESCRIPTOR_BASE_H_

#include <libMXF++/metadata/SubDescriptor.h>

namespace mxfpp
{

	class JPEGXSSubDescriptorBase : public SubDescriptor
	{

	public:
		friend class MetadataSetFactory<JPEGXSSubDescriptorBase>;

		static const mxfKey setKey;

	public:
		JPEGXSSubDescriptorBase(HeaderMetadata *headerMetadata);
		virtual ~JPEGXSSubDescriptorBase();

		uint16_t getJPEGXSPpih() const;
		uint16_t getJPEGXSPlev() const;
		uint16_t getJPEGXSWf() const;
		uint16_t getJPEGXSHf() const;
		uint8_t getJPEGXSNc() const;
		uint16_t getJPEGXSCw() const;
		uint16_t getJPEGXSHsl() const;
		uint32_t getJPEGXSMaximumBitRate() const;
		std::vector<uint8_t> getJPEGXSComponentTable() const;

		void setJPEGXSPpih(uint16_t value);
		void setJPEGXSPlev(uint16_t value);
		void setJPEGXSWf(uint16_t value);
		void setJPEGXSHf(uint16_t value);
		void setJPEGXSNc(uint8_t value);
		void setJPEGXSCw(uint16_t value);
		void setJPEGXSHsl(uint16_t value);
		void setJPEGXSMaximumBitRate(uint32_t value);
		void setJPEGXSComponentTable(std::vector<uint8_t> value);

	protected:
		JPEGXSSubDescriptorBase(HeaderMetadata *headerMetadata, ::MXFMetadataSet *cMetadataSet);

	};
};

#endif