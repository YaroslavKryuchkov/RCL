#include "model_workers.hpp"

#include <vector>
#include <iostream>

#include "vector.hpp"
#include "vertex_triangle.hpp"

namespace rcl
{

namespace
{
    void Triangulate(std::vector<Vertex>& verts, HittableList& triangles)
    {
        int num = verts.size();
        if(num < 3) return;

        if(num == 3)
        {
            triangles.Add(std::make_shared<VertexTriangle>(verts[0], verts[1], verts[2]));
            return;
        }

        for(int i = 3; i < num; i++)
        {
            triangles.Add(std::make_shared<VertexTriangle>(verts[0], verts[i-2], verts[i-1]));
        }
    }

    void ReadVertexInfo(std::string& str, std::vector<vec3>& vertPos,
        std::vector<rcl::vec2>& vertTexCoord, std::vector<vec3>& vertNormal)
    {
        if (str.find("v ") == 0)
        {
            int spacePos;
            rcl::vec3 vNewVertexPos(0.0f);

            str = str.substr(2, str.size());
            spacePos = str.find(" ");
            vNewVertexPos.x = std::stod(str.substr(0, spacePos));

            str = str.substr(spacePos + 1, str.size());
            spacePos = str.find(" ");
            vNewVertexPos.y = std::stod(str.substr(0, spacePos));

            str = str.substr(spacePos + 1, str.size());
            vNewVertexPos.z = std::stod(str.substr(0, str.size()));

            vertPos.push_back(vNewVertexPos);
        }
        else if (str.find("vt ") == 0)
        {
            rcl::vec2 vNewTexCoord(0.0f);
            int spacePos;

            str = str.substr(3, str.size());
            spacePos = str.find(" ");
            vNewTexCoord.x = std::stod(str.substr(0, spacePos));

            str = str.substr(spacePos + 1, str.size());
            vNewTexCoord.y = std::stod(str.substr(0, str.size()));

            vertTexCoord.push_back(vNewTexCoord);
        }
        else if (str.find("vn ") == 0)
        {
            rcl::vec3 vNewVertexNormal(0.0f);
            int spacePos;

            str = str.substr(3, str.size());
            spacePos = str.find(" ");
            vNewVertexNormal.x = std::stod(str.substr(0, spacePos));

            str = str.substr(spacePos + 1, str.size());
            spacePos = str.find(" ");
            vNewVertexNormal.y = std::stod(str.substr(0, spacePos));

            str = str.substr(spacePos + 1, str.size());
            vNewVertexNormal.z = std::stod(str.substr(0, str.size()));

            vertNormal.push_back(vNewVertexNormal);
        }
    }

    void ReadFaceInfo(std::string& str, std::vector<vec3>& vertPos,
        std::vector<vec2>& vertTexCoord, std::vector<vec3>& vertNormal, HittableList& triangles)
    {
        std::vector<Vertex> newVertexes;
        str = str.substr(2, str.size());

        if(str.find("//") != str.npos)
        {
            while (str.size())
            {
                Vertex newVertex;
                int spacePos;

                spacePos = str.find("//");
                newVertex.coord = vertPos[std::stoi(str.substr(0, spacePos)) - 1];
                str = str.substr(spacePos + 2, str.size());

                spacePos = str.find(" ");
                newVertex.normal = vertNormal[std::stoi(str.substr(0, spacePos)) - 1];

                newVertexes.push_back(newVertex);

                if(spacePos == -1) break;
                str = str.substr(spacePos, str.size());

            }
        }
        else if (str.find("/") != str.npos)
        {
            int slashPos;
            int spacePos;

            slashPos = str.find("/");
            spacePos = str.find(" ", slashPos + 1);
            slashPos = str.find("/", slashPos + 1);

            if(slashPos < spacePos)
                while(str.size())
                {
                    rcl::Vertex newVertex;
                    int slashPos2;

                    slashPos2 = str.find("/");
                    newVertex.coord = vertPos[std::stoi(str.substr(0, slashPos2)) - 1];
                    str = str.substr(slashPos2 + 1, str.size());

                    slashPos2 = str.find("/");
                    newVertex.uv = vertTexCoord[std::stoi(str.substr(0, slashPos2)) - 1];
                    str = str.substr(slashPos2 + 1, str.size());

                    slashPos2 = str.find(" ");
                    newVertex.normal = vertNormal[std::stoi(str.substr(0, slashPos2)) - 1];
                    newVertexes.push_back(newVertex);
                    
                    if(slashPos2 == -1) break;
                    str = str.substr(slashPos2, str.size());
                }
            else
                while(str.size())
                {
                    rcl::Vertex newVertex;
                    int slashPos2;

                    slashPos2 = str.find("/");
                    newVertex.coord = vertPos[std::stoi(str.substr(0, slashPos2)) - 1];
                    str = str.substr(slashPos2 + 1, str.size());

                    slashPos2 = str.find(" ");
                    newVertex.uv = vertTexCoord[std::stoi(str.substr(0, slashPos2)) - 1];
                    str = str.substr(1, str.size());

                    newVertexes.push_back(newVertex);
                }
        }
        else
        {
            while (str.size())
            {
                rcl::Vertex newVertex;
                int spacePos;

                spacePos = str.find(" ");
                newVertex.coord = vertPos[std::stoi(str.substr(0, spacePos)) - 1];
                str = str.substr(1, str.size());

                newVertexes.push_back(newVertex);
            }
        }
        
        Triangulate(newVertexes, triangles);
    }

    void ReadObjFile(std::ifstream& fstr, HittableList& triangles)
    {
        std::string line;

        std::vector<vec3> vertPos;
        std::vector<vec2> vertTexCoord;
        std::vector<vec3> vertNormal;

        int i = 0;

        while (fstr)
        {
            std::getline(fstr, line);
            if(line.size() < 1) break;

            if(line.find("v") == 0)
                ReadVertexInfo(line, vertPos, vertTexCoord, vertNormal);

            if(line.find("f") == 0)
                ReadFaceInfo(line, vertPos, vertTexCoord, vertNormal, triangles);
        }
    }
        
}

void ImportOBJ(const char* path, HittableList& triangles)
{
    std::ifstream fstr(path);

	if (!fstr.is_open())
	{
		std::cout << "Unable to find or open the file : " << path;
		return;
	}

	ReadObjFile(fstr, triangles);

	fstr.close();
}

}