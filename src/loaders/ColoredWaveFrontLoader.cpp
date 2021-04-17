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
    bool useGroups = !sceneManager->getParameters()->getAttributeAsBool(scene::OBJ_LOADER_IGNORE_GROUPS);
    bool useMaterials = !sceneManager->getParameters()->getAttributeAsBool(scene::OBJ_LOADER_IGNORE_MATERIAL_FILES);

    while (bufferPointer != bufferEnd) {
        switch (bufferPointer[0]) {
            case 'm': { // mtllib
                if (useMaterials) {
                    c8 name[WORD_BUFFER_LENGTH];
                    bufferPointer = goAndCopyNextWord(name, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
                    readMaterial(name, relativePath);
                }
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
                    case 'n': { // normal
                        core::vector3df normal;
                        bufferPointer = readVector3(bufferPointer, normal, bufferEnd);
                        normalsBuffer.push_back(normal);
                        break;
                    }
                    case 't': { // texture coords
                        core::vector2df textureCoords;
                        bufferPointer = readUV(bufferPointer, textureCoords, bufferEnd);
                        textureCoordBuffer.push_back(textureCoords);
                        break;
                    }
                    case 'c': { // vertex color
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
                if (useGroups) {
                    if (group[0] != 0) {
                        groupName = group;
                    } else {
                        groupName = "default";
                    }
                }
                materialHasChanged = true;
                break;
            }
            case 's': { // smoothing
                c8 smooth[WORD_BUFFER_LENGTH];
                bufferPointer = goAndCopyNextWord(smooth, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
                if (core::stringc("off") == smooth) {
                    smoothingGroup = 0;
                } else {
                    smoothingGroup = core::strtol10(smooth);
                }
                break;
            }
            case 'u': { // usemtl
                c8 name[WORD_BUFFER_LENGTH];
                bufferPointer = goAndCopyNextWord(name, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
                materialName = name;
                materialHasChanged = true;
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
                        normalsBuffer.size()
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
                    if (indices[1] != -1) {
                        vertex.Color = vertexColorBuffer[indices[1]];
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

const c8* ColoredWaveFrontLoader::readTextures(const c8* bufferPointer, const c8* const bufferEnd, SObjMtl* currentMaterial, const io::path& relativePath)
{
    ETextureType type = CTT_DIFFUSE_COLOR;

    if ((!strncmp(bufferPointer, "map_bump", 8)) || (!strncmp(bufferPointer, "bump", 4))) {
        type = CTT_SPECULAR_COLOR;
    } else if ((!strncmp(bufferPointer, "map_d", 5)) || (!strncmp(bufferPointer, "map_opacity", 11))) {
        type = CTT_AMBIENT_COLOR;
    } else if (!strncmp(bufferPointer, "map_refl", 8)) {
        type = CTT_SHININESS;
    }

    c8 textureNameBuffer[WORD_BUFFER_LENGTH];
    bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);

    f32 bumpiness = 6.0f;
    bool clamp = false;

    while (textureNameBuffer[0] == '-') {
        if (!strncmp(bufferPointer, "-bm", 3)) {
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
            currentMaterial->meshbuffer->Material.MaterialTypeParam = core::fast_atof(textureNameBuffer);
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
            continue;
        } else if (!strncmp(bufferPointer, "-blendu", 7)) {
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
        } else if (!strncmp(bufferPointer, "-blendv", 7)) {
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
        } else if (!strncmp(bufferPointer, "-cc", 3)) {
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
        } else if (!strncmp(bufferPointer, "-clamp", 6)) {
            bufferPointer = readBool(bufferPointer, clamp, bufferEnd);
        } else if (!strncmp(bufferPointer, "-texres", 7)) {
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
        } else if (!strncmp(bufferPointer, "-type", 5)) {
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
        } else if (!strncmp(bufferPointer, "-mm", 3)) {
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
        } else if (!strncmp(bufferPointer, "-o", 2)) {
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
            if (!core::isdigit(textureNameBuffer[0])) {
                continue;
            }
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
            if (!core::isdigit(textureNameBuffer[0])) {
                continue;
            }
        } else if (!strncmp(bufferPointer, "-s", 2)) {
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
            if (!core::isdigit(textureNameBuffer[0])) {
                continue;
            }
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
            if (!core::isdigit(textureNameBuffer[0])) {
                continue;
            }
        } else if (!strncmp(bufferPointer, "-t", 2)) {
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
            if (!core::isdigit(textureNameBuffer[0])) {
                continue;
            }
            bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
            if (!core::isdigit(textureNameBuffer[0])) {
                continue;
            }
        }

        bufferPointer = goAndCopyNextWord(textureNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
    }

    if (type == CTT_SPECULAR_COLOR && core::isdigit(textureNameBuffer[0])) {
        currentMaterial->meshbuffer->Material.MaterialTypeParam = core::fast_atof(textureNameBuffer);
    }
    if (clamp) {
        currentMaterial->meshbuffer->Material.setFlag(video::EMF_TEXTURE_WRAP, video::ETC_CLAMP);
    }

    io::path textureName(textureNameBuffer);
    textureName.replace('\\', '/');
    video::ITexture* texture = nullptr;
    bool isNewTexture = false;

    if (textureName.size()) {
        io::path textureNameWithUserPath(sceneManager->getParameters()->getAttributeAsString(scene::OBJ_TEXTURE_PATH));
        if (textureNameWithUserPath.size()) {
            textureNameWithUserPath += '/';
            textureNameWithUserPath += textureName;
        }
        if (fileSystem->existFile(textureNameWithUserPath)) {
            texture = sceneManager->getVideoDriver()->getTexture(textureNameWithUserPath);
        } else if (fileSystem->existFile(textureName)) {
            isNewTexture = sceneManager->getVideoDriver()->findTexture(textureName) == 0;
            texture = sceneManager->getVideoDriver()->getTexture(textureName);
        } else {
            isNewTexture = sceneManager->getVideoDriver()->findTexture(relativePath + textureName) == 0;
            texture = sceneManager->getVideoDriver()->getTexture(relativePath + textureName);
        }
    }

    if (texture) {
        switch (type) {
            case CTT_DIFFUSE_COLOR:
                currentMaterial->meshbuffer->Material.setTexture(0, texture);
                break;
            case CTT_SPECULAR_COLOR:
                if (isNewTexture) {
                    sceneManager->getVideoDriver()->makeNormalMapTexture(texture, bumpiness);
                }
                currentMaterial->meshbuffer->Material.setTexture(1, texture);
                currentMaterial->meshbuffer->Material.MaterialType = video::EMT_PARALLAX_MAP_SOLID;
                currentMaterial->meshbuffer->Material.MaterialTypeParam = 0.035f;
                break;
            case CTT_AMBIENT_COLOR:
                currentMaterial->meshbuffer->Material.setTexture(0, texture);
                currentMaterial->meshbuffer->Material.MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
                break;
            case CTT_SHININESS:
                currentMaterial->meshbuffer->Material.setTexture(1, texture);
                currentMaterial->meshbuffer->Material.MaterialType = video::EMT_REFLECTION_2_LAYER;
                break;
        }

        currentMaterial->meshbuffer->Material.DiffuseColor.set(
            currentMaterial->meshbuffer->Material.DiffuseColor.getAlpha(), 255, 255, 255
        );
    }

    return bufferPointer;
}

void ColoredWaveFrontLoader::readMaterial(const c8* fileName, const io::path& relativePath)
{
    const io::path realFile(fileName);
    io::IReadFile* materialReader;

    if (fileSystem->existFile(realFile)) {
        materialReader = fileSystem->createAndOpenFile(realFile);
    } else if (fileSystem->existFile(relativePath + realFile)) {
        materialReader = fileSystem->createAndOpenFile(relativePath + realFile);
    } else if (fileSystem->existFile(fileSystem->getFileBasename(realFile))) {
        materialReader = fileSystem->createAndOpenFile(fileSystem->getFileBasename(realFile));
    } else {
        materialReader = fileSystem->createAndOpenFile(relativePath + fileSystem->getFileBasename(realFile));
    }

    if (!materialReader) {
        return;
    }

    const long fileSize = materialReader->getSize();
    if (!fileSize) {
        materialReader->drop();
        return;
    }

    c8* buffer = new c8[fileSize];
    materialReader->read((void*)buffer, fileSize);
    const c8* bufferEnd = buffer + fileSize;

    SObjMtl* currentMaterial = nullptr;

    const c8* bufferPointer = buffer;

    while (bufferPointer != bufferEnd) {
        switch (*bufferPointer) {
            case 'n': { // newmtl
                if (currentMaterial) {
                    materials.push_back(currentMaterial);
                }
                c8 materialNameBuffer[WORD_BUFFER_LENGTH];
                bufferPointer = goAndCopyNextWord(materialNameBuffer, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
                currentMaterial = new SObjMtl();
                currentMaterial->name = materialNameBuffer;
                break;
            }
            case 'i': { // illum
                if (currentMaterial) {
                    const u32 COLOR_BUFFER_LENGTH = 16;
                    c8 illumString[COLOR_BUFFER_LENGTH];
                    bufferPointer = goAndCopyNextWord(illumString, bufferPointer, COLOR_BUFFER_LENGTH, bufferEnd);
                    currentMaterial->illumination = (c8)atol(illumString);        
                }
                break;
            }
            case 'N': {
                if (currentMaterial) {
                    switch (bufferPointer[1]) {
                        case 's': { // Ns - shininess
                            const u32 COLOR_BUFFER_LENGTH = 16;
                            c8 shininessString[COLOR_BUFFER_LENGTH];
                            bufferPointer = goAndCopyNextWord(shininessString, bufferPointer, COLOR_BUFFER_LENGTH, bufferEnd);
                            f32 shininessValue = core::fast_atof(shininessString);
                            shininessValue *= 0.128f;
                            currentMaterial->meshbuffer->Material.Shininess = shininessValue;
                            break;
                        }
                        case 'i': { // Ni - refraction index
                            c8 refractionString[WORD_BUFFER_LENGTH];
                            bufferPointer = goAndCopyNextWord(refractionString, bufferPointer, WORD_BUFFER_LENGTH, bufferEnd);
                            break;
                        }
                    }
                }
                break;
            }
            case 'K': {
                if (currentMaterial) {
                    switch (bufferPointer[1]) {
                        case 'd': { // Kd - diffuse
                            bufferPointer = readColor(bufferPointer, currentMaterial->meshbuffer->Material.DiffuseColor, bufferEnd);
                            break;
                        }
                        case 's': { // Ks - specular
                            bufferPointer = readColor(bufferPointer, currentMaterial->meshbuffer->Material.SpecularColor, bufferEnd);
                            break;
                        }
                        case 'a': { // Ka - ambient
                            bufferPointer = readColor(bufferPointer, currentMaterial->meshbuffer->Material.AmbientColor, bufferEnd);
                            break;
                        }
                        case 'e': { // Ke - emissive
                            bufferPointer = readColor(bufferPointer, currentMaterial->meshbuffer->Material.EmissiveColor, bufferEnd);
                            break;
                        }
                        default:
                            break;
                    }
                }
                break;
            }
            case 'b': // bump
            case 'm': { // texture maps
                if (currentMaterial) {
                    bufferPointer = readTextures(bufferPointer, bufferEnd, currentMaterial, relativePath);
                }
                break;
            }
            case 'd': { // transparency
                const u32 COLOR_BUFFER_LENGTH = 16;
                c8 transparencyString[COLOR_BUFFER_LENGTH];
                bufferPointer = goAndCopyNextWord(transparencyString, bufferPointer, COLOR_BUFFER_LENGTH, bufferEnd);
                f32 value = core::fast_atof(transparencyString);
                currentMaterial->meshbuffer->Material.DiffuseColor.setAlpha((s32)(value * 255));
                if (value < 1.0f) {
                    currentMaterial->meshbuffer->Material.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
                }
                break;
            }
            case 'T': {
                if (currentMaterial) {
                    switch (bufferPointer[1]) {
                        case 'f': { // Tf - transmitivity
                            const u32 COLOR_BUFFER_LENGTH = 16;
                            c8 redString[COLOR_BUFFER_LENGTH];
                            c8 greenString[COLOR_BUFFER_LENGTH];
                            c8 blueString[COLOR_BUFFER_LENGTH];

                            bufferPointer = goAndCopyNextWord(redString, bufferPointer, COLOR_BUFFER_LENGTH, bufferEnd);
                            bufferPointer = goAndCopyNextWord(greenString, bufferPointer, COLOR_BUFFER_LENGTH, bufferEnd);
                            bufferPointer = goAndCopyNextWord(blueString, bufferPointer, COLOR_BUFFER_LENGTH, bufferEnd);

                            f32 transparency = (core::fast_atof(redString) + core::fast_atof(greenString) + core::fast_atof(blueString)) / 3;

                            currentMaterial->meshbuffer->Material.DiffuseColor.setAlpha((s32)(transparency * 255));
                            if (transparency < 1.0f) {
                                currentMaterial->meshbuffer->Material.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
                            }

                            break;
                        }
                        default:
                            break;
                    }
                }
                break;
            }
            default:
                break;
        }

        bufferPointer = goNextLine(bufferPointer, bufferEnd);
    }

    if (currentMaterial) {
        materials.push_back(currentMaterial);
    }

    delete[] buffer;
    materialReader->drop();
}

ColoredWaveFrontLoader::SObjMtl* ColoredWaveFrontLoader::findMaterial(const core::stringc& materialName, const core::stringc& groupName)
{
    ColoredWaveFrontLoader::SObjMtl* defaultMaterial = nullptr;

    for (u32 i = 0; i < materials.size(); ++i) {
        if (materials[i]->name == materialName) {
            if (materials[i]->group == groupName) {
                return materials[i];
            } else {
                defaultMaterial = materials[i];
            }
        }
    }

    if (defaultMaterial) {
        materials.push_back(new SObjMtl(*defaultMaterial));
        materials.getLast()->group = groupName;
        return materials.getLast();
    } else if (groupName.size()) {
        materials.push_back(new SObjMtl(*materials[0]));
        materials.getLast()->group = groupName;
        return materials.getLast();
    }

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

bool ColoredWaveFrontLoader::retrieveVertexIndices(c8* vertexData, s32* indices, const c8* bufferEnd, u32 vbSize, u32 vtSize, u32 vnSize)
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
                }
            } else {
                indices[indexType] -= 1;
            }

            word[0] = '\0';
            i = 0;

            if (*pointer == '/') {
                if (++indexType > 2) {
                    indexType = 0;
                }
            } else {
                while (++indexType < 3) {
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
