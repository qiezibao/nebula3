#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::AnimBuilderSaver
    
    Save AnimBuilder object into NAX3 file.
    
    (C) 2009 Radon Labs GmbH
*/
#include "io/uri.h"
#include "toolkitutil/animutil/animbuilder.h"
#include "toolkitutil/platform.h"
#include "io/stream.h"
#include "system/byteorder.h"

//------------------------------------------------------------------------------
namespace ToolkitUtil
{
class AnimBuilderSaver
{
public:
    /// save NAX3 file
    static bool SaveNax3(const IO::URI& uri, AnimBuilder& animBuilder, Platform::Code platform);

private:
    /// write header to stream
    static void WriteHeader(const Ptr<IO::Stream>& stream, AnimBuilder& animBuilder, const System::ByteOrder& byteOrder);
    /// write clip headers to stream
    static void WriteClips(const Ptr<IO::Stream>& stream, AnimBuilder& animBuilder, const System::ByteOrder& byteOrder);
    /// write clip anim events to stream
    static void WriteClipAnimEvents(const Ptr<IO::Stream>& stream, AnimBuilderClip& clip, const System::ByteOrder& byteOrder);
    /// write clip anim curves to stream
    static void WriteClipCurves(const Ptr<IO::Stream>& stream, AnimBuilderClip& clip, const System::ByteOrder& byteOrder);
    /// write keys to stream
    static void WriteKeys(const Ptr<IO::Stream>& stream, AnimBuilder& animBuilder, const System::ByteOrder& byteOrder);
};

} // namespace ToolkitUtil
    