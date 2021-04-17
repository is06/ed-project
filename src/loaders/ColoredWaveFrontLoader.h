#ifndef COLORED_WAVEFRONT_LOADER_H
#define COLORED_WAVEFRONT_LOADER_H

#include <irrlicht/irrlicht.h>

using namespace irr;

/**
 * Modified WaveFront format loader base on work by Irrlicht contributors
 */
class ColoredWaveFrontLoader : public scene::IMeshLoader
{
public:
    ColoredWaveFrontLoader(scene::ISceneManager* sceneManager, io::IFileSystem* fileSystem);
    virtual ~ColoredWaveFrontLoader();

    virtual bool isALoadableFileExtension(const io::path& filename) const;
    virtual scene::IAnimatedMesh* createMesh(io::IReadFile* file);

private:
    struct SObjMtl
    {
        SObjMtl() :
            meshbuffer(0),
            bumpiness(1.0f),
            illumination(0),
            recalculateNormals(false)
        {
            meshbuffer = new scene::SMeshBuffer();
            meshbuffer->Material.Shininess = 0.0f;
            meshbuffer->Material.AmbientColor = video::SColorf(0.2f, 0.2f, 0.2f, 1.0f).toSColor();
            meshbuffer->Material.DiffuseColor = video::SColorf(0.8f, 0.8f, 0.8f, 1.0f).toSColor();
            meshbuffer->Material.SpecularColor = video::SColorf(1.0f, 1.0f, 1.0f, 1.0f).toSColor();
        }

        SObjMtl(const SObjMtl& o) :
            name(o.name),
            group(o.group),
            bumpiness(o.bumpiness),
            illumination(o.illumination),
            recalculateNormals(false)
        {
            meshbuffer = new scene::SMeshBuffer();
            meshbuffer->Material = o.meshbuffer->Material;
        }

        core::map<video::S3DVertex, int> vertMap;
        scene::SMeshBuffer *meshbuffer;
        core::stringc name;
        core::stringc group;
        f32 bumpiness;
        c8 illumination;
        bool recalculateNormals;
    };

    enum ETextureType {
        CTT_DIFFUSE_COLOR = 0,
        CTT_SPECULAR_COLOR = 1,
        CTT_AMBIENT_COLOR = 2,
        CTT_SHININESS = 3
    };

    const c8* readTextures(const c8* bufferPointer, const c8* const bufferEnd, SObjMtl* currentMaterial, const io::path& relativePath);
    const c8* goFirstWord(const c8* bufferPointer, const c8* const bufferEnd, bool acrossNewlines=true);
    const c8* goNextWord(const c8* bufferPointer, const c8* const bufferEnd, bool acrossNewlines=true);
    const c8* goNextLine(const c8* bufferPointer, const c8* const bufferEnd);
    u32 copyWord(c8* outBuffer, const c8* inBuffer, u32 outBufferLength, const c8* const bufferEnd);
    core::stringc copyLine(const c8* inBuffer, const c8* const bufferEnd);
    const c8* goAndCopyNextWord(c8* outBuffer, const c8* inBuffer, u32 outBufferLength, const c8* const bufferEnd);
    void readMaterial(const c8* fileName, const io::path& relativePath);
    SObjMtl* findMaterial(const core::stringc& materialName, const core::stringc& groupName);
    const c8* readColor(const c8* bufferPointer, video::SColor& color, const c8* const bufferEnd);
    const c8* readVector3(const c8* bufferPointer, core::vector3df& vector, const c8* const bufferEnd);
    const c8* readUV(const c8* bufferPointer, core::vector2df& vector, const c8* const bufferEnd);
    const c8* readBool(const c8* bufferPointer, bool& flag, const c8* const bufferEnd);
    bool retrieveVertexIndices(c8* vertexData, s32* indices, const c8* bufferEnd, u32 vbSize, u32 vtSize, u32 vnSize);
    void cleanUp();

    core::array<SObjMtl*> materials;

    scene::ISceneManager* sceneManager;
    io::IFileSystem* fileSystem;
};

#endif
