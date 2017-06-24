//PARSER OBJ
void split(string &in, vector<string> &out,char sep=' ') {
    out.clear();
    std::istringstream iss(in);
    std::string sub;

    while(std::getline(iss, sub, sep)) {
        out.push_back(sub);
    }
}

void processVertex(vector<string> &in,vector<vec3> &out,vec3 &total) {
    out.push_back(vec3(atof(in[1].c_str()),
                       atof(in[2].c_str()),
                       atof(in[3].c_str())
                       ));
    total=total+out.back();
}

void processNormal(vector<string> &in,vector<vec3> &out) {
    vec3 tmp;
    processVertex(in,out,tmp);
}

void processTexCoord(vector<string> &in,vector<vec2> &out) {
    out.push_back(vec2(atof(in[1].c_str()),
                       atof(in[2].c_str())
                       ));
}

void decompressVertex(string vertexStr,
                      vector<vec3> &vertices,vector<vec3> &normals, vector<vec2> &texCoords,
                      vector<float> &outV, vector<float> &outN, vector<float> &outT) {
    vector<string> tmp;
    split(vertexStr,tmp,'/');
    outV.push_back(vertices[atoi(tmp[0].c_str())-1].x);
    outV.push_back(vertices[atoi(tmp[0].c_str())-1].y);
    outV.push_back(vertices[atoi(tmp[0].c_str())-1].z);
    outT.push_back(texCoords[atoi(tmp[1].c_str())-1].x);
    outT.push_back(texCoords[atoi(tmp[1].c_str())-1].y);
    outN.push_back(normals[atoi(tmp[2].c_str())-1].x);
    outN.push_back(normals[atoi(tmp[2].c_str())-1].y);
    outN.push_back(normals[atoi(tmp[2].c_str())-1].z);
}

vec3 getPos(string vertexStr,vector<vec3> &vertices) {
    vector<string> tmp;
    split(vertexStr,tmp,'/');
    return vertices[atoi(tmp[0].c_str())-1];

}

void processFace(vector<string> &in,
                 vector<vec3> &vertices,vector<vec3> &normals, vector<vec2> &texCoords,
                 vector<float> &outV, vector<float> &outN, vector<float> &outT) {
    decompressVertex(in[1],vertices,normals,texCoords,outV,outN,outT);
    decompressVertex(in[2],vertices,normals,texCoords,outV,outN,outT);
    decompressVertex(in[3],vertices,normals,texCoords,outV,outN,outT);
    decompressVertex(in[1],vertices,normals,texCoords,outV,outN,outT);
    decompressVertex(in[3],vertices,normals,texCoords,outV,outN,outT);
    decompressVertex(in[4],vertices,normals,texCoords,outV,outN,outT);
}

void processObj(string filename,vector<float> &outV, vector<float> &outN, vector<float> &outT) {
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<vec2> texCoords;
    ifstream inFile;
    inFile.open(filename.c_str());
    string line;
    vector<string> tmp;
    vec3 total=vec3(0,0,0);
    while (getline(inFile,line)) {
        split(line,tmp);
        if (tmp.size()>0) {
            if (tmp[0]=="v") {
                processVertex(tmp,vertices,total);
                //printf("wierzcholek %f,%f,%f\n",vertices.back().x,vertices.back().y,vertices.back().z);
            }
            else if (tmp[0]=="vn") {
                processNormal(tmp,normals);
                //printf("normalna %f,%f,%f\n",normals.back().x,normals.back().y,normals.back().z);
                }
            else if (tmp[0]=="vt") {
                processTexCoord(tmp,texCoords);
                //printf("wsp. tex. %f,%f\n",texCoords.back().x,texCoords.back().y);
            }
        }
    }
    inFile.close();
    total=total/(float)vertices.size();
    for (int i=0;i<vertices.size();i++) {
        vertices[i]=vertices[i]-total;
    }
    inFile.open(filename.c_str());
    while (getline(inFile,line)) {
        split(line,tmp);
        if (tmp.size()>0) {
            if (tmp[0]=="f") {
                processFace(tmp,vertices,normals,texCoords,outV,outN,outT);
            }
        }
    }
    inFile.close();
    //printf("%d %d %d\n",vertices.size(),normals.size(),texCoords.size());
}


//w main na pocz¹tku poza pêtl¹ while
 processObj("obj/sprocket.obj",objV,objN,objT);


//w drawscene
glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
glEnableClientState(GL_TEXTURE_COORD_ARRAY); //Podczas rysowania u¿ywaj tablicy wsp. teksturowania
glEnableClientState(GL_NORMAL_ARRAY); //Podczas rysowania u¿ywaj tablicy wektorów normalnych
glVertexPointer(3,GL_FLOAT,0,objV.data()); //Ustaw tablicê myCubeVertices jako tablicê wierzcho³ków
glTexCoordPointer(2,GL_FLOAT,0,objT.data()); //Ustaw tablicê myCubeTexCoords jako tablicê wsp. teksturowania
glNormalPointer(GL_FLOAT,0,objN.data()); //Ustaw tablicê myCubeVertices jako tablicê wektorów normalnych - tutaj akurat wsp. wierzcho³ka=suma wektorów normalnych œcian s¹siaduj¹cych
glDrawArrays(GL_TRIANGLES,0,objV.size()/3); //Rysuj model

glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
glEnableClientState(GL_NORMAL_ARRAY);