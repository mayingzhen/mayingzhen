#ifndef BUNDLE_H_
#define BUNDLE_H_

#include "Font.h"

namespace ma
{

/**
 * Represents a ma bundle file (.gpb) that contains a
 * collection of binary game assets that can be loaded.
 */
class Bundle : public RefCountObject
{
    friend class PhysicsController;
    friend class SceneLoader;

public:

    /**
     * Returns a Bundle for the given resource path.
     *
     * The specified path must reference a valid ma bundle file.
     * If the bundle is already loaded, the existing bundle is returned
     * with its reference count incremented. When no longer needed, the
     * release() method must be called. Note that calling release() does
     * NOT free any actual game objects created/returned from the Bundle
     * instance and those objects must be released separately.
     * @script{create}
     */
    static Bundle* create(const char* path);

    /**
     * Loads a font with the specified ID from the bundle.
     *
     * @param id The ID of the font to load.
     * 
     * @return The loaded font, or NULL if the font could not be loaded.
     * @script{create}
     */
    Font* loadFont(const char* id);

    /**
     * Determines if this bundle contains a top-level object with the given ID.
     *
     * This method performs a case-sensitive comparison.
     *
     * @param id The ID of the object to search for.
     */
    bool contains(const char* id) const;

    /**
     * Returns the number of top-level objects in this bundle.
     */
    unsigned int getObjectCount() const;

    /**
     * Returns the unique identifier of the top-level object at the specified index in this bundle.
     *
     * @param index The index of the object.
     * 
     * @return The ID of the object at the given index, or NULL if index is invalid.
     */
    const char* getObjectId(unsigned int index) const;

private:

    class Reference
    {
    public:
        std::string id;
        unsigned int type;
        unsigned int offset;

        /**
         * Constructor.
         */
        Reference();

        /**
         * Destructor.
         */
        ~Reference();
    };


    Bundle(const char* path);

    /**
     * Destructor.
     */
    ~Bundle();

    /**
     * Hidden copy assignment operator.
     */
    Bundle& operator=(const Bundle&);

    /**
     * Finds a reference by ID.
     */
    Reference* find(const char* id) const;


    /**
     * Returns the ID of the object at the current file position.
     * Returns NULL if not found.
     * 
     * @return The ID string or NULL if not found.
     */
    const char* getIdFromOffset() const;

    /**
     * Returns the ID of the object at the given file offset by searching through the reference table.
     * Returns NULL if not found.
     *
     * @param offset The file offset.
     * 
     * @return The ID string or NULL if not found.
     */
    const char* getIdFromOffset(unsigned int offset) const;

    /**
     * Gets the path to the bundle's default material file, if it exists.
     * 
     * @return The bundle's default material path. Returns an empty string if the default material does not exist.
     */
    const std::string& getMaterialPath();

    /**
     * Seeks the file pointer to the object with the given ID and type
     * and returns the relevant Reference.
     *
     * @param id The ID string to search for.
     * @param type The object type.
     * 
     * @return The reference object or NULL if there was an error.
     */
    Reference* seekTo(const char* id, unsigned int type);

    /**
     * Seeks the file pointer to the first object that matches the given type.
     * 
     * @param type The object type.
     * 
     * @return The reference object or NULL if there was an error.
     */
    Reference* seekToFirstType(unsigned int type);


    /**
     * Reads an unsigned int from the current file position.
     *
     * @param ptr A pointer to load the value into.
     * 
     * @return True if successful, false if an error occurred.
     */
    bool read(unsigned int* ptr);

    /**
     * Reads an unsigned char from the current file position.
     * 
     * @param ptr A pointer to load the value into.
     * 
     * @return True if successful, false if an error occurred.
     */
    bool read(unsigned char* ptr);

    /**
     * Reads a float from the current file position.
     * 
     * @param ptr A pointer to load the value into.
     * 
     * @return True if successful, false if an error occurred.
     */
    bool read(float* ptr);

    /**
     * Reads an array of values and the array length from the current file position.
     * 
     * @param length A pointer to where the length of the array will be copied to.
     * @param ptr A pointer to the array where the data will be copied to.
     * 
     * @return True if successful, false if an error occurred.
     */
    template <class T>
    bool readArray(unsigned int* length, T** ptr);

    /**
     * Reads an array of values and the array length from the current file position.
     * 
     * @param length A pointer to where the length of the array will be copied to.
     * @param values A pointer to the vector to copy the values to. The vector will be resized if it is smaller than length.
     * 
     * @return True if successful, false if an error occurred.
     */
    template <class T>
    bool readArray(unsigned int* length, std::vector<T>* values);

    /**
     * Reads an array of values and the array length from the current file position.
     * 
     * @param length A pointer to where the length of the array will be copied to.
     * @param values A pointer to the vector to copy the values to. The vector will be resized if it is smaller than length.
     * @param readSize The size that reads will be performed at, size must be the same as or smaller then the sizeof(T)
     * 
     * @return True if successful, false if an error occurred.
     */
    template <class T>
    bool readArray(unsigned int* length, std::vector<T>* values, unsigned int readSize);
    
    /**
     * Reads 16 floats from the current file position.
     *
     * @param m A pointer to float array of size 16.
     * 
     * @return True if successful, false if an error occurred.
     */
    bool readMatrix(float* m);

    /**
     * Reads an xref string from the current file position.
     * 
     * @param id The string to load the ID string into.
     * 
     * @return True if successful, false if an error occurred.
     */
    bool readXref(std::string& id);

private:

    /**
     * Skips over a Node's data within a bundle.
     *
     * @return True if the Node was successfully skipped; false otherwise.
     */
    bool skipNode();

    std::string _path;
    std::string _materialPath;
    unsigned int _referenceCount;
    Reference* _references;
    Stream* _stream;
};

}

#endif
