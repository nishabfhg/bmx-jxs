/*
 * Copyright (C) 2011, British Broadcasting Corporation
 * All Rights Reserved.
 *
 * Author: Philip de Nier
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

#include <bmx/clip_writer/ClipWriterTrack.h>
#include <bmx/as02/AS02PictureTrack.h>
#include <bmx/as02/AS02DVTrack.h>
#include <bmx/as02/AS02UncTrack.h>
#include <bmx/as02/AS02D10Track.h>
#include <bmx/as02/AS02MPEG2LGTrack.h>
#include <bmx/as02/AS02AVCITrack.h>
#include <bmx/as02/AS02PCMTrack.h>
#include <bmx/as11/AS11Track.h>
#include <bmx/mxf_op1a/OP1APictureTrack.h>
#include <bmx/mxf_op1a/OP1ADVTrack.h>
#include <bmx/mxf_op1a/OP1AUncTrack.h>
#include <bmx/mxf_op1a/OP1AD10Track.h>
#include <bmx/mxf_op1a/OP1AMPEG2LGTrack.h>
#include <bmx/mxf_op1a/OP1AAVCITrack.h>
#include <bmx/mxf_op1a/OP1APCMTrack.h>
#include <bmx/avid_mxf/AvidPictureTrack.h>
#include <bmx/avid_mxf/AvidDVTrack.h>
#include <bmx/avid_mxf/AvidD10Track.h>
#include <bmx/avid_mxf/AvidMPEG2LGTrack.h>
#include <bmx/avid_mxf/AvidMJPEGTrack.h>
#include <bmx/avid_mxf/AvidVC3Track.h>
#include <bmx/avid_mxf/AvidAVCITrack.h>
#include <bmx/avid_mxf/AvidUncTrack.h>
#include <bmx/avid_mxf/AvidPCMTrack.h>
#include <bmx/d10_mxf/D10MPEGTrack.h>
#include <bmx/d10_mxf/D10PCMTrack.h>
#include <bmx/MXFUtils.h>
#include <bmx/Utils.h>
#include <bmx/BMXException.h>
#include <bmx/Logging.h>

using namespace std;
using namespace bmx;



bool ClipWriterTrack::IsSupported(ClipWriterType clip_type, EssenceType essence_type, Rational sample_rate)
{
    switch (clip_type)
    {
        case CW_AS02_CLIP_TYPE:
            return AS02Track::IsSupported(essence_type, sample_rate);
        case CW_AS11_OP1A_CLIP_TYPE:
            return AS11Track::IsSupported(AS11_OP1A_CLIP_TYPE, essence_type, sample_rate);
        case CW_AS11_D10_CLIP_TYPE:
            return AS11Track::IsSupported(AS11_D10_CLIP_TYPE, essence_type, sample_rate);
        case CW_OP1A_CLIP_TYPE:
            return OP1ATrack::IsSupported(essence_type, sample_rate);
        case CW_AVID_CLIP_TYPE:
            return AvidTrack::IsSupported(essence_type, sample_rate);
        case CW_D10_CLIP_TYPE:
            return D10Track::IsSupported(essence_type, sample_rate);
        case CW_UNKNOWN_CLIP_TYPE:
            break;
    }

    return false;
}

ClipWriterTrack::ClipWriterTrack(EssenceType essence_type, AS02Track *track)
{
    mClipType = CW_AS02_CLIP_TYPE;
    mEssenceType = essence_type;
    mAS02Track = track;
    mAS11Track = 0;
    mOP1ATrack = 0;
    mAvidTrack = 0;
    mD10Track = 0;
}

ClipWriterTrack::ClipWriterTrack(EssenceType essence_type, AS11Track *track)
{
    if (track->GetClipType() == AS11_OP1A_CLIP_TYPE)
        mClipType = CW_AS11_OP1A_CLIP_TYPE;
    else
        mClipType = CW_AS11_D10_CLIP_TYPE;
    mEssenceType = essence_type;
    mAS02Track = 0;
    mAS11Track = track;
    mOP1ATrack = 0;
    mAvidTrack = 0;
    mD10Track = 0;
}

ClipWriterTrack::ClipWriterTrack(EssenceType essence_type, OP1ATrack *track)
{
    mClipType = CW_OP1A_CLIP_TYPE;
    mEssenceType = essence_type;
    mAS02Track = 0;
    mAS11Track = 0;
    mOP1ATrack = track;
    mAvidTrack = 0;
    mD10Track = 0;
}

ClipWriterTrack::ClipWriterTrack(EssenceType essence_type, AvidTrack *track)
{
    mClipType = CW_AVID_CLIP_TYPE;
    mEssenceType = essence_type;
    mAS02Track = 0;
    mAS11Track = 0;
    mOP1ATrack = 0;
    mAvidTrack = track;
    mD10Track = 0;
}

ClipWriterTrack::ClipWriterTrack(EssenceType essence_type, D10Track *track)
{
    mClipType = CW_D10_CLIP_TYPE;
    mEssenceType = essence_type;
    mAS02Track = 0;
    mAS11Track = 0;
    mOP1ATrack = 0;
    mAvidTrack = 0;
    mD10Track = track;
}

ClipWriterTrack::~ClipWriterTrack()
{
}

void ClipWriterTrack::SetOutputTrackNumber(uint32_t track_number)
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
            mAS02Track->SetOutputTrackNumber(track_number);
            break;
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
            mAS11Track->SetOutputTrackNumber(track_number);
            break;
        case CW_OP1A_CLIP_TYPE:
            mOP1ATrack->SetOutputTrackNumber(track_number);
            break;
        case CW_AVID_CLIP_TYPE:
            mAvidTrack->SetOutputTrackNumber(track_number);
            break;
        case CW_D10_CLIP_TYPE:
            mD10Track->SetOutputTrackNumber(track_number);
            break;
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }
}

void ClipWriterTrack::SetAspectRatio(Rational aspect_ratio)
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
        {
            AS02PictureTrack *pict_track = dynamic_cast<AS02PictureTrack*>(mAS02Track);
            if (pict_track)
                pict_track->SetAspectRatio(aspect_ratio);
            break;
        }
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
        {
            mAS11Track->SetAspectRatio(aspect_ratio);
            break;
        }
        case CW_OP1A_CLIP_TYPE:
        {
            OP1APictureTrack *pict_track = dynamic_cast<OP1APictureTrack*>(mOP1ATrack);
            if (pict_track)
                pict_track->SetAspectRatio(aspect_ratio);
            break;
        }
        case CW_AVID_CLIP_TYPE:
        {
            AvidPictureTrack *pict_track = dynamic_cast<AvidPictureTrack*>(mAvidTrack);
            if (pict_track)
                pict_track->SetAspectRatio(aspect_ratio);
            break;
        }
        case CW_D10_CLIP_TYPE:
        {
            D10MPEGTrack *mpeg_track = dynamic_cast<D10MPEGTrack*>(mD10Track);
            if (mpeg_track)
                mpeg_track->SetAspectRatio(aspect_ratio);
            break;
        }
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }
}

void ClipWriterTrack::SetComponentDepth(uint32_t depth)
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
        {
            AS02DVTrack *dv_track = dynamic_cast<AS02DVTrack*>(mAS02Track);
            AS02UncTrack *unc_track = dynamic_cast<AS02UncTrack*>(mAS02Track);
            if (dv_track)
                dv_track->SetComponentDepth(depth);
            else if (unc_track)
                unc_track->SetComponentDepth(depth);
            break;
        }
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
        {
            mAS11Track->SetComponentDepth(depth);
            break;
        }
        case CW_OP1A_CLIP_TYPE:
        {
            OP1ADVTrack *dv_track = dynamic_cast<OP1ADVTrack*>(mOP1ATrack);
            OP1AUncTrack *unc_track = dynamic_cast<OP1AUncTrack*>(mOP1ATrack);
            if (dv_track)
                dv_track->SetComponentDepth(depth);
            else if (unc_track)
                unc_track->SetComponentDepth(depth);
            break;
        }
        case CW_AVID_CLIP_TYPE:
        {
            AvidDVTrack *dv_track = dynamic_cast<AvidDVTrack*>(mAvidTrack);
            if (dv_track)
                dv_track->SetComponentDepth(depth);
            break;
        }
        case CW_D10_CLIP_TYPE:
            break;
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }
}

void ClipWriterTrack::SetSampleSize(uint32_t size)
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
        {
            AS02D10Track *d10_track = dynamic_cast<AS02D10Track*>(mAS02Track);
            if (d10_track)
                d10_track->SetSampleSize(size);
            break;
        }
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
        {
            mAS11Track->SetSampleSize(size);
            break;
        }
        case CW_OP1A_CLIP_TYPE:
        {
            OP1AD10Track *d10_track = dynamic_cast<OP1AD10Track*>(mOP1ATrack);
            if (d10_track)
                d10_track->SetSampleSize(size);
            break;
        }
        case CW_AVID_CLIP_TYPE:
        {
            AvidD10Track *d10_track = dynamic_cast<AvidD10Track*>(mAvidTrack);
            if (d10_track)
                d10_track->SetSampleSize(size);
            break;
        }
        case CW_D10_CLIP_TYPE:
        {
            D10MPEGTrack *mpeg_track = dynamic_cast<D10MPEGTrack*>(mD10Track);
            if (mpeg_track)
                mpeg_track->SetSampleSize(size);
            break;
        }
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }
}

void ClipWriterTrack::SetAVCIMode(AVCIMode mode)
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
        {
            AS02AVCITrack *avci_track = dynamic_cast<AS02AVCITrack*>(mAS02Track);
            if (avci_track) {
                switch (mode)
                {
                    case AVCI_PASS_MODE:
                    case AVCI_FIRST_OR_ALL_FRAME_HEADER_MODE:
                        avci_track->SetMode(AS02_AVCI_FIRST_OR_ALL_FRAME_HEADER_MODE);
                        break;
                    case AVCI_FIRST_FRAME_HEADER_MODE:
                        avci_track->SetMode(AS02_AVCI_FIRST_FRAME_HEADER_MODE);
                        break;
                    case AVCI_ALL_FRAME_HEADER_MODE:
                        avci_track->SetMode(AS02_AVCI_ALL_FRAME_HEADER_MODE);
                        break;
                    default:
                        log_warn("AVCI mode %d not supported\n", mode);
                        break;
                }
            }
            break;
        }
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
        {
            switch (mode)
            {
                case AVCI_PASS_MODE:
                case AVCI_FIRST_OR_ALL_FRAME_HEADER_MODE:
                    mAS11Track->SetAVCIMode(OP1A_AVCI_FIRST_OR_ALL_FRAME_HEADER_MODE);
                    break;
                case AVCI_FIRST_FRAME_HEADER_MODE:
                    mAS11Track->SetAVCIMode(OP1A_AVCI_FIRST_FRAME_HEADER_MODE);
                    break;
                case AVCI_ALL_FRAME_HEADER_MODE:
                    mAS11Track->SetAVCIMode(OP1A_AVCI_ALL_FRAME_HEADER_MODE);
                    break;
                default:
                    log_warn("AVCI mode %d not supported\n", mode);
                    break;
            }
            break;
        }
        case CW_OP1A_CLIP_TYPE:
        {
            OP1AAVCITrack *avci_track = dynamic_cast<OP1AAVCITrack*>(mOP1ATrack);
            if (avci_track) {
                switch (mode)
                {
                    case AVCI_PASS_MODE:
                    case AVCI_FIRST_OR_ALL_FRAME_HEADER_MODE:
                        avci_track->SetMode(OP1A_AVCI_FIRST_OR_ALL_FRAME_HEADER_MODE);
                        break;
                    case AVCI_FIRST_FRAME_HEADER_MODE:
                        avci_track->SetMode(OP1A_AVCI_FIRST_FRAME_HEADER_MODE);
                        break;
                    case AVCI_ALL_FRAME_HEADER_MODE:
                        avci_track->SetMode(OP1A_AVCI_ALL_FRAME_HEADER_MODE);
                        break;
                    default:
                        log_warn("AVCI mode %d not supported\n", mode);
                        break;
                }
            }
            break;
        }
        case CW_AVID_CLIP_TYPE:
        {
            AvidAVCITrack *avci_track = dynamic_cast<AvidAVCITrack*>(mAvidTrack);
            if (avci_track) {
                switch (mode)
                {
                    case AVCI_PASS_MODE:
                    case AVCI_NO_OR_ALL_FRAME_HEADER_MODE:
                        avci_track->SetMode(AVID_AVCI_NO_OR_ALL_FRAME_HEADER_MODE);
                        break;
                    case AVCI_NO_FRAME_HEADER_MODE:
                        avci_track->SetMode(AVID_AVCI_NO_FRAME_HEADER_MODE);
                        break;
                    case AVCI_ALL_FRAME_HEADER_MODE:
                        avci_track->SetMode(AVID_AVCI_ALL_FRAME_HEADER_MODE);
                        break;
                    default:
                        log_warn("AVCI mode %d not supported\n", mode);
                        break;
                }
            }
            break;
        }
        case CW_D10_CLIP_TYPE:
            break;
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }
}

void ClipWriterTrack::SetAVCIHeader(const unsigned char *data, uint32_t size)
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
        {
            AS02AVCITrack *avci_track = dynamic_cast<AS02AVCITrack*>(mAS02Track);
            if (avci_track)
                avci_track->SetHeader(data, size);
            break;
        }
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
        {
            mAS11Track->SetAVCIHeader(data, size);
            break;
        }
        case CW_OP1A_CLIP_TYPE:
        {
            OP1AAVCITrack *avci_track = dynamic_cast<OP1AAVCITrack*>(mOP1ATrack);
            if (avci_track)
                avci_track->SetHeader(data, size);
            break;
        }
        case CW_AVID_CLIP_TYPE:
        {
            AvidAVCITrack *avci_track = dynamic_cast<AvidAVCITrack*>(mAvidTrack);
            if (avci_track)
                avci_track->SetHeader(data, size);
            break;
        }
        case CW_D10_CLIP_TYPE:
            break;
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }
}

void ClipWriterTrack::SetAFD(uint8_t afd)
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
        {
            AS02PictureTrack *picture_track = dynamic_cast<AS02PictureTrack*>(mAS02Track);
            if (picture_track)
                picture_track->SetAFD(afd);
            break;
        }
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
        {
            mAS11Track->SetAFD(afd);
            break;
        }
        case CW_OP1A_CLIP_TYPE:
        {
            OP1APictureTrack *picture_track = dynamic_cast<OP1APictureTrack*>(mOP1ATrack);
            if (picture_track)
                picture_track->SetAFD(afd);
            break;
        }
        case CW_AVID_CLIP_TYPE:
            break;
        case CW_D10_CLIP_TYPE:
        {
            D10MPEGTrack *mpeg_track = dynamic_cast<D10MPEGTrack*>(mD10Track);
            if (mpeg_track)
                mpeg_track->SetAFD(afd);
            break;
        }
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }
}

void ClipWriterTrack::SetInputHeight(uint32_t height)
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
        case CW_OP1A_CLIP_TYPE:
        case CW_D10_CLIP_TYPE:
            break;
        case CW_AVID_CLIP_TYPE:
        {
            AvidUncTrack *unc_track = dynamic_cast<AvidUncTrack*>(mAvidTrack);
            if (unc_track)
                unc_track->SetInputHeight(height);
            break;
        }
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }
}

void ClipWriterTrack::SetSamplingRate(Rational sampling_rate)
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
        {
            AS02PCMTrack *pcm_track = dynamic_cast<AS02PCMTrack*>(mAS02Track);
            if (pcm_track)
                pcm_track->SetSamplingRate(sampling_rate);
            break;
        }
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
        {
            mAS11Track->SetSamplingRate(sampling_rate);
            break;
        }
        case CW_OP1A_CLIP_TYPE:
        {
            OP1APCMTrack *pcm_track = dynamic_cast<OP1APCMTrack*>(mOP1ATrack);
            if (pcm_track)
                pcm_track->SetSamplingRate(sampling_rate);
            break;
        }
        case CW_AVID_CLIP_TYPE:
        {
            AvidPCMTrack *pcm_track = dynamic_cast<AvidPCMTrack*>(mAvidTrack);
            if (pcm_track)
                pcm_track->SetSamplingRate(sampling_rate);
            break;
        }
        case CW_D10_CLIP_TYPE:
        {
            D10PCMTrack *pcm_track = dynamic_cast<D10PCMTrack*>(mD10Track);
            if (pcm_track)
                pcm_track->SetSamplingRate(sampling_rate);
            break;
        }
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }
}

void ClipWriterTrack::SetQuantizationBits(uint32_t bits)
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
        {
            AS02PCMTrack *pcm_track = dynamic_cast<AS02PCMTrack*>(mAS02Track);
            if (pcm_track)
                pcm_track->SetQuantizationBits(bits);
            break;
        }
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
        {
            mAS11Track->SetQuantizationBits(bits);
            break;
        }
        case CW_OP1A_CLIP_TYPE:
        {
            OP1APCMTrack *pcm_track = dynamic_cast<OP1APCMTrack*>(mOP1ATrack);
            if (pcm_track)
                pcm_track->SetQuantizationBits(bits);
            break;
        }
        case CW_AVID_CLIP_TYPE:
        {
            AvidPCMTrack *pcm_track = dynamic_cast<AvidPCMTrack*>(mAvidTrack);
            if (pcm_track)
                pcm_track->SetQuantizationBits(bits);
            break;
        }
        case CW_D10_CLIP_TYPE:
        {
            D10PCMTrack *pcm_track = dynamic_cast<D10PCMTrack*>(mD10Track);
            if (pcm_track)
                pcm_track->SetQuantizationBits(bits);
            break;
        }
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }
}

void ClipWriterTrack::SetChannelCount(uint32_t count)
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
        {
            AS02PCMTrack *pcm_track = dynamic_cast<AS02PCMTrack*>(mAS02Track);
            if (pcm_track)
                pcm_track->SetChannelCount(count);
            break;
        }
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
        {
            mAS11Track->SetChannelCount(count);
            break;
        }
        case CW_OP1A_CLIP_TYPE:
        {
            OP1APCMTrack *pcm_track = dynamic_cast<OP1APCMTrack*>(mOP1ATrack);
            if (pcm_track)
                pcm_track->SetChannelCount(count);
            break;
        }
        case CW_AVID_CLIP_TYPE:
        {
            AvidPCMTrack *pcm_track = dynamic_cast<AvidPCMTrack*>(mAvidTrack);
            if (pcm_track)
                pcm_track->SetChannelCount(count);
            break;
        }
        case CW_D10_CLIP_TYPE:
        {
            D10PCMTrack *pcm_track = dynamic_cast<D10PCMTrack*>(mD10Track);
            if (pcm_track)
                pcm_track->SetChannelCount(count);
            break;
        }
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }
}

void ClipWriterTrack::SetLocked(bool locked)
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
        {
            AS02PCMTrack *pcm_track = dynamic_cast<AS02PCMTrack*>(mAS02Track);
            if (pcm_track)
                pcm_track->SetLocked(locked);
            break;
        }
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
        {
            mAS11Track->SetLocked(locked);
            break;
        }
        case CW_OP1A_CLIP_TYPE:
        {
            OP1APCMTrack *pcm_track = dynamic_cast<OP1APCMTrack*>(mOP1ATrack);
            if (pcm_track)
                pcm_track->SetLocked(locked);
            break;
        }
        case CW_AVID_CLIP_TYPE:
        {
            AvidPCMTrack *pcm_track = dynamic_cast<AvidPCMTrack*>(mAvidTrack);
            if (pcm_track)
                pcm_track->SetLocked(locked);
            break;
        }
        case CW_D10_CLIP_TYPE:
        {
            D10PCMTrack *pcm_track = dynamic_cast<D10PCMTrack*>(mD10Track);
            if (pcm_track)
                pcm_track->SetLocked(locked);
            break;
        }
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }
}

void ClipWriterTrack::SetAudioRefLevel(int8_t level)
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
        {
            AS02PCMTrack *pcm_track = dynamic_cast<AS02PCMTrack*>(mAS02Track);
            if (pcm_track)
                pcm_track->SetAudioRefLevel(level);
            break;
        }
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
        {
            mAS11Track->SetAudioRefLevel(level);
            break;
        }
        case CW_OP1A_CLIP_TYPE:
        {
            OP1APCMTrack *pcm_track = dynamic_cast<OP1APCMTrack*>(mOP1ATrack);
            if (pcm_track)
                pcm_track->SetAudioRefLevel(level);
            break;
        }
        case CW_AVID_CLIP_TYPE:
        {
            AvidPCMTrack *pcm_track = dynamic_cast<AvidPCMTrack*>(mAvidTrack);
            if (pcm_track)
                pcm_track->SetAudioRefLevel(level);
            break;
        }
        case CW_D10_CLIP_TYPE:
        {
            D10PCMTrack *pcm_track = dynamic_cast<D10PCMTrack*>(mD10Track);
            if (pcm_track)
                pcm_track->SetAudioRefLevel(level);
            break;
        }
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }
}

void ClipWriterTrack::SetDialNorm(int8_t dial_norm)
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
        {
            AS02PCMTrack *pcm_track = dynamic_cast<AS02PCMTrack*>(mAS02Track);
            if (pcm_track)
                pcm_track->SetDialNorm(dial_norm);
            break;
        }
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
        {
            mAS11Track->SetDialNorm(dial_norm);
            break;
        }
        case CW_OP1A_CLIP_TYPE:
        {
            OP1APCMTrack *pcm_track = dynamic_cast<OP1APCMTrack*>(mOP1ATrack);
            if (pcm_track)
                pcm_track->SetDialNorm(dial_norm);
            break;
        }
        case CW_AVID_CLIP_TYPE:
        {
            AvidPCMTrack *pcm_track = dynamic_cast<AvidPCMTrack*>(mAvidTrack);
            if (pcm_track)
                pcm_track->SetDialNorm(dial_norm);
            break;
        }
        case CW_D10_CLIP_TYPE:
        {
            D10PCMTrack *pcm_track = dynamic_cast<D10PCMTrack*>(mD10Track);
            if (pcm_track)
                pcm_track->SetDialNorm(dial_norm);
            break;
        }
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }
}

void ClipWriterTrack::SetSequenceOffset(uint8_t offset)
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
        {
            AS02PCMTrack *pcm_track = dynamic_cast<AS02PCMTrack*>(mAS02Track);
            if (pcm_track)
                pcm_track->SetSequenceOffset(offset);
            break;
        }
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
            mAS11Track->SetSequenceOffset(offset);
        case CW_OP1A_CLIP_TYPE:
        {
            OP1APCMTrack *pcm_track = dynamic_cast<OP1APCMTrack*>(mOP1ATrack);
            if (pcm_track)
                pcm_track->SetSequenceOffset(offset);
            break;
        }
        case CW_AVID_CLIP_TYPE:
        {
            AvidPCMTrack *pcm_track = dynamic_cast<AvidPCMTrack*>(mAvidTrack);
            if (pcm_track)
                pcm_track->SetSequenceOffset(offset);
            break;
        }
        case CW_D10_CLIP_TYPE:
        {
            D10PCMTrack *pcm_track = dynamic_cast<D10PCMTrack*>(mD10Track);
            if (pcm_track)
                pcm_track->SetSequenceOffset(offset);
            break;
        }
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }
}

void ClipWriterTrack::WriteSamples(const unsigned char *data, uint32_t size, uint32_t num_samples)
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
            mAS02Track->WriteSamples(data, size, num_samples);
            break;
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
            mAS11Track->WriteSamples(data, size, num_samples);
            break;
        case CW_OP1A_CLIP_TYPE:
            mOP1ATrack->WriteSamples(data, size, num_samples);
            break;
        case CW_AVID_CLIP_TYPE:
            mAvidTrack->WriteSamples(data, size, num_samples);
            break;
        case CW_D10_CLIP_TYPE:
            mD10Track->WriteSamples(data, size, num_samples);
            break;
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }
}

bool ClipWriterTrack::IsPicture() const
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
            return mAS02Track->IsPicture();
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
            return mAS11Track->IsPicture();
        case CW_OP1A_CLIP_TYPE:
            return mOP1ATrack->IsPicture();
        case CW_AVID_CLIP_TYPE:
            return mAvidTrack->IsPicture();
        case CW_D10_CLIP_TYPE:
            return mD10Track->IsPicture();
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }

    return false;
}

uint32_t ClipWriterTrack::GetSampleSize() const
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
            return mAS02Track->GetSampleSize();
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
            return mAS11Track->GetSampleSize();
        case CW_OP1A_CLIP_TYPE:
            return mOP1ATrack->GetSampleSize();
        case CW_AVID_CLIP_TYPE:
            return mAvidTrack->GetSampleSize();
        case CW_D10_CLIP_TYPE:
            return mD10Track->GetSampleSize();
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }

    return 0;
}

uint32_t ClipWriterTrack::GetInputSampleSize() const
{
    switch (mClipType)
    {
        case CW_AVID_CLIP_TYPE:
        {
            AvidUncTrack *unc_track = dynamic_cast<AvidUncTrack*>(mAvidTrack);
            if (unc_track)
                return unc_track->GetInputSampleSize();
            break;
        }
        default:
            break;
    }

    return GetSampleSize();
}

uint32_t ClipWriterTrack::GetAVCISampleWithoutHeaderSize() const
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
        {
            AS02AVCITrack *avci_track = dynamic_cast<AS02AVCITrack*>(mAS02Track);
            if (avci_track)
                return avci_track->GetSampleWithoutHeaderSize();
            break;
        }
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
        {
            return mAS11Track->GetAVCISampleWithoutHeaderSize();
        }
        case CW_OP1A_CLIP_TYPE:
        {
            OP1AAVCITrack *avci_track = dynamic_cast<OP1AAVCITrack*>(mOP1ATrack);
            if (avci_track)
                return avci_track->GetSampleWithoutHeaderSize();
            break;
        }
        case CW_AVID_CLIP_TYPE:
        {
            AvidAVCITrack *avci_track = dynamic_cast<AvidAVCITrack*>(mAvidTrack);
            if (avci_track)
                return avci_track->GetSampleWithoutHeaderSize();
            break;
        }
        case CW_D10_CLIP_TYPE:
            break;
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }

    return 0;
}

bool ClipWriterTrack::IsSingleField() const
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
        case CW_OP1A_CLIP_TYPE:
        case CW_D10_CLIP_TYPE:
            break;
        case CW_AVID_CLIP_TYPE:
        {
            AvidMJPEGTrack *mjpeg_track = dynamic_cast<AvidMJPEGTrack*>(mAvidTrack);
            if (mjpeg_track)
                return mjpeg_track->IsSingleField();
            break;
        }
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }

    return false;
}

vector<uint32_t> ClipWriterTrack::GetShiftedSampleSequence() const
{
    switch (mClipType)
    {
        case CW_AS02_CLIP_TYPE:
        {
            AS02PCMTrack *pcm_track = dynamic_cast<AS02PCMTrack*>(mAS02Track);
            if (pcm_track)
                return pcm_track->GetShiftedSampleSequence();
            break;
        }
        case CW_AS11_OP1A_CLIP_TYPE:
        case CW_AS11_D10_CLIP_TYPE:
            return mAS11Track->GetShiftedSampleSequence();
        case CW_OP1A_CLIP_TYPE:
        {
            OP1APCMTrack *pcm_track = dynamic_cast<OP1APCMTrack*>(mOP1ATrack);
            if (pcm_track)
                return pcm_track->GetShiftedSampleSequence();
            break;
        }
        case CW_AVID_CLIP_TYPE:
        {
            AvidPCMTrack *pcm_track = dynamic_cast<AvidPCMTrack*>(mAvidTrack);
            if (pcm_track)
                return pcm_track->GetShiftedSampleSequence();
            break;
        }
        case CW_D10_CLIP_TYPE:
        {
            D10PCMTrack *pcm_track = dynamic_cast<D10PCMTrack*>(mD10Track);
            if (pcm_track)
                return pcm_track->GetShiftedSampleSequence();
            break;
        }
        case CW_UNKNOWN_CLIP_TYPE:
            BMX_ASSERT(false);
            break;
    }

    return vector<uint32_t>(1);
}
