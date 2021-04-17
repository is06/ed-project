#include "ColoredWaveFrontLoader.h"

static const u32 WORD_BUFFER_LENGTH = 512;

ColoredWaveFrontLoader::ColoredWaveFrontLoader(scene::ISceneManager* sceneManager, io::IFileSystem* fileSystem)
{
    this->sceneManager = sceneManager;
    this->fileSystem = fileSystem;
}

bool ColoredWaveFrontLoader::isALoadableFileExtension(const io::path& filename) const
{
    return core::hasFileExtension(filename, "cwf");
}

scene::IAnimatedMesh* ColoredWaveFrontLoader::createMesh(io::IReadFile* file)
{
    // File data
    const long fileSize = file->getSize();
    if (!fileSize) {
        return nullptr;
    }
    const io::path fullName = file->getFileName();
    const io::path relativePath = fileSystem->getFileDir(fullName) + "/";

    // File buffer
    c8* buffer = new c8[fileSize];
    memset(buffer, 0, fileSize);
    file->read((void*)buffer, fileSize);
    const c8* const bufferEnd = buffer + fileSize;
    const c8* bufferPointer = buffer;

    // Mesh buffers
    core::array<core::vector3df> vertexBuffer;
    core::array<video::SColor> vertexColorBuffer;
    core::array<core::vector3df> normalsBuffer;
    core::array<core::vector2df> textureCoordBuffer;

    // Temporary objects
    SObjMtl* currentMaterial = new SObjMtl();
    materials.push_back(currentMaterial);
    u32 smoothingGroup = 0;

    // Process file
    core::stringc groupName, materialName;
    bool materialHasChanged = false;

    while (bufferPointer != bufferEnd) {
        switch (bufferPointer[0]) {
            case 'm': { // mtllib
                c8 name[WORD_BUFFER_LENGTH];
                bufferPointer = goAndCopyNextWord(name, bufferPointer, WORD_BUFFER_LENGTH, bufferPointer);
                readMaterial(name, relativePath);
                break;
            }
            case 'v': { // vertices
                switch (bufferPointer[1]) {
                    case ' ': { // vertex coords
                        core::vector3df coords;
                        bufferPointer = readVector3(bufferPointer, coords, bufferEnd);
                        vertexBuffer.push_back(coords);
                        break;
                    }
                    case 'n': {
                        core::vector3df normal;
                        bufferPointer = readVector3(bufferPointer, normal, bufferEnd);
                        normalsBuffer.push_back(normal);
                        break;
                    }
                    case 't': {
                        core::vector2df textureCoords;
                        bufferPointer = readUV(bufferPointer, textureCoords, bufferEnd);
                        textureCoordBuffer.push_back(textureCoords);
                        break;
                    }
                    case 'c': {
                        video::SColor color;
                        bufferPointer = readColor(bufferPointer, color, bufferEnd);
                        vertexColorBuffer.push_back(color);
                        break;
                    } 
                }
                break;
            }
            case 'g': { // group
                c8 group[WORD_BUFFER_LENGTH];
                bufferPointer = goAndCopyNextWord(group, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
                if (group[0] != 0) {
                    groupName = group;
                } else {
                    groupName = "default";
                }
                materialHasChanged = true;
                break;
            }
            case 's': { // smoothing

                break;
            }
            case 'u': { // usemtl

                break;
            }
            case 'f': { // face
                c8 vertexWord[WORD_BUFFER_LENGTH];
                video::S3DVertex vertex;

                if (materialHasChanged) {
                    SObjMtl* useMaterial = findMaterial(materialName, groupName);
                    if (useMaterial) {
                        currentMaterial = useMaterial;
                    }
                    materialHasChanged = false;
                }
                if (currentMaterial) {
                    vertex.Color = currentMaterial->meshbuffer->Material.DiffuseColor;
                }

                const core::stringc wordBuffer = copyLine(bufferPointer, bufferEnd);
                const c8* linePointer = wordBuffer.c_str();
                const c8* const endPointer = linePointer + wordBuffer.size();

                core::array<int> faceCorners;
                faceCorners.reallocate(32);

                linePointer = goNextWord(linePointer, endPointer);

                while (linePointer[0] != 0) {
                    s32 indices[4];
                    indices[1] = indices[2] = indices[3] = -1;
                    u32 wordLength = copyWord(vertexWord, linePointer, WORD_BUFFER_LENGTH, endPointer);

                    retrieveVertexIndices(
                        vertexWord,
                        indices,
                        vertexWord + wordLength + 1,
                        vertexBuffer.size(),
                        textureCoordBuffer.size(),
                        normalsBuffer.size(),
                        vertexColorBuffer.size()
                    );

                    // Coords
                    vertex.Pos = vertexBuffer[indices[0]];

                    // Texture coords
                    if (indices[1] != -1) {
                        vertex.TCoords = textureCoordBuffer[indices[1]];
                    } else {
                        vertex.TCoords.set(0.0f, 0.0f);
                    }

                    // Normal
                    if (indices[2] != -1) {
                        vertex.Normal = normalsBuffer[indices[2]];
                    } else {
                        vertex.Normal.set(0.0f, 0.0f, 0.0f);
                        currentMaterial->recalculateNormals = true;
                    }

                    // Vertex colors
                    if (indices[3] != -1) {
                        vertex.Color = vertexColorBuffer[indices[3]];
                    }

                    int vertexLocation;
                    core::map<video::S3DVertex, int>::Node* node = currentMaterial->vertMap.find(vertex);
                    if (node) {
                        vertexLocation = node->getValue();
                    } else {
                        currentMaterial->meshbuffer->Vertices.push_back(vertex);
                        vertexLocation = currentMaterial->meshbuffer->Vertices.size() - 1;
                        currentMaterial->vertMap.insert(vertex, vertexLocation);
                    }

                    faceCorners.push_back(vertexLocation);
                    linePointer = goNextWord(linePointer, endPointer);
                }

                for (u32 i = 1; i < faceCorners.size() - 1; ++i) {
                    currentMaterial->meshbuffer->Indices.push_back(faceCorners[i+1]);
                    currentMaterial->meshbuffer->Indices.push_back(faceCorners[i]);
                    currentMaterial->meshbuffer->Indices.push_back(faceCorners[0]);
                }
                faceCorners.set_used(0);
                faceCorners.reallocate(32);

                break;
            }
            case '#':
            default:
                break;
        }

        bufferPointer = goNextLine(bufferPointer, bufferEnd);
    }

    scene::SMesh* mesh = new scene::SMesh();

    for (u32 m = 0; m < materials.size(); ++m) {
        if (materials[m]->meshbuffer->getIndexCount() > 0) {
            materials[m]->meshbuffer->recalculateBoundingBox();
            if (materials[m]->recalculateNormals) {
                sceneManager->getMeshManipulator()->recalculateNormals(materials[m]->meshbuffer);
            }
            if (materials[m]->meshbuffer->Material.MaterialType == video::EMT_PARALLAX_MAP_SOLID) {
                scene::SMesh sourceMesh;
                sourceMesh.addMeshBuffer(materials[m]->meshbuffer);
                scene::IMesh* tangentMesh = sceneManager->getMeshManipulator()->createMeshWithTangents(&sourceMesh);
                mesh->addMeshBuffer(tangentMesh->getMeshBuffer(0));
                tangentMesh->drop();
            } else {
                mesh->addMeshBuffer(materials[m]->meshbuffer);
            }
        }
    }

    scene::SAnimatedMesh* animatedMesh = nullptr;
    if (mesh->getMeshBufferCount() != 0) {
        mesh->recalculateBoundingBox();
        animatedMesh = new scene::SAnimatedMesh();
        animatedMesh->Type = scene::EAMT_OBJ;
        animatedMesh->addMesh(mesh);
        animatedMesh->recalculateBoundingBox();
    }

    delete[] buffer;
    cleanUp();
    mesh->drop();

    return animatedMesh;
}

void ColoredWaveFrontLoader::readMaterial(const c8* fileName, const io::path& relativePath)
{

}

ColoredWaveFrontLoader::SObjMtl* ColoredWaveFrontLoader::findMaterial(const core::stringc& materialName, const core::stringc& groupName)
{
    ColoredWaveFrontLoader::SObjMtl* material = nullptr;

    return nullptr;
}

const c8* ColoredWaveFrontLoader::readColor(const c8* bufferPointer, video::SColor& color, const c8* const bufferEnd)
{
    const u32 BUFFER_LENGTH = 16;
    c8 colorString[BUFFER_LENGTH];

    color.setAlpha(255);

    bufferPointer = goAndCopyNextWord(colorString, bufferPointer, BUFFER_LENGTH, bufferEnd);
    color.setRed((s32)(core::fast_atof(colorString) * 255.0f));

    bufferPointer = goAndCopyNextWord(colorString, bufferPointer, BUFFER_LENGTH, bufferEnd);
    color.setGreen((s32)(core::fast_atof(colorString) * 255.0f));

    bufferPointer = goAndCopyNextWord(colorString, bufferPointer, BUFFER_LENGTH, bufferEnd);
    color.setBlue((s32)(core::fast_atof(colorString) * 255.0f));

    return bufferPointer;
}

const c8* ColoredWaveFrontLoader::readVector3(const c8* bufferPointer, core::vector3df& vector, const c8* const bufferEnd)
{
    const u32 BUFFER_LENGTH = 256;
    c8 wordBuffer[BUFFER_LENGTH];

    bufferPointer = goAndCopyNextWord(wordBuffer, bufferPointer, BUFFER_LENGTH, bufferEnd);
    vector.X = -core::fast_atof(wordBuffer);

    bufferPointer = goAndCopyNextWord(wordBuffer, bufferPointer, BUFFER_LENGTH, bufferEnd);
    vector.Y = core::fast_atof(wordBuffer);

    bufferPointer = goAndCopyNextWord(wordBuffer, bufferPointer, BUFFER_LENGTH, bufferEnd);
    vector.Z = core::fast_atof(wordBuffer);

    return bufferPointer;
}

const c8* ColoredWaveFrontLoader::readUV(const c8* bufferPointer, core::vector2df& vector, const c8* const bufferEnd)
{
    const u32 BUFFER_LENGTH = 256;
    c8 wordBuffer[BUFFER_LENGTH];

    bufferPointer = goAndCopyNextWord(wordBuffer, bufferPointer, BUFFER_LENGTH, bufferEnd);
    vector.X = core::fast_atof(wordBuffer);

    bufferPointer = goAndCopyNextWord(wordBuffer, bufferPointer, BUFFER_LENGTH, bufferEnd);
    vector.Y = 1 - core::fast_atof(wordBuffer);

    return bufferPointer;
}

const c8* ColoredWaveFrontLoader::readBool(const c8* bufferPointer, bool& flag, const c8* const bufferEnd)
{
    const u32 BUFFER_LENGTH = 8;
    c8 flagString[BUFFER_LENGTH];

    bufferPointer = goAndCopyNextWord(flagString, bufferPointer, BUFFER_LENGTH, bufferEnd);
    flag = strcmp(flagString, "off") != 0;
    
    return bufferPointer;
}

const c8* ColoredWaveFrontLoader::goFirstWord(const c8* bufferPointer, const c8* const bufferEnd, bool acrossNewlines)
{
    if (acrossNewlines) {
        while ((bufferPointer != bufferEnd) && core::isspace(*bufferPointer)) {
            ++bufferPointer;
        }
    } else {
        while ((bufferPointer != bufferEnd) && core::isspace(*bufferPointer) && (*bufferPointer != '\n')) {
            ++bufferPointer;
        }
    }

    return bufferPointer;
}

const c8* ColoredWaveFrontLoader::goNextWord(const c8* bufferPointer, const c8* const bufferEnd, bool acrossNewlines)
{
    while ((bufferPointer != bufferEnd) && !core::isspace(*bufferPointer)) {
        ++bufferPointer;
    }

    return goFirstWord(bufferPointer, bufferEnd, acrossNewlines);
}

const c8* ColoredWaveFrontLoader::goNextLine(const c8* bufferPointer, const c8* const bufferEnd)
{
    while (bufferPointer != bufferEnd) {
        if (*bufferPointer == '\n' || *bufferPointer == '\r') {
            break;
        }
        ++bufferPointer;
    }

    return goFirstWord(bufferPointer, bufferEnd);
}

u32 ColoredWaveFrontLoader::copyWord(c8* outBuffer, const c8* const inBuffer, u32 outBufferLength, const c8* const bufferEnd)
{
    if (!outBufferLength) {
        return 0;
    }
    if (!inBuffer) {
        *outBuffer = 0;
        return 0;
    }

    u32 i = 0;
    while (inBuffer[i]) {
        if (core::isspace(inBuffer[i]) || &(inBuffer[i]) == bufferEnd) {
            break;
        }
        ++i;
    }
    
    u32 length = core::min_(i, outBufferLength - 1);
    for (u32 j = 0; j < length; ++j) {
        outBuffer[j] = inBuffer[j];
    }

    outBuffer[length] = 0;

    return length;
}

core::stringc ColoredWaveFrontLoader::copyLine(const c8* inBuffer, const c8* bufferEnd)
{
    if (!inBuffer) {
        return core::stringc();
    }

    const c8* pointer = inBuffer;
    while (pointer < bufferEnd) {
        if (*pointer == '\n' || *pointer == '\r') {
            break;
        }
        ++pointer;
    }
    
    return core::stringc(inBuffer, (u32)(pointer - inBuffer + ((pointer < bufferEnd) ? 1 : 0)));
}

const c8* ColoredWaveFrontLoader::goAndCopyNextWord(c8* outBuffer, const c8* inBuffer, u32 outBufferLength, const c8* bufferEnd)
{
    inBuffer = goNextWord(inBuffer, bufferEnd, false);
    copyWord(outBuffer, inBuffer, outBufferLength, bufferEnd);
    return inBuffer;
}

bool ColoredWaveFrontLoader::retrieveVertexIndices(c8* vertexData, s32* indices, const c8* bufferEnd, u32 vbSize, u32 vtSize, u32 vnSize, u32 vcSize)
{
    c8 word[16] = "";
    const c8* pointer = goFirstWord(vertexData, bufferEnd);
    u32 indexType = 0;

    u32 i = 0;
    while (pointer != bufferEnd) {
        if ((core::isdigit(*pointer)) || (*pointer == '-')) {
            word[i++] = *pointer;
        } else if (*pointer == '/' || *pointer == ' ' || *pointer == '\0') {
            word[i] = '\0';
            indices[indexType] = core::strtol10(word);
            if (indices[indexType] < 0) {
                switch (indexType) {
                    case 0:
                        indices[indexType] += vbSize;
                        break;
                    case 1:
                        indices[indexType] += vtSize;
                        break;
                    case 2:
                        indices[indexType] += vnSize;
                        break;
                    case 3:
                        indices[indexType] += vcSize;
                        break;
                }
            } else {
                indices[indexType] -= 1;
            }

            word[0] = '\0';
            i = 0;

            if (*pointer == '/') {
                if (++indexType > 3) {
                    indexType = 0;
                }
            } else {
                while (++indexType < 4) {
                    indices[indexType] = -1;
                }
                ++pointer;
                break;
            }
        }

        ++pointer;
    }

    return true;
}

void ColoredWaveFrontLoader::cleanUp()
{
    for (u32 i = 0; i < materials.size(); ++i) {
        materials[i]->meshbuffer->drop();
        delete materials[i];
    }

    materials.clear();
}

ColoredWaveFrontLoader::~ColoredWaveFrontLoader()
{
    if (fileSystem) {
        fileSystem->drop();
    }
}
