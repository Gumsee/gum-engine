#include "DebugDrawer.h"



DebugDrawer::DebugDrawer(ShaderProgram *shader)
{
	this->shader = shader;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)offsetof(vec3, x));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	vertices.resize(100000);
}


DebugDrawer::~DebugDrawer()
{
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
}

void DebugDrawer::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
{
	vertices.clear();
	vertices.push_back(vec3(from.x(), from.y(), from.z()));
	vertices.push_back(vec3(to.x(), to.y(), to.z()));

	glBindVertexArray(VAO);
	shader->use();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	shader->LoadUniform("color", vec3(color.x(), color.y(), color.z()));
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINES, 0, vertices.size());
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	shader->unuse();
	glBindVertexArray(0);
}

void DebugDrawer::drawSphere(const btVector3 & p, btScalar radius, const btVector3 & color)
{
}

void DebugDrawer::drawTriangle(const btVector3 & a, const btVector3 & b, const btVector3 & c, const btVector3 & color, btScalar alpha)
{
}

void DebugDrawer::drawContactPoint(const btVector3 & PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3 & color)
{
}

void DebugDrawer::reportErrorWarning(const char * warningString)
{
}

void DebugDrawer::draw3dText(const btVector3 & location, const char * textString)
{
}

void DebugDrawer::setDebugMode(int debugMode)
{
	this->m_debugMode = debugMode;
}

void DebugDrawer::prepare()
{
	vertices.clear();
}

void DebugDrawer::finish()
{
	glBindVertexArray(VAO);
	shader->use();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINES, 0, vertices.size());
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	shader->unuse();
	glBindVertexArray(0);
}
