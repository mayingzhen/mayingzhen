//#include "Base.h"
#include "MeshBatch.h"
#include "Material.h"

namespace ma
{

MeshBatch::MeshBatch(VertexDeclaration* vertexFormat, 
					 PRIMITIVE_TYPE primitiveType, Material* material, 
					 bool indexed, unsigned int initialCapacity, 
					 unsigned int growSize)
{
// 	_indexed = indexed;
// 	_capacity = initialCapacity;
// 	_growSize = growSize;
// 
// 
// 	m_pDeclaration = vertexFormat;
// 	m_ePrimitiveType = primitiveType;
// 	m_pMaterial = material;

    resize(initialCapacity);
}

MeshBatch::~MeshBatch()
{
    //SAFE_RELEASE(_material);
    //SAFE_DELETE_ARRAY(_vertices);
    //SAFE_DELETE_ARRAY(_indices);
}

MeshBatch* MeshBatch::create(VertexDeclaration* vertexFormat, PRIMITIVE_TYPE primitiveType, const char* materialPath, bool indexed, unsigned int initialCapacity, unsigned int growSize)
{
    Material* material = Material::create(materialPath);
    if (material == NULL)
    {
        GP_ERROR("Failed to create material for mesh batch from file '%s'.", materialPath);
        return NULL;
    }
    MeshBatch* batch = create(vertexFormat, primitiveType, material, indexed, initialCapacity, growSize);
    //SAFE_RELEASE(material); // batch now owns the material
    return batch;
}

MeshBatch* MeshBatch::create(VertexDeclaration* vertexFormat, PRIMITIVE_TYPE primitiveType, Material* material, bool indexed, unsigned int initialCapacity, unsigned int growSize)
{
    ASSERT(material);

    MeshBatch* batch = new MeshBatch(vertexFormat, primitiveType, material, indexed, initialCapacity, growSize);

    //material->addRef();

    return batch;
}

void MeshBatch::add(const void* vertices, size_t size, unsigned int vertexCount, const unsigned short* indices, unsigned int indexCount)
{
//     ASSERT(vertices);
//     
//     unsigned int newVertexCount = _vertexCount + vertexCount;
//     unsigned int newIndexCount = _indexCount + indexCount;
//     if (m_ePrimitiveType == PRIM_TRIANGLESTRIP && _vertexCount > 0)
//         newIndexCount += 2; // need an extra 2 indices for connecting strips with degenerate triangles
//     
//     // Do we need to grow the batch?
//     while (newVertexCount > _vertexCapacity || (_indexed && newIndexCount > _indexCapacity))
//     {
//         if (_growSize == 0)
//             return; // growing disabled, just clip batch
//         if (!resize(_capacity + _growSize))
//             return; // failed to grow
//     }
//     
//     // Copy vertex data.
//     ASSERT(_verticesPtr);
//     unsigned int vBytes = vertexCount * _vertexFormat->GetStreanmStride();
//     memcpy(_verticesPtr, vertices, vBytes);
//     
//     // Copy index data.
//     if (_indexed)
//     {
//         ASSERT(indices);
//         ASSERT(_indicesPtr);
// 
//         if (_vertexCount == 0)
//         {
//             // Simply copy values directly into the start of the index array.
//             memcpy(_indicesPtr, indices, indexCount * sizeof(unsigned short));
//         }
//         else
//         {
//             if (_primitiveType == PRIM_TRIANGLESTRIP)
//             {
//                 // Create a degenerate triangle to connect separate triangle strips
//                 // by duplicating the previous and next vertices.
//                 _indicesPtr[0] = *(_indicesPtr-1);
//                 _indicesPtr[1] = _vertexCount;
//                 _indicesPtr += 2;
//             }
//             
//             // Loop through all indices and insert them, with their values offset by
//             // 'vertexCount' so that they are relative to the first newly inserted vertex.
//             for (unsigned int i = 0; i < indexCount; ++i)
//             {
//                 _indicesPtr[i] = indices[i] + _vertexCount;
//             }
//         }
//         _indicesPtr += indexCount;
//         _indexCount = newIndexCount;
//     }
//     
//     _verticesPtr += vBytes;
//     _vertexCount = newVertexCount;
}

void MeshBatch::updateVertexAttributeBinding()
{
//    ASSERT(_material);

    // Update our vertex attribute bindings.
//     for (unsigned int i = 0, techniqueCount = _material->getTechniqueCount(); i < techniqueCount; ++i)
//     {
//         Technique* t = _material->getTechniqueByIndex(i);
//         ASSERT(t);
//         for (unsigned int j = 0, passCount = t->getPassCount(); j < passCount; ++j)
//         {
//             Pass* p = t->getPassByIndex(j);
//             ASSERT(p);
//             VertexAttributeBinding* b = VertexAttributeBinding::create(_vertexFormat, _vertices, p->getEffect());
//             p->setVertexAttributeBinding(b);
//             SAFE_RELEASE(b);
//         }
//     }
}

unsigned int MeshBatch::getCapacity() const
{
    return _capacity;
}

void MeshBatch::setCapacity(unsigned int capacity)
{
    resize(capacity);
}

bool MeshBatch::resize(unsigned int capacity)
{
//     if (capacity == 0)
//     {
//         GP_ERROR("Invalid resize capacity (0).");
//         return false;
//     }
// 
//     if (capacity == _capacity)
//         return true;
// 
//     // Store old batch data.
//     unsigned char* oldVertices = _vertices;
//     unsigned short* oldIndices = _indices;
// 
//     unsigned int vertexCapacity = 0;
//     switch (_primitiveType)
//     {
//     case PRIM_LINELIST:
//         vertexCapacity = capacity * 2;
//         break;
//     case PRIM_LINESTRIP:
//         vertexCapacity = capacity + 1;
//         break;
//     case PRIM_POINTLIST:
//         vertexCapacity = capacity;
//         break;
//     case PRIM_TRIANGLELIST:
//         vertexCapacity = capacity * 3;
//         break;
//     case PRIM_TRIANGLESTRIP:
//         vertexCapacity = capacity + 2;
//         break;
//     default:
//         GP_ERROR("Unsupported primitive type for mesh batch (%d).", _primitiveType);
//         return false;
//     }
// 
//     // We have no way of knowing how many vertices will be stored in the batch
//     // (we only know how many indices will be stored). Assume the worst case
//     // for now, which is the same number of vertices as indices.
//     unsigned int indexCapacity = vertexCapacity;
//     if (_indexed && indexCapacity > USHRT_MAX)
//     {
//         GP_ERROR("Index capacity is greater than the maximum unsigned short value (%d > %d).", indexCapacity, USHRT_MAX);
//         return false;
//     }
// 
//     // Allocate new data and reset pointers.
//     unsigned int voffset = _verticesPtr - _vertices;
//     unsigned int vBytes = vertexCapacity * _vertexFormat->GetStreanmStride();
//     _vertices = new unsigned char[vBytes];
//     if (voffset >= vBytes)
//         voffset = vBytes - 1;
//     _verticesPtr = _vertices + voffset;
// 
//     if (_indexed)
//     {
//         unsigned int ioffset = _indicesPtr - _indices;
//         _indices = new unsigned short[indexCapacity];
//         if (ioffset >= indexCapacity)
//             ioffset = indexCapacity - 1;
//         _indicesPtr = _indices + ioffset;
//     }
// 
//     // Copy old data back in
//     if (oldVertices)
//         memcpy(_vertices, oldVertices, min(_vertexCapacity, vertexCapacity) * _vertexFormat->GetStreanmStride());
//     SAFE_DELETE_ARRAY(oldVertices);
//     if (oldIndices)
//         memcpy(_indices, oldIndices, min(_indexCapacity, indexCapacity) * sizeof(unsigned short));
//     SAFE_DELETE_ARRAY(oldIndices);
// 
//     // Assign new capacities
//     _capacity = capacity;
//     _vertexCapacity = vertexCapacity;
//     _indexCapacity = indexCapacity;
// 
//     // Update our vertex attribute bindings now that our client array pointers have changed
//     updateVertexAttributeBinding();

    return true;
}

void MeshBatch::add(const float* vertices, unsigned int vertexCount, const unsigned short* indices, unsigned int indexCount)
{
    add(vertices, sizeof(float), vertexCount, indices, indexCount);
}
    
void MeshBatch::start()
{
    _vertexCount = 0;
    _indexCount = 0;
   // _verticesPtr = _vertices;
   // _indicesPtr = _indices;
}

void MeshBatch::finish()
{
}

void MeshBatch::draw()
{
//     if (_vertexCount == 0 || (_indexed && _indexCount == 0))
//         return; // nothing to draw
// 
// 	Technique* pTech = _material->GetTechnique();
// 	pTech->Begin();
// 
// 	pTech->SetTexture("u_texture",_material->GetDiffuseMap());
// 	//pTech->SetMatrix("u_projectionMatrix",)
// 
// 	GetRenderDevice()->DrawMeshBatch(this,pTech);
// 
// 	pTech->End();
    // Not using VBOs, so unbind the element array buffer.
    // ARRAY_BUFFER will be unbound automatically during pass->bind().
//     GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 ) );
// 
//     ASSERT(_material);
//     if (_indexed)
//         ASSERT(_indices);
// 
//     // Bind the material.
//     Technique* technique = _material->getTechnique();
//     ASSERT(technique);
//     unsigned int passCount = technique->getPassCount();
//     for (unsigned int i = 0; i < passCount; ++i)
//     {
//         Pass* pass = technique->getPassByIndex(i);
//         ASSERT(pass);
//         pass->bind();
// 
//         if (_indexed)
//         {
//             GL_ASSERT( glDrawElements(_primitiveType, _indexCount, GL_UNSIGNED_SHORT, (GLvoid*)_indices) );
//         }
//         else
//         {
//             GL_ASSERT( glDrawArrays(_primitiveType, 0, _vertexCount) );
//         }
// 
//         pass->unbind();
//     }
}
    

}
