#pragma once
// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;
class bone
{
public:
	string name;
	vec3 pos;
	quat q;
	bone *parent = NULL;
	vector<bone*> kids;
	int index;
	mat4 *mat = NULL;
	void write_to_VBO(vec3 origin,vector<vec3> &vpos)
	{
		vpos.push_back(origin);
		vec3 endp = origin + pos;
		vpos.push_back(endp);
		for (int i = 0; i < kids.size(); i++)
			kids[i]->write_to_VBO(endp, vpos);

	}

};
int readtobone(bone **root);