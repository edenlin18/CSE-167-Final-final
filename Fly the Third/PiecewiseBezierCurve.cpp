#include "PiecewiseBezierCurve.h"


PiecewiseBezierCurve::PiecewiseBezierCurve(int n, Vector3d* c, int r, bool _debug){
	N = n;
	resolution = r;
	debug = _debug;
	selected = -1;
	int size = 3 * N + 1;
	cp = new Vector3d[size];
	Cs = new Matrix4d[N];
	for (int i = 0; i < N; i++){
		Vector3d start = c[i];
		Vector3d end = c[i + 1];
		Vector3d n1 = end - start;
		Vector3d n2 = n1;
		n2.scale(0.7);
		n1.scale(0.35);
		cp[i * 3] = start;
		if (i != 0)
			cp[i * 3 + 1] = start - cp[i * 3 - 1] + start;
		else
			cp[i * 3 + 1] = start + n1;
		cp[i * 3 + 2] = start + n2;
		cp[i * 3 + 3] = end;
	}
	Bez = Matrix4d(-1, 3, -3, 1,
		3, -6, 3, 0,
		-3, 3, 0, 0,
		1, 0, 0, 0);
	for (int i = 0; i < N; i++){
		Matrix4d m = Matrix4d(cp[i * 3][0], cp[i * 3 + 1][0], cp[i * 3 + 2][0], cp[i * 3 + 3][0],
			cp[i * 3][1], cp[i * 3 + 1][1], cp[i * 3 + 2][1], cp[i * 3 + 3][1],
			cp[i * 3][2], cp[i * 3 + 1][2], cp[i * 3 + 2][2], cp[i * 3 + 3][2],
			0, 0, 0, 0);
		Cs[i] = m * Bez;
	}
	if (debug){
		root = new MatrixTransform(Matrix4d());
		cpBall = new Sphere*[size];
		ballPosition = new MatrixTransform*[size];
		for (int i = 0; i < size; i++){
			t.makeTranslate(cp[i][0], cp[i][1], cp[i][2]);
			MatrixTransform * mt = new MatrixTransform(t);
			ballPosition[i] = mt;
			root->addChild(mt);
			cpBall[i] = new Sphere(0.05, 20, 20, Vector3d(1, 0, 0), draw::SOLID);
			mt->addChild(cpBall[i]);
		}
	}
	else
		root = NULL;
	
}

bool PiecewiseBezierCurve::setCp(int index, Vector3d c){
	if (index >= 0 && index < 3 * N + 1){
		int i = index % 3;
		int mi = index / 3;
		if (i == 0 && index != 0)
			mi -= 1;
		cp[index] = c;
		if (debug){
			t.makeTranslate(cp[index][0], cp[index][1], cp[index][2]);
			ballPosition[index]->setMatrix(t);
		}
		
		recalulateBez(mi);
		switch (i){
		case 0:	// adjust right adjacent cp if there are two adjacent cps 
			if (index != 0 && index != 3 * N){
				cp[index + 1] = cp[index] - cp[index - 1] + cp[index];
				if (debug){
					t.makeTranslate(cp[index + 1][0], cp[index + 1][1], cp[index + 1][2]);
					ballPosition[index + 1]->setMatrix(t);
				}
			}
			if (mi + 1 < N)
				recalulateBez(mi + 1);
			break;
		case 1:	// adjust left adjacent cp if index of the adjacent cp is not 0 
			if (index - 1 != 0){
				cp[index - 1] = (cp[index - 2] + cp[index]).scale(0.5);
				if (debug){
					t.makeTranslate(cp[index - 1][0], cp[index - 1][1], cp[index - 1][2]);
					ballPosition[index - 1]->setMatrix(t);
				}
			}
			if (mi - 1 >= 0)
				recalulateBez(mi - 1);
			break;
		case 2: // adjust right adjacent cp if index of the adjacent cp is not 3 * N
			if (index + 1 != 3 * N){
				cp[index + 1] = (cp[index + 2] + cp[index]).scale(0.5);
				if (debug){
					t.makeTranslate(cp[index + 1][0], cp[index + 1][1], cp[index + 1][2]);
					ballPosition[index + 1]->setMatrix(t);
				}
			}
			if (mi + 1 < N)
				recalulateBez(mi + 1);
		}
		
		return true;
	}
	else
		return false;
}

Vector3d PiecewiseBezierCurve::getCp(int index){
	return cp[index];
}

Vector3d PiecewiseBezierCurve::compute(double _t){
	int i = floor(N * _t);
	double t = N * _t - i;
	double tt = t * t;
	double ttt = tt * t;
	Vector4d T = Vector4d(ttt, tt, t, 1);
	return (Cs[i] * T).getVector3d();
}

void PiecewiseBezierCurve::render(Matrix4d m){
	double incr = (double)1 / (double)resolution;
	glColor3f(0, 1, 0);
	glLineWidth(3);
	double t = incr;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(m.getPointer());
	glBegin(GL_LINE_STRIP);
	glVertex3f(cp[0][0], cp[0][1], cp[0][2]);
	for (int i = 1; i < resolution; i++){
		Vector3d v = compute(t);
		glVertex3f(v[0], v[1], v[2]);
		t += incr;
	}
	glVertex3f(cp[3 * N][0], cp[3 * N][1], cp[3 * N][2]);
	glEnd();
	if (debug){
		root->draw(m);
	}
}

void PiecewiseBezierCurve::selectNext(){
	if (selected != -1){
		cpBall[selected]->setColor(Vector3d(1, 0, 0));
	}
	selected++;
	selected = selected % (3 * N + 1);
	cpBall[selected]->setColor(Vector3d(0, 0, 1));
		
}

void PiecewiseBezierCurve::selectPrev(){
	if (selected != -1){
		cpBall[selected]->setColor(Vector3d(1, 0, 0));
	}
	if (selected == 0 || selected == -1)
		selected = 3 * N;
	else{
		selected--;
	}
	cpBall[selected]->setColor(Vector3d(0, 0, 1));
}

void PiecewiseBezierCurve::deSelect(){
	if (selected != -1)
		cpBall[selected]->setColor(Vector3d(1, 0, 0));
	selected = -1;
}

int PiecewiseBezierCurve::getSelected(){
	return selected;
}

void PiecewiseBezierCurve::recalulateBez(int i){
	Matrix4d m = Matrix4d(cp[i * 3][0], cp[i * 3 + 1][0], cp[i * 3 + 2][0], cp[i * 3 + 3][0],
		cp[i * 3][1], cp[i * 3 + 1][1], cp[i * 3 + 2][1], cp[i * 3 + 3][1],
		cp[i * 3][2], cp[i * 3 + 1][2], cp[i * 3 + 2][2], cp[i * 3 + 3][2],
		0, 0, 0, 0);
	Cs[i] = m * Bez;
}