#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include "common.h"
#include "apple.h"
#include "bike.h"
#include "tree.h"
#include "chick.h"
#include "house.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "shaders.h"
#include "math_3d.h"
#include "pipeline.h"
#include "camera.h"
#include "glut_backend.h"
#include "texture.h"
#include "lighting_technique.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

struct Vertex
{
	Vector3f m_pos;
	Vector2f m_tex;
	Vector3f m_normal;

	Vertex() {}

	Vertex(Vector3f pos, Vector2f tex, Vector3f normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};

static const float FieldDepth = 20.0f;
static const float FieldWidth = 10.0f;

class StickFigureApp : public ICallbacks
{
public:
	StickFigureApp() {
		m_pGameCamera = 0;
		m_pTexture = 0;
		m_pEffect = 0;
		m_scale = 0.0f;
		m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_directionalLight.AmbientIntensity = 0.3f;
		m_directionalLight.Direction = Vector3f(0.0f, 0.0, 1.0);
		m_directionalLight.DiffuseIntensity = 0.7f;

		m_persProjInfo.FOV = 60.0f;
		m_persProjInfo.Height = WINDOW_HEIGHT;
		m_persProjInfo.Width = WINDOW_WIDTH;
		m_persProjInfo.zNear = 1.0f;
		m_persProjInfo.zFar = 100.0f;
	}

	~StickFigureApp()
	{
		delete m_pEffect;
		delete m_pGameCamera;
		delete m_pTexture;
	}

	bool Init()
	{
		Vector3f Pos(5.0f, 1.0f, -3.0f);
		Vector3f Target(0.0f, 0.0f, 1.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);
		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		CreateVertexBuffer();

		m_pEffect = new LightingTechnique();

		if (!m_pEffect->Init())
		{
			return false;
		}

		m_pEffect->Enable();

		m_pEffect->SetTextureUnit(0);

		m_pTexture = new Texture(GL_TEXTURE_2D, "data/textures/bricks.bmp");

		if (!m_pTexture->LoadBMP()) {
			return false;
		}

		return true;
	}

	void Run()
	{
		GLUTBackendRun(this);
	}

	virtual void RenderSceneCB()
	{
		m_pGameCamera->OnRender();

		glClear(GL_COLOR_BUFFER_BIT);

		m_scale += 0.0057f;

		PointLight pl[2];
		pl[0].DiffuseIntensity = 0.5f;
		pl[0].Color = Vector3f(1.0f, 0.5f, 0.0f);
		pl[0].Position = Vector3f(3.0f, 1.0f, FieldDepth * (cosf(m_scale) + 1.0f) / 2.0f);
		pl[0].Attenuation.Linear = 0.1f;
		pl[1].DiffuseIntensity = 0.5f;
		pl[1].Color = Vector3f(0.0f, 0.5f, 1.0f);
		pl[1].Position = Vector3f(7.0f, 1.0f, FieldDepth * (sinf(m_scale) + 1.0f) / 2.0f);
		pl[1].Attenuation.Linear = 0.1f;
		m_pEffect->SetPointLights(2, pl);

		Pipeline p;
		p.WorldPos(0.0f, 0.0f, 1.0f);
		p.SetCamera(*m_pGameCamera);
		p.SetPerspectiveProj(m_persProjInfo);
		m_pEffect->SetWVP(p.GetWVPTrans());
		const Matrix4f& WorldTransformation = p.GetWorldTrans();
		m_pEffect->SetWorldMatrix(WorldTransformation);
		m_pEffect->SetDirectionalLight(m_directionalLight);
		m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
		m_pEffect->SetMatSpecularIntensity(1.0f);
		m_pEffect->SetMatSpecularPower(32);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
		m_pTexture->Bind(GL_TEXTURE0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// glDrawElements(GL_TRIANGLES, 2, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glutSwapBuffers();
	}

	virtual void KeyboardCB(OGLDEV_KEY OgldevKey)
	{
		switch (OgldevKey) {
		case OGLDEV_KEY_ESCAPE:
		case OGLDEV_KEY_q:
			GLUTBackendLeaveMainLoop();
			break;

		case OGLDEV_KEY_a:
			m_directionalLight.AmbientIntensity += 0.05f;
			break;

		case OGLDEV_KEY_s:
			m_directionalLight.AmbientIntensity -= 0.05f;
			break;

		default:
			m_pGameCamera->OnKeyboard(OgldevKey);
		}
	}

	virtual void PassiveMouseCB(int x, int y)
	{
		m_pGameCamera->OnMouse(x, y);
	}

protected:
private:
	void CreateVertexBuffer()
	{
		const Vector3f Normal = Vector3f(0.0f, 1.0f, 0.0f);
		Vertex vertices[6] = {
			Vertex(Vector3f(0.0f, 0.0f, 0.0f),             Vector2f(0.0f, 0.0f), Normal),
			Vertex(Vector3f(0.0f, 0.0f, FieldDepth),       Vector2f(0.0f, 1.0f), Normal),
			Vertex(Vector3f(FieldWidth, 0.0f, 0.0f),       Vector2f(1.0f, 0.0f), Normal),

			Vertex(Vector3f(FieldWidth, 0.0f, 0.0f),       Vector2f(1.0f, 0.0f), Normal),
			Vertex(Vector3f(0.0f, 0.0f, FieldDepth),       Vector2f(0.0f, 1.0f), Normal),
			Vertex(Vector3f(FieldWidth, 0.0f, FieldDepth), Vector2f(1.0f, 1.0f), Normal)
		};

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	}

	GLuint m_VBO;
	LightingTechnique* m_pEffect;
	Texture* m_pTexture;
	Camera* m_pGameCamera;
	float m_scale;
	DirectionalLight m_directionalLight;
	PersProjInfo m_persProjInfo;
};

int main(int argc, char** argv)
{
	/*************** stick figure evaluation part *****************/
	/*
	bool standard = false;
	std::string fileName = standard ? file_proc::getPathByName("house", file_proc::STD_FIG) : file_proc::getPathByName("house_test", file_proc::TEST_FIG);
	cv::Mat ori = cv::imread(fileName);

	MyObject *house = new House(ori, standard);
	house->calFeature();
	house->plotFeature();

	if (standard) {
		house->updateStdFeature();
	}
	else {
		house->transform();
		house->evaluate();
	}
	*/

	GLUTBackendInit(argc, argv, false, false);

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "StickFigure App")) {
		return 1;
	}

	StickFigureApp* pApp = new StickFigureApp();

	if (!pApp->Init()) {
		return 1;
	}

	pApp->Run();

	delete pApp;

	return 0;

	return(0);
}
