
#include <iostream>
#include <fstream>
#include "../logmanager/logmanager.h"
#include "../data/object.h"
#include "assetmanager.h"

bool AssetManager::startup() {
    LogManager::log_info("AssetManager started.", 2);
    return true;
}

object* AssetManager::load_object(std::string name) {
    LogManager::log_info("-- loading object '" + name + "'", 3);

    std::string filename = "resources/" + name + ".hr";
    std::ifstream is (filename, std::ifstream::binary);
    if (!is) {
        LogManager::log_error("Failed to open " + filename + ".");
        return NULL;
    }

    // get length of file:
    is.seekg (0, is.end);
    int filesize = is.tellg();
    is.seekg (0, is.beg);

    char* buffer = new char[filesize];
    is.read(buffer, filesize);
    if(!is) {
        LogManager::log_warning("Failed to read " + filename + ".", 0);
        delete[] buffer;
        return NULL;
    }
    is.close();

    if(filesize < 10) {
        LogManager::log_warning("The file " + filename + "is *really* small (not loading it). Corrupt?", 0);
        delete[] buffer;
        return NULL;
    }
    if(buffer[0] != 'h' || buffer[1] != 'r' || buffer[2] != '0' || buffer[3] != '0') {
        LogManager::log_warning("The file " + filename + " does not start with the correct header. Corrupt?", 0);
        delete[] buffer;
        return NULL;
    }

    bool has_pos = (buffer[4] == 'p');
    bool has_col = (buffer[5] == 'c');
    bool has_nor = (buffer[6] == 'n');

    float* data = (float*)(buffer + 8);
    mesh m = mesh_from_data(data, filesize - 8, has_pos, has_col, has_nor);
    delete [] buffer;

    object* o = new object();
    o->meshes.push_back(m);

    return o;
}

void AssetManager::shutdown() {
    LogManager::log_info("AssetManager stopped.", 2);
}

// data_size is in bytes
mesh AssetManager::mesh_from_data(float * data, int data_size, bool has_pos, bool has_col, bool has_nor) {
    int stride = (has_pos ? 4 : 0) + (has_col ? 4 : 0) + (has_nor ? 4 : 0);
    stride = sizeof(float) * stride;

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint buffer_gl;
    glGenBuffers(1, &buffer_gl);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_gl);
    glBufferData(GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW);

    int offset = 0;
    if(has_pos) {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        offset += 4*sizeof(GL_FLOAT);
    }
    if(has_col) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        offset += 4*sizeof(GL_FLOAT);
    }
    if(has_nor) {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        offset += 4*sizeof(GL_FLOAT);
    }
    assert(offset == stride);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    mesh m;
    m.vao = vao;
    m.mode = GL_TRIANGLES;
    m.first = 0;
    m.count = data_size / stride;

    return m;
}

