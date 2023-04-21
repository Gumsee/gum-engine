#include "Object.h"
#include "../Managers/ShaderManager.h"

//Rendering stuff
void Object::render()
{
    prepareRender();
	/*glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); 

	glStencilMask(0x00); // make sure we don't update the stencil buffer while drawing the floor
	
	glStencilFunc(GL_ALWAYS, 1, 0xFF); 
	glStencilMask(0xFF); */
    renderMesh();

	//renderOutline();
    finishRender();

}

void Object::prepareRender()
{	
	if ((bool)Settings::getSetting(Settings::WIREFRAMED)) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
	else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

	//glEnable(GL_DEPTH_TEST); 
	
	if(pMaterial->isTransparent()) { glEnable(GL_BLEND); } 
	else { glDisable(GL_BLEND); }
	
	if(pProperties->inverseCulling) { glCullFace(GL_FRONT); }
	else { glCullFace(GL_BACK); }

	if(!pProperties->hasbackface) { glDisable(GL_CULL_FACE); }
	
	//Texture related
	ShaderProgram::getCurrentlyBoundShader()->loadUniform("hasTexture", (int)pMaterial->hasTexture());
	ShaderProgram::getCurrentlyBoundShader()->loadUniform("hasNormalMap", (int)pMaterial->hasNormalMap());
	ShaderProgram::getCurrentlyBoundShader()->loadUniform("hasSpecularMap", (int)pMaterial->hasSpecularMap());
	ShaderProgram::getCurrentlyBoundShader()->loadUniform("hasReflectionMap", (int)pMaterial->hasReflectionMap());
	ShaderProgram::getCurrentlyBoundShader()->loadUniform("hasRefractionMap", (int)pMaterial->hasRefractionMap());
	ShaderProgram::getCurrentlyBoundShader()->loadUniform("hasRoughnessMap", (int)pMaterial->hasRoughnessMap());
	ShaderProgram::getCurrentlyBoundShader()->loadUniform("hasDisplacementMap", (int)pMaterial->hasDisplacementMap());
	ShaderProgram::getCurrentlyBoundShader()->loadUniform("hasAmbientOcclusionMap", (int)pMaterial->hasAmbientOcclusionMap());

	ShaderProgram::getCurrentlyBoundShader()->loadUniform("TextureMultiplier", pMaterial->getTextureMultiplier());
	ShaderProgram::getCurrentlyBoundShader()->loadUniform("ReflectionFactor", *pMaterial->getReflectivity());
	ShaderProgram::getCurrentlyBoundShader()->loadUniform("RefractionFactor", *pMaterial->getRefractivity());
	ShaderProgram::getCurrentlyBoundShader()->loadUniform("specularFactor", *pMaterial->getSpecularity());
	ShaderProgram::getCurrentlyBoundShader()->loadUniform("roughnessFactor", *pMaterial->getRoughness());
	ShaderProgram::getCurrentlyBoundShader()->loadUniform("color", pMaterial->getColor());
	ShaderProgram::getCurrentlyBoundShader()->loadUniform("flipNormal", (int)pMaterial->hasFlippedNormals());
}


void Object::renderMesh()
{
	glEnable(GL_BLEND);
	pMaterial->bindTextures();

	for(size_t i = 0; i < vInstances.size(); i++)
	{
		vInstances[i]->updateMatrix();
		if(vInstances[i]->getBody() != nullptr || vInstances[i]->transformChanged)
		{
			pTransMatricesVBO->setSingleData(vInstances[i]->getMatrix(), i);
        }

        vInstances[i]->transformChanged = false;
    }

	ShaderProgram::getCurrentlyBoundShader()->loadUniform("isInstanced", (int)pProperties->isInstanced);
	ShaderProgram::getCurrentlyBoundShader()->loadUniform("transformationMatrix", getInstance(0)->getMatrix());

	//////////////
	//Only update matrices when rigidbody is active or when theres no rigidbody check for the transformchanged boolean
	//////////////

	if(pProperties->isInstanced || !vInstances[0]->renderOutline)
	{
		pVertexArrayObject->bind();
		if(pProperties->renderTessellated)
		{
			if(pProperties->isInstanced)
			{
				glDrawElementsInstanced(GL_PATCHES, pVertexArrayObject->numVertices(), GL_UNSIGNED_INT, 0, vInstances.size());
			}
			else
			{
				glDrawElements(GL_PATCHES, pVertexArrayObject->numVertices(), GL_UNSIGNED_INT, 0);
			}	
		}
		else
		{
			if(pProperties->isInstanced)
			{
				glDrawElementsInstanced(GL_TRIANGLES, pVertexArrayObject->numVertices(), GL_UNSIGNED_INT, 0, vInstances.size());
			}
			else
			{
				glDrawElements(GL_TRIANGLES, pVertexArrayObject->numVertices(), GL_UNSIGNED_INT, 0);
			}	
		}

		pVertexArrayObject->unbind();
	}
	pMaterial->unbindTextures();
}


void Object::finishRender()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}
