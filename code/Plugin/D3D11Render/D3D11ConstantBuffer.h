
#pragma once



namespace ma
{

/// Hardware constant buffer.
class ConstantBuffer : public Referenced/*, public GPUObject* /*/
{
    //URHO3D_OBJECT(ConstantBuffer, Object);

public:
    /// Construct.
    ConstantBuffer(/*Context* context*/);
    /// Destruct.
    virtual ~ConstantBuffer();

    /// Release buffer.
    virtual void Release();

    /// Set size and create GPU-side buffer. Return true on success.
    bool SetSize(unsigned size);
    /// Set a generic parameter and mark buffer dirty.
    void SetParameter(unsigned offset, unsigned size, const void* data);
    /// Set a Vector3 array parameter and mark buffer dirty.
    void SetVector3ArrayParameter(unsigned offset, unsigned rows, const void* data);
    /// Apply to GPU.
    void Apply();

    /// Return size.
    unsigned GetSize() const { return size_; }

    /// Return whether has unapplied data.
    bool IsDirty() const { return dirty_; }

	ID3D11Buffer* GetD3D11Buffer() {return m_pD3D11Buffer;}

private:
    /// Shadow data.
    vector<unsigned char> shadowData_;
    /// Buffer byte size.
    unsigned size_;
    /// Dirty flag.
    bool dirty_;

	ID3D11Buffer* m_pD3D11Buffer;
};

}
