#include "Bundle.h"

#define BUNDLE_VERSION_MAJOR            1
#define BUNDLE_VERSION_MINOR            2

#define BUNDLE_TYPE_SCENE               1
#define BUNDLE_TYPE_NODE                2
#define BUNDLE_TYPE_ANIMATIONS          3
#define BUNDLE_TYPE_ANIMATION           4
#define BUNDLE_TYPE_ANIMATION_CHANNEL   5
#define BUNDLE_TYPE_MODEL               10
#define BUNDLE_TYPE_MATERIAL            16
#define BUNDLE_TYPE_EFFECT              18
#define BUNDLE_TYPE_CAMERA              32
#define BUNDLE_TYPE_LIGHT               33
#define BUNDLE_TYPE_MESH                34
#define BUNDLE_TYPE_MESHPART            35
#define BUNDLE_TYPE_MESHSKIN            36
#define BUNDLE_TYPE_FONT                128

// For sanity checking string reads
#define BUNDLE_MAX_STRING_LENGTH        5000

namespace ma
{

static std::vector<Bundle*> __bundleCache;

Bundle::Bundle(const char* path) :
    _path(path), _referenceCount(0), _references(NULL), _stream(NULL)/*, _trackedNodes(NULL)*/
{
}

Bundle::~Bundle()
{

    // Remove this Bundle from the cache.
    std::vector<Bundle*>::iterator itr = std::find(__bundleCache.begin(), __bundleCache.end(), this);
    if (itr != __bundleCache.end())
    {
        __bundleCache.erase(itr);
    }

    SAFE_DELETE_ARRAY(_references);

    if (_stream)
    {
        SAFE_DELETE(_stream);
    }
}

template <class T>
bool Bundle::readArray(unsigned int* length, T** ptr)
{
    ASSERT(length);
    ASSERT(ptr);
    ASSERT(_stream);

    if (!read(length))
    {
        GP_ERROR("Failed to read the length of an array of data (to be read into an array).");
        return false;
    }
    if (*length > 0)
    {
        *ptr = new T[*length];
        if (_stream->read(*ptr, sizeof(T), *length) != *length)
        {
            GP_ERROR("Failed to read an array of data from bundle (into an array).");
            SAFE_DELETE_ARRAY(*ptr);
            return false;
        }
    }
    return true;
}

template <class T>
bool Bundle::readArray(unsigned int* length, std::vector<T>* values)
{
    ASSERT(length);
    ASSERT(_stream);

    if (!read(length))
    {
        GP_ERROR("Failed to read the length of an array of data (to be read into a std::vector).");
        return false;
    }
    if (*length > 0 && values)
    {
        values->resize(*length);
        if (_stream->read(&(*values)[0], sizeof(T), *length) != *length)
        {
            GP_ERROR("Failed to read an array of data from bundle (into a std::vector).");
            return false;
        }
    }
    return true;
}

template <class T>
bool Bundle::readArray(unsigned int* length, std::vector<T>* values, unsigned int readSize)
{
    ASSERT(length);
    ASSERT(_stream);
    ASSERT(sizeof(T) >= readSize);

    if (!read(length))
    {
        GP_ERROR("Failed to read the length of an array of data (to be read into a std::vector with a specified single element read size).");
        return false;
    }
    if (*length > 0 && values)
    {
        values->resize(*length);
        if (_stream->read(&(*values)[0], readSize, *length) != *length)
        {
            GP_ERROR("Failed to read an array of data from bundle (into a std::vector with a specified single element read size).");
            return false;
        }
    }
    return true;
}

static std::string readString(Stream* stream)
{
    ASSERT(stream);

    unsigned int length;
    if (stream->read(&length, 4, 1) != 1)
    {
        GP_ERROR("Failed to read the length of a string from a bundle.");
        return std::string();
    }

    // Sanity check to detect if string length is far too big.
    ASSERT(length < BUNDLE_MAX_STRING_LENGTH);

    std::string str;
    if (length > 0)
    {
        str.resize(length);
        if (stream->read(&str[0], 1, length) != length)
        {
            GP_ERROR("Failed to read string from bundle.");
            return std::string();
        }
    }
    return str;
}

Bundle* Bundle::create(const char* path)
{
    ASSERT(path);

    // Search the cache for this bundle.
    for (size_t i = 0, count = __bundleCache.size(); i < count; ++i)
    {
        Bundle* p = __bundleCache[i];
        ASSERT(p);
        if (p->_path == path)
        {
            // Found a match
            //p->addRef();
            return p;
        }
    }

    // Open the bundle.
    Stream* stream = FileSystem::open(path);
    if (!stream)
    {
        GP_ERROR("Failed to open file '%s'.", path);
        return NULL;
    }

    // Read the GPB header info.
    char sig[9];
    if (stream->read(sig, 1, 9) != 9 || memcmp(sig, "\xABGPB\xBB\r\n\x1A\n", 9) != 0)
    {
        SAFE_DELETE(stream);
        GP_ERROR("Invalid GPB header for bundle '%s'.", path);
        return NULL;
    }

    // Read version.
    unsigned char ver[2];
    if (stream->read(ver, 1, 2) != 2)
    {
        SAFE_DELETE(stream);
        GP_ERROR("Failed to read GPB version for bundle '%s'.", path);
        return NULL;
    }
    if (ver[0] != BUNDLE_VERSION_MAJOR || ver[1] != BUNDLE_VERSION_MINOR)
    {
        SAFE_DELETE(stream);
        GP_ERROR("Unsupported version (%d.%d) for bundle '%s' (expected %d.%d).", (int)ver[0], (int)ver[1], path, BUNDLE_VERSION_MAJOR, BUNDLE_VERSION_MINOR);
        return NULL;
    }

    // Read ref table.
    unsigned int refCount;
    if (stream->read(&refCount, 4, 1) != 1)
    {
        SAFE_DELETE(stream);
        GP_ERROR("Failed to read ref table for bundle '%s'.", path);
        return NULL;
    }

    // Read all refs.
    Reference* refs = new Reference[refCount];
    for (unsigned int i = 0; i < refCount; ++i)
    {
        if ((refs[i].id = readString(stream)).empty() ||
            stream->read(&refs[i].type, 4, 1) != 1 ||
            stream->read(&refs[i].offset, 4, 1) != 1)
        {
            SAFE_DELETE(stream);
            GP_ERROR("Failed to read ref number %d for bundle '%s'.", i, path);
            SAFE_DELETE_ARRAY(refs);
            return NULL;
        }
    }

    // Keep file open for faster reading later.
    Bundle* bundle = new Bundle(path);
    bundle->_referenceCount = refCount;
    bundle->_references = refs;
    bundle->_stream = stream;

    return bundle;
}

Bundle::Reference* Bundle::find(const char* id) const
{
    ASSERT(id);
    ASSERT(_references);

    // Search the ref table for the given id (case-sensitive).
    for (unsigned int i = 0; i < _referenceCount; ++i)
    {
        if (_references[i].id == id)
        {
            // Found a match
            return &_references[i];
        }
    }

    return NULL;
}


const char* Bundle::getIdFromOffset() const
{
    ASSERT(_stream);
    return getIdFromOffset((unsigned int) _stream->position());
}

const char* Bundle::getIdFromOffset(unsigned int offset) const
{
    // Search the ref table for the given offset.
    if (offset > 0)
    {
        ASSERT(_references);
        for (unsigned int i = 0; i < _referenceCount; ++i)
        {
            if (_references[i].offset == offset && _references[i].id.length() > 0)
            {
                return _references[i].id.c_str();
            }
        }
    }
    return NULL;
}

const std::string& Bundle::getMaterialPath()
{
    if (_materialPath.empty())
    {
        int pos = _path.find_last_of('.');
        if (pos > 2)
        {
            _materialPath = _path.substr(0, pos);
            _materialPath.append(".material");
            if (!FileSystem::fileExists(_materialPath.c_str()))
            {
                _materialPath.clear();
            }
        }
    }
    return _materialPath;
}

Bundle::Reference* Bundle::seekTo(const char* id, unsigned int type)
{
    Reference* ref = find(id);
    if (ref == NULL)
    {
        GP_ERROR("No object with name '%s' in bundle '%s'.", id, _path.c_str());
        return NULL;
    }

    if (ref->type != type)
    {
        GP_ERROR("Object '%s' in bundle '%s' has type %d (expected type %d).", id, _path.c_str(), (int)ref->type, (int)type);
        return NULL;
    }

    // Seek to the offset of this object.
    ASSERT(_stream);
    if (_stream->seek(ref->offset, SEEK_SET) == false)
    {
        GP_ERROR("Failed to seek to object '%s' in bundle '%s'.", id, _path.c_str());
        return NULL;
    }

    return ref;
}

Bundle::Reference* Bundle::seekToFirstType(unsigned int type)
{
    ASSERT(_references);
    ASSERT(_stream);

    for (unsigned int i = 0; i < _referenceCount; ++i)
    {
        Reference* ref = &_references[i];
        if (ref->type == type)
        {
            // Found a match.
            if (_stream->seek(ref->offset, SEEK_SET) == false)
            {
                GP_ERROR("Failed to seek to object '%s' in bundle '%s'.", ref->id.c_str(), _path.c_str());
                return NULL;
            }
            return ref;
        }
    }
    return NULL;
}

bool Bundle::read(unsigned int* ptr)
{
    return _stream->read(ptr, sizeof(unsigned int), 1) == 1;
}

bool Bundle::read(unsigned char* ptr)
{
    return _stream->read(ptr, sizeof(unsigned char), 1) == 1;
}

bool Bundle::read(float* ptr)
{
    return _stream->read(ptr, sizeof(float), 1) == 1;
}

bool Bundle::readMatrix(float* m)
{
    return _stream->read(m, sizeof(float), 16) == 16;
}

Font* Bundle::loadFont(const char* id)
{
    ASSERT(id);
    ASSERT(_stream);

    // Seek to the specified font.
    Reference* ref = seekTo(id, BUNDLE_TYPE_FONT);
    if (ref == NULL)
    {
        GP_ERROR("Failed to load ref for font '%s'.", id);
        return NULL;
    }

    // Read font family.
    std::string family = readString(_stream);
    if (family.empty())
    {
        GP_ERROR("Failed to read font family for font '%s'.", id);
        return NULL;
    }

    // Read font style and size.
    unsigned int style, size;
    if (_stream->read(&style, 4, 1) != 1)
    {
        GP_ERROR("Failed to read style for font '%s'.", id);
        return NULL;
    }
    if (_stream->read(&size, 4, 1) != 1)
    {
        GP_ERROR("Failed to read size for font '%s'.", id);
        return NULL;
    }

    // Read character set.
    std::string charset = readString(_stream);

    // Read font glyphs.
    unsigned int glyphCount;
    if (_stream->read(&glyphCount, 4, 1) != 1)
    {
        GP_ERROR("Failed to read glyph count for font '%s'.", id);
        return NULL;
    }
    if (glyphCount == 0)
    {
        GP_ERROR("Invalid glyph count (must be greater than 0) for font '%s'.", id);
        return NULL;
    }

    Font::Glyph* glyphs = new Font::Glyph[glyphCount];
    if (_stream->read(glyphs, sizeof(Font::Glyph), glyphCount) != glyphCount)
    {
        GP_ERROR("Failed to read glyphs for font '%s'.", id);
        SAFE_DELETE_ARRAY(glyphs);
        return NULL;
    }

    // Read texture attributes.
    unsigned int width, height, textureByteCount;
    if (_stream->read(&width, 4, 1) != 1)
    {
        GP_ERROR("Failed to read texture width for font '%s'.", id);
        SAFE_DELETE_ARRAY(glyphs);
        return NULL;
    }
    if (_stream->read(&height, 4, 1) != 1)
    {
        GP_ERROR("Failed to read texture height for font '%s'.", id);
        SAFE_DELETE_ARRAY(glyphs);
        return NULL;
    }
    if (_stream->read(&textureByteCount, 4, 1) != 1)
    {
        GP_ERROR("Failed to read texture byte count for font '%s'.", id);
        SAFE_DELETE_ARRAY(glyphs);
        return NULL;
    }
    if (textureByteCount != (width * height))
    {
        GP_ERROR("Invalid texture byte count for font '%s'.", id);
        SAFE_DELETE_ARRAY(glyphs);
        return NULL;
    }
	
	// ConvetUV
	for (UINT i = 0; i < glyphCount; ++i)
	{
		//Font::Glyph* glyphs = new Font::Glyph[glyphCount];	
		float fHalftw = GetRenderDevice()->GetHalfPixelOffset(0.5f / width);
		float fHalfth = GetRenderDevice()->GetHalfPixelOffset(0.5f / height);
		glyphs[i].uvs[0] += fHalftw;
		glyphs[i].uvs[1] += fHalfth;
		glyphs[i].uvs[2] += fHalftw;
		glyphs[i].uvs[3] += fHalfth;
	}


    // Read texture data.
    unsigned char* textureData = new unsigned char[textureByteCount];
    if (_stream->read(textureData, 1, textureByteCount) != textureByteCount)
    {
        GP_ERROR("Failed to read texture data for font '%s'.", id);
        SAFE_DELETE_ARRAY(glyphs);
        SAFE_DELETE_ARRAY(textureData);
        return NULL;
    }

    // Create the texture for the font.
	Texture* texture = GetRenderDevice()->CreateTexture(width,height,FMT_A8,false);
	DataStream* pDataSteam = new DataStream(textureData,textureByteCount);
	GetRenderSystem()->TexStreamComplete(texture,pDataSteam);

    // Free the texture data (no longer needed).
    //SAFE_DELETE_ARRAY(textureData);

    if (texture == NULL)
    {
        GP_ERROR("Failed to create texture for font '%s'.", id);
        SAFE_DELETE_ARRAY(glyphs);
        return NULL;
    }

    // Create the font.
    Font* font = Font::create(family.c_str(), Font::PLAIN, size, glyphs, glyphCount, texture);

    // Free the glyph array.
    SAFE_DELETE_ARRAY(glyphs);

    // Release the texture since the Font now owns it.
    //SAFE_RELEASE(texture);

    if (font)
    {
        font->_path = _path;
        font->_id = id;
    }

    return font;
}


bool Bundle::contains(const char* id) const
{
    return (find(id) != NULL);
}

unsigned int Bundle::getObjectCount() const
{
    return _referenceCount;
}

const char* Bundle::getObjectId(unsigned int index) const
{
    ASSERT(_references);
    return (index >= _referenceCount ? NULL : _references[index].id.c_str());
}

Bundle::Reference::Reference()
    : type(0), offset(0)
{
}

Bundle::Reference::~Reference()
{
}


}
