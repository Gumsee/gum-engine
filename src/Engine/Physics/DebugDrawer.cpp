#include "DebugDrawer.h"
#include "PhysicsDebugShader.h"
#include "../Shaders/ShaderManager.h"
#include "../Rendering/Camera.h"
#include "System/MemoryManagement.h"
#include "System/Output.h"
#include <btBulletDynamicsCommon.h>



DebugDrawer::DebugDrawer()
{
    
    pVAO = new VertexArrayObject();
    pVBO = new VertexBufferObject<vec3>();
    pVAO->addAttribute(pVBO, 0, 3, GL_FLOAT, sizeof(vec3), 0);

    initShader();
}


DebugDrawer::~DebugDrawer()
{
    Gum::_delete(pVAO);
    Gum::_delete(pVBO);
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
{
	//glBegin(GL_LINES);
	//glColor3f(fromColor.getX(), fromColor.getY(), fromColor.getZ());
	//glVertex3d(from.getX(), from.getY(), from.getZ());
	//glColor3f(toColor.getX(), toColor.getY(), toColor.getZ());
	//glVertex3d(to.getX(), to.getY(), to.getZ());
	//glEnd();
	//
    Gum::Output::print("drawing line1");
}

void DebugDrawer::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
{
    //Gum::Output::print(vec3(from.x(), from.y(), from.z()).toString() + " " + vec3(to.x(), to.y(), to.z()).toString());
	vertices.clear();
	vertices.push_back(vec3(from.x(), from.y(), from.z()));
	vertices.push_back(vec3(to.x(), to.y(), to.z()));
    pVBO->setData(vertices, GL_STATIC_DRAW);

	pVAO->bind();
	pShaderProgram->use();
	pShaderProgram->loadUniform("color", vec4(color.x(), color.y(), color.z(), 1.0f));
    pShaderProgram->loadUniform("viewMatrix", Camera::getActiveCamera()->getViewMatrix());
    pShaderProgram->loadUniform("projectionMatrix", Camera::getActiveCamera()->getProjectionMatrix());
	glDrawArrays(GL_LINES, 0, vertices.size());
	pShaderProgram->unuse();
	pVAO->unbind();
}

void DebugDrawer::drawSphere(const btVector3 & p, btScalar radius, const btVector3 & color)
{
    Gum::Output::print("drawing sphere");
}

void DebugDrawer::drawTriangle(const btVector3 & a, const btVector3 & b, const btVector3 & c, const btVector3 & color, btScalar alpha)
{
    Gum::Output::print("drawing triangle");
}

void DebugDrawer::drawContactPoint(const btVector3 & PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3 & color)
{
    Gum::Output::print("drawContactPoint");
}

void DebugDrawer::reportErrorWarning(const char * warningString)
{
    Gum::Output::print("reportErrorWarning");
}

void DebugDrawer::draw3dText(const btVector3 & location, const char * textString)
{
    Gum::Output::print("draw3dText");
}

void DebugDrawer::setDebugMode(int debugMode)
{
	this->m_debugMode = debugMode;
}

void DebugDrawer::initShader()
{
    if(pShaderProgram == nullptr)
	{
		pShaderProgram = new ShaderProgram();
        pShaderProgram->addShader(new Shader(PhysicsDebuggingVertexShader, Shader::VERTEX_SHADER));
        pShaderProgram->addShader(new Shader(PhysicsDebuggingFragmentShader, Shader::FRAGMENT_SHADER));
        pShaderProgram->build("PhysicsDebuggingpShaderProgram");
        pShaderProgram->addUniform("color");
        Gum::ShaderManager::addShaderProgram(pShaderProgram);
    }
}