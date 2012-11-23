#pragma once
//------------------------------------------------------------------------------
/**
    @class Legacy::Nvx2OrderFreeStreamReader
  
    A stream reader which reads legacy nvx2 binary mesh files.
    It allows reading of nvx2 with big endianess byte order.

    (C) 2007 Radon Labs GmbH
*/    
#include "core/config.h"
#if NEBULA3_LEGACY_SUPPORT
#include "io/streamreader.h"
#include "coregraphics/vertexbuffer.h"
#include "coregraphics/indexbuffer.h"
#include "coregraphics/primitivegroup.h"
#include "coregraphics/base/vertexbufferbase.h"
#include "coregraphics/base/memoryvertexbufferloaderbase.h"
#include "coregraphics/base/indexbufferbase.h"
#include "coregraphics/base/memoryindexbufferloaderbase.h"  
#include "system/byteorder.h"

//------------------------------------------------------------------------------
namespace Legacy
{
class Nvx2OrderFreeStreamReader : public IO::StreamReader
{
    __DeclareClass(Nvx2OrderFreeStreamReader);
public:
    /// contructor
    Nvx2OrderFreeStreamReader();
    /// destructor
    virtual ~Nvx2OrderFreeStreamReader();

    /// enable/disable raw mode (raw mode does not setup vertex/index buffers), default is false
    void SetRawMode(bool b);
    /// get raw mode flag
    bool IsRawMode() const;
    /// set the intended resource usage (default is UsageImmutable)
    void SetUsage(Base::ResourceBase::Usage usage);
    /// get resource usage
    Base::ResourceBase::Usage GetUsage() const;
    /// set the intended resource access (default is AccessNone)
    void SetAccess(Base::ResourceBase::Access access);
    /// get the resource access
    Base::ResourceBase::Access GetAccess() const;
    /// begin reading from the stream, read entire data
    virtual bool Open();
    /// end reading from the stream, destroys loaded objects
    virtual void Close();
    /// get vertex buffer (not valid in raw mode)
    const Ptr<Base::VertexBufferBase>& GetVertexBuffer() const;
    /// get index buffer (not valid in raw mode)
    const Ptr<Base::IndexBufferBase>& GetIndexBuffer() const;
    /// get primitive groups
    const Util::Array<CoreGraphics::PrimitiveGroup>& GetPrimitiveGroups() const;
    /// get pointer to raw vertex data
    float* GetVertexData() const;
    /// get pointer to raw index data
    ushort* GetIndexData() const;
    /// get number of vertices
    SizeT GetNumVertices() const;
    /// get number of indices
    SizeT GetNumIndices() const;
    /// get vertex width
    SizeT GetVertexWidth() const;
    /// get number of edges
    SizeT GetNumEdges() const;
    /// get vertex components
    const Util::Array<CoreGraphics::VertexComponent>& GetVertexComponents() const;

    /// set the specialized vertex buffer
    void SetVertexBuffer(const Ptr<Base::VertexBufferBase>& vBuf);
    /// set the specialized vertex buffer reader
    void SetVertexBufferLoader(const Ptr<Base::MemoryVertexBufferLoaderBase>& vBufLoader);

    /// helper method to convert vertex buffer endianess
    void ConvertVertexBufferEndianess(void* vertexPtr, SizeT numVertices, const Util::Array<CoreGraphics::VertexComponent>& vertexComps);

    /// set source platform endianess
    void SetSourcePlatformEndianess(System::ByteOrder::Type type);
    /// get source platform endianess
    System::ByteOrder::Type GetSourcePlatformEndianess() const;

private:
    /// read header data from stream
    void ReadHeaderData();
    /// read primitive groups from stream
    void ReadPrimitiveGroups();
    /// setup vertex components array
    void SetupVertexComponents();
    /// update primitive group bounding boxes
    void UpdateGroupBoundingBoxes();
    /// setup the vertex buffer object (not called in raw mode)
    void SetupVertexBuffer();
    /// setup the index buffer object (not called in raw mode)
    void SetupIndexBuffer();

    /// Nebula2 vertex components, see Nebula2's nMesh2 class for details
    enum N2VertexComponent
    {
        N2Coord        = (1<<0),      // 3 floats
        N2Normal       = (1<<1),      // 3 floats
        N2NormalUB4N   = (1<<2),      // 4 unsigned bytes, normalized
        N2Uv0          = (1<<3),      // 2 floats
        N2Uv0S2        = (1<<4),      // 2 shorts, 4.12 fixed point
        N2Uv1          = (1<<5),      // 2 floats
        N2Uv1S2        = (1<<6),      // 2 shorts, 4.12 fixed point
        N2Uv2          = (1<<7),      // 2 floats
        N2Uv2S2        = (1<<8),      // 2 shorts, 4.12 fixed point
        N2Uv3          = (1<<9),      // 2 floats
        N2Uv3S2        = (1<<10),     // 2 shorts, 4.12 fixed point
        N2Color        = (1<<11),     // 4 floats
        N2ColorUB4N    = (1<<12),     // 4 unsigned bytes, normalized
        N2Tangent      = (1<<13),     // 3 floats
        N2TangentUB4N  = (1<<14),     // 4 unsigned bytes, normalized
        N2Binormal     = (1<<15),     // 3 floats
        N2BinormalUB4N = (1<<16),     // 4 unsigned bytes, normalized
        N2Weights      = (1<<17),     // 4 floats
        N2WeightsUB4N  = (1<<18),     // 4 unsigned bytes, normalized
        N2JIndices     = (1<<19),     // 4 floats
        N2JIndicesUB4  = (1<<20),     // 4 unsigned bytes

        N2NumVertexComponents = 21,
        N2AllComponents = ((1<<N2NumVertexComponents) - 1),
    };

    Base::ResourceBase::Usage usage;
    Base::ResourceBase::Access access;

    bool rawMode;
    Ptr<Base::VertexBufferBase> vertexBuffer;
    Ptr<Base::IndexBufferBase> indexBuffer;
    Ptr<Base::MemoryVertexBufferLoaderBase> vertexBufferLoader;    
    Ptr<Base::MemoryIndexBufferLoaderBase> indexBufferLoader;

    Util::Array<CoreGraphics::PrimitiveGroup> primGroups;
    void* mapPtr;
    void* groupDataPtr;
    void* vertexDataPtr;
    void* indexDataPtr;

    SizeT groupDataSize;
    SizeT vertexDataSize;
    SizeT indexDataSize;
    
    uint numGroups;
    uint numVertices;
    uint vertexWidth;
    uint numIndices;
    uint numEdges;
    uint vertexComponentMask;
    Util::Array<CoreGraphics::VertexComponent> vertexComponents;  
    System::ByteOrder::Type sourceEndianess;
};

//------------------------------------------------------------------------------
/**
*/
inline void
Nvx2OrderFreeStreamReader::SetVertexBuffer(const Ptr<Base::VertexBufferBase>& vBuf)
{
    this->vertexBuffer = vBuf;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Nvx2OrderFreeStreamReader::SetVertexBufferLoader(const Ptr<Base::MemoryVertexBufferLoaderBase>& vBufLoader)
{
    this->vertexBufferLoader = vBufLoader;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Nvx2OrderFreeStreamReader::SetRawMode(bool b)
{
    this->rawMode = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Nvx2OrderFreeStreamReader::IsRawMode() const
{
    return this->rawMode;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Base::VertexBufferBase>&
Nvx2OrderFreeStreamReader::GetVertexBuffer() const
{
    n_assert(!this->rawMode && this->vertexBuffer.isvalid());
    return this->vertexBuffer;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Base::IndexBufferBase>&
Nvx2OrderFreeStreamReader::GetIndexBuffer() const
{
    n_assert(!this->rawMode && this->indexBuffer.isvalid());
    return this->indexBuffer;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<CoreGraphics::PrimitiveGroup>&
Nvx2OrderFreeStreamReader::GetPrimitiveGroups() const
{
    return this->primGroups;
}

//------------------------------------------------------------------------------
/**
*/
inline float*
Nvx2OrderFreeStreamReader::GetVertexData() const
{
    return (float*) this->vertexDataPtr;
}

//------------------------------------------------------------------------------
/**
*/
inline ushort*
Nvx2OrderFreeStreamReader::GetIndexData() const
{
    return (ushort*) this->indexDataPtr;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
Nvx2OrderFreeStreamReader::GetNumVertices() const
{
    return this->numVertices;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
Nvx2OrderFreeStreamReader::GetNumIndices() const
{
    return this->numIndices;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
Nvx2OrderFreeStreamReader::GetVertexWidth() const
{
    return this->vertexWidth;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
Nvx2OrderFreeStreamReader::GetNumEdges() const
{
    return this->numEdges;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<CoreGraphics::VertexComponent>&
Nvx2OrderFreeStreamReader::GetVertexComponents() const
{
    return this->vertexComponents;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Nvx2OrderFreeStreamReader::SetUsage(Base::ResourceBase::Usage usage_)
{
    this->usage = usage_;
}

//------------------------------------------------------------------------------
/**
*/
inline Base::ResourceBase::Usage
Nvx2OrderFreeStreamReader::GetUsage() const
{
    return this->usage;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Nvx2OrderFreeStreamReader::SetAccess(Base::ResourceBase::Access access_)
{
    this->access = access_;
}

//------------------------------------------------------------------------------
/**
*/
inline Base::ResourceBase::Access
Nvx2OrderFreeStreamReader::GetAccess() const
{
    return this->access;
}  

//------------------------------------------------------------------------------
/**
*/
inline void 
Nvx2OrderFreeStreamReader::SetSourcePlatformEndianess(System::ByteOrder::Type type)
{
    this->sourceEndianess = type;
}

//------------------------------------------------------------------------------
/**
*/
inline System::ByteOrder::Type 
Nvx2OrderFreeStreamReader::GetSourcePlatformEndianess() const
{
    return this->sourceEndianess;
}

} // namespace Legacy
//------------------------------------------------------------------------------
#endif // NEBULA3_LEGACY_SUPPORT