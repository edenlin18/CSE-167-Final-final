#define _CRT_SECURE_NO_WARNINGS
#include "Parser.h"

using namespace std;

Object* Parser::parse_object(char* fileName) {
	FILE* fp;
	unsigned int vertexCounter = 0;
	unsigned int normalCounter = 0;
	unsigned int faceCounter = 0;
	unsigned int colorCounter = 0;
	GLdouble val1, val2, val3, val4, val5, val6, dump;
	Object* obj = new Object();

	cerr << "Start Parsing " << fileName << ", Please Wait......\n";

	fp = fopen(fileName, "rb");
	if (fp == NULL) {
		cerr << "error loading file" << endl;
		//exit(-1);
	}

	char c1, c2;
	char line[128];

	while (fgets(line, 128, fp)) {
		c1 = line[0];
		c2 = line[1];
		switch (c1) {
			case 'v':
				if (c2 == 'n') {
					sscanf(line, "vn %lf %lf %lf\n", &val1, &val2, &val3);
					obj->pushNormal(val1, val2, val3);
					//cerr << "normals: " << val1 << ", " << val2 << ", " << val3 << endl;
					++normalCounter;
				}
				else if (c2 == 't') {
					break;
				}
				else if(c2 == ' ') {
					sscanf(line, "v %lf %lf %lf\n", &val1, &val2, &val3);
					obj->pushVertex(val1, val2, val3);
					//cerr << "vertex: " << val1 << ", " << val2 << ", " << val3 << endl;
					++vertexCounter;

					GLdouble r = ((double) rand() * 1.0 / (double) RAND_MAX);
					GLdouble g = ((double) rand() * 1.0 / (double) RAND_MAX);
					GLdouble b = ((double) rand() * 1.0 / (double) RAND_MAX);

					obj->pushColor(r, g, b);
					++colorCounter;
				}

				break;
			case 'f':
				sscanf(line, "f %lf/%lf/%lf %lf/%lf/%lf %lf/%lf/%lf\n", &val1, &dump, &val4, &val2, &dump, &val5, &val3, &dump, &val6);
				obj->pushFace(val1, val2, val3, val4, val5, val6);
				//cerr << "faces: " << val1 << ", " << val2 << ", " << val3 << ", " << val4 << ", " << val5 << ", " << val6 << endl;
				++faceCounter;
				break;
			default:
				break;
		}
		//system("PAUSE");
	}

	cerr << "Parsing " << fileName << " Completed!!!" << endl;
	cerr << "Number of vertex parsed: " << vertexCounter << endl;
	cerr << "Number of normal parsed: " << normalCounter << endl;
	cerr << "Number of color parsed: " << colorCounter << endl;
	cerr << "Number of face parsed: " << faceCounter << endl;
	fclose(fp);

	obj->setMid();

	return obj;
}