
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


std::vector<object*> AssetManager::parse_lev(const std::string filename){
    std::ifstream input("resources/Levels/" + filename + ".lev");
    std::vector<object*> objects;
    int iter_num = 0;
    for(std::string line ; getline(input, line) ; ) {
        /* Check if the line is a .obj and line is not a comment*/
        if(line.find(".obj") != -1 && line.find("#") != 0){
            if(line.find(".obj") == 0){
                LogManager::log_error("You specified an invalid object.");
                continue;
            }
            std::string object_name = line;
            getline(input, line);
            double scale = 1.0;
            if(line.find("Scale:") != -1){
                line = line.substr(line.find(":")+1);
                size_t index;
                scale = std::stod(line, &index);
            }
            objects.push_back(new object(("resources/" + object_name).c_str(), false, scale));
            glm::dmat4 transformation;
            for(int i = 0; i < 4; i++){
                getline(input, line);
                size_t index;
                transformation[i][0] = std::stod(line, &index);
                line = line.substr(index);
                transformation[i][1] = std::stod(line, &index);
                line = line.substr(index);
                transformation[i][2] = std::stod(line, &index);
                line = line.substr(index);
                transformation[i][3] = std::stod(line, &index);
            }
            objects.back()->set_transformation(transformation);
        }
        /* Check if the next line is for a .hr file and line is not a comment*/
        else if(line.find(".hr") != -1  && line.find("#") != 0){
            if(line.find(".hr") == 0){
                LogManager::log_error("You specified an invalid object.");
                continue;
            }
            objects.push_back(AssetManager::load_object(line.substr(0,line.find(".hr"))));
            glm::dmat4 transformation;
            for(int i = 0; i < 4; i++){
                getline(input, line);
                size_t index;
                transformation[i][0] = std::stod(line, &index);
                line = line.substr(index);
                transformation[i][1] = std::stod(line, &index);
                line = line.substr(index);
                transformation[i][2] = std::stod(line, &index);
                line = line.substr(index);
                transformation[i][3] = std::stod(line, &index);
            }
            objects.back()->set_transformation(transformation);
        }
        /* Check for .stl */
        else if(line.find(".stl") != -1){
            LogManager::log_error("Currently only .hr and .obj files are supported. Please run the python converter to turn the .stl into a .hr file.");
        }
        /* Handle comments */
        else if(line.find("#") != -1){
            if(iter_num == 0){
                while(line.find("#")==0){
                    line = line.substr(1);
                }
                int last_octothorpe = line.find("#");
                if(last_octothorpe > 1){
                    line = line.substr(0, last_octothorpe);
                }
                LogManager::log_info("Loading Level: " + line, 2);
            }
            else{
                while(line.find("#")==0){
                    line = line.substr(1);
                }
                int last_octothorpe = line.find("#");
                if(last_octothorpe > 1){
                    line = line.substr(0, line.find("#"));
                }
                LogManager::log_info(line, 3);
            }
        }
        iter_num++;
    }
    return objects;
}
